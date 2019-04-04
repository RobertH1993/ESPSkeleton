#include <Arduino.h>
#include <stdint.h>

struct ConfigData{
  uint8_t wifi_config_initialized;
  char wifi_ssid[32];
  char wifi_password[32];

  uint8_t wifi_DHCP_or_static;
  uint8_t wifi_static_ip[4];
  uint8_t wifi_static_gateway[4];
  uint8_t wifi_static_netmask[4];
  uint8_t wifi_static_dns1[4];
  uint8_t wifi_static_dns2[4];

  uint8_t mqtt_or_tcp;
  char mqtt_server[64];
  uint16_t mqtt_port;
  char mqtt_username[16];
  char mqtt_password[16];
  char mqtt_topic[32];
};

/**
 * @brief Save the current config contained in the struct to EEPROM.
 */
void storage_save_current_config();

/**
 * @brief Load the config from EEPROM
 */
void storage_load_config_from_eeprom();

/**
 * @brief Get handle to the in memory config
 */
struct ConfigData* storage_get_config();

/**
 * @brief Reset full storage area to zero's
 */
void storage_wipe_data();
