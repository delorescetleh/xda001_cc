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
* File Name    : r_cg_adc.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 2023/2/18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
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
* Function Name: R_ADC_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Create(void)
{
    volatile uint16_t w_count;

    ADCEN = 1U;     /* enables input clock supply */
    ADM0 = 0x00U;  /* disable AD conversion and clear ADM0 register */
    ADMK = 1U;      /* disable INTAD interrupt */
    ADIF = 0U;      /* clear INTAD interrupt flag */
    /* Set INTAD low priority */
    ADPR1 = 1U;
    ADPR0 = 1U;
    /* Set ANI10 pin */
    PMC0 |= 0x20U;
    PM0 |= 0x20U;
    /* Set ADC registers */
    ADM0 = _08_AD_CONVERSION_CLOCK_32 | _04_AD_TIME_MODE_LOWVOLTAGE_1;
    ADM1 = _80_AD_TRIGGER_HARDWARE_NOWAIT | _20_AD_CONVMODE_ONESELECT | _03_AD_TRIGGER_INTIT;
    ADM2 = _80_AD_POSITIVE_INTERVOLT | _00_AD_AREA_MODE_1 | _01_AD_RESOLUTION_8BIT;
    ADUL = _FF_AD_ADUL_VALUE;
    ADLL = _00_AD_ADLL_VALUE;
    ADS = _0A_AD_INPUT_CHANNEL_10;

    /* Change the waitting time according to the system */
    for (w_count = 0U; w_count < AD_WAITTIME; w_count++)
    {
        NOP();  
    }

    ADCE = 1U;      /* enables A/D voltage comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Start
* Description  : This function starts the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Start(void)
{
    ADIF = 0U;      /* clear INTAD interrupt flag */
    ADMK = 0U;      /* enable INTAD interrupt */
    ADCS = 1U;      /* enables conversion operation */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Stop(void)
{
    ADCS = 0U;      /* stops conversion operation */
    ADMK = 1U;      /* disable INTAD interrupt */
    ADIF = 0U;      /* clear INTAD interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Set_OperationOn
* Description  : This function enables comparator operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Set_OperationOn(void)
{
    ADCE = 1U;      /* enables A/D voltage comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Set_OperationOff
* Description  : This function stops comparator operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Set_OperationOff(void)
{
    ADCE = 0U;      /* stops A/D voltage comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Get_Result_8bit
* Description  : This function returns the higher 8 bits conversion result.
* Arguments    : buffer -
*                    the address where to write the conversion result
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Get_Result_8bit(uint8_t * const buffer)
{
    *buffer = ADCRH;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
