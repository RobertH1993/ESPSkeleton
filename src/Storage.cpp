#include "Storage.h"
#include <EEPROM.h>

struct ConfigData data = {};


//Public
void storage_save_current_config(){
  EEPROM.begin(sizeof(struct ConfigData));
  EEPROM.put(0, data);
  EEPROM.end();
}

void storage_load_config_from_eeprom(){
  EEPROM.begin(sizeof(struct ConfigData));
  EEPROM.get(0, data);
  EEPROM.end();
}

struct ConfigData* storage_get_config(){
  return &data;
}

//Public
void storage_wipe_data(){
  EEPROM.begin(sizeof(struct ConfigData));
  for(uint16_t i = 0; i < sizeof(struct ConfigData); i++){
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
}
