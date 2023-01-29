#include "L_factory.h"
extern uint8_t low_power;
extern uint8_t F_Done = 0;
extern uint8_t LORA_F_Done = 0;
extern uint8_t EEPROM_F_Done = 0;
extern uint8_t PT100_F_Done = 0;
extern uint8_t PCB_TEMPERATURE_F_Done = 0;
extern uint8_t BLE_F_Done = 0;
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
        UART1_TXD_MODE = PIN_MODE_AS_INPUT;
    BLE_RESET = PIN_LEVEL_AS_LOW;
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
   
    // L_BLE_INIT();

    // R_DTCD10_Start();
    R_UART1_Create();
    R_UART1_Stop();
    BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
    UART1_TXD_MODE = PIN_MODE_AS_INPUT;
    BLE_RESET = PIN_LEVEL_AS_LOW;
    BLE_POW_CNT = POWER_ON;
    delayInMs(2);
    BLE_RESET = PIN_LEVEL_AS_HIGH;
    delayInMs(1000);
    L_BLE_RESET_TO_FACTORY_SETTING();
    while (1)
    {
        clrBleBuffer();
        delayInMs(1000);
        BLE_RESET = PIN_LEVEL_AS_LOW;
        delayInMs(2);
        BLE_RESET = PIN_LEVEL_AS_HIGH;
	    delayInMs(1000);
        // L_BLE_FACTORY_MODE_SETTING();
    }
    dataFlashStart();
    dataFlashRead((pfdl_u08 *)&board,0);
    dataFlashEnd();

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

    L_EEPROM_procedure_init();
    L_PCB_TEMP_procedure_init();
    // L_BLE_F_procedure_init();
    L_Lora_procedure_init();
    L_PT100_Calibration_Procedure_init();
    R_IT8Bit0_Channel0_Start();
    while (1)
    {
        if(low_power){
            L_LORA_STOP();
            LORA_RESET = PIN_LEVEL_AS_LOW;

            BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
            BLE_RESET = PIN_LEVEL_AS_LOW;
            BLE_POW_CNT = POWER_OFF;

            R_PGA_DSAD_Stop();
            L_EEPROM_STOP();
            P_STATUS = PIN_LEVEL_AS_HIGH;
            break;
        }

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
                            if (PT100_F_Done)
                            {
                                 BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
                                BLE_F_Done = 1;
                                if(0)//(L_BLE_INIT())
				{
                                    // R_DTCD10_Stop();
                                    if (L_BLE_FACTORY_MODE_SETTING()){
                                        BLE_F_Done = 1;
                                        board[HARDWARE] = 0B00001111;
                                    }
                                }else{
                                    board[HARDWARE] = 0B00001110;
				    BLE_F_Done = 0;
                                    R_DTCD10_Stop();
                                    R_UART1_Stop();
                                    // while(1){
                                    BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
                                    BLE_RESET = PIN_LEVEL_AS_LOW;
                                    delayInMs(2);
                                    BLE_POW_CNT = POWER_OFF;
                                }
                                delayInMs(1000);
                                board[TESTED] |= F_DSADC_READY;
                                R_IT8Bit0_Channel0_Stop();
                                R_RTC_Stop();
                                board[LORA_INTV] = 0x03;
                                
                                P_STATUS = 0;
                                DataFlashWrite();
                                P_STATUS = 1;
                                delayInMs(1000);
                                F_Done = 1;
                            }
                    }
                }
            }
        }else{
            P_STATUS = !P_STATUS;
            if (BLE_F_Done){
                delayInMs(1000);
            }else{
                delayInMs(10000);
            }

        }

        if(events)
        {
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if ((pt100_calibration_process)&&(!PT100_F_Done))
                {
                    L_PT100_Calibration_Procedure();
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
                P_STATUS = 0;
            }
        }
    }
}
