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
* File Name    : r_cg_pga_dsad_user.c
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

#pragma interrupt r_pga_dsad_conversion_interrupt(vect=INTDSAD)
#pragma interrupt r_pga_dsad_scan_interrupt(vect=INTDSADS)
/* Start user code for pragma. Do not edit comment generated here */



/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
union 
{
    uint32_t whole;
    struct {
        uint16_t b0, b1;
    } byte;
} vm1,vm2;
union 
{
    int32_t whole;
    struct {
        uint16_t b0, b1;
    } byte;
} vm0,vm3;

uint8_t dsadc_ready = 0;
int32_t VM0, VM1, VM2, VM3;
void parseSingle_DSADC_Result(uint32_t BufferH, uint32_t BufferL, int32_t *result);
uint8_t Gtotal0(uint8_t channel);
int32_t convert_signal_end_value_as_uv(int32_t value,uint8_t multiplex_channel);
int32_t convert_differential_value_as_uv(int32_t value,uint8_t multiplex_channel);
int32_t convert_differential_uv_value_as_uv(int32_t value, uint8_t multiplex_channel);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_pga_dsad_conversion_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_pga_dsad_conversion_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    switch (DSADMVC)
    {
    case 0x20:
        vm0.byte.b0 = DSADMV0;
        vm0.byte.b1 = DSADMV1;
        break;
    case 0x40:
        vm1.byte.b0 = DSADMV0;
        vm1.byte.b1 = DSADMV1;
        break;
    case 0x60:
        vm2.byte.b0 = DSADMV0;
        vm2.byte.b1 = DSADMV1;
        break;
    case 0x80:
        vm3.byte.b0 = DSADMV0;
        vm3.byte.b1 = DSADMV1;
        break;
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_pga_dsad_scan_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_pga_dsad_scan_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    R_PGA_DSAD_Stop();
    VM0 = vm0.whole >> 8;
    VM1 = vm1.whole >> 8;
    VM2 = vm2.whole >> 8;
    VM3 = vm3.whole >> 8;
    dsadc_ready = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

int32_t convert_differential_value_as_uv(int32_t value,uint8_t multiplex_channel)
{
    return value*125/Gtotal0(multiplex_channel)/8192*625/100;
}
int32_t convert_signal_end_value_as_uv(int32_t value,uint8_t multiplex_channel)
{
    return value*125/Gtotal0(multiplex_channel)/8192*625/100+(-800000/Gtotal0(multiplex_channel)+1000000);
}

uint8_t Gtotal0(uint8_t channel)
{
	uint8_t result=0;
    uint8_t PGACTL = 0;
    uint8_t Gset02 = 0;
    uint8_t Gset01 = 0;
    switch (channel)
    {
    case 0:
        PGACTL = PGA0CTL0;
        break;
    case 1:
        PGACTL = PGA1CTL0;
        break;
    case 2:
        PGACTL = PGA2CTL0;
        break;
    case 3:
        PGACTL = PGA3CTL0;
        break;
    }
    Gset02 = 0x01<<((PGACTL & (0B00000011)));
    if (PGACTL & (0B00010000)){
        Gset01 = 8;
    }else{
        Gset01 = ((PGACTL & (0B00001100)) >> 2)+1;
    }
    result = Gset01 * Gset02;
    return result;
}
/* End user code. Do not edit comment generated here */
