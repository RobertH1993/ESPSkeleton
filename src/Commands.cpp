#include "Commands.h"
#include "Config.h"

#define CPU_IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

//Public
uint8_t command_get_opcode(char *command){
  return command[0];
}

//Public
char* command_get_text_data(char *command){
  return &command[1];
}

//Public
uint8_t* command_get_byte_data(char *command, uint8_t offset){
  return ((uint8_t*)&command[offset + 1]);
}

//Public
uint8_t command_get_single_byte_data(char *command, uint8_t offset){
  return command[offset + 1];
}

//Public
uint16_t command_get_uint16_data(char *command, uint8_t offset){
  uint8_t data[2];
  memcpy(data, &command[offset + 1], 2);
  uint16_t *ret_value = (uint16_t*)data;

  if(!CPU_IS_BIG_ENDIAN){
    uint8_t tmp = data[0];
    data[0] = data[1];
    data[1] = tmp;
  }

  return *ret_value;
}
