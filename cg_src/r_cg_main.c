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
* Copyright (C) 2017, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2023/2/15
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
#include "r_cg_dac.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#define RECORD_COUNTDOWN_SEC                5
#define BATTERY_COUNTDOWN_SEC               10
#define DSADC_COUNTDOWN_SEC                 RECORD_COUNTDOWN_SEC
#define LORA_COUNTDOWN_SEC                  RECORD_COUNTDOWN_SEC
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint16_t record_rtc_counter = RECORD_COUNTDOWN_SEC;
uint16_t battery_rtc_counter = BATTERY_COUNTDOWN_SEC;
uint16_t dsadc_rtc_counter = DSADC_COUNTDOWN_SEC;
uint16_t lora_rtc_counter = LORA_COUNTDOWN_SEC;
uint8_t c = 0;
void processMode(void);
extern uint8_t mode=0;
extern uint8_t events=0;
extern void goToSleep(void);
extern float vbat;
struct battery_struct battery_data;
struct dsadc_struct dsadc_data;
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
    while (1)
    {
        if(events)
        {
            if(events & DSADC_NOTIFICATION_EVENT)
            {
                events &= ~DSADC_NOTIFICATION_EVENT;
                DSADC_PROCESS();
            }
            if(events & ADC10_NOTIFICATION_EVENT)
            {
                events &= ~ADC10_NOTIFICATION_EVENT;
                BATTERY_PROCESS();
            }
            if(events & RTC_NOTIFICATION_EVENT)
            {
                events &= ~RTC_NOTIFICATION_EVENT;
                if (battery_rtc_counter>=BATTERY_COUNTDOWN_SEC)
                {
                    battery_rtc_counter = 0;
                    battery_procedure_init(&battery_data);
                    R_IT8Bit0_Channel0_Start();
                }
                battery_rtc_counter++;
                if (dsadc_rtc_counter>=DSADC_COUNTDOWN_SEC)
                {
                    dsadc_rtc_counter = 0;
                    dsadc_procedure_init(&dsadc_data);
                    R_IT8Bit0_Channel0_Start();
                }
                dsadc_rtc_counter++;
            }
            if(events&TIMER_PERIODIC_EVENT)
            {
                events &= ~TIMER_PERIODIC_EVENT;           
                battery_procedure();
                dsadc_procedure();
            }  
        }
    }
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
    R_RTC_Start();
    R_TAU0_Channel6_Start();


    // R_DAC0_Start();
    // R_AMP0_Start();
    // R_AMP2_Start();
    // R_AMP_Set_PowerOn();
    // R_PGA_DSAD_Start();
    // while(1){

    // }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */
