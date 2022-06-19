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
* Creation Date: 2022/6/19
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_tau.h"
#include "r_cg_it8bit.h"
#include "r_cg_rtc.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_amp.h"
#include "r_cg_adc.h"
#include "r_cg_sau.h"
#include "r_cg_iica.h"
#include "r_cg_dtc.h"
#include "r_cg_elc.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#pragma address (EVENTS=0xFE900U)
#pragma address (dataFlash =0xF1F00U)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile unsigned char EVENTS;
volatile unsigned char dataFlash;
mode_t Mode = NORMAL_MODE;
uint8_t rtc_counter = 0;
int16_t pcbTemperature=250;
int PT100result;
uint8_t loraProcessIntervalByMinutes = 1;
uint8_t dsadc_ready = 0;
uint8_t lora_data_ready = 0;
uint8_t analogProcess = 0;
uint8_t loraProcess = 0;
uint8_t analogProcessTimeOutCounter = 0;
uint8_t loraProcessTimeOutCounter = 0;
uint8_t countToEnableLoraProcess = 0;
uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t *hardWareSetting=0;
uint8_t *factorySetting=0;
uint8_t dubReadBuffer[10]={0};
uint8_t dubWriteBuffer[10]= {0x37, 0x10, 0x01, 0x02, 0x03, 0x04, 0x01, 0x20, 0x11, 0xFF};
void process(mode_t Mode);
void normal_process(void);
void factory_process(void);
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
    LORA_POW_CNT = POWER_ON;
    BLE_POW_CNT = POWER_OFF;
    EPROM_POW_CNT = POWER_OFF;
    delayInMs(2000);
    Mode = FACTORY_MODE;
    Mode = NORMAL_MODE;
    getFactroySetting(hardWareSetting, factorySetting, dubReadBuffer);
    process(Mode);
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
void process(mode_t Mode){
    if (Mode==FACTORY_MODE){
        factory_process();
    } else{
        normal_process();
    }
}

void factory_process(void){
    if (L_BLE_INIT())
    {
        data[9] = L_BLE_FACTORY_MODE_SETTING();
   }
}
void normal_process(void){

    L_BLE_INIT();
    L_BLE_STOP();
    R_INTC1_Start();
    // R_DTCD10_Start();
    EVENTS = 0; 
    EVENTS = RTC_NOTIFICATION_EVENT; // start when power on
    R_RTC_Start();
    delayInMs(500);
    while (1)
    {
                //         if (LORA_STA){
                //     data[7] = 0x11;
                // } else {
                //     data[7] = 0xCC;
                // }
        if(EVENTS){
            if (EVENTS&RTC_NOTIFICATION_EVENT)
            {
                EVENTS &= (~RTC_NOTIFICATION_EVENT);
                init_pcb_temperature(); // set parameter for dtc0,dtc1 , this parameter could automatically fetch pcb temperature without MCU controller
                init_dsadc(&dsadc_ready);
                R_IT8Bit0_Channel0_Start();
                R_IT8Bit0_Channel1_Start();
                dsadc_ready = 0;
                analogProcess = 1;
                analogProcessTimeOutCounter = 0;
                R_DTCD0_Start();
                R_PGA_DSAD_Create();
                R_PGA_DSAD_Start();
                L_EEPROM_INIT();
                countToEnableLoraProcess++; 
                if (countToEnableLoraProcess==loraProcessIntervalByMinutes){
                    loraProcess = 6;
                    loraProcessTimeOutCounter = 0;
                    LORA_INIT();
                }
            }

            if (EVENTS & TIMER_PERIODIC_EVENT)
            {
                EVENTS &= ~TIMER_PERIODIC_EVENT;
                if (analogProcess)
                {
                    analogProcessTimeOutCounter++;
                    if (analogProcessTimeOutCounter>10){
                        dsadc_ready = 1;
                        PT100result = -500; // means record value will become 0, send to Lora "000" mean ERR
                    }

                    if (dsadc_ready)
                    {
                        analogProcess = 0;
                        analogProcessTimeOutCounter = 0;
                        get_pt100_result(&PT100result);
                        R_DTCD0_Stop();
                        R_ADC_Stop();
                        R_PGA_DSAD_Stop();
                        R_IT8Bit0_Channel1_Stop();
                        PT100result= PT100result/5 + 100; // Record Temperature as 0~999 (as -50degC to 450 degC)
                        if (PT100result>=1000){
                            PT100result = 0; // means record value will become 0, send to Lora "000" mean ERR
                        }
                        doEepromWriteRecords((uint16_t)PT100result);
                        L_EEPROM_STOP();
                    }
                    get_pcb_temperature(&pcbTemperature);
                }
                if (!BLE_NO_CONNECT)
                {
                    checkAppCommand();
                }
                if (loraProcess){
                    // loraProcessTimeOutCounter++;
                    // if (loraProcessTimeOutCounter>20){
                    //     loraProcess = 0;
                    //     loraProcessTimeOutCounter = 0;
                    //     L_LORA_STOP();
                    // }
                    switch (loraProcess)
                    {
                    case 6:
                        LORA_READY = PIN_LEVEL_AS_LOW;
                        // LORA_STA should be high
                        loraProcess--;
                        break;
                    case 5:
                        LORA_READY = PIN_LEVEL_AS_HIGH;
                        // LORA_STA should be high
                        loraProcess--;
                        break;
                    case 4:
                        LORA_READY = PIN_LEVEL_AS_LOW;
                        // LORA_STA should be high
                        loraProcess--;
                        break;
                    case 3:
                        if (!LORA_STA)// wait till LORA_STA turn to low
                        {
                            LORA_READY = PIN_LEVEL_AS_LOW;
                            loraProcess--;
                        }
                        break;
                    case 2:
                        if (!LORA_STA)// LORA_STA should be low
                        {
                            // do send lora message;
                            loraProcess--;
                        }
                        break;
                    case 1:;
                        if (LORA_STA)// Wait till LORA_STA turn to high
                        {
                            // stop lora process
                            L_LORA_STOP();
                            loraProcess--;
                        }
                        break;
                    default:
                        break;
                    }
                }

                if ((!analogProcess)&BLE_NO_CONNECT&(!loraProcess))
                {
                    R_IT8Bit0_Channel0_Stop();
                    set_TXD0_as_Input_Mode();
                    set_TXD1_as_Input_Mode();
                    L_BLE_STOP();
                    R_IICA0_Stop();
                    R_DTCD0_Stop();
                    R_ADC_Stop();
                    L_PGA_STOP();
                    if (P_TEST)
                    {
                        HALT();
                    }
                    else
                    {
                        STOP();
                    }
                }
            }
        }
    }
}
/* End user code. Do not edit comment generated here */
