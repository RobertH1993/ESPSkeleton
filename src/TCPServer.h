#include <Arduino.h>
#include <stdint.h>

#define TCP_SERVER_STATE_ACK 1
#define TCP_SERVER_STATE_NACK 0

/**
 * @brief Start the TCP server
 */
void tcp_server_start();

/**
 * @brief Stop the TCP server
 */
void tcp_server_stop();

/**
 * @brief Get a command from the connected TCP client
 * 
 * @param out Buffer to store the command received from the client int
 * 
 * @return Returns the length of the command in number of bytes, 0 meaning no command was received
 */
uint8_t tcp_server_get_client_command(char *out);

/**
 * @brief Send data to the currently connected client
 */
void tcp_server_send_data_to_connected_client(uint8_t opcode, uint8_t state, uint8_t* data, uint8_t len);
