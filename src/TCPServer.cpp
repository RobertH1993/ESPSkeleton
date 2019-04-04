#include "TCPServer.h"
#include <ESP8266WiFi.h>
#include "Config.h"

#define TCP_SERVER_STOPPED 0
#define TCP_SERVER_STARTED 1

//Forward declarations
void tcp_server_handle_connections();
void tcp_server_disconnect_client();

uint8_t server_status = TCP_SERVER_STOPPED;
WiFiServer tcp_server(23);

//Connection details about connected client, if any
char command_buffer[COMMAND_BUF_LENGTH];
uint8_t command_length = 0;
WiFiClient current_client;
uint64_t client_connection_time = 0;

//Public
void tcp_server_start(){
  if(server_status == TCP_SERVER_STOPPED){
    tcp_server.begin();
    tcp_server.setNoDelay(true);
    server_status = TCP_SERVER_STARTED;
  }
}

//Public
void tcp_server_stop(){
  if(server_status == TCP_SERVER_STARTED){
    tcp_server_disconnect_client();
    tcp_server.close();
    tcp_server.stop();
    server_status = TCP_SERVER_STOPPED;
  }
}

//Public
uint8_t tcp_server_get_client_command(char *out){
  tcp_server_handle_connections();

  //Check if we have a connected client and if it has sent data
  if(!current_client || !current_client.connected()) return 0;
  while(current_client.available()){
    char data = current_client.read();
    
    //Check invalid commands / states
    if(command_length >= COMMAND_BUF_LENGTH){ //Buffer full disconnect client
      tcp_server_disconnect_client();
      return 0;
    }

    //Check if we received a command delimiter
    if(data == '\n'){
      command_buffer[command_length] = 0;
      command_length +=1;
      memcpy(out, command_buffer, command_length);

      client_connection_time = millis();
      uint8_t ret = command_length;
      command_length = 0;
      return ret;
    }

    //Copy data to buffer
    command_buffer[command_length] = data;
    command_length += 1;

    return 0;
  }
}

//Public
void tcp_server_send_data_to_connected_client(uint8_t opcode, uint8_t state, uint8_t* data, uint8_t len){
  if(!current_client || !current_client.connected()) return;

  //Create packet
  uint8_t packet_buffer[COMMAND_BUF_LENGTH] = {};
  packet_buffer[0] = opcode;
  packet_buffer[1] = state;
  memcpy(&packet_buffer[2], data, len);
  packet_buffer[2 + len + 1] = '\n';

  //Send to client (could go wrong with 0 bytes??)
  current_client.print((char*)packet_buffer);
}

//Private
void tcp_server_handle_connections(){
  if(server_status != TCP_SERVER_STARTED) return;

  //Check if a client is already connected, and if it is timed-out
  if(current_client.connected()){
    if((millis() - client_connection_time) > 5000){
      tcp_server_disconnect_client();
    }
    return;
  }

  //Accept new connection
  current_client = tcp_server.available();
  if(current_client && current_client.connected()){
    client_connection_time = millis();
  }
}

//Private
void tcp_server_disconnect_client(){
  if(current_client){
    current_client.stop();
    command_length = 0;
  }
}
