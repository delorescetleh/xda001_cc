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
* File Name    : r_cg_rtc.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
* Creation Date: 2023/1/22
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the real-time clock module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
    RTCWEN = 1U;    /* enables input clock supply */
    RTCE = 0U;      /* stops counter operation */
    RTCMK = 1U;     /* disable INTRTC interrupt */
    RTCIF = 0U;     /* clear INTRTC interrupt flag */
    /* Set INTRTC low priority */
    RTCPR1 = 1U;
    RTCPR0 = 1U;
    /* Set register RTCC0 */
    RTCC0 = _00_RTC_COUNTER_STOP | _00_RTC_RTC1HZ_DISABLE | _00_RTC_12HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1;
    /* Disenable input clock */
    RTCWEN = 0U;    /* stops input clock supply */
}
/***********************************************************************************************************************
* Function Name: R_RTC_Start
* Description  : This function enables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Start(void)
{
    volatile uint16_t w_count;

    RTCWEN = 1U;    /* enables input clock supply */
    RTCIF = 0U;     /* clear INTRTC interrupt flag */
    RTCMK = 0U;     /* enable INTRTC interrupt */
    RTCE = 1U;      /* starts counter operation */

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < RTC_WAITTIME_2FRTC; w_count++)
    {
        NOP();
    }

    RTCWEN = 0U;    /* stops input clock supply */
}
/***********************************************************************************************************************
* Function Name: R_RTC_Stop
* Description  : This function disables the real-time clock.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Stop(void)
{
    volatile uint16_t w_count;

    RTCWEN = 1U;    /* enables input clock supply */
    RTCE = 0U;      /* stops counter operation */
    RTCMK = 1U;     /* disable INTRTC interrupt */
    RTCIF = 0U;     /* clear INTRTC interrupt flag */

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < RTC_WAITTIME_2FRTC; w_count++)
    {
        NOP();
    }

    RTCWEN = 0U;    /* stops input clock supply */
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOn
* Description  : This function enables constant-period interrupt.
* Arguments    : period -
*                    the constant period of INTRTC
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_RTC_Set_ConstPeriodInterruptOn(rtc_int_period_t period)
{
    MD_STATUS status = MD_OK;

    if ((period < HALFSEC) || (period > ONEMONTH))
    {
        status = MD_ARGERROR;
    }
    else
    {
        RTCWEN = 1U;    /* enables input clock supply */
        RTCMK = 1U;     /* disable INTRTC interrupt */
        RTCC0 = (RTCC0 & (uint8_t)~_07_RTC_INTRTC_CLEAR) | period;
        RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;
        RTCIF = 0U;     /* clear INTRTC interrupt flag */
        RTCMK = 0U;     /* enable INTRTC interrupt */
        RTCWEN = 0U;    /* stops input clock supply */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_RTC_Set_ConstPeriodInterruptOff
* Description  : This function disables constant-period interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Set_ConstPeriodInterruptOff(void)
{
    RTCWEN = 1U;    /* enables input clock supply */
    RTCC0 &= (uint8_t)~(_07_RTC_INTRTC_CLEAR);
    RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;
    RTCIF = 0U;     /* clear INTRTC interrupt flag */
    RTCWEN = 0U;    /* stops input clock supply */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
