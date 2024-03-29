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
* File Name    : r_cg_dtc.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DTC module.
* Creation Date: 2023/1/22
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dtc.h"
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

#pragma address dtc_vectortable = 0x0FFD00U
uint8_t __near dtc_vectortable[40U];

#pragma address dtc_controldata_10 = 0x0FFD90U
st_dtc_data_t __near dtc_controldata_10;
/***********************************************************************************************************************
* Function Name: R_DTC_Create
* Description  : This function initializes the DTC module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTC_Create(void)
{
    /* Enable input clock supply */
    DTCEN = 1U;     /* enables input clock supply */
    /* Disable all DTC channels operation */
    DTCEN0 = 0x00U;
    DTCEN1 = 0x00U;
    DTCEN2 = 0x00U;
    DTCEN3 = 0x00U;
    /* Set base address */
    DTCBAR = 0xFDU;
    /* Set DTCD10 */
    dtc_vectortable[13U] = 0x90U;
    dtc_controldata_10.dtccr = _00_DTC_DATA_SIZE_8BITS | _00_DTC_REPEAT_INT_DISABLE | _00_DTC_CHAIN_TRANSFER_DISABLE | 
                               _00_DTC_SOURCE_ADDR_FIXED | _00_DTC_REPEAT_AREA_DEST | _01_DTC_TRANSFER_MODE_REPEAT;
    dtc_controldata_10.dtbls = _01_DTCD10_TRANSFER_BLOCKSIZE;
    dtc_controldata_10.dtcct = _A0_DTCD10_TRANSFER_BYTE;
    dtc_controldata_10.dtrld = _A0_DTCD10_TRANSFER_BYTE;
    dtc_controldata_10.dtsar = _FF46_DTCD10_SRC_ADDRESS;
    dtc_controldata_10.dtdar = _FC00_DTCD10_DEST_ADDRESS;
}
/***********************************************************************************************************************
* Function Name: R_DTCD10_Start
* Description  : This function enables DTCD10 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD10_Start(void)
{
     DTCEN1 |= _04_DTC_UART1R_ACTIVATION_ENABLE;
}
/***********************************************************************************************************************
* Function Name: R_DTCD10_Stop
* Description  : This function disables DTCD10 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DTCD10_Stop(void)
{
    DTCEN1 &= (uint8_t)~_04_DTC_UART1R_ACTIVATION_ENABLE;
}

/* Start user code for adding. Do not edit comment generated here */
uint8_t reset_DTC10(void){
    uint8_t howFarWeAlreadyGo = dtc_controldata_10.dtdar - _FC00_DTCD10_DEST_ADDRESS;
    dtc_controldata_10.dtdar = _FC00_DTCD10_DEST_ADDRESS;
    dtc_controldata_10.dtbls = _01_DTCD10_TRANSFER_BLOCKSIZE;
    return howFarWeAlreadyGo;
}
/* End user code. Do not edit comment generated here */
