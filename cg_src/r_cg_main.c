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
* Creation Date: 2022/6/15
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_tau.h"
#include "r_cg_rtc.h"
#include "r_cg_it.h"
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
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile unsigned char EVENTS;
mode_t Mode = NORMAL_MODE;
uint8_t rtc_counter = 0;
uint16_t timer_100ms_counter = 0;
int16_t pcbTemperature;
int PT100result;
uint8_t dsadc_ready = 0;
uint8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
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

    // Mode = FACTORY_MODE;
    Mode = NORMAL_MODE;

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
    init_dsadc(&dsadc_ready);
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
    init_pcb_temperature(); // set parameter for dtc0,dtc1 , this parameter could automatically fetch pcb temperature without MCU controller
    delayInMs(1000);
    L_BLE_INIT();
    R_RTC_Start();
    EVENTS = 0;
    EVENTS |= (RTC_NOTIFICATION_EVENT); // start when power on
    while (1)
    {
        if(EVENTS){
            if (EVENTS&RTC_NOTIFICATION_EVENT)
            {
                EVENTS &= (~RTC_NOTIFICATION_EVENT);
		        EVENTS |= (PT100_NOTIFICATION_EVENT);
                timer_100ms_counter = 0;
                R_IT_Start();
                dsadc_ready = 0;
                R_DTCD0_Start();
                R_PGA_DSAD_Start();
            }
            if (EVENTS&PT100_NOTIFICATION_EVENT)
            {
                if (dsadc_ready){
                    EVENTS &= (~PT100_NOTIFICATION_EVENT);
                    dsadc_ready = 0;
                    get_pt100_result(&PT100result);
                }
            }
            if (EVENTS & TIMER_PERIODIC_EVENT)
            {
                EVENTS &= ~TIMER_PERIODIC_EVENT;
		        timer_100ms_counter++;

                if (timer_100ms_counter == 5)
                { // 0.5s , finish pcb temperature fetch
                    get_pcb_temperature(&pcbTemperature);
                    R_DTCD0_Stop();
                    R_ADC_Stop();
                    R_ADC_Set_OperationOff();
                }
                if (!BLE_NO_CONNECT)
                {
                    // timer_100ms_counter--;
                    checkAppCommand();
                }else{
                    if (timer_100ms_counter > 100)
                    { // 10s time out to turn off analog parts
                        events &= ~PCB_TEMPERATURE_NOTIFICATION_EVENT;
                        events &= ~PT100_NOTIFICATION_EVENT;
                        timer_100ms_counter = 0;
                        R_IT_Stop();
                    }
                }
            }
            //HALT();
        }else{ // if no events go to sleep
            // set_TXD0_as_Input_Mode();
            // set_TXD1_as_Input_Mode();
            // L_BLE_STOP();
            // R_DTCD10_Stop();
            // R_IICA0_Stop();
            // R_DTCD0_Stop();
            // R_ADC_Stop();
            // R_ADC_Set_OperationOff();
            // L_PGA_STOP();
            // if (P_TEST){
            //     HALT();
            // }else{
            //     STOP();
            // }
        }
    }
}
/* End user code. Do not edit comment generated here */
