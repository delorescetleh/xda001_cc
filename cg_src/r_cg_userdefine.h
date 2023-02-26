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
* File Name    : r_cg_userdefine.h
* Version      : Code Generator for RL78/H1D V1.00.03.02 [08 Nov 2021]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file includes user definition.
* Creation Date: 2023/2/27
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_it8bit.h"
#include "r_cg_rtc.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_amp.h"
#include "r_cg_adc.h"
#include "r_cg_sau.h"
#include "r_cg_iica.h"
#include "r_cg_dtc.h"
#include "r_cg_intp.h"
# include "r_cg_tau.h"
# include "r_cg_dac.h"
# include "r_cg_it.h"

# include "L_useful.h"
# include "L_factory.h"
# include "L_EEPROM.h"
#include "L_PT100.h"
#include "L_BLE.h"
#include "L_Lora.h"
#include "L_normal.h"
#include "L_BAT.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>


#define RECORD_DATA_SIZE 1440


/*BOARD SUPPORT AREA START*/
#define UART1_TXD                     P5_bit.no0
#define UART1_TXD_MODE                PM5_bit.no0
#define UART1_RXD                     P5_bit.no1
#define UART1_RXD_MODE                PM5_bit.no1

#define LORA_POW_CNT                  P7_bit.no1
#define LORA_POW_CNT_MODE             PM7_bit.no1
#define LORA_STA                      P7_bit.no0// Input , v7 only LORA_RTS
#define LORA_STA_MODE                 PM7_bit.no0
#define LORA_READY                    P3_bit.no2// Output ,v7 only LORA_CTS
#define LORA_READY_MODE               PM3_bit.no2
#define LORA_RESET                    P3_bit.no5
#define LORA_RESET_MODE               PM3_bit.no5
#define LORA_GPIO16                   P3_bit.no0 
#define LORA_GPIO16_MODE              PM3_bit.no0


#define UART0_TXD                     P3_bit.no7
#define UART0_TXD_MODE                PM3_bit.no7
#define UART0_RXD                     P3_bit.no6
#define UART0_RXD_MODE                PM3_bit.no6

#define BAT_ADC_ON_MODE               PM0_bit.no1 // v7 only
#define BAT_ADC_ON                    P0_bit.no1 // v7 only
#define BUZ0_MODE                       PM0_bit.no2 // v7 only
#define BUZ0                          P0_bit.no2 // v7 only

#define BLE_POW_CNT_MODE              PM0_bit.no4   // Input , v7 only
#define BLE_POW_CNT                   P0_bit.no4    // Input , v7 only

#define LORA_CTS_MODE                 LORA_READY_MODE      // Output ,v7 only
#define BLE_CTS_MODE                  PM7_bit.no6      // Output ,v7 only

#define LORA_RTS                        LORA_STA        // Input , v7 only
#define LORA_CTS                        LORA_READY      // Output ,v7 only
#define BLE_RTS                         P13_bit.no7    // Input , v7 only
#define BLE_CTS                         P7_bit.no6      // Output ,v7 only


#define H1D_LED                         P7_bit.no7      // Output ,v7 only
/*BOARD SUPPORT AREA END*/

#define LORA_SLEEP_WAIT_TIME      5
#define LORA_CYCLE_TIME_BASE       30 // unit : second
#define BLE_FACTORY_SETTING_FINISH 0xFF
#define BLE_SHUT_DOWN 0xA5
enum POWER_MODE
{
    POWER_SAVING,
    POWER_NORMAL,
    POWER_STOP_CHARGING,
};
extern enum POWER_MODE power_mode;
enum ADC_MODE
{
    BATTERY_VOLTAGE_FETCH,
    ADC_STANDBY
};
extern enum ADC_MODE adc_mode;

