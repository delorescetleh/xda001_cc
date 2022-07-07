#include "r_cg_userdefine.h"
#define EEPROM_PROCESS_START 2
#define EEPROM_PROCESS_END 1
#define EEPROM_PROCESS_TIMEOUT 10

extern uint8_t eeprom_process;
extern uint8_t eeprom_process_timeout_counter;
extern int eeprom;
extern void L_EEPROM_procedure(void);