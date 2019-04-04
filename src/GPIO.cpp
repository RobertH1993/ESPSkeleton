#include "GPIO.h"

#define DEBOUNCE_TIME 300

//Struct to define input
struct InputData {
  uint8_t pin;
  uint8_t last_state;
  uint64_t last_check_time;
};

//All defined inputs
struct InputData inputs[] = {
  { //GPIO_BUTTON_ID_RESET
    .pin = 13,
    .last_state = 0,
    .last_check_time = 0
  }
};

void GPIO_init(){
  for(uint8_t i = 0; i < 1; i++){
    pinMode(inputs[i].pin, INPUT);
  }
}

uint8_t GPIO_get_single_button_state_debounced(uint8_t button_id){
  uint8_t current_state = digitalRead(inputs[button_id].pin);
  if(inputs[button_id].last_check_time == 0){
    inputs[button_id].last_check_time = millis();
    inputs[button_id].last_state = current_state;
  }else if((millis() - inputs[button_id].last_check_time) > DEBOUNCE_TIME){
    inputs[button_id].last_check_time = 0;
    if(inputs[button_id].last_state == current_state) return current_state;
  }
  return 0;
}
