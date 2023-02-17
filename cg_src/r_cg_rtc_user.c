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
* File Name    : r_cg_rtc_user.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
* Creation Date: 2023/2/17
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_adc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_rtc_interrupt(vect=INTRTC)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint16_t rtc_counter = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_rtc_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_rtc_interrupt(void)
{
    if (1U == RIFG)
    {
        RTCWEN = 1U;
        RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;    /* clear RIFG */
        RTCWEN = 0U;
        r_rtc_callback_constperiod();
    }
}
/***********************************************************************************************************************
* Function Name: r_rtc_callback_constperiod
* Description  : This function is real-time clock constant-period interrupt service handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_rtc_callback_constperiod(void)
{
    /* Start user code. Do not edit comment generated here */
    events |= RTC_NOTIFICATION_EVENT;

    // if (!rtc_counter)
    // {
    //     R_IT8Bit0_Channel0_Start();
    //     switch (mode)
    //     {
    //     case factory_mode:
    //         factory_mode_init_setting();
    //         break;        
    //     case normal_mode:
    //         normal_mode_init_setting();
    //         break;
    //     case factory_test_mode:
    //         factory_test_mode_init_setting();
    //         break;
    //     }
    // }
    // rtc_counter--;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

// void factory_test_mode_init_setting(void){
//     rtc_counter = RTC_TIME_SPEED / 10;
//     L_EEPROM_procedure_init();
//     L_PCB_TEMP_procedure_init();
//     L_PT100_Procedure_init();
//     if (countDownLoRaCounter()==0)
//     {
//         L_Lora_procedure_init();
//     }
// }
// void factory_mode_init_setting(void){
//     // rtc_counter = RTC_TIME_SPEED / 3;
// }
// void normal_mode_init_setting(void){
//     rtc_counter = RTC_TIME_SPEED;
//     L_EEPROM_procedure_init();
//     L_PCB_TEMP_procedure_init();
//     L_PT100_Procedure_init();
//     if (countDownLoRaCounter()==0)
//     {
//         L_Lora_procedure_init();
//     }
// }

/* End user code. Do not edit comment generated here */
