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
* Creation Date: 2022/6/29
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_it8bit.h"
#include "r_cg_rtc.h"
#include "r_cg_it.h"
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
board_t board;
mode_t mode;
int PT100_Temperature;
uint16_t Record_Data;
uint8_t lora_data_ready = 0;
uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t bleShutDown = 0;


void factory_test_process(void);
void lora_programming_process(void);
void normal_process(void);
void factory_process(void);

void goToSleep(void);
void turnOffAll(void);

void processMode(mode_t mode);

void PCB_TEMP_procedure(void);
void PT100_procedure(void);
void LoRa_procedure(void);
void BLE_procedure(void);
void BLE_ShutDown_procedure(void);
void DataFlashWrite(void);

uint8_t F_Done = 0;
uint8_t LORA_F_Done = 0;
uint8_t EEPROM_F_Done = 0;
uint8_t DSADC_F_Done = 0;
uint8_t ADC_F_Done = 0;
extern uint8_t BLE_F_Done = 0;
uint8_t DSADC_temperature_calibration_process = 2;

extern int16_t temperatureCalibrationOffset[3] = {0};


extern int16_t pcbTemperature=250;
extern uint32_t Rpt100 = 0;
extern float Ipt100=0.001543572;
extern uint32_t guessIpt100=150000;
extern uint32_t K=1;
extern uint8_t analogProcessDone = 0;
extern uint8_t dsadcProcessTimeOutCounter = 0;
extern uint8_t adcProcessTimeOutCounter = 0;
extern uint8_t loraProcessTimeOutCounter = 0;
// extern uint8_t lora_start_time_delay_count = 0;
extern uint8_t bleShutDownProcess = 0;

extern uint8_t bleProcess=0;
extern uint8_t adcProcess=10;
extern uint8_t dsadcProcess=15;
extern uint8_t loraProcess = 0;
extern uint8_t events=0;
extern uint8_t dsadc_ready=0;
// extern uint8_t *loraProcessIntervalTime;

extern uint8_t countToEnableLoraProcess = 0;
extern uint8_t dubReadBuffer[10]={0};
extern uint8_t dubWriteBuffer[10]= {0x37, 0x10, 0x01, 0x02, 0x03, 0x04, 0x01, 0x20, 0x11, 0xFF};
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

    
    if(IS_LORA_PROGRAMMING)
    {
        mode = lora_programming_mode;
    }else if(IN_FACTORY)
    {
        mode = factory_mode;
    }else{
        mode = normal_mode;
    }
    // mode = factory_test_mode;
    // mode = factory_mode;

    processMode(mode);
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
void factory_test_process(void){
    L_BLE_STOP();
    R_RTC_Start();
    R_IT8Bit0_Channel0_Start();//400mS
    delayInMs(10);
    while (1)
    {
        if (events)
        {
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if (dsadcProcess)
                {
                    PT100_procedure();
                }
                if (adcProcess)
                {
                    PCB_TEMP_procedure();
                }
                if (loraProcess)
                {
                    if (!dsadcProcess)
                    {
                        LoRa_procedure();
                    }
                }
            }
        }
        if ((!dsadcProcess) & (!loraProcess) & (!adcProcess))
        {
            goToSleep();
        }
    }
}

void lora_programming_process(void){
    R_INTC1_Stop();
    L_BLE_STOP();
    R_ADC_Stop();
    R_PGA_DSAD_Stop();
    R_IICA0_Stop();
    R_ADC_Set_OperationOff();
    // R_DTCD0_Stop();
    R_DTCD10_Stop();
    // R_DTCD8_Stop();
    R_IT8Bit0_Channel0_Stop();
    R_IT8Bit0_Channel1_Stop();
    R_IT_Stop();
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

void normal_process(void){
    
    dataFlashStart();
    dataFlashRead((pfdl_u08 *)&board,0);
    dataFlashEnd();
    resetLoRaCounter(board.F_LORA_INTV);
    DSADC_temperature_calibration_process = 0;
    
    L_BLE_INIT();
    L_BLE_STOP();
    R_INTC1_Start();
    L_LORA_STOP();
    R_RTC_Start();
    while (1)
    {
        if(events)
        {
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 200mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if (dsadcProcess)
                {
                    //dsadcProcess = 0;
                    PT100_procedure();
                }
                if (adcProcess)
                {
                    //  adcProcess = 0;
                    PCB_TEMP_procedure();
                }
                if (loraProcess)
                {
                    // analogProcessDone = 0;
                    if ((!adcProcess)&(!dsadcProcess))
                    {
                        LoRa_procedure();
                    }
                }
                if (bleProcess)
                {
                   BLE_procedure();
                }
                if (bleShutDownProcess)
                {
                    BLE_ShutDown_procedure();
                }
            }
        }
        if ((BLE_NO_CONNECT)||(bleShutDown))
        {
            if ((!dsadcProcess) & (!loraProcess) & (!adcProcess)& (!bleShutDownProcess))
            {
                goToSleep();
            }
        }
    }
}

