/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017, 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2022/7/8
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_it8bit.h"
#include "r_cg_rtc.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_amp.h"
#include "r_cg_adc.h"
#include "r_cg_sau.h"
#include "r_cg_iica.h"
#include "r_cg_dtc.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#pragma address (dataFlash =0xF1F00U)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
// volatile unsigned char EVENTS;
volatile unsigned char dataFlash;

int different;
int DSADC_Temperature;
int Record_Temperature;
uint8_t lora_data_ready = 0;
uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void turnOffAll(void);
void processMode(void);
// void PCB_TEMP_procedure(void);
// void F_PT100_procedure(void);
// void PT100_procedure(void);
// void LoRa_procedure(void);
// void BLE_procedure(void);
// void BLE_ShutDown_procedure(void);
// void USER_Calibrartion_DSADC_procedure(void);

// void DataFlashWrite(void);


// uint8_t DSADC_temperature_calibration_process = 2;

extern uint8_t board[10] = {0};
extern int16_t temperatureCalibrationOffset[3] = {0};
extern uint8_t mode=0;
extern uint16_t Record_Data=0;
extern int16_t user_Temperature=0;
// extern int16_t pcbTemperature=250;

// extern float Ipt100=0.001543572;
// extern uint32_t guessIpt100=150000;
// extern uint32_t K=1;
// extern uint8_t analogProcessDone = 0;
// extern uint8_t dsadcProcessTimeOutCounter = 0;
// extern uint8_t adcProcessTimeOutCounter = 0;
// extern uint8_t loraProcessTimeOutCounter = 0;
// extern uint8_t lora_start_time_delay_count = 0;
// extern uint8_t bleShutDownProcess = 0;
// extern uint8_t bleProcess=0;
// extern uint8_t adcProcess=10;
// extern uint8_t dsadcProcess=15;
// extern uint8_t loraProcess = 0;
extern uint8_t events=0;
extern uint8_t USER_DSADC_temperature_calibration_process = 0;
extern uint8_t countToEnableLoraProcess = 0;
extern void goToSleep(void);
/* End user code. Do not edit comment generated here */

static void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    LORA_RESET = PIN_LEVEL_AS_LOW;
    BLE_RESET = PIN_LEVEL_AS_LOW;
    LORA_POW_CNT = POWER_OFF; /* LORA_RESET Should be output mode and set to low make sure lora module no working when system start*/ 
    BLE_POW_CNT = POWER_OFF; /* Take Max 300mA */ 
    EPROM_POW_CNT = POWER_OFF;/* Take Max 30mA */

    delayInMs(1000);
    if(IS_LORA_PROGRAMMING)
    {
        mode = lora_programming_mode;
    }else{
        if(P_TEST)
        {
            mode = factory_mode;
        }
        else
        {
            mode = normal_mode;
        }
    } 
    mode = factory_test_mode;
    // mode =  lora_programming_mode;
    mode = factory_mode;
    mode = normal_mode;

    processMode();
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */


void goToSleep(void){
    R_IT8Bit0_Channel0_Stop();
    if(P_TEST)
    {
        HALT();
    }
    else
    {
        STOP();
    }
}


void BLE_ShutDown_procedure(void)
{
    // switch (bleShutDownProcess)
    // {
    // case 200:
    //     R_INTC1_Stop();
    //     L_BLE_STOP();
    //     bleShutDownProcess--;
    //     break;
    // case 190:
    //     BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
    //     BLE_RESET = PIN_LEVEL_AS_LOW;
    //     BLE_POW_CNT = POWER_OFF;
    //     delayInMs(2);
    //     BLE_RESET_MODE = PIN_MODE_AS_INPUT;
    //     BLE_UART_RXD_IND_MODE = PIN_MODE_AS_INPUT;
    //     delayInMs(30);
    //     bleShutDown = 1;
    //     bleShutDownProcess=1;
    //     break;
    
    // default:
    //     if(bleShutDownProcess){
    //     bleShutDownProcess--;
    //     }

    //     break;
    // }
}


void processMode(void)
{
    switch (mode)
    {
    case normal_mode:
        normal_process();
        break;
    case factory_mode:
        factory_process();
        break;
    case lora_programming_mode:
        lora_programming_process();
        break;
    case factory_test_mode:
        factory_test_process();
        break;                    
    default:
        normal_process();
        break;
    }
}

extern void DataFlashWrite(void){
    dataFlashStart();
    dataFlashWrite((pfdl_u08 *)&board[0], 0);
    dataFlashEnd();
}
extern void setLoraIntervalTime(uint8_t lora_intv){
    board[F_LORA_INTV] = lora_intv;
    resetLoRaCounter(lora_intv);
    DataFlashWrite();
}
extern void USER_Calibrartion_DSADC_procedure(void){
    switch (USER_DSADC_temperature_calibration_process)
    {
    case 10:
        if(!pt100_process)
        {
            board[F_DSADC_TEMPERATURE_SENSOR_OFFSET] = user_Temperature - DSADC_Temperature;
            USER_DSADC_temperature_calibration_process--;
        }
        break;
    case 9:
        DataFlashWrite();
        USER_DSADC_temperature_calibration_process = 0;
        break;
    case 1:
        /* code */
        break;
    default:
        break;
    }
}
/* End user code. Do not edit comment generated here */
