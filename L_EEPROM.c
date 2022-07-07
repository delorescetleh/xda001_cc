#include "L_EEPROM.h"
// #include "r_cg_userdefine.h"

extern uint16_t Record_Data;
extern int eeprom=0;
extern uint8_t eeprom_process=EEPROM_PROCESS_START;
extern uint8_t eeprom_process_timeout_counter=0;

void L_EEPROM_procedure(void)
{
    eeprom_process_timeout_counter++;
    if (eeprom_process_timeout_counter > EEPROM_PROCESS_TIMEOUT)
    {
       eeprom_process = EEPROM_PROCESS_END;
    }
    switch (eeprom_process)
    {
        case EEPROM_PROCESS_START:
            doEepromWriteRecords(Record_Data);
            eeprom_process--;
            break;
        case EEPROM_PROCESS_END:
            L_EEPROM_STOP();
            eeprom_process_timeout_counter = 0;
            eeprom_process--;
        break;    
        default:
            eeprom_process--;
        break;
    }
}
