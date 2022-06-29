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
* File Name    : r_cg_port.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Port module.
* Creation Date: 2022/6/29
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
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
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    PMC0 = _C7_PMC0_DEFAULT_VALUE | _00_PMCn5_NOT_USE | _00_PMCn4_NOT_USE | _08_PMCn3_DI_ON;
    P0 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _20_Pn5_OUTPUT_1 | _10_Pn4_OUTPUT_1 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0;
    P3 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _00_Pn0_OUTPUT_0;
    P5 = _08_Pn3_OUTPUT_1 | _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    P7 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _02_Pn1_OUTPUT_1 | _00_Pn0_OUTPUT_0;
    PU0 = _80_PUn7_PULLUP_ON | _40_PUn6_PULLUP_ON | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | 
          _00_PUn3_PULLUP_OFF | _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF;
    PU7 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PIM0 = _00_PIMn7_TTL_OFF | _00_PIMn4_TTL_OFF | _00_PIMn3_TTL_OFF;
    POM0 = _00_POMn7_NCH_OFF | _00_POMn6_NCH_OFF | _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _04_POMn2_NCH_ON;
    POM3 = _00_POMn7_NCH_OFF | _00_POMn6_NCH_OFF | _20_POMn5_NCH_ON;
    POM5 = _04_POMn2_NCH_ON | _00_POMn1_NCH_OFF | _00_POMn0_NCH_OFF;
    PM0 = _01_PM0_DEFAULT_VALUE | _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _00_PMn5_MODE_OUTPUT | 
          _00_PMn4_MODE_OUTPUT | _08_PMn3_MODE_INPUT | _00_PMn2_MODE_OUTPUT | _02_PMn1_MODE_INPUT;
    PM3 = _1A_PM3_DEFAULT_VALUE | _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _00_PMn5_MODE_OUTPUT | 
          _00_PMn2_MODE_OUTPUT | _01_PMn0_MODE_INPUT;
    PM5 = _F0_PM5_DEFAULT_VALUE | _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _02_PMn1_MODE_INPUT | 
          _01_PMn0_MODE_INPUT;
    PM7 = _3C_PM7_DEFAULT_VALUE | _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _00_PMn1_MODE_OUTPUT | 
          _01_PMn0_MODE_INPUT;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
