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
* File Name    : r_cg_dac.h
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DAC module.
* Creation Date: 2023/2/14
***********************************************************************************************************************/
#ifndef DAC_H
#define DAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    D/A Converter Mode Register 0 (DACM0) 
*/
/* D/A converter operation mode selection (DAMD0) */
#define _00_DA0_NORMAL_MODE                 (0x00U) /* normal mode */
#define _01_DA0_TIME_OUTPUT_MODE            (0x01U) /* real-time output mode */
/* D/A converter operation mode selection (DAMD1) */
#define _00_DA1_NORMAL_MODE                 (0x00U) /* normal mode */
#define _02_DA1_TIME_OUTPUT_MODE            (0x02U) /* real-time output mode */
/* D/A conversion data format selection (DPSEL) */
#define _00_DA1_FLUSH_RIGHT_FORMAT          (0x00U) /* flush-right */
#define _80_DA1_FLUSH_LEFT_FORMAT           (0x80U) /* flush-left */

/*
    D/A Converter Mode Register 1 (DACM1) 
*/
/* D/A converter reference voltage source selection (DAVR1) */
#define _00_DA1_REFERENCE_AVDD              (0x00U) /* AVDD */
#define _02_DA1_REFERENCE_SBIAS             (0x02U) /* SBIAS output */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define DA0_WAITTIME                        (0x0CU) /* set the waiting time */
#define _0A_DA0_COUVERSION_VALUE            (0x0AU) /* set the analog voltage value */
#define DA1_WAITTIME                        (0x78U) /* set the waiting time */
#define _0000_DA1_COUVERSION_VALUE          (0x0000U) /* set the analog voltage value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_DAC_Create(void);
void R_DAC0_Start(void);
void R_DAC0_Stop(void);
void R_DAC0_Set_ConversionValue(uint8_t regvalue);
void R_DAC1_Start(void);
void R_DAC1_Stop(void);
void R_DAC1_Set_ConversionValue(uint8_t regvalue);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
