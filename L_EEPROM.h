#include "r_cg_userdefine.h"
#define EEPROM_PROCESS_START 2
#define EEPROM_PROCESS_END 1
#define EEPROM_PROCESS_TIMEOUT 10

extern uint8_t eeprom_process;
void L_EEPROM_procedure(void);
void L_EEPROM_procedure_init(void);