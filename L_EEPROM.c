#include "L_EEPROM.h"
extern uint16_t Record_Data=0;
extern int eeprom=0;
extern uint8_t eeprom_process=0;
extern uint8_t eeprom_process_timeout_counter=EEPROM_PROCESS_TIMEOUT;


void L_EEPROM_procedure_init(void){
    eeprom_process = EEPROM_PROCESS_START;
    eeprom_process_timeout_counter = EEPROM_PROCESS_TIMEOUT;
}

void L_EEPROM_procedure(void)
{
    eeprom_process_timeout_counter--;
    if (!eeprom_process_timeout_counter)
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
            if (mode == factory_mode)
            {
                if(eeprom_process_timeout_counter)
                {
                    EEPROM_F_Done = 1;
                }
                else
                {
                    EEPROM_F_Done = 0;
                }
            }
            L_EEPROM_STOP();
            eeprom_process_timeout_counter = 0;
            eeprom_process--;
        break;    
        default:
            eeprom_process--;
        break;
    }
}
