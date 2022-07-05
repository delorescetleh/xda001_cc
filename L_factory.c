#include "L_factory.h"


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
    // L_BLE_STOP();
    // R_RTC_Start();
    // R_IT8Bit0_Channel0_Start();
    // delayInMs(10);
    // while (1)
    // {
    //     if (events)
    //     {
    //         if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
    //         {
    //             events &= ~TIMER_PERIODIC_EVENT;
    //             if (dsadcProcess)
    //             {
    //                 PT100_procedure();
    //             }
    //             if (adcProcess)
    //             {
    //                 PCB_TEMP_procedure();
    //             }
    //             if (loraProcess)
    //             {
    //                 if (!dsadcProcess)
    //                 {
    //                     LoRa_procedure();
    //                 }
    //             }
    //         }
    //     }
    //     if ((!dsadcProcess) & (!loraProcess) & (!adcProcess))
    //     {
    //         goToSleep();
    //     }
    // }
}


void factory_process(void){
    // board[F_DSADC_TEMPERATURE_SENSOR_OFFSET] = 0;
    // board[TESTED] = 0;
    // board[HARDWARE] = 0;
    // board[F_LORA_INTV] = 1;
    // DSADC_temperature_calibration_process = 2;
    // resetLoRaCounter(board[F_LORA_INTV]);
    // // resetDSADC(0);
    // R_RTC_Start();
    // while (1)
    // {
    //     if (!F_Done)
    //     {
    //         if (ADC_F_Done)
    //         {
    //             board[TESTED] |= F_ADC_READY;
    //             if (LORA_F_Done)
    //             {
    //                 board[TESTED] |= F_LORA_READY;
    //                 if (BLE_F_Done)
    //                 {
    //                     board[TESTED] |= F_BLE_READY;
    //                     if (DSADC_F_Done)
    //                     {
    //                         board[TESTED] |= F_DSADC_READY;
    //                         if (!dsadcProcess)
    //                         {
    //                             R_IT8Bit0_Channel0_Stop();
    //                             R_RTC_Stop();
    //                             board[F_LORA_INTV] = 0x03;
    //                             board[HARDWARE] = 0B00001111;
    //                             DataFlashWrite();
    //                             F_Done = 1;
    //                             // P_STATUS = 1;
    //                             while (1)
    //                             {
    //                             }
                                
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     if(events)
    //     {
    //         if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
    //         {
    //             events &= ~TIMER_PERIODIC_EVENT;
    //             if (dsadcProcess)
    //             {
    //                 F_PT100_procedure();
    //             }
    //             if (adcProcess)
    //             {
    //                 PCB_TEMP_procedure();
    //             }
    //             if (loraProcess)
    //             {
    //                 if ((!adcProcess)&(!dsadcProcess))
    //                 {
    //                     LoRa_procedure();
    //                 }
    //             }
    //             if (bleProcess)
    //             {
    //                 if(!BLE_F_Done){
    //                     F_BLE_procedure();
    //                 }
    //             }
    //             if (bleShutDownProcess)
    //             {
    //                 BLE_ShutDown_procedure();
    //             }
    //             P_STATUS = !P_STATUS;
    //         }
    //     }
    // }
}
