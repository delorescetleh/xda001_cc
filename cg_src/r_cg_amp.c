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
* File Name    : r_cg_amp.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for AMP module.
* Creation Date: 2023/2/17
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_amp.h"
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
* Function Name: R_AMP_Create
* Description  : This function initializes the comparator module..
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP_Create(void)
{
    AFEEN = 1U;     /* enables input clock supply */
    AFEPON = 1U;    /* power on AFE */

    while (0U == AFESTAT)
    {
        ;/* Wait until AFE stabilize */
    }
    
    AMPEN = 1U;     /* enables input clock supply */
    AMPC = 0U; /* stop all AMP units */
    AMPPON = 0U;    /* power-off (default) */
    AMPMC = _80_AMP_CH2_3_HIGHSPEED | _20_AMP_CH1_HIGHSPEED;
    AMPTRS = _03_AMP_ELC_TRIGGER_SOURCE_2;
    AMPTRM = _00_AMP2_TRIGGER_SOFTWARE | _00_AMP0_TRIGGER_SOFTWARE;
    AMP0S = _40_AMP0_AMP0N_INPUT4 | _10_AMP0_DAC0;
    AMP2S = _80_AMP2_AMP2_FED | _01_AMP2_AMP2P_INPUT6;
}
/***********************************************************************************************************************
* Function Name: R_AMP0_Start
* Description  : This function starts the AMP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP0_Start(void)
{
    AMPE1 = 1U;     /* enables comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_AMP0_Stop
* Description  : This function stops the AMP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP0_Stop(void)
{
    AMPE1 = 0U;     /* disable comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_AMP2_Start
* Description  : This function starts the AMP2.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP2_Start(void)
{
    AMPE3 = 1U;     /* enables comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_AMP2_Stop
* Description  : This function stops the AMP2.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP2_Stop(void)
{
    AMPE3 = 0U;     /* disable comparator operation */
}
/***********************************************************************************************************************
* Function Name: R_AMP_Set_PowerOn
* Description  : This function starts the clock supplied for AMP.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_AMP_Set_PowerOn(void)
{
    AMPPON = 1U;    /* power-on */
}

/* Start user code for adding. Do not edit comment generated here */
void R_AMP_Set_PowerOff(void)
{
    AMPPON = 0U;    /* power-on */
}
/* End user code. Do not edit comment generated here */
