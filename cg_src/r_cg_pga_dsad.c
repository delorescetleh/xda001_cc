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
* File Name    : r_cg_pga_dsad.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2023/1/23
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_pga_dsad.h"
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
* Function Name: R_PGA_DSAD_Create
* Description  : This function initializes the PGA_DSAD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Create(void)
{
    AFEEN = 1U;     /* enables input clock supply */
    AFEPON = 1U;    /* power on AFE */

    while (0U == AFESTAT) 
    {
        ;/* Wait until AFE stabilize  */
    }

    PGAEN = 1U;     /* enables input clock supply */
    DSADMR = _00_PGA_DSAD_MODE_NORMAL;
    AFECKS = _0B_AFE_CLOCK_DIVIDED4;
    VSBIAS = _13_VSBIAS_OUTPUT_VOLTAGE_15;
    PGAPON = 1U;    /* power on PGA and DS A/D */

    while (0U == PGASTAT) 
    {
        ;/* Wait until PGA stabilize  */
    }

    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADSMK = 1U;   /* disable INTDSADS interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    /* Set INTDSAD low priority */
    DSADPR1 = 1U;
    DSADPR0 = 1U;
    /* Set INTDSADS low priority */
    DSADSPR1 = 1U;
    DSADSPR0 = 1U;
    DSADMR |= _00_PGA_DSAD_TRIGGER_SOFTWARE;
    DSADCTL = _00_DSAD_CONVERSION_STOP | _00_DSAD_STABILIZATION_TIME_3T | _00_DSAD_SCAN_MODE_CONTINUOUS | 
              _00_DSAD_INPUT3_ENABLE | _00_DSAD_INPUT2_ENABLE | _02_DSAD_INPUT1_DISABLE | _00_DSAD_INPUT0_ENABLE;
    PGA0CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _10_PGA_MULTIPLEXERn_GSET01_8 | _03_PGA_MULTIPLEXERn_GSET02_8;
    PGA2CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _00_PGA_MULTIPLEXERn_GSET01_1 | _00_PGA_MULTIPLEXERn_GSET02_1;
    PGA3CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _0C_PGA_MULTIPLEXERn_GSET01_4 | _02_PGA_MULTIPLEXERn_GSET02_4;
    PGA0CTL1 = _00_PGA_MULTIPLEXER0_DIFFERENTIAL | _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA2CTL1 = _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA3CTL1 = _40_PGA_MULTIPLEXER3_EXTERNAL_DIFFERENTIAL | _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA0CTL2 = _01_PGA_MULTIPLEXER0_SAMPLE_COUNT;
    PGA2CTL2 = _01_PGA_MULTIPLEXER2_SAMPLE_COUNT;
    PGA3CTL2 = _01_PGA_MULTIPLEXER3_SAMPLE_COUNT;
    PGA0CTL3 = _80_PGA_MULTIPLEXERn_MUMBER_MODE_255 | _0C_PGA_MULTIPLEXERn_AVG_MEAM_VALUE | 
               _03_PGA_MULTIPLEXERn_AVG_UNIT_64;
    PGA2CTL3 = _00_PGA_MULTIPLEXERn_MUMBER_MODE_8032 | _08_PGA_MULTIPLEXERn_AVG_CONVERSION_OCCURS | 
               _02_PGA_MULTIPLEXERn_AVG_UNIT_32;
    PGA3CTL3 = _80_PGA_MULTIPLEXERn_MUMBER_MODE_255 | _0C_PGA_MULTIPLEXERn_AVG_MEAM_VALUE | 
               _03_PGA_MULTIPLEXERn_AVG_UNIT_64;
    PGABOD = _00_DSAD_DISCONNECTION_DETECTION_UNUSED;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Start
* Description  : This function starts PGA_DSAD module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Start(void)
{
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADMK = 0U;    /* enable INTDSAD interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    DSADSMK = 0U;   /* enable INTDSADS interrupt */
    DSADST = 1U;    /* start A/D conversion */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Stop
* Description  : This function stops PGA_DSAD module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Stop(void)
{
    DSADST = 0U;    /* stop A/D conversion */
    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADSMK = 1U;   /* disable INTDSADS interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_Result
* Description  : This function returns the PGA_DSAD conversion result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_Result(uint16_t * const bufferH, uint16_t * const bufferL)
{
    *bufferH = DSADCR1;
    *bufferL = DSADCR0;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_AverageResult
* Description  : This function returns the PGA_DSAD conversion average result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH, uint16_t * const bufferL)
{
    *bufferH = DSADMV1;
    *bufferL = DSADMV0;
}

/* Start user code for adding. Do not edit comment generated here */

void L_DSADC_CREATE_FOR_R_LINE_I_REF(void){
AFEEN = 1U;     /* enables input clock supply */
    AFEPON = 1U;    /* power on AFE */

    while (0U == AFESTAT) 
    {
        ;/* Wait until AFE stabilize  */
    }

    PGAEN = 1U;     /* enables input clock supply */
    DSADMR = _00_PGA_DSAD_MODE_NORMAL;
    AFECKS = _0B_AFE_CLOCK_DIVIDED4;
    VSBIAS = _14_VSBIAS_OUTPUT_VOLTAGE_16;
    PGAPON = 1U;    /* power on PGA and DS A/D */

    while (0U == PGASTAT) 
    {
        ;/* Wait until PGA stabilize  */
    }

    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADSMK = 1U;   /* disable INTDSADS interrupt */
    DSADSIF = 0U;   /* clear INTDSADS interrupt flag */
    /* Set INTDSAD low priority */
    DSADPR1 = 1U;
    DSADPR0 = 1U;
    /* Set INTDSADS low priority */
    DSADSPR1 = 1U;
    DSADSPR0 = 1U;
    DSADMR |= _00_PGA_DSAD_TRIGGER_SOFTWARE;
    DSADCTL = _00_DSAD_CONVERSION_STOP | _00_DSAD_STABILIZATION_TIME_3T | _00_DSAD_SCAN_MODE_CONTINUOUS | 
              _00_DSAD_INPUT3_ENABLE | _00_DSAD_INPUT2_ENABLE | _02_DSAD_INPUT1_DISABLE | _00_DSAD_INPUT0_ENABLE;
    PGA0CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _10_PGA_MULTIPLEXERn_GSET01_8 | _03_PGA_MULTIPLEXERn_GSET02_8;
    PGA2CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _00_PGA_MULTIPLEXERn_GSET01_1 | _00_PGA_MULTIPLEXERn_GSET02_1;
    PGA3CTL0 = _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512 | _04_PGA_MULTIPLEXERn_GSET01_2 | _01_PGA_MULTIPLEXERn_GSET02_2;
    PGA0CTL1 = _00_PGA_MULTIPLEXER0_DIFFERENTIAL | _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA2CTL1 = _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA3CTL1 = _40_PGA_MULTIPLEXER3_EXTERNAL_DIFFERENTIAL | _10_PGA_MULTIPLEXERn_OFFSET_16;
    PGA0CTL2 = _01_PGA_MULTIPLEXER0_SAMPLE_COUNT;
    PGA2CTL2 = _01_PGA_MULTIPLEXER2_SAMPLE_COUNT;
    PGA3CTL2 = _01_PGA_MULTIPLEXER3_SAMPLE_COUNT;
    PGA0CTL3 = _80_PGA_MULTIPLEXERn_MUMBER_MODE_255 | _0C_PGA_MULTIPLEXERn_AVG_MEAM_VALUE | 
               _03_PGA_MULTIPLEXERn_AVG_UNIT_64;
    PGA2CTL3 = _00_PGA_MULTIPLEXERn_MUMBER_MODE_8032 | _08_PGA_MULTIPLEXERn_AVG_CONVERSION_OCCURS | 
               _02_PGA_MULTIPLEXERn_AVG_UNIT_32;
    PGA3CTL3 = _80_PGA_MULTIPLEXERn_MUMBER_MODE_255 | _0C_PGA_MULTIPLEXERn_AVG_MEAM_VALUE | 
               _02_PGA_MULTIPLEXERn_AVG_UNIT_32;
    PGABOD = _00_DSAD_DISCONNECTION_DETECTION_UNUSED;
    }

/* End user code. Do not edit comment generated here */
