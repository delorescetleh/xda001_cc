#include "L_factory.h"
// #include "L_PT100.h"
// #include "L_PCB_TEMP.h"

extern uint8_t F_Done = 0;
extern uint8_t LORA_F_Done = 0;
extern uint8_t EEPROM_F_Done = 0;
extern uint8_t PT100_F_Done = 0;
extern uint8_t PCB_TEMPERATURE_F_Done = 0;
extern uint8_t BLE_F_Done = 0;


extern uint8_t pcb_temperature_process;
extern uint8_t pt100_process;
void prepareDataToLora_factory_test(void);
void lora_programming_process(void){
    R_INTC1_Stop();
    L_BLE_STOP();
    R_ADC_Stop();
    R_PGA_DSAD_Stop();
    R_IICA0_Stop();
    R_ADC_Set_OperationOff();
    R_DTCD10_Stop();
    R_IT8Bit0_Channel0_Stop();
    R_RTC_Stop();
    LORA_READY_MODE = PIN_MODE_AS_INPUT;
    UART0_TXD_MODE = PIN_MODE_AS_INPUT;
    LORA_POW_CNT = PIN_LEVEL_AS_LOW;
    delayInMs(10);
    LORA_RESET = PIN_LEVEL_AS_HIGH;
    while(1){
         goToSleep();
    }
}

void factory_test_process(void){
    board[LORA_INTV] = 1;
    L_BLE_STOP();
    R_RTC_Start();
    R_IT8Bit0_Channel0_Start();
    delayInMs(10);
    while (1)
    {
        if (events)
        {
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if (pt100_process)
                {
                    L_PT100_Procedure();
                }
                if (pcb_temperature_process)
                {
                    L_PCB_TEMP_procedure();
                }
                if (lora_process)
                {
                    L_Lora_procedure();
                }
                if (eeprom_process)
                {
                    L_EEPROM_procedure();
                }
            }
        }
        if ((!pt100_process) & (!pcb_temperature_process) & (!lora_process)& (!eeprom_process))
        {
            goToSleep();
        }
    }
}

void prepareDataToLora_factory_test(void)
{
    sendToLora[0] = '{';
    sendToLora[1] = (uint8_t)((Record_Data / 100) + 0x30);
    sendToLora[2] = (uint8_t)(((Record_Data % 100) / 10) + 0x30);
    sendToLora[3] = (uint8_t)((Record_Data % 10) + 0x30);
    sendToLora[4] = 0x30;
    sendToLora[5] = 0x30;
    sendToLora[6] = 0x30;
    sendToLora[7] = '}';
}
void factory_process(void){
    board[DSADC_TEMPERATURE_SENSOR_OFFSET] = 0;
    board[TESTED] = 0;
    board[HARDWARE] = 0;
    board[LORA_INTV] = 1;
    R_RTC_Start();
    while (1)
    {
        if (!F_Done)
        {
            if (PCB_TEMPERATURE_F_Done)
            {
                board[TESTED] |= F_ADC_READY;
                if (EEPROM_F_Done)
                {
                    board[TESTED] |= F_EEPROM_READY;
                    if (LORA_F_Done)
                    {
                        board[TESTED] |= F_LORA_READY;
                        if (BLE_F_Done)
                        {
                            board[TESTED] |= F_BLE_READY;
                            if (PT100_F_Done)
                            {
                                board[TESTED] |= F_DSADC_READY;
                                R_IT8Bit0_Channel0_Stop();
                                R_RTC_Stop();
                                board[LORA_INTV] = 0x03;
                                board[HARDWARE] = 0B00001111;
                                DataFlashWrite();
                                F_Done = 1;
                                P_STATUS = 1;
                                while (1)
                                {
                                }
                            }
                        }
                    }
                }
            }
        }

        if(events)
        {
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if ((pt100_process)&&(!PT100_F_Done))
                {
                    L_PT100_Procedure();
                }
                if ((pcb_temperature_process)&&(!PCB_TEMPERATURE_F_Done))
                {
                    L_PCB_TEMP_procedure();
                }
                if ((lora_process)&&(!LORA_F_Done))
                {
                    L_Lora_procedure();
                }
                if ((eeprom_process)&&(!EEPROM_F_Done))
                {
                    L_EEPROM_procedure();
                }
                if ((ble_process) && (!BLE_F_Done))
                {
                    L_BLE_F_procedure();
                }
                P_STATUS = !P_STATUS;
            }
        }
    }
}
