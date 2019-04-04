#include <Arduino.h>
#include <stdint.h>

//Multiple defines, a configuration client has not yet been implemented so the opcodes use 'normal' characters

//WiFi setup command, used to configure a new WiFi SSID and password
//#define COMMAND_OPCODE_WIFI_SSID_SETUP 0x01
#define COMMAND_OPCODE_WIFI_SSID_SETUP 0x30
//#define COMMAND_OPCODE_WIFI_PASSWORD_SETUP 0x02
#define COMMAND_OPCODE_WIFI_PASSWORD_SETUP 0x31
//#define COMMAND_OPCODE_WIFI_INITIALIZE 0x03
#define COMMAND_OPCODE_WIFI_INITIALIZE 0x32


//#define COMMAND_OPCODE_WIFI_DHCP_OR_STATIC 0x04
#define COMMAND_OPCODE_WIFI_DHCP_OR_STATIC 0x33
//#define COMMAND_OPCODE_WIFI_STATIC_IP 0x05
#define COMMAND_OPCODE_WIFI_STATIC_IP 0x34
//#define COMMAND_OPCODE_WIFI_STATIC_GATEWAY 0x06
#define COMMAND_OPCODE_WIFI_STATIC_GATEWAY 0x35
//#define COMMAND_OPCODE_WIFI_STATIC_NETMASK 0x07
#define COMMAND_OPCODE_WIFI_STATIC_NETMASK 0x36
//#define COMMAND_OPCODE_WIFI_STATIC_DNS1 0x08
#define COMMAND_OPCODE_WIFI_STATIC_DNS1 0x37
//#define COMMAND_OPCODE_WIFI_STATIC_DNS2 0x09
#define COMMAND_OPCODE_WIFI_STATIC_DNS2 0x38
//#define COMMAND_OPCODE_MQTT_OR_TCP 0x10
#define COMMAND_OPCODE_MQTT_OR_TCP 0x39
//#define COMMAND_OPCODE_MQTT_HOST 0x11
#define COMMAND_OPCODE_MQTT_SERVER 0x40
//#define COMMAND_OPCODE_MQTT_PORT 0x12
#define COMMAND_OPCODE_MQTT_PORT 0x41
//#define COMMAND_OPCODE_MQTT_USERNAME 0x13
#define COMMAND_OPCODE_MQTT_USERNAME 0x42
//#define COMMAND_OPCODE_MQTT_PASSWORD 0x14
#define COMMAND_OPCODE_MQTT_PASSWORD 0x43
//#define COMMAND_OPCODE_MQTT_TOPIC 0x15
#define COMMAND_OPCODE_MQTT_TOPIC 0x44

/**
 * @brief Get the opcode from a command
 * 
 * @return Returns the opcode
 */
uint8_t command_get_opcode(char *command);

/**
 * @brief Get the data portion of the command
 * 
 * @return Returns pointer to the start of the data portion
 */
char* command_get_text_data(char *command);

/**
 * @brief Get a byte pointer to the data
 * 
 * @param offset The offset to the byte in the data portion
 */
uint8_t* command_get_byte_data(char *command, uint8_t offset = 0);

/**
 * @brief Get a single byte from data portion
 */
uint8_t command_get_single_byte_data(char *command, uint8_t offset = 0);

/**
 * @brief Get a uint16_t from the command buffer, this functions handles endianess
 */
uint16_t command_get_uint16_data(char *command, uint8_t offset = 0);
