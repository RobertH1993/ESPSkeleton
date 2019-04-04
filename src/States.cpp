#include "States.h"
#include "Storage.h"
#include "Wifi.h"
#include "TCPServer.h"
#include "Commands.h"
#include "Config.h"
#include "MQTTClient.h"
#include "Display.h"


//State offsets (Global)
#define STATE_ENTRY_FUNCTION 0
#define STATE_MAIN_FUNCTION 1
#define STATE_EXIT_FUNCTION 2

#define EMPTY_EVENT {0, 0}

//Forward declarations
struct StateEvent state_setup_entry(uint8_t variable);
struct StateEvent state_setup_main(uint8_t variable);
struct StateEvent state_setup_exit(uint8_t variable);
struct StateEvent state_connecting_entry(uint8_t variable);
struct StateEvent state_connecting_main(uint8_t variable);
struct StateEvent state_connecting_exit(uint8_t variable);
struct StateEvent state_idle_entry(uint8_t variable);
struct StateEvent state_idle_main(uint8_t variable);
struct StateEvent state_idle_exit(uint8_t variable);

//Array holding the entry / main / exit functions for every state
struct StateEvent (*state_functions[3][3]) (uint8_t variable) = {
  {state_setup_entry, state_setup_main, state_setup_exit},
  {state_connecting_entry, state_connecting_main, state_connecting_exit},
  {state_idle_entry, state_idle_main, state_idle_exit}
};

uint8_t current_state = STATE_SETUP;
uint8_t current_function = STATE_ENTRY_FUNCTION;
uint8_t current_variable = 0;

//Public
void state_switch(uint8_t new_state, uint8_t variable){
  //Call exit function of current state
  state_functions[current_state][STATE_EXIT_FUNCTION](current_variable);

  //Change state and call entry function
  current_state = new_state;
  current_variable = variable;
  current_function = STATE_ENTRY_FUNCTION;
}

//Public
struct StateEvent state_run_current(){
  //Run current state
  struct StateEvent event = state_functions[current_state][current_function](current_variable);

  //Switch from entry to main if needed
  if(current_function == STATE_ENTRY_FUNCTION){
    current_function = STATE_MAIN_FUNCTION;
  }

  return event;
}


//Private
struct StateEvent state_setup_entry(uint8_t variable){
  storage_load_config_from_eeprom();
  struct ConfigData* data = storage_get_config();

  Serial.print("Wifi initialized: ");
  Serial.println(data->wifi_config_initialized);
  if(data->wifi_config_initialized == 1){
    return {EVENT_SWITCH_TO_CONNECTING, 0};
  }

  wifi_init_ap_mode("ESP6288", "password123"); 
  tcp_server_start();

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_setup_main(uint8_t variable){
  //Blink WiFi LED
  if(wifi_ap_mode_get_number_of_connected_clients() != 0){
    wifi_blink_status_led(600);
  } else {
    wifi_blink_status_led(1000);
  }
  
  //Get command
  char command_buffer[COMMAND_BUF_LENGTH];
  uint8_t command_length = tcp_server_get_client_command(command_buffer);
  if(command_length == 0) return EMPTY_EVENT;

  //Get config data
  struct ConfigData* data = storage_get_config();

  //Handle commands (SET)
  if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_SSID_SETUP){ // ---- Wifi setup
    char *ssid = command_get_text_data(command_buffer);
    strncpy(data->wifi_ssid, ssid, 32);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_PASSWORD_SETUP){
    char *password = command_get_text_data(command_buffer);
    strncpy(data->wifi_password, password, 32);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_INITIALIZE){
    data->wifi_config_initialized = 1;
    storage_save_current_config();
    return {EVENT_SWITCH_TO_CONNECTING, 0};
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_DHCP_OR_STATIC){ // ----- Static IP setup
    data->wifi_DHCP_or_static = command_get_single_byte_data(command_buffer, 0);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_STATIC_IP){
    memcpy(data->wifi_static_ip, command_get_byte_data(command_buffer, 0), 4);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_STATIC_GATEWAY){
    memcpy(data->wifi_static_gateway, command_get_byte_data(command_buffer, 0), 4);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_STATIC_NETMASK){
    memcpy(data->wifi_static_netmask, command_get_byte_data(command_buffer, 0), 4);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_STATIC_DNS1){
    memcpy(data->wifi_static_dns1, command_get_byte_data(command_buffer, 0), 4);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_WIFI_STATIC_DNS2){
    memcpy(data->wifi_static_dns2, command_get_byte_data(command_buffer, 0), 4);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_MQTT_SERVER){ // ------- MQTT setup
    char *server = command_get_text_data(command_buffer);
    strncpy(data->mqtt_server, server, 64);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_MQTT_PORT){
    data->mqtt_port = command_get_uint16_data(command_buffer);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_MQTT_USERNAME){
    char *username = command_get_text_data(command_buffer);
    strncpy(data->mqtt_username, username, 16);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_MQTT_PASSWORD){
    char *password = command_get_text_data(command_buffer);
    strncpy(data->mqtt_password, password, 16);
  }else if(command_get_opcode(command_buffer) == COMMAND_OPCODE_MQTT_TOPIC){
    char *topic = command_get_text_data(command_buffer);
    strncpy(data->mqtt_topic, topic, 32);
  }

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_setup_exit(uint8_t variable){
  tcp_server_stop();
  wifi_stop_ap_mode();
  wifi_set_status_led(0);

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_connecting_entry(uint8_t variable){
  struct ConfigData* data = storage_get_config();

  if(data->wifi_DHCP_or_static == 1){
    wifi_set_static_configuration(data->wifi_static_ip,
                                  data->wifi_static_netmask,
                                  data->wifi_static_gateway,
                                  data->wifi_static_dns1,
                                  data->wifi_static_dns2);
                                  
  }
  wifi_connect_to_ap(data->wifi_ssid, data->wifi_password);

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_connecting_main(uint8_t variable){
  if(wifi_is_connected_to_ap()){
    wifi_set_status_led(1);
    Serial.println("Connected to AP");
    return {EVENT_SWITCH_TO_IDLE, 0};
  }
  
  wifi_blink_status_led(300);
  return EMPTY_EVENT;
}

//Private
struct StateEvent state_connecting_exit(uint8_t variable){
}

//Private
struct StateEvent state_idle_entry(uint8_t variable){
  struct ConfigData* data = storage_get_config();
  //TODO get MQTT server port from config
  mqtt_init_client(data->mqtt_server, 1883, data->mqtt_username, data->mqtt_password);
  mqtt_subscribe_to_topic("generic/test");

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_idle_main(uint8_t variable){
  //check if we got disconnected from wifi, if so return to connecting state
  if(!wifi_is_connected_to_ap()){
    return {EVENT_SWITCH_TO_CONNECTING, 0};
  }

  //Check if we are connected to the MQTT server
  if(mqtt_connected() != 1){
    Serial.println("Connection to MQTT server");
    mqtt_connect();
    return EMPTY_EVENT;
  }

  mqtt_loop();

  return EMPTY_EVENT;
}

//Private
struct StateEvent state_idle_exit(uint8_t variable){
  //TODO disconnect from MQTT server
  wifi_stop_sta_mode();
  return EMPTY_EVENT;
}
