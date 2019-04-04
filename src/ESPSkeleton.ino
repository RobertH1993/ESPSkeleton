#include "States.h"
#include "Storage.h"
#include "Display.h"
#include "GPIO.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Started !");

  uint16_t i = 1;
  if(*((char*)&i) == 1){
    Serial.println("Little endian");
  }else{
    Serial.println("Big endian");
  }

  //This can be removed after EEPROM has been loaded with the config data (configuration can also be done through the TCP server)
  storage_load_config_from_eeprom();
  struct ConfigData* data = storage_get_config();
  strncpy(data->mqtt_server, "server.nl", 64);
  strncpy(data->mqtt_username, "abc", 16);
  strncpy(data->mqtt_password, "abc", 16);
  storage_save_current_config();
  
  GPIO_init();
  //display_init();
}

void loop() {
  //Run state machine
  struct StateEvent event = state_run_current();
  if(event.event == EVENT_SWITCH_TO_CONNECTING){
    state_switch(STATE_CONNECTING, event.variable);
  }else if(event.event == EVENT_SWITCH_TO_SETUP){
    state_switch(STATE_SETUP, event.variable);
  }else if(event.event == EVENT_SWITCH_TO_IDLE){
    state_switch(STATE_IDLE, event.variable);
  }

  //Check reset button (this button is valid in every state)
  if(GPIO_get_single_button_state_debounced(GPIO_BUTTON_ID_RESET) == 1){
    struct ConfigData* data = storage_get_config();
    data->wifi_config_initialized = 0;
    storage_save_current_config();
    state_switch(STATE_SETUP, 0);
  }
}
