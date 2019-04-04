#include <Arduino.h>
#include <stdint.h>

#define GPIO_BUTTON_ID_RESET 0
#define GPIO_BUTTON_ID_WPS 1


/**
 * @brief Init the GPIO module
 */
void GPIO_init();

/**
 * @brief Get the state of a certain button
 * 
 * @return Returns 1 if button is pressed 0 otherwise
 */
uint8_t GPIO_get_single_button_state_debounced(uint8_t button_id);
