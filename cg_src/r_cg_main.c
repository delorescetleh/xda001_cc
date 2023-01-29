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
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2023/1/23
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_it8bit.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_amp.h"
#include "r_cg_dac.h"
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
void processMode(void);
extern uint8_t mode=0;
extern uint8_t events=0;
const int32_t ntc_lookup_table[] = {1597024, 1596780, 1596518, 1596238, 1595938, 1595617, 1595275, 1594908, 1594517, 1594100, 1593655, 1593181, 1592676, 1592139, 1591567, 1590960, 1590314, 1589629, 1588902, 1588131, 1587314, 1586449, 1585533, 1584564, 1583539, 1582456, 1581312, 1580104, 1578830, 1577487, 1576071, 1574580, 1573011, 1571359, 1569623, 1567798, 1565881, 1563869, 1561757, 1559544, 1557224, 1554793, 1552249, 1549585, 1546802, 1543892, 1540852, 1537678, 1534367, 1530914, 1527316, 1523566, 1519664, 1515603, 1511382, 1506994, 1502435, 1497702, 1492793, 1487705, 1482434, 1476970, 1471318, 1465476, 1459433, 1453186, 1446743, 1440090, 1433236, 1426170, 1418889, 1411404, 1403696, 1395788, 1387658, 1379310, 1370753, 1361971, 1352991, 1343769, 1334357, 1324731, 1314890, 1304865, 1294620, 1284185, 1273511, 1262670, 1251653, 1240449, 1229039, 1217511, 1205790, 1193844, 1181836, 1169603, 1157247, 1144808, 1132164, 1119429, 1106554, 1093571, 1080519, 1067443, 1054158, 1040804, 1027421, 1014054, 1000609, 987120, 973470, 959840, 946272, 932812, 919149, 905480, 892035, 878467, 865001, 851462, 838095, 824712, 811337, 798246, 784973, 772057, 759001, 746097, 733378, 720577, 708014, 695726, 683423, 671118, 659169, 647265, 635418, 623646, 612346, 600781, 589740, 578452, 567742, 556805, 546068, 535551, 525273, 515254, 505047, 495600, 485988, 476207, 466755, 457653, 448403, 439529, 431050, 422447, 414266, 405970, 397558, 389598, 382112, 373946, 366859, 359089, 351828, 345098, 338295, 331417, 324464, 318077, 311626, 305109, 299187, 293211, 287179, 281092, 275634, 270130, 264580, 259686, 254048, 249077, 244068, 239022, 234667, 229550, 225134, 220690, 216216, 212466, 207939, 204144, 199562, 195721, 191859, 187976, 184854, 180931};
int ntc_result = -999;
int I_REF = 0;
double Rptc = 0;
extern void goToSleep(void);
void DSADC_TEST(void);
void BLE_POW_TEST(void);
int ntc_convert(uint32_t Rntc);
/* End user code. Do not edit comment generated here */

static void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1)
    {
        if(events)
        {
            if(events & RTC_CONSTPERIOD_EVENT)//R_IT8Bit0_Channel1 , 10mS
            {
            }
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel1 , 10mS
            {
            }
        }
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    BLE_POW_TEST(); // DO NOT TEST IN AUTO RUN
    
    //DSADC_TEST();// DSADCTEST


    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void BLE_POW_TEST(void)
{
        // LDO ON , BYPASS OFF , operation if battery power over 3.8 volt
        BLE_POW_CNT = 1;
        BLE_POW_BYPASS = 1;
        // LDO OFF , BYPASS ON , operation if battery power under 3.8 volt, may no necessary
        BLE_POW_CNT = 0;
        BLE_POW_BYPASS = 0;

        //LDO OFF, BYPASS OFF
        BLE_POW_CNT = 0;
        BLE_POW_BYPASS = 1;
}


void DSADC_TEST(void)
{

    // L_DSADC_CREATE_FOR_R_LINE_I_REF();
    // L_AMP_CREATE_FOR_RLINE_I_REF();
    // PCB TEMPERATURE
    R_AMP2_Start();
    
    // CONST CURRENT
    R_DAC0_Set_ConversionValue(11);
    R_AMP0_Start();
    R_DAC0_Start();

    R_AMP_Set_PowerOn();

    R_PGA_DSAD_Start();

    while(1)
    {
        if (dsadc_ready)
        {
            /* code */
                dsadc_ready = 0;
                VM0 = convert_differential_value_as_uv(VM0,0);
                // VM0 = convert_signal_end_value_as_uv(VM0, 0);
                VM1 = convert_signal_end_value_as_uv(VM1, 1);
                VM2 = convert_signal_end_value_as_uv(VM2, 2);
                VM3 = convert_differential_value_as_uv(VM3, 3);

                ntc_result = ntc_convert(VM2);
                I_REF = VM3;
                R_PGA_DSAD_Start();
        }
    }
}

int ntc_convert(uint32_t v_ntc)
{
    const float offset=1.5;
    double result;
    int  index = 0;
    for (index = 0; index < 200;index++)
    {
        if (ntc_lookup_table[index]<v_ntc)
        {
            result =(((double)(v_ntc-ntc_lookup_table[index])) / (ntc_lookup_table[index + 1] - ntc_lookup_table[index])+index-50+offset)*10;
            break;
        }
    }
    return result;
}

void goToSleep(void){

    if(P_TEST)
    {
        HALT();
    }
    else
    {
        // E1 TESTING
        //HALT();
        //
        // NORMAL
         STOP();
        //
    }
}
// void processMode(void)
// {
//     switch (mode)
//     {
//     case normal_mode:
//         normal_process();
//         break;
//     case factory_mode:
//         factory_process();
//         break;
//     case lora_programming_mode:
//         lora_programming_process();
//         break;
//     case factory_test_mode:
//         factory_test_process();
//         break;                    
//     }
// }
/* End user code. Do not edit comment generated here */
