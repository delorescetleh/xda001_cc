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
* File Name    : r_cg_elc.h
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ELC module.
* Creation Date: 2022/6/25
***********************************************************************************************************************/
#ifndef ELC_H
#define ELC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Event output destination select register n (ELSELRn) 
*/
/* Event output destination select register n (ELSELn3 - ELSELn0) */
#define _00_ELC_EVENT_LINK_OFF            (0x00U) /* event link disabled */
#define _01_ELC_EVENT_LINK_AD             (0x01U) /* select operation of peripheral function 1 to link */
#define _02_ELC_EVENT_LINK_TAU00          (0x02U) /* select operation of peripheral function 2 to link */
#define _03_ELC_EVENT_LINK_TAU01          (0x03U) /* select operation of peripheral function 3 to link */
#define _04_ELC_EVENT_LINK_AMP0           (0x04U) /* select operation of peripheral function 4 to link */
#define _05_ELC_EVENT_LINK_AMP1           (0x05U) /* select operation of peripheral function 5 to link */
#define _06_ELC_EVENT_LINK_AMP2           (0x06U) /* select operation of peripheral function 6 to link */
#define _07_ELC_EVENT_LINK_DA0            (0x07U) /* select operation of peripheral function 7 to link */
#define _08_ELC_EVENT_LINK_DA1            (0x08U) /* select operation of peripheral function 8 to link */
#define _09_ELC_EVENT_LINK_DSAD           (0x09U) /* select operation of peripheral function 9 to link */
#define _0A_ELC_EVENT_LINK_TMRJ0          (0x0AU) /* select operation of peripheral function 10 to link */
#define _0B_ELC_EVENT_LINK_TMRJ1          (0x0BU) /* select operation of peripheral function 11 to link */
#define _0C_ELC_EVENT_LINK_AMP3           (0x0CU) /* select operation of peripheral function 12 to link */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define ELC_DESTINATION_COUNT             (26U)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_ELC_Create(void);
void R_ELC_Stop(uint32_t event);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
