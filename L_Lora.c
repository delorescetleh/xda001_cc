#include "L_Lora.h"
// #include "r_cg_userdefine.h"

extern uint8_t lora_process=LORA_PROCESS_START;
extern uint8_t lora_process_timeout_counter=LORA_PROCESS_TIMEOUT;
uint16_t Record_Data1 = 0;
void prepareDataToLora(void);
void L_Lora_procedure(void)
{
    lora_process_timeout_counter--;
    if (!lora_process_timeout_counter)
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
        prepareDataToLora();
        lora_process--;
    case 9:
        LORA_READY = PIN_LEVEL_AS_LOW;
        doSendLoraData();
        lora_process--;
        break;
    case 8:
        if (LORA_STA) // LORA_STA Turn High means Lora got
        {
            lora_process--;
        }
        break;
    case LORA_PROCESS_END:
        if (mode == factory_mode)
        {
                if(lora_process_timeout_counter)
                {
                    LORA_F_Done = 1;
                }
                else
                {
                    LORA_F_Done = 0;
                }
        }
        L_LORA_STOP();
        lora_process_timeout_counter=LORA_PROCESS_TIMEOUT;
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


void prepareDataToLora(void)
{
    switch (mode)
    {
        case factory_mode:
            sendToLora[0] = '{';
            sendToLora[1] = (uint8_t)((Record_Data / 100) + 0x30);
            sendToLora[2] = (uint8_t)(((Record_Data % 100) / 10) + 0x30);
            sendToLora[3] = (uint8_t)((Record_Data % 10) + 0x30);
            sendToLora[4] = 0x30;
            sendToLora[5] = 0x31;
            sendToLora[6] = 0x32;
            sendToLora[7] = '}';
            break;
        case normal_mode:;
            sendToLora[0] = '{';
            sendToLora[1] = (uint8_t)((Record_Data / 100) + 0x30);
            sendToLora[2] = (uint8_t)(((Record_Data % 100) / 10) + 0x30);
            sendToLora[3] = (uint8_t)((Record_Data % 10) + 0x30);
            sendToLora[4] = 0x30;
            sendToLora[5] = 0x30;
            sendToLora[6] = 0x30;
            sendToLora[7] = '}';
            break;
        case factory_test_mode:
            sendToLora[0] = '{';
            sendToLora[1] = (uint8_t)(pcb_temperature / 120);
            sendToLora[2] = (uint8_t)(pcb_temperature % 120);
            sendToLora[3] = 'F';
            sendToLora[4] = (uint8_t)( Vm0 / 14400);
            sendToLora[5] = (uint8_t)((Vm0 % 14400)/120);
            sendToLora[6] = (uint8_t)((Vm0 % 14400)%120);
            sendToLora[7] = (uint8_t)(Vm1 / 14400);
            sendToLora[8] = (uint8_t)((Vm1 % 14400)/120);
            sendToLora[9] = (uint8_t)((Vm1 % 14400)%120);
            sendToLora[10] = (uint8_t)( Vm2 / 14400);
            sendToLora[11] = (uint8_t)((Vm2 % 14400)/120);
            sendToLora[12] = (uint8_t)((Vm2 % 14400)%120);
            sendToLora[13] = (uint8_t)(Vm3 / 14400);
            sendToLora[14] = (uint8_t)((Vm3 % 14400)/120);
            sendToLora[15] = (uint8_t)((Vm3 % 14400)%120);
            sendToLora[16] = '}';
            break;
        default:
            break;
    }
}