enum PT100_DATA_FETCH_RESULT_TYPE
{
    PT100_SUCCESS,
    PT100_LINE_ERROR,
    PT100_SENSE_ERROR,
};
extern enum PT100_DATA_FETCH_RESULT_TYPE pt100_data_fetch_result_type;
extern enum ADC_MODE adc_mode;


enum ble_process_t
{
    BLE_PROCESS_END                         ,
    BLE_PROCESS_START                       ,
    BLE_SET_NAME                            ,
    BLE_CHECK_ENTER_BINARY_MODE,
    BLE_CHECK_COMMAND                         ,

    BLE_CHANGE_LORA_FETCH_TIME                ,// A1020x00
    BLE_SEND_DATA_TO_PHONE                    ,// A2020000
    BLE_BINARY_MODE_EXIT,
    BLE_GOTO_SLEEP                              ,// A3020000
    BLE_TEMPERATURE_OFFSET                    ,// A402
    BLE_POWER_OFF                      ,
} ;
extern enum ble_process_t ble_process;


enum lora_process_t
{
    LORA_PROCESS_END                    ,
    LORA_PROCESS_START                  ,
    LORA_INIT_CHECK,
    LORA_SEND_MESSAGE                   ,
    LORA_SEND_MESSAGE2                   ,
    LORA_POWER_OFF                      ,
} ;
extern enum lora_process_t lora_process;

enum battery_process_t
{
    BATTERY_PROCESS_END                     ,
    BATTERY_PROCESS_START                   ,
    POWER_OFF_BATTERY_FETCH                 ,
    SAVE_BAT_DATA                           ,
} ;
extern enum battery_process_t battery_process;

enum dsadc_process_t
{
    DSADC_PROCESS_END                     ,
    DSADC_PROCESS_START                   ,
    DSADC_PROCESS_WAIT_FETCH_FINISH       ,    
    POWER_OFF_DSADC_FETCH                 ,
    SAVE_DSADC_DATA                       ,
} ;
extern enum dsadc_process_t dsadc_process;

#define	VBAT_ADC_RAW_CONVERTION_RATE					 	0.01886809269; // ((ADCvalue/255) * 1.45) * ((R5+R6)/R6)
extern uint32_t adc10_mean;
// SHOULD SET TIMER LET TIME_SPEED * BASE_TIME = 1 MIN
#define TEST_FACTORY_MODE_NOW           0x00
#define RTC_TIME_SPEED 60
#define LORA_START_TIME_DELAY_SEC 10
#define WORK_WITH_E1 1

//########################################################################
#define TIMER_PERIODIC_EVENT                        0x01 // should not use in V4 version , change to OVER_TIME_EVENT 
#define DSADC_NOTIFICATION_EVENT                    0x02
#define BLE_NOTIFICATION_EVENT                      0x04
#define ADC8_NOTIFICATION_EVENT                     0x08
#define ADC10_NOTIFICATION_EVENT                    0x10
#define UART0_NOTIFICATION_EVENT                    0x20
#define RTC_NOTIFICATION_EVENT                      0x40
#define UART1_NOTIFICATION_EVENT                    0x80


// DATA FLASH REGISTER 
#define   TESTED                          0   // 1 BYTE  , SHOW H/W TESTED 
#define   HARDWARE                        1   // 1 BYTE  , SUPPORTED HW
#define   LORA_INTV                       2   // 1 BYTE
#define   PT100_R_LINE                    3   // 2 BYTE 
#define   DSADC_TEMPERATURE_SENSOR_OFFSET 5   // 2 BYTE 
#define   FW_VERSION                      7   // 1 BYTE  




#define BLE_CONNECTED               0x01
#define BLE_DISCONNECTED            0x00

#define EEPROM_SLAVE_ADDR_B0           0xa0
#define EEPROM_SLAVE_ADDR_B1           0xa2



#define MAX_ADC_BUF 8

#define PIN_MODE_AS_INPUT 1U
#define PIN_MODE_AS_OUTPUT 0U
#define PIN_LEVEL_AS_LOW 0
#define PIN_LEVEL_AS_HIGH 1




