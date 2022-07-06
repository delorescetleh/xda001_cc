#include "L_Lora.h"
#include "r_cg_userdefine.h"

extern uint8_t lora_process=LORA_PROCESS_START;
extern uint8_t lora_process_timeout_counter=0;
uint16_t Record_Data1 = 0;
uint16_t convertTemperatureFormat(int dsadc_temperature);
void L_Lora_procedure(void)
{
    lora_process_timeout_counter++;
    if (lora_process_timeout_counter > LORA_PROCESS_TIMEOUT)
    {
       lora_process = LORA_PROCESS_END;
    }
    switch (lora_process)
    {
    case 12:
        L_LORA_INIT();
        lora_process--;
        break;
    case 11:
        if (checkLoraMessage())
        {
            LORA_READY = PIN_LEVEL_AS_LOW;
            lora_process--;
        }
        break;
    case 10:
        LORA_READY = PIN_LEVEL_AS_LOW;    
        lora_process--;
    case 9:
        LORA_READY = PIN_LEVEL_AS_LOW;
        doSendLoraData(convertTemperatureFormat(dsadc_temperature), (uint16_t)pcbTemperature);
        lora_process--;
        break;
    case 8:
        if (LORA_STA) // LORA_STA Turn High means Lora got
        {
            lora_process--;
            // if(mode==factory_mode){
            //     LORA_F_Done = 1;
            // }
        }
        break;
    case LORA_PROCESS_END:
        L_LORA_STOP();
        loraProcessTimeOutCounter = 0;
        lora_process--;
        break;  
    default:
        if (lora_process)
        {
            lora_process--;
        }
    break;
    }
}

uint16_t convertTemperatureFormat(int dsadc_temperature)
{
    return (dsadc_temperature/5+100);
}
