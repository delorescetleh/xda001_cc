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
* File Name    : r_cg_pga_dsad_user.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2022/6/11
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
# define DSADC_SKIP_LENGTH 0
# define DSADC_BUF_SIZE 48
# define DSADC_RESULT_BUF_SIZE 4 
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint16_t dsadc_buf[DSADC_BUF_SIZE];
uint8_t dsadc_counter = 0;
float PT100result;
uint32_t ds_adc_result0[DSADC_RESULT_BUF_SIZE];
// uint32_t ds_adc_result1[DSADC_RESULT_BUF_SIZE];
// uint32_t ds_adc_result2[DSADC_RESULT_BUF_SIZE];
uint32_t ds_adc_result3[DSADC_RESULT_BUF_SIZE];
// uint32_t ds_adc_result4[DSADC_RESULT_BUF_SIZE];
float r0_r1[DSADC_RESULT_BUF_SIZE];
float b0_r1[DSADC_RESULT_BUF_SIZE];
float i1[DSADC_RESULT_BUF_SIZE];
float r100[DSADC_RESULT_BUF_SIZE];
uint8_t skip=0;
void parseDifferential_DSADC_Result(uint16_t *BufferH, uint16_t *BufferL, uint32_t *result);
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
    if (skip<DSADC_SKIP_LENGTH){
        skip++;
    }else{
        dsadc_buf[dsadc_counter * 3] = DSADCRC;
        R_PGA_DSAD_Get_AverageResult((uint16_t *const)(&dsadc_buf[0]+dsadc_counter*3+1), (uint16_t *const)(&dsadc_buf[0]+dsadc_counter*3+2));
        dsadc_counter = dsadc_counter + 1;
        if (dsadc_counter > (DSADC_BUF_SIZE/3)){
            dsadc_counter = 0;
            skip = 0;
            R_PGA_DSAD_Stop();
            get_pt100_result(&PT100result);
        }
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
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void L_PGA_STOP(void){
    //stop PGA , DSADC for powersaving
    PGAPON = 0U; /* power off PGA and DS A/D */
    PGAEN = 0U;  /* disable input clock supply */
    AFEPWS = 0U; /* power off AFE */
    AFEEN = 0U;  /* disable input clock supply */
    R_PGA_DSAD_Stop();
}
void get_dsadc_result(void){
    uint16_t _DSADCRC,avgBufferH,avgBufferL;
    uint32_t *result0 = &ds_adc_result0[0];
    // uint32_t *result1 = &ds_adc_result1[0];
    // uint32_t *result2 = &ds_adc_result2[0];
    uint32_t *result3 = &ds_adc_result3[0];
    uint16_t i = 0;
    for (i = 0; i <(DSADC_BUF_SIZE/3);i++){
        _DSADCRC=dsadc_buf[i * 3];
        avgBufferH=dsadc_buf[i * 3+1];
        avgBufferL=dsadc_buf[i * 3+2];

        switch (_DSADCRC)
        {
        case 0x20:
            *result0 = (((uint32_t)avgBufferH << 8) | ((uint32_t)avgBufferL >> 8)) & 0x00ffffff;
            if (((*result0 & 0x00800000) >> 23) == 1)
            {
                *result0 += 0xff000000;
            }
            result0++;
            break;
        // case 0x40:
        //     // *result1 = (((uint32_t)avgBufferH << 8) | ((uint32_t)avgBufferL >> 8)) & 0x00ffffff;
        //     result1++;
        //     break;
        // case 0x60:
        //     // *result2 = (((uint32_t)avgBufferH << 8) | ((uint32_t)avgBufferL >> 8)) & 0x00ffffff;
        //     result2++;
        //     break;
        case 0x80:
            *result3 = (((uint32_t)avgBufferH << 8) | ((uint32_t)avgBufferL >> 8)) & 0x00ffffff;
            if (((*result3 & 0x00800000) >> 23) == 1)
            {
                *result3 += 0xff000000;
            }
            result3++;
            break;
        }
    }
    for (i = 0; i < DSADC_RESULT_BUF_SIZE;i++){
        r0_r1[i] = ds_adc_result0[i]*0.00149012; //uV
        b0_r1[i] = ds_adc_result3[i]*0.023842; //uV
        r100[i]=(b0_r1[i]-r0_r1[i]*2)/1000000/0.00154155;//ohm
    }
    R_PGA_DSAD_Start();
}

void get_pt100_result(float *result){
    uint16_t _DSADCRC,BufferH,BufferL;
    uint32_t *result0 = &ds_adc_result0[0];
    uint32_t *result3 = &ds_adc_result3[0];
    uint16_t i = 0;
    double pt100 = 0;
    for (i = 0; i <(DSADC_BUF_SIZE/3);i++){
        _DSADCRC=dsadc_buf[i * 3];
        BufferH=dsadc_buf[i * 3+1];
        BufferL=dsadc_buf[i * 3+2];

        switch (_DSADCRC)
        {
        case 0x20:
            parseDifferential_DSADC_Result(BufferH, BufferL, result0);
            result0++;
            break;
        case 0x80:
            parseDifferential_DSADC_Result(BufferH, BufferL, result3);
            result3++;
            break;
        }
    }
    for (i = 0; i < DSADC_RESULT_BUF_SIZE;i++){
        r0_r1[i] = ds_adc_result0[i]*0.00149012; //uV
        b0_r1[i] = ds_adc_result3[i]*0.023842; //uV
        r100[i]=(b0_r1[i]-r0_r1[i]*2)/1000000/0.00154155;//ohm
        pt100 += r100[i];
    }
    pt100 = pt100 / DSADC_RESULT_BUF_SIZE;
    *result = (float)(pt100-100)/0.367725;
    R_PGA_DSAD_Start();
}

void parseDifferential_DSADC_Result(uint16_t *BufferH,uint16_t *BufferL,uint32_t *result){
            *result = (((uint32_t)BufferH << 8) | ((uint32_t)BufferL >> 8)) & 0x00ffffff;
            if (((*result & 0x00800000) >> 23) == 1)
            {
                *result += 0xff000000;
            }
}

/* End user code. Do not edit comment generated here */
