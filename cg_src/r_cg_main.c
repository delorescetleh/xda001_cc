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
* Creation Date: 2023/2/24
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
#define RECORD_COUNTDOWN_SEC                30
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

uint8_t lora_countdown_sec =LORA_COUNTDOWN_SEC;

uint16_t record_rtc_counter = RECORD_COUNTDOWN_SEC;
uint16_t battery_rtc_counter = BATTERY_COUNTDOWN_SEC;
uint16_t dsadc_rtc_counter = DSADC_COUNTDOWN_SEC;
uint16_t lora_rtc_counter = LORA_COUNTDOWN_SEC;
uint8_t c = 0;
extern uint8_t mode=0;
extern uint8_t events=0;
extern void goToSleep(void);
// struct battery_struct battery_data;
// struct dsadc_struct dsadc_data;
uint8_t main_process_timer_working=0;
uint8_t ble_process_cycle_counter=10;
uint8_t semaphore=0;
enum PT100_DATA_FETCH_RESULT_TYPE pt100_data_fetch_result_type=PT100_SUCCESS;
void MAIN_PROCESS_TIMER_START(void);
void MAIN_PROCESS_TIMER_STOP(void);
extern float vbat;
void battery_test_loop(void);
void ble_test_loop(void);
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
                if (lora_process_timeout_counter)
                {
                    lora_process_timeout_counter--;
                }
                if (lora_process_timer_counter)
                {
                    lora_process_timeout_counter--;
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
                H1D_LED = !H1D_LED;
                battery_procedure();
                dsadc_procedure();
                lora_procedure();
                ble_procedure();
            }
        }
	else{
	    if((battery_process==BATTERY_PROCESS_END)&&(dsadc_process==DSADC_PROCESS_END)&&(lora_process==LORA_PROCESS_END))
            {
                H1D_LED = PIN_LEVEL_AS_LOW;
                MAIN_PROCESS_TIMER_STOP();
                goToSleep();
                //HALT();
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
    L_LORA_STOP();
    L_PT100_STOP();
    L_BAT_STOP();
    EI();
    R_RTC_Start();
    //goToSleep();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void battery_test_loop(void){
R_ADC_Create();

R_ADC_Set_OperationOn();
R_ADC_Start();
while(1)
{
// R_ADC_Start();
// delayInMs(10);
}
}
void ble_test_loop(void){
    while (1)
    {
        R_UART1_Receive(receivedFromBle, 4);
         delayInMs(1000);
        if (ble_received_end)
        {
        //     //
            ble_received_end = 0;
            //     //R_DTCD10_Start();
            //     // memset(receivedFromBle, 0, 255);
            //     //memclr(receivedFromBle ,255);
           
            // R_UART1_Send((uint8_t *)"AT+BINREQACK\r", 14);
            R_UART1_Send(receivedFromBle, 4);
        //     //break;
        }else{
            R_UART1_Send("nop\r", 4);
        }
    }
    while (1)
    {
        delayInMs(1);
    }
}
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
        H1D_LED = PIN_LEVEL_AS_LOW;
        BUZ0_MODE=PIN_MODE_AS_INPUT;
        BAT_ADC_ON=PIN_LEVEL_AS_HIGH;
        UART1_TXD_MODE = PIN_MODE_AS_INPUT;
        BLE_CTS_MODE = PIN_MODE_AS_INPUT;
        LORA_POW_CNT_MODE=PIN_MODE_AS_INPUT; 
        BLE_POW_CNT_MODE = PIN_MODE_AS_INPUT;
	    // STOP();
        HALT();
}
/* End user code. Do not edit comment generated here */
