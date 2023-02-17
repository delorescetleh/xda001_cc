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
* File Name    : r_cg_tau.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 2023/2/17
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_tau.h"
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
* Function Name: R_TAU0_Create
* Description  : This function initializes the TAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Create(void)
{
    TAU0EN = 1U;    /* enables input clock supply */
    TPS0 = _0000_TAU0_CKM3_fCLK_8 | _0000_TAU0_CKM2_fCLK_1 | _0000_TAU0_CKM1_fCLK_0 | _0000_TAU0_CKM0_fCLK_0;
    /* Stop all channels */
    TT0 = _0800_TAU_CH3_H8_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | 
          _0040_TAU_CH6_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | 
          _0008_TAU_CH3_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | 
          _0001_TAU_CH0_STOP_TRG_ON;
    TMMK00 = 1U;    /* disable INTTM00 interrupt */
    TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    TMMK01 = 1U;    /* disable INTTM01 interrupt */
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
    TMMK01H = 1U;   /* disable INTTM01H interrupt */
    TMIF01H = 0U;   /* clear INTTM01H interrupt flag */
    TMMK02 = 1U;    /* disable INTTM02 interrupt */
    TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
    TMMK03 = 1U;    /* disable INTTM03 interrupt */
    TMIF03 = 0U;    /* clear INTTM03 interrupt flag */
    TMMK03H = 1U;   /* disable INTTM03H interrupt */
    TMIF03H = 0U;   /* clear INTTM03H interrupt flag */
    TMMK04 = 1U;    /* disable INTTM04 interrupt */
    TMIF04 = 0U;    /* clear INTTM04 interrupt flag */
    TMMK05 = 1U;    /* disable INTTM05 interrupt */
    TMIF05 = 0U;    /* clear INTTM05 interrupt flag */
    TMMK06 = 1U;    /* disable INTTM06 interrupt */
    TMIF06 = 0U;    /* clear INTTM06 interrupt flag */
    TMMK07 = 1U;    /* disable INTTM07 interrupt */
    TMIF07 = 0U;    /* clear INTTM07 interrupt flag */
    /* Set INTTM01 low priority */
    TMPR101 = 1U;
    TMPR001 = 1U;
    /* Channel 1 used as interval timer */
    TMR01 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_16BITS_MODE | 
            _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_TIMN_EDGE_FALLING | _0000_TAU_MODE_INTERVAL_TIMER | 
            _0001_TAU_START_INT_USED;
    TDR01 = _3E7F_TAU_TDR01_VALUE;
    TOM0 &= (uint16_t)~_0002_TAU_CH1_SLAVE_OUTPUT;
    TOL0 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TO0 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_VALUE_1;
    TOE0 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_ENABLE;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel1_Start
* Description  : This function starts TAU0 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel1_Start(void)
{
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
    TMMK01 = 0U;    /* enable INTTM01 interrupt */
    TS0 |= _0002_TAU_CH1_START_TRG_ON;
}
/***********************************************************************************************************************
* Function Name: R_TAU0_Channel1_Stop
* Description  : This function stops TAU0 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TAU0_Channel1_Stop(void)
{
    TT0 |= _0002_TAU_CH1_STOP_TRG_ON;
    TMMK01 = 1U;    /* disable INTTM01 interrupt */
    TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
