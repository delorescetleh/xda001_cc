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
* File Name    : r_cg_adc_user.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 2022/7/8
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_dtc.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_adc_interrupt(vect=INTAD)
/* Start user code for pragma. Do not edit comment generated here */
// #pragma address (adc_buf = 0xFF900U)
// #pragma address (ads_buf = 0xFFa00U)
#define SENSOR_REF_TEMP_SCALED (250)
#define INT_REF_V_SCALED (145000L)
#define INT_REF_TEMP_SCALED (105000L)
#define TEMP_SENSOR_GAIN_SCALED (36)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint16_t volt_buf[MAX_ADC_BUF];
volatile uint16_t temp_buf[MAX_ADC_BUF];
uint16_t ADCtemp=0,ADCvolt=0;
int16_t g_tempv_int;
int16_t voltIndex=MAX_ADC_BUF;
int16_t tempIndex=MAX_ADC_BUF;
// uint8_t adc_counter = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_adc_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_adc_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    switch (ADS)
    {
    case _80_AD_INPUT_TEMPERSENSOR:
        R_ADC_Get_Result(&ADCtemp);
        temp_buf[tempIndex] = ADCtemp;
        ADS = _81_AD_INPUT_INTERREFVOLT;
        tempIndex--;
        if (!tempIndex){
            tempIndex = MAX_ADC_BUF;
        }
        break;
    case _81_AD_INPUT_INTERREFVOLT:
        R_ADC_Get_Result(&ADCvolt);
        volt_buf[voltIndex] = ADCvolt;
        ADS = _80_AD_INPUT_TEMPERSENSOR;
        voltIndex--;
        if (!voltIndex){
            voltIndex = MAX_ADC_BUF;
        }
        break;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void init_pcb_temperature(void){
    memclr((uint8_t *)&temp_buf[0], MAX_ADC_BUF*2);
    memclr((uint8_t *)&volt_buf[0], MAX_ADC_BUF*2);
}
void get_pcb_temperature(int *pcbTemperature){
    int i;
    int16_t temp = 0;
    for (i = 1; i < MAX_ADC_BUF;i++){
        g_tempv_int = (int16_t)(((INT_REF_V_SCALED)*temp_buf[i] /
                                volt_buf[i]) -
                                (INT_REF_TEMP_SCALED));

        temp += (int16_t)(-(g_tempv_int / (TEMP_SENSOR_GAIN_SCALED)) +
                                    SENSOR_REF_TEMP_SCALED);
    }
    *pcbTemperature = temp / (MAX_ADC_BUF-1);
}
/* End user code. Do not edit comment generated here */
