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
* Creation Date: 2023/2/14
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
# define DSADC_BUF_SIZE 36
# define DSADC_RESULT_BUF_SIZE 1
# define PT100_TEMPERATURE_RATE (double) 385
# define PT100_BASE (double) 100000 // mOhm 
# define TYPICAL_IPT100 1263 // uA
# define TYPICAL_R_LINE_PT100 1000 // mohm
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint16_t RL = TYPICAL_R_LINE_PT100;// k ohm 
int32_t Ipt100=TYPICAL_IPT100; // uA
int32_t Vpt100 = 0; // uV
int16_t temperatureOffset=0; // degC

uint32_t VM0 = 0; // value
uint32_t VM1 = 0; // value
uint32_t VM2 = 0; // value
uint32_t VM3 = 0; // value 

extern int32_t Vm0 = 0; // uV
extern int32_t Vm1 = 0; // uV
extern int32_t Vm2 = 0; // uV
extern int32_t Vm3 = 0; // uV


// extern double Vm0 = 0; // uV
// extern double Vm1 = 0; // uV
// extern double Vm2 = 0; // uV
// extern double Vm3 = 0; // uV
int32_t ipt100_by_target_temperature_Vpt100 = 0;
int32_t rpt100_by_target_temperature = 0;
int16_t new_Temperature = 0;
void parseDifferential_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
void parseSingle_DSADC_Result(uint32_t BufferH, uint32_t BufferL, uint32_t *result);
void calculate_dsadc_result(void);
void getIpt(void);
void getRL(void);
void getRPT100(void);
void getR_Line_Base_On(int32_t target_temperature);
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
    // switch (DSADMVC)
    // {
    //     case 0x20:
    //         parseDifferential_DSADC_Result(DSADMV1,DSADMV0,&VM0);
    //         break;
    //     case 0x40:
    //         parseSingle_DSADC_Result(DSADMV1,DSADMV0,&VM1);
    //         break;
    //     case 0x60:
    //         parseSingle_DSADC_Result(DSADMV1,DSADMV0,&VM2);
    //         break;
    //     case 0x80:
    //         parseDifferential_DSADC_Result(DSADMV1,DSADMV0,&VM3);
    //         break;
    // }
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
    // dsadc_ready = 1;
    // R_PGA_DSAD_Stop();
    // calculate_dsadc_result();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
// void L_PGA_STOP(void){
//     PGAPON = 0U; /* power off PGA and DS A/D */
//     PGAEN = 0U;  /* disable input clock supply */
//     AFEPWS = 0U; /* power off AFE */
//     AFEEN = 0U;  /* disable input clock supply */
//     R_PGA_DSAD_Stop();
// }


// void getR_Line_Base_On(int32_t target_temperature)
// {
//     // R = R0(1+aT)    Rpt100 =PT100_R0_BASE(1+PT100_TEMPERATURE_RATE*Temperature)
//     // rpt100_by_target_temperature = PT100_R0_BASE * (1 + PT100_TEMPERATURE_RATE / 100000 * target_temperature/10);
//     //rpt100_by_target_temperature = PT100_BASE * (1 + PT100_TEMPERATURE_RATE * ((double)(target_temperature)) / 1000000); // mohm
//     //rpt100_by_target_temperature = target_temperature * PT100_TEMPERATURE_RATE / 10 + PT100_BASE; // mohm
//     //ipt100_by_target_temperature_Vpt100 = Vpt100*1000 /rpt100_by_target_temperature ;  // uV *1000 / mOhm = uA
//     // RL = Vm0 *1000 / ipt100_by_target_temperature_Vpt100; // uV / uA =  mohm
//     RL =(uint16_t) (Vm0*target_temperature/10*PT100_TEMPERATURE_RATE/Vpt100 + Vm0*(PT100_BASE/10)/(Vpt100/10)); // uV / uA =  mohm
//     board[PT100_R_LINE + 1] = RL >> 8;
//     board[PT100_R_LINE] = RL & (0x00FF);
// }