#define POWER_OFF  1
#define POWER_ON  0




#define TRUE 1
#define FALSE 0

/* Data flash Definitions for process switching */
#define R_PFDL_SAM_TARGET_ERASE TRUE    /* Pre-erase setting (executed for TRUE)       */
#define R_PFDL_SAM_DIRECT_READ  TRUE    /* Setting for direct reading of data flash    */
                                        /* Note: This is possible only at byte access  */
                                        /* with accesses to data flash memory enabled. */
/* Basic data */
#define DATA_FLASH_BLOCK_SIZE   0x400l  /* Standard block size                     */
#define DATA_FLASH_TARGET_BLOCK 0       /* Writing start block (0x0:F1000H)        */
#define DATA_FLASH_SIZE   10      /* Write data size                         */
#define DATA_FLASH_DREAD_OFSET  0x1000  /* Direct reading offset address           */

/* PFDL initial settings */
#define R_PFDL_SAM_FDL_FRQ      16      /* Sets the frequency (16 MHz)             */
#define R_PFDL_SAM_FDL_VOL      0x01    /* Sets the voltage mode (full-speed mode) */

/* Control macro for sample program */
#define R_PFDL_SAM_WDT_RESET()  WDTE = 0xAC /* Watchdog timer reset                */

#define MAX_BLE_DATA_LENGTH 160
#define BLE_CMD_LENGTH 2


#define I2C_ERROR 0x01
#define I2C_RECEIVED_END 0x02
#define I2C_SEND_END 0x04
#define EEPROM_DATA_SIZE 2 //unit:(bytes)
#define EEPROM_READ_BLOCK_SIZE 100 //unit:(bytes)
#define TOTAL_RECORD_QTY 43200 //unit:(QTY)
#define LORA_ERROR_BYTE 4
#define FCNT_START 121
#define MAX_LORA_SENDING_PROCESS_TIMES 3


#define BLE_BUFFER_SIZE 160
#define LORA_BUFFER_SIZE 160

#define eepromIndexStorageAddressinEEPROM 90000




extern uint8_t sendToLora[20];
extern uint8_t lora_process_timeout_counter;
extern uint8_t ble_process_timeout_counter;
extern uint8_t mode;
extern float Rpt100;
extern uint8_t events;
extern uint8_t dsadc_ready;
extern int dsadc_temperature;

extern uint8_t USER_DSADC_temperature_calibration_process;
extern int16_t user_Temperature;
extern uint8_t board[DATA_FLASH_SIZE];
extern uint16_t Record_Data;
extern void DataFlashWrite(void);
extern void goToSleep(void);
extern void setLoraIntervalTime(uint8_t lora_intv);
extern int32_t Vm0; // uV
extern int32_t Vm1; // uV
extern int32_t Vm2; // uV
extern int32_t Vm3; // uV
extern uint8_t receivedFromBle[160];
extern uint8_t F_Done;
extern uint8_t LORA_F_Done;
extern uint8_t EEPROM_F_Done;
extern uint8_t PT100_F_Done;
extern uint8_t PCB_TEMPERATURE_F_Done;
extern uint8_t BLE_F_Done;
extern uint8_t setBleDeviceNameCommand[];
extern uint16_t K;
extern uint8_t ADC8;
extern uint8_t ADC10;
extern uint8_t lora_process_timer_counter;
extern uint16_t lora_countdown_sec;
extern uint8_t ble_received_end;
extern uint8_t semaphore;
extern uint8_t sendToBle[160];
extern double pt100_temperature ;
extern double pcb_temperature ;
extern uint8_t ble_process_timer_counter;
extern uint16_t record_data[RECORD_DATA_SIZE] ;
extern int16_t record_data_index ;
extern uint16_t record_qty;
/* End user code. Do not edit comment generated here */
#endif
