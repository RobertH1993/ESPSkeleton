#include <Arduino.h>
#include <stdint.h>
/**
 * @brief Connect to the MQTT server
 * 
 * @param server A FQDN pointing to the server to connect to
 * 
 * @param port The port to connect to
 * 
 * @param client_id A name to identify ourself to the MQTT server, this name will be appended
 * with random characters
 */
void mqtt_init_client(char *server, uint16_t port = 1883, char *username = NULL, char *password = NULL);

/**
 * @brief Add a topic to subscribe to
 * 
 * @param topic The topic to subscribe to, max 31 characters long
 */
uint8_t mqtt_subscribe_to_topic(char *topic);

/*
 * @brief Publish a message to a certain topic
 */
uint8_t mqtt_publish_text_message(char *topic, char *message, uint8_t retained = 0);

/**
 * @brief Publish binary data to a certain topic
 */
uint8_t mqtt_publish_binary_data(char *topic, uint8_t* data, uint8_t len, uint8_t retained = 0);

/**
 * @biref Publish binary byte, handy for states
 */
uint8_t mqtt_publish_binary_byte(char *topic, uint8_t data, uint8_t retained = 0);

/**
 * @brief Connect the MQTT client to the server
 * 
 * @param username The username to authenticate with against the MQTT server
 * 
 * @param password The password to authenticate with against the MQTT server
 * 
 * @return Returns 1 if connected 0 otherwise
 * 
 * @warning Client must be initialized before calling this functions!
 */
uint8_t mqtt_connect();

/**
 * @brief This functions needs to be called to keep the mqtt client alive
 * 
 * @return Returns 1 if the client is still connected, 0 otherwise
 */
uint8_t mqtt_loop();

/**
 * @brief Check if the client is currently connected to the server
 * 
 * @return Returns 1 if connected 0 otherwise
 */
uint8_t mqtt_connected();
