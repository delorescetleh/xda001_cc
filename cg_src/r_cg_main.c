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
* Creation Date: 2023/2/17
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
#include "r_cg_sau.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
#define RECORD_COUNTDOWN_SEC                10
#define BATTERY_COUNTDOWN_SEC               RECORD_COUNTDOWN_SEC
#define DSADC_COUNTDOWN_SEC                 RECORD_COUNTDOWN_SEC
#define LORA_COUNTDOWN_SEC                  RECORD_COUNTDOWN_SEC

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
typedef struct main_data_struct
{
    struct battery_struct battery_data;
    struct dsadc_struct dsadc_data;
} main_data_t;
main_data_t main_data;

uint16_t record_rtc_counter = RECORD_COUNTDOWN_SEC;
uint16_t battery_rtc_counter = BATTERY_COUNTDOWN_SEC;
uint16_t dsadc_rtc_counter = DSADC_COUNTDOWN_SEC;
uint16_t lora_rtc_counter = LORA_COUNTDOWN_SEC-5;
uint8_t c = 0;
extern uint8_t mode=0;
extern uint8_t events=0;
extern void goToSleep(void);
// struct battery_struct battery_data;
// struct dsadc_struct dsadc_data;
uint8_t main_process_timer_working=0;
uint8_t lora_process_rtc_timer_counter=LORA_SLEEP_WAIT_TIME;
enum PT100_DATA_FETCH_RESULT_TYPE pt100_data_fetch_result_type=PT100_SUCCESS;
void MAIN_PROCESS_TIMER_START(void);
void MAIN_PROCESS_TIMER_STOP(void);
extern float vbat;
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
            if(events & UART0_NOTIFICATION_EVENT)
            {
                events &= ~UART0_NOTIFICATION_EVENT;
                LORA_PROCESS();
            }
            if(events & UART1_NOTIFICATION_EVENT)
            {
                events &= ~UART1_NOTIFICATION_EVENT;
            }         
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
                if (lora_process_rtc_timer_counter)
                {
                    lora_process_rtc_timer_counter--;
                }
                if ((battery_rtc_counter>=BATTERY_COUNTDOWN_SEC)&&(lora_process!=LORA_PROCESS_END))
                {
                    battery_rtc_counter = 0;
                    battery_procedure_init(&main_data.battery_data);
                    MAIN_PROCESS_TIMER_START();
                }
                battery_rtc_counter++;
                if (dsadc_rtc_counter>=DSADC_COUNTDOWN_SEC)
                {
                    dsadc_rtc_counter = 0;
                    dsadc_procedure_init(&main_data.dsadc_data);
                    MAIN_PROCESS_TIMER_START();
                }
                dsadc_rtc_counter++;
                if (lora_rtc_counter>=LORA_COUNTDOWN_SEC)
                {
                    lora_rtc_counter = 0;
                    lora_procedure_init(&main_data.battery_data.Vbat,&main_data.dsadc_data.pt100_temperature);
                    MAIN_PROCESS_TIMER_START();
                }
                lora_rtc_counter++;
            }
            if(events&TIMER_PERIODIC_EVENT)
            {
                events &= ~TIMER_PERIODIC_EVENT;           
                battery_procedure();
                dsadc_procedure();
                lora_procedure();
            }
        }
        if((battery_process==BATTERY_PROCESS_END)&&(dsadc_process==DSADC_PROCESS_END)&&(lora_process==LORA_PROCESS_END))
        {
            MAIN_PROCESS_TIMER_STOP();
            goToSleep();
        }else{
            //HALT();
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
    L_LORA_STOP();
    L_PT100_STOP();
    L_BAT_STOP();
    EI();
    R_RTC_Start();

    
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void MAIN_PROCESS_TIMER_START(void){
    if(!main_process_timer_working){
        main_process_timer_working = 1;
        R_IT8Bit0_Channel0_Start();
    }
}
void MAIN_PROCESS_TIMER_STOP(void)
{
    R_IT8Bit0_Channel0_Stop();
    main_process_timer_working = 0;
}
void goToSleep(void)
{
        R_ADC_Stop();
        R_DAC0_Stop();
        R_AMP0_Stop();
        R_AMP2_Stop();
        R_INTC0_Stop();
        R_AMP_Set_PowerOff();
        R_IT8Bit0_Channel0_Stop();
        R_TAU0_Channel1_Stop();
        R_TAU0_Channel6_Stop();
        R_UART1_Stop();
        R_UART0_Stop();
        R_INTC0_Stop();
        
        //BUZ0 = !BUZ0;
         BUZ0_MODE=PIN_MODE_AS_INPUT;
        BAT_ADC_ON_MODE=PIN_MODE_AS_INPUT;
        UART1_TXD_MODE = PIN_MODE_AS_INPUT;
        BLE_CTS_MODE = PIN_MODE_AS_INPUT;
        LORA_POW_CNT_MODE=PIN_MODE_AS_INPUT; 
        BLE_POW_CNT_MODE = PIN_MODE_AS_INPUT;
	STOP();
        //HALT();
}
/* End user code. Do not edit comment generated here */
