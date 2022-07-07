#include "L_Lora.h"
// #include "r_cg_userdefine.h"

extern uint8_t lora_process=LORA_PROCESS_START;
extern uint8_t lora_process_timeout_counter=0;
uint16_t Record_Data1 = 0;
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
        sendToLora[0] = '{';
        sendToLora[1] = (uint8_t)((Record_Data / 100) + 0x30);
        sendToLora[2] = (uint8_t)(((Record_Data % 100) / 10) + 0x30);
        sendToLora[3] = (uint8_t)((Record_Data % 10) + 0x30);
        sendToLora[4] =  0x30;
        sendToLora[5] =  0x30;
        sendToLora[6] =  0x30;
        sendToLora[7] = '}';
        doSendLoraData();
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
        lora_process_timeout_counter = 0;
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


