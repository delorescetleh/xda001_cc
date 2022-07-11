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
* Creation Date: 2022/7/11
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
# define DSADC_RESULT_BUF_SIZE 1
# define DSADC_DIFF_PGA_GAIN_64 (float) 1.49012 //mV = (1.6 V /64)*(1/2^24)*1000
# define SHIFT_20Bit_BASE_1M_ERROR (float) 0.9536743164 // 1,000,000 / 2^20  = 0.9536743164
# define SHIFT_16Bit_BASE_1M_ERROR (float) 15.25878906 // 1,000,000 / 
# define SHIFT_12Bit_BASE_1M_ERROR (float) 244.14 // 1,000,000 / 2^12  = 244.14
# define PT100_TEMPERATURE_RATE  385
# define PT100_BASE (int) 100000
# define vsbias 14404169
# define TYPE_Vm0 1743
# define TYPE_Im0_AT_311 1555

# define TYPE_IPT100 1263 // uA
# define TYPE_R_LINE_PT100 500 // K ohm
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

uint16_t BOARD_R_ref = 500; // k ohm , it should be calibration by factory
uint16_t R_LINE_PT100 = TYPE_R_LINE_PT100;  // k ohm 
uint16_t IPT100 = TYPE_IPT100;

uint16_t RL = 1110;
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

uint32_t vm0_recent[8]={0};
int lastVm0 = 0;

extern uint32_t VM0 = 0;
extern uint32_t VM1 = 0;
extern uint32_t VM2 = 0;
extern uint32_t VM3 = 0;

extern uint32_t Vm0 = 0;
extern uint32_t Vm1 = 0;
extern uint32_t Vm2 = 0;
extern uint32_t Vm3 = 0;

uint16_t Ipt100=0; // uA
uint16_t IPT100_BASE_R_LINE_PT100 = 0;
double _Ipt100[10]={0};
double _IPT100[10]={0};
double _IPT100_BASE_R_LINE_PT100[10]={0};
int8_t Ipt100_index=0;
uint32_t R_PT100_base_Ipt100[8] = {0};
uint32_t R_PT100_base_IPT100[8] = {0};
uint32_t R_PT100_base_IPT100_BASE_R_LINE_PT100[8] = {0};
uint32_t fetchCounter = 0;
int16_t temperatureOffset;
double Vpt100 = 0;
double Vr_ref = 0;
extern uint16_t Ipt100_PT100_TEMPERATURE = 0;
extern uint16_t IPT100_PT100_TEMPERATURE = 0;
extern uint16_t IPT100_BASE_R_LINE_PT100_PT100_TEMPERATURE = 0;
void parseDifferential_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
void parseSingle_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
double getIpt(void);
uint16_t getK(void);
void setK(void);
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
            parseDifferential_DSADC_Result(DSADMV1,DSADMV0,&VM0);
            break;
        case 0x40:
            parseSingle_DSADC_Result(DSADMV1,DSADMV0,&VM1);
            break;
        case 0x60:
            parseSingle_DSADC_Result(DSADMV1,DSADMV0,&VM2);
            break;
        case 0x80:
            parseDifferential_DSADC_Result(DSADMV1,DSADMV0,&VM3);
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
        dsadc_ready=1;
        R_PGA_DSAD_Stop();
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
        getRL();
        getIpt();
        Rpt100 = Vpt100 / Ipt100;
        temperatureOffset = pcb_temperature - ((double)(Rpt100)- PT100_BASE) / PT100_TEMPERATURE_RATE ;
        board[PT100_R_LINE + 1] = RL >> 8;
        board[PT100_R_LINE] = RL & (0x00FF);
        board[DSADC_TEMPERATURE_SENSOR_OFFSET+1] = temperatureOffset>>8;
        board[DSADC_TEMPERATURE_SENSOR_OFFSET] = temperatureOffset & (0x00FF);
        break;
    case factory_test_mode:
        getRL(); 
        getIpt();
        Rpt100 = Vpt100 / Ipt100;
        temperatureOffset = pcb_temperature - ((double)(Rpt100)- PT100_BASE) / PT100_TEMPERATURE_RATE ;
        board[PT100_R_LINE + 1] = RL >> 8;
        board[PT100_R_LINE] = RL & (0x00FF);
        board[DSADC_TEMPERATURE_SENSOR_OFFSET+1] = temperatureOffset>>8;
        board[DSADC_TEMPERATURE_SENSOR_OFFSET] = temperatureOffset & (0x00FF);
       break;
    case normal_mode:
        getRL();
        getIpt();
        Rpt100 = Vpt100 / Ipt100;
        temperatureOffset=((board[DSADC_TEMPERATURE_SENSOR_OFFSET+1]<<8)|board[DSADC_TEMPERATURE_SENSOR_OFFSET]);
        break;
    }
    *result = ((double)(Rpt100)- PT100_BASE) / (PT100_TEMPERATURE_RATE) + temperatureOffset ;
}

// uint: uV , base on  1 ohm pull up resister 
uint16_t getV_Rref(void) 
{
    return (Vm2 - Vm1); // PCB_BOARD_Version3 
}

void getRL(void){
    double ipt100 = 0;
    double rpt100 = 0;
    
    switch (mode)
    {
    case factory_mode:
        rpt100 = 100 * (1 + 0.000392 * pcb_temperature);
        ipt100 = Vpt100 / rpt100;
        Ipt = ipt100;
        RL = Vm0 * 1000 / ipt100;

        break;
    case factory_test_mode:
        rpt100 = 100 * (1 + 0.000392 * pcb_temperature);
        ipt100 = Vpt100 / rpt100;
        Ipt = ipt100;
        RL = Vm0 * 1000 / ipt100;
        break;
    case normal_mode:
        RL=((board[PT100_R_LINE+1]<<8)|board[PT100_R_LINE]);
        break;
    }

}


// unit: uA
double getIpt(void)
{
    // if (((lastVm0-(int)(Vm0))>8)||((lastVm0-(int)(Vm0))<-8)){
    //     Ipt100 = ((double)getV_Rref()*1000)/((double)getRref());// unit: uA
    //     // lastVm0 = Vm0;
    // } else {
        Ipt100 = Vm0 / RL;
    // }
    return Ipt100;
}

// unit : K ohm
uint16_t getR_Line(void) 
{
    uint16_t rpt100 = pcb_temperature * PT100_TEMPERATURE_RATE + PT100_BASE;
    uint16_t ipt100 = (Vm3 - Vm0) / rpt100;
    R_LINE_PT100 = Vm0*1000 / ipt100;
    return R_LINE_PT100;
}



// unit : K ohm, Board resistor + 1 R
uint16_t getRref(void) 
{
    return BOARD_R_ref;
}

void get_pt100_result(void)
{
    Vm0 = ((((((VM0 *125) / DSADC_RESULT_BUF_SIZE))>>6)>>13)*625)/100;//uV
    Vm1 = ((((((VM1) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
    Vm2 = ((((((VM2) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
    Vm3 = ((((((VM3 *125) / DSADC_RESULT_BUF_SIZE))>>2)>>13)*625)/100;//uV
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