void goToSleep(void){
    R_IT8Bit0_Channel0_Stop();
    if(1)// (P_TEST)
    {
        HALT();
    }
    else
    {
        STOP();
    }
}

void PCB_TEMP_procedure(void)
{
    switch (adcProcess)
    {
    case 10:
        init_pcb_temperature();
        R_ADC_Create();
        adcProcess--;
        break;
    case 9:
        R_ADC_Start();
        adcProcess--;
        break;
    case 8:
        get_pcb_temperature(&pcbTemperature);
        adcProcess--;
        break;    
    case 7:
        get_pcb_temperature(&pcbTemperature);
        adcProcess--;
        break;
    case 6:
        get_pcb_temperature(&pcbTemperature);
        adcProcess=1;
        break;
    case 1:
        get_pcb_temperature(&pcbTemperature);
        R_ADC_Stop();
        ADCEN = 0U;
        if (factory_mode){
            if((pcbTemperature>100)&(pcbTemperature<400)){
                ADC_F_Done=1;
            }
        }
        adcProcess--;
        break;    
    default:
        adcProcess--;
        break;
    }
}

void PT100_procedure(void){
    dsadcProcessTimeOutCounter++;
    if (dsadcProcessTimeOutCounter > 20)
    {
        dsadcProcess = 12;
    }
    switch (dsadcProcess)
    {
    case 15:
        analogProcessDone = 0;
        clr_dsadc_buf();
        L_EEPROM_INIT();
        R_PGA_DSAD_Create();
        dsadcProcess--;
        break;
    case 14:
        R_PGA_DSAD_Start();
        dsadcProcess--;
        break;
    case 13:
        if ((dsadc_ready)&(!adcProcess))
        {
            dsadc_ready = 0;
            get_pt100_result(&PT100_Temperature, &board.F_DSADC_TEMPERATURE_SENSOR_OFFSET);
            dsadcProcess--;
        }
        break;
    case 12:
        if (DSADC_temperature_calibration_process)
            {
                temperatureCalibrationOffset[DSADC_temperature_calibration_process] = pcbTemperature - PT100_Temperature;
                if (DSADC_temperature_calibration_process == 1)
                {
                    board.F_DSADC_TEMPERATURE_SENSOR_OFFSET = (temperatureCalibrationOffset[1]*80 + temperatureCalibrationOffset[2]*20) / 100;
                    DSADC_F_Done = 1;
                }
                DSADC_temperature_calibration_process--;
                dsadcProcess = 10;
            }else{
            L_PGA_STOP();
            Record_Data = (uint16_t)(PT100_Temperature / 5 + 100); // Record Temperature as 0~999 (as -50degC to 450 degC)
            if (Record_Data >= 1000)
            {
                Record_Data = 0; // means record value will become 0, send to Lora "000" mean ERR
            }
            dsadcProcess--;
        }
        break;
    case 11:
        EEPROM_F_Done=doEepromWriteRecords((uint16_t)Record_Data);
        dsadcProcess--;
        break;
    case 10:
        L_EEPROM_STOP();
        dsadcProcessTimeOutCounter = 0;
        dsadcProcess=1;
        break;
    case 1:
        dsadcProcessTimeOutCounter = 0;
        dsadcProcess--;
        break;
    default:
        if (dsadcProcess)
        dsadcProcess--;
        break;
    }
}

