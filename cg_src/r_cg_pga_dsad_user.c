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
* Creation Date: 2022/7/8
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
// #pragma address (dsadc_buf=0xFF800U)
# define DSADC_SKIP_LENGTH 0
# define DSADC_BUF_SIZE 36
# define DSADC_RESULT_BUF_SIZE 4
# define DSADC_DIFF_PGA_GAIN_64 (float) 1.49012 //mV = (1.6 V /64)*(1/2^24)*1000
# define SHIFT_20Bit_BASE_1M_ERROR (float) 0.9536743164 // 1,000,000 / 2^20  = 0.9536743164
# define SHIFT_16Bit_BASE_1M_ERROR (float) 15.25878906 // 1,000,000 / 
# define SHIFT_12Bit_BASE_1M_ERROR (float) 244.14 // 1,000,000 / 2^12  = 244.14
# define PT100_TEMPERATURE_RATE (float) 36.7725
# define PT100_BASE (int) 100000
# define PCBtemp30 (int) 354
# define PCBtemp40 (int) 401
# define PCBtemp50 (int) 501
# define IPT100_30 (float) 1.55033
# define IPT100_40 (float) 1.56772
# define IPT100_50 (float) 1.59959
# define Rate_30 (float) 0.039522727
# define Rate_40 (float) 0.032536842
# define Rate_50 (float) 0.026135294
# define vsbias 14404169

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
struct DSADC_BUFs
{
    uint16_t _DSADMV1;
    uint16_t _DSADMV0;
};

struct DSADC_BUFs dsadc_buffer0[DSADC_RESULT_BUF_SIZE];
struct DSADC_BUFs dsadc_buffer1[DSADC_RESULT_BUF_SIZE];
struct DSADC_BUFs dsadc_buffer2[DSADC_RESULT_BUF_SIZE];
struct DSADC_BUFs dsadc_buffer3[DSADC_RESULT_BUF_SIZE];
uint32_t current = 0;
double Ipt = 0;
uint16_t dsadc_buf[DSADC_BUF_SIZE];
uint8_t dsadc_counter = 0;
uint32_t ds_adc_result0[DSADC_RESULT_BUF_SIZE];
uint32_t ds_adc_result1[DSADC_RESULT_BUF_SIZE];
uint32_t ds_adc_result2[DSADC_RESULT_BUF_SIZE];
uint32_t ds_adc_result3[DSADC_RESULT_BUF_SIZE];
int result0 = DSADC_RESULT_BUF_SIZE - 1;
int result1 = DSADC_RESULT_BUF_SIZE - 1;
int result2 = DSADC_RESULT_BUF_SIZE - 1;
int result3 = DSADC_RESULT_BUF_SIZE - 1;

extern uint32_t Vm0 = 0;
extern uint32_t Vm1 = 0;
extern uint32_t Vm2 = 0;
extern uint32_t Vm3 = 0;

double Ipt100=0;

uint32_t fetchCounter = 0;
int16_t temperatureOffset;
double Vpt100 = 0;

void parseDifferential_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
void parseSingle_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
double getIpt(void);
uint16_t getK(void);
void setK(void);
uint16_t buffer_[4] = {0};
extern uint16_t K = 10000;
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
    // uint8_t index = dsadc_counter * 2;
    switch (DSADMVC)
    {
        case 0x20:
            dsadc_buffer0[result0]._DSADMV1 = DSADMV1;
            dsadc_buffer0[result0]._DSADMV0 = DSADMV0;
            if (result0)
            {
                result0--;
            }else
            {
                result0 = DSADC_RESULT_BUF_SIZE - 1;
            }
                fetchCounter++;
            break;
        case 0x40:
            dsadc_buffer1[result1]._DSADMV1 = DSADMV1;
            dsadc_buffer1[result1]._DSADMV0 = DSADMV0;
            if (result1)
            {
                result1--;
            }
            else
            {
                result1 = DSADC_RESULT_BUF_SIZE - 1;
            }
                fetchCounter++;
            break;
        case 0x60:
            dsadc_buffer2[result2]._DSADMV1 = DSADMV1;
            dsadc_buffer2[result2]._DSADMV0 = DSADMV0;
            if (result2)
            {
                result2--;
            }
            else
            {
                result2 = DSADC_RESULT_BUF_SIZE - 1;
            }
                fetchCounter++;
            break;
        case 0x80:
            dsadc_buffer3[result3]._DSADMV1 = DSADMV1;
            dsadc_buffer3[result3]._DSADMV0 = DSADMV0;
            if (result3)
            {
                result3--;
            }
            else
            {
                result3 = DSADC_RESULT_BUF_SIZE - 1;
            }
                fetchCounter++;
            break;
    }
    // dsadc_buf[index]= DSADMV1;
    // dsadc_buf[index+1] = DSADMV0;
    // dsadc_counter = dsadc_counter + 1;
    if (fetchCounter>50)// (dsadc_counter >= (DSADC_BUF_SIZE / 2))
    {
        dsadc_ready=1;
        result0 = DSADC_RESULT_BUF_SIZE - 1;
        result1 = DSADC_RESULT_BUF_SIZE - 1;
        result2 = DSADC_RESULT_BUF_SIZE - 1;
        result3 = DSADC_RESULT_BUF_SIZE - 1;
        fetchCounter = 0;
        R_PGA_DSAD_Stop();
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
    PGAPON = 0U; /* power off PGA and DS A/D */
    PGAEN = 0U;  /* disable input clock supply */
    AFEPWS = 0U; /* power off AFE */
    AFEEN = 0U;  /* disable input clock supply */
    R_PGA_DSAD_Stop();
}



