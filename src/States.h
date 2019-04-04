#ifndef __STATES_H
#define __STATES_H

#include <Arduino.h>
#include <stdint.h>

//States (Global)
#define STATE_SETUP 0
#define STATE_CONNECTING 1
#define STATE_IDLE 2

//Events and there variables
#define EVENT_SWITCH_TO_CONNECTING 1
#define EVENT_SWITCH_TO_SETUP 2
#define EVENT_SWITCH_TO_IDLE 3


struct StateEvent {
  uint8_t event;
  uint8_t variable;
};

/**
 * @brief switch from the current state to a new state, and run the exit function of current state
 * 
 * @warning Entry function of new state will be ran when state_run_current is runned
 */
void state_switch(uint8_t new_state, uint8_t variable);

/**
 * @brief Run the current state
 * 
 * @return Returns an event ID
 */
struct StateEvent state_run_current();

#endif
