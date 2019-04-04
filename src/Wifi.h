#include <Arduino.h>
#include <stdint.h>

/**
 * @brief Set the device into AP mode
 * 
 * @return Returns 1 on success
 */
uint8_t wifi_init_ap_mode(char *ssid, char *password);

/**
 * @brief Get the number of connected WiFi-clients when in AP-MODE
 * 
 * @return Returns the number of connected devices
 */
uint8_t wifi_ap_mode_get_number_of_connected_clients();

/**
 * @brief Stops AP mode of the device
 * 
 * @return Returns 1 on success
 */
uint8_t wifi_stop_ap_mode();

/**
 * @brief Set static IP configuration for STA mode
 * 
 * @warning Should be called before wifi_connect_to_ap
 */
void wifi_set_static_configuration(uint8_t* ip_address, uint8_t* netmask, uint8_t* gateway, uint8_t* dns_server1, uint8_t* dns_server2); 

/**
 * @brief Starts connecting to an accesspoint in the background
 */
void wifi_connect_to_ap(char *ssid, char *password);

/**
 * @brief Check if we are connected to an AP
 * 
 * @return Returns 1 on connection to AP or 0 otherwise
 */
uint8_t wifi_is_connected_to_ap();

/**
 * @brief Will stop a connection between ESP and an AP
 */
void wifi_stop_sta_mode();

/**
 * @brief Control the wifi status led
 * 
 * @param state 1 means on 0 means off
 */
void wifi_set_status_led(uint8_t state);

/*
 * @brief Let the WiFi status led blink
 * 
 * @warning Must be called in a loop
 */
void wifi_blink_status_led(uint16_t delay_miliseconds);

/**
 * @brief Send a NTP request for the current Epoch time
 * 
 * @param ntp_server a byte array containing the 4 octets of the ip address
 */
void wifi_send_NTP_request(uint8_t* ntp_server);

/**
 * @brief Get a NTP response
 * 
 * @return Returns UTC epoch time or 0 on error/no packet
 */
uint32_t wifi_get_NTP_response();