void L_get_pt100_result(int *result){
    get_pt100_result();
    switch (mode)
    {
    case factory_mode:
        K =((Vpt100* 10000/getIpt())/ IDEA_TEST_R) ;
        setK();
        break;
    case factory_test_mode:
        K = 10000;
        break;
    default:
        K = getK();
        break;
    }
    Rpt100 = (Vpt100*10000/getIpt())/K;
    *result = ((int)Rpt100 - PT100_BASE) / PT100_TEMPERATURE_RATE ;
}


void setK(void){
    board[TEMPERATURE_SENSOR_K_VALUE + 1] = K>>8;
    board[TEMPERATURE_SENSOR_K_VALUE] = K&(0x00FF);
}
uint16_t getK(void)
{
    return (uint16_t)(((uint16_t) board[TEMPERATURE_SENSOR_K_VALUE+1]<<8)|board[TEMPERATURE_SENSOR_K_VALUE]);
}

double getIpt(void)
{
    if (pcb_temperature>500)
    {
        Ipt100 = ((pcb_temperature - PCBtemp50)/1000) * Rate_50 + IPT100_50;
    }
    else
    {
        if (pcb_temperature > 400)
        {
            Ipt100 = ((pcb_temperature - PCBtemp40)/1000) * Rate_40 + IPT100_40;
        }
        else
        {
            if (pcb_temperature > 100)
            {
                Ipt100 = ((pcb_temperature - PCBtemp30)/1000) * Rate_30 + IPT100_30;
            }
            else
            {
                Ipt100 = 1.543572;
            }
        }
    }
    Ipt100 = 1.543572;
    return Ipt100;
}

void get_pt100_result(void)
{
    
    uint16_t i = 0;
    Vm0 = 0;
    Vm1 = 0;
    Vm2 = 0;
    Vm3 = 0;
    
    for (i = 0; i < DSADC_RESULT_BUF_SIZE; i++)
    {
        parseDifferential_DSADC_Result(dsadc_buffer0[i]._DSADMV1,dsadc_buffer0[i]._DSADMV0, &ds_adc_result0[i]);
        parseSingle_DSADC_Result(dsadc_buffer1[i]._DSADMV1, dsadc_buffer1[i]._DSADMV0, &ds_adc_result1[i]);
        parseSingle_DSADC_Result(dsadc_buffer2[i]._DSADMV1, dsadc_buffer2[i]._DSADMV0, &ds_adc_result2[i]);
        parseDifferential_DSADC_Result(dsadc_buffer3[i]._DSADMV1, dsadc_buffer3[i]._DSADMV0, &ds_adc_result3[i]);
       // Vpt100 += (((ds_adc_result3[i] << 4) - (ds_adc_result0[i] << 1)) >> 16);
        Vm0 += ds_adc_result0[i];
        Vm1 += ds_adc_result1[i];
        Vm2 += ds_adc_result2[i];
        Vm3 += ds_adc_result3[i];
    }
    Vm0 = ((((((Vm0 *125) / DSADC_RESULT_BUF_SIZE))>>6)>>13)*625)/100;//uV
    Vm1 = ((((((Vm1) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
    Vm2 = ((((((Vm2) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
    Vm3 = ((((((Vm3 *125) / DSADC_RESULT_BUF_SIZE))>>2)>>13)*625)/100;//uV
    Vpt100 = (Vm3-Vm0*2); //mV
}


void parseDifferential_DSADC_Result(uint32_t BufferH,uint32_t BufferL,uint32_t *result){
            *result = ((BufferH << 8) | (BufferL >> 8)) & 0x00ffffff;
            if (((*result & 0x00800000) >> 23) == 1)
            {
                *result += 0xff000000;
            }
}

void parseSingle_DSADC_Result(uint32_t BufferH,uint32_t BufferL,uint32_t *result){
            *result = ((BufferH << 8) | (BufferL >> 8)) & 0x00ffffff;
}

void clr_dsadc_buf(void){
    memclr((uint8_t *)&dsadc_buf[0], DSADC_BUF_SIZE*2);
}
/* End user code. Do not edit comment generated here */