// void getR_Line_Base_On_100100_RPT100(void)
// {
//     // R = R0(1+aT)    Rpt100 =PT100_R0_BASE(1+PT100_TEMPERATURE_RATE*Temperature)
//     // rpt100_by_target_temperature = PT100_R0_BASE * (1 + PT100_TEMPERATURE_RATE / 100000 * target_temperature/10);
//     //rpt100_by_target_temperature = PT100_BASE * (1 + PT100_TEMPERATURE_RATE * ((double)(target_temperature)) / 1000000); // mohm
//     //rpt100_by_target_temperature = target_temperature * PT100_TEMPERATURE_RATE / 10 + PT100_BASE; // mohm
//     //ipt100_by_target_temperature_Vpt100 = Vpt100*1000 /rpt100_by_target_temperature ;  // uV *1000 / mOhm = uA
//     // RL = Vm0 *1000 / ipt100_by_target_temperature_Vpt100; // uV*1000 / uA =  mohm
//     Ipt100=(Vm3 - Vm0) / 100100;
//     RL =(uint16_t) (Vm0*1000/Ipt100); // uV / uA =  mohm
//     board[PT100_R_LINE + 1] = RL >> 8;
//     board[PT100_R_LINE] = RL & (0x00FF);
// }


// void L_pt100_calibration(int *result)
// {
//     getR_Line_Base_On(pcb_temperature);
//     Ipt100 = Vm0 * 1000 / RL;        // uA
//     Rpt100 = Vpt100 * RL / Vm0; // mohm
//     new_Temperature = ((Rpt100 - PT100_BASE) * 10) / PT100_TEMPERATURE_RATE;
//     temperatureOffset = pcb_temperature - new_Temperature;
//     board[DSADC_TEMPERATURE_SENSOR_OFFSET + 1] = temperatureOffset >> 8;
//     board[DSADC_TEMPERATURE_SENSOR_OFFSET] = temperatureOffset & (0x00FF);
//     *result = ((Rpt100 - PT100_BASE )*10) / PT100_TEMPERATURE_RATE + temperatureOffset;
// }

// void L_get_pt100_result(int *result){
//     RL = ((board[PT100_R_LINE + 1] << 8) | board[PT100_R_LINE]);
//     Ipt100 = (Vm0 * 1000) / RL;
//     Rpt100 = Vpt100/10*(RL)/ Vm0*10  ;
//     temperatureOffset = ((board[DSADC_TEMPERATURE_SENSOR_OFFSET + 1] << 8) | board[DSADC_TEMPERATURE_SENSOR_OFFSET]);
//     dsadc_temperature = ((Rpt100 - PT100_BASE) * 10) / PT100_TEMPERATURE_RATE;
//     *result = dsadc_temperature + temperatureOffset;
// }

// void calculate_dsadc_result(void)
// {
//     Vm0 = ((((VM0 *125)>>6)>>13)*625)/100;//uV
//     // Vm1 = ((((((VM1) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
//     // Vm2 = ((((((VM2) / DSADC_RESULT_BUF_SIZE)*125)>>0)>>13)*625+20000000)/100;//uV
//     Vm3 = ((((VM3 *125))>>13)*625)/100;//uV
//     Vpt100 = (Vm3-Vm0*2); //mV
// }

// void parseDifferential_DSADC_Result(uint32_t BufferH,uint32_t BufferL,uint32_t *result){
//             *result = ((BufferH << 8) | (BufferL >> 8)) & 0x00ffffff;
//             if (((*result & 0x00800000) >> 23) == 1)
//             {
//                 *result += 0xff000000;
//             }
// }

// void parseSingle_DSADC_Result(uint32_t BufferH,uint32_t BufferL,uint32_t *result){
//             *result = ((BufferH << 8) | (BufferL >> 8)) & 0x00ffffff;
// }

// void clr_dsadc_buf(void){
//     // memclr((uint8_t *)&dsadc_buf[0], DSADC_BUF_SIZE*2);
//     Vm0=0;
//     Vm1=0;
//     Vm2=0;
//     Vm3=0;
//     VM0 = 0;
//     VM1 = 0;
//     VM2 = 0;
//     VM3 = 0;
// }
/* End user code. Do not edit comment generated here */
