#include "MQTTClient.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient MQTT_client;
PubSubClient mqtt(MQTT_client);
char client_id[16] = {};
uint8_t n_subscribed_topics = 0;
char subscribed_topics[16][32] = {};

char *auth_username = NULL;
char *auth_password = NULL;

//Forward declarations
void mqtt_callback(char *topic, byte *payload, unsigned int length);

//Public
void mqtt_init_client(char *server, uint16_t port, char *username, char *password){
  mqtt.setServer(server, port);
  mqtt.setCallback(mqtt_callback);
  randomSeed(micros());

  //Generate a random client id
  for(uint8_t i = 0; i < 8; i++){
    client_id[i] = (char)random(48, 57);
  }
  client_id[8] = 0;

  auth_username = username;
  auth_password = password;
}

//Public
uint8_t mqtt_connect(){
  uint8_t connection_status = 0;
  if(!auth_username || !auth_password){
    connection_status = mqtt.connect(client_id);
  } else {
    connection_status = mqtt.connect(client_id, auth_username, auth_password);
  }

  //Subscribe to topics
  if(connection_status == 1){
    for(uint8_t i = 0; i < n_subscribed_topics; i++){
      Serial.println(subscribed_topics[i]);
      mqtt.subscribe(subscribed_topics[i]);
    }
  }

  return connection_status;
}

//Public
uint8_t mqtt_loop(){
  return mqtt.loop();
}

//Public
uint8_t mqtt_connected(){
  return mqtt.connected();
}

//Public
uint8_t mqtt_subscribe_to_topic(char *topic){
  if(n_subscribed_topics > 16) return 0;
  strncpy(subscribed_topics[n_subscribed_topics], topic, 32);
  n_subscribed_topics += 1;

  //If connected directly subscribe, else subscribe on connect
  if(mqtt.connected()){
    mqtt.subscribe(topic);
  }
}

//Public
uint8_t mqtt_publish_text_message(char *topic, char *message, uint8_t retained){
  return mqtt.publish(topic, message, retained);
}

//Public
uint8_t mqtt_publish_binary_data(char *topic, uint8_t* data, uint8_t len, uint8_t retained){
  return mqtt.publish(topic, (byte*)data, len, retained);
}

//Public
uint8_t mqtt_publish_binary_byte(char *topic, uint8_t data, uint8_t retained){
 return mqtt.publish(topic, (byte*)&data, 1, retained); 
}

//Private
void mqtt_callback(char *topic, byte *payload, unsigned int length){
  Serial.println("got message");
  //Handle MQTT commands here!
}