void LoRa_procedure(void){
    loraProcessTimeOutCounter++;
    if (loraProcessTimeOutCounter > 15)
    {
        loraProcess = 1;
    }
    switch (loraProcess)
    {
    case 9:
        L_LORA_INIT();
        loraProcess--;
        break;
    case 8:
        if (checkLoraMessage())
        {
            LORA_READY = PIN_LEVEL_AS_LOW;
            loraProcess--;
        }
        break;
    case 7:
        LORA_READY = PIN_LEVEL_AS_LOW;    
        loraProcess--;
    case 6:
        LORA_READY = PIN_LEVEL_AS_LOW;
        doSendLoraData(Record_Data, (uint16_t)pcbTemperature);
        loraProcess--;
        break;
    case 5:
        if (LORA_STA) // LORA_STA Turn High means Lora got
        {
            loraProcess--;
            if(factory_mode){
                LORA_F_Done = 1;
            }
        }
        break;
    // case 4: // stop lora process
    //     loraProcess=1;
    //     break;
    case 1:
        L_LORA_STOP();
        analogProcessDone = 0;
        loraProcessTimeOutCounter = 0;
        loraProcess--;
        break;
    default:
        if (loraProcess)
        {
            loraProcess--;
        }
        break;
    }
}

void BLE_procedure(void)
{
    switch (bleProcess)
    {
    case 5:
        R_DTCD10_Stop();
        bleProcess--;
        break;
    case 4:
        checkAppCommand();
        if (!bleShutDownProcess){
            if(BLE_NO_CONNECT){
                bleProcess=1;
            }else{
                bleProcess = 10;
                R_DTCD10_Start();
            }
        }else{
            bleProcess=1;
        }
        break;
    case 1:
        L_BLE_STOP();
        bleProcess--;
        break;
    default:
    if(bleProcess)
        bleProcess--;
        break;
    }
}

void BLE_ShutDown_procedure(void)
{

    switch (bleShutDownProcess)
    {
    case 200:
        R_INTC1_Stop();
        L_BLE_STOP();
        bleShutDownProcess--;
        break;
    case 190:
        BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
        BLE_RESET = PIN_LEVEL_AS_LOW;
        BLE_POW_CNT = POWER_OFF;
        delayInMs(2);
        BLE_RESET_MODE = PIN_MODE_AS_INPUT;
        BLE_UART_RXD_IND_MODE = PIN_MODE_AS_INPUT;
        delayInMs(30);
        bleShutDown = 1;
        bleShutDownProcess=1;
        break;
    
    default:
        if(bleShutDownProcess){
        bleShutDownProcess--;
        }

        break;
    }
}
void factory_process(void){
    board.F_DSADC_TEMPERATURE_SENSOR_OFFSET = 0;
    board.TESTED = 0;
    board.HARDWARE = 0;
    board.F_LORA_INTV = 1;
    DSADC_temperature_calibration_process = 2;
    resetLoRaCounter(board.F_LORA_INTV);
    R_RTC_Start();
    while (1)
    {
        if (!F_Done)
        {
            if (ADC_F_Done)
            {
                board.TESTED |= F_ADC_READY;
                if (LORA_F_Done)
                {
                    board.TESTED |= F_LORA_READY;
                    if (BLE_F_Done)
                    {
                        board.TESTED |= F_BLE_READY;
                        if (DSADC_F_Done)
                        {
                            board.TESTED |= F_DSADC_READY;
                            if (!dsadcProcess)
                            {
                                R_IT8Bit0_Channel0_Stop();
                                R_RTC_Stop();
                                board.F_LORA_INTV = 3;
                                board.HARDWARE = 0B00001111;
                                DataFlashWrite();
                                F_Done = 1;
                                P_STATUS = 1;
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
                if (dsadcProcess)
                {
                    PT100_procedure();
                }
                if (adcProcess)
                {
                    PCB_TEMP_procedure();
                }
                if (loraProcess)
                {
                    if ((!adcProcess)&(!dsadcProcess))
                    {
                        LoRa_procedure();
                    }
                }
                if (bleProcess)
                {
                    if(!BLE_F_Done){
                        F_BLE_procedure();
                    }
                }
                if (bleShutDownProcess)
                {
                    BLE_ShutDown_procedure();
                }
                P_STATUS = !P_STATUS;
            }
        }
    }
}

void processMode(mode_t mode)
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

void DataFlashWrite(void){
    dataFlashStart();
    dataFlashWrite((pfdl_u08 *)&board, 0);
    dataFlashEnd();
}
extern void setLoraIntervalTime(uint8_t lora_intv){
    board.F_LORA_INTV = lora_intv;
    resetLoRaCounter(lora_intv);
    DataFlashWrite();
}
/* End user code. Do not edit comment generated here */
