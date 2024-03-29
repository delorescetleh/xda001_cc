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
* Creation Date: 2023/1/23
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

# include "L_useful.h"
// # include "L_factory.h"
// # include "L_EEPROM.h"
// #include "L_PT100.h"
// #include "L_BLE.h"
// #include "L_PCB_TEMP.h"
// #include "L_Lora.h"
// # include "L_normal.h"
// SHOULD SET TIMER LET TIME_SPEED * BASE_TIME = 1 MIN
#define TEST_FACTORY_MODE_NOW           0x00
#define RTC_TIME_SPEED 60
#define LORA_START_TIME_DELAY_SEC 10
#define WORK_WITH_E1 1

//########################################################################
#define TIMER_PERIODIC_EVENT                        0x01 // should not use in V4 version , change to OVER_TIME_EVENT 
// #define DSADC_NOTIFICATION_EVENT                    0x02
// #define BLE_TIMER_PERIODIC_EVENT                    0x04
// #define LoRA_NOTIFICATION_EVENT                     0x08
// #define EEPROM_NOTIFICATION_EVENT                   0x10
// #define ADC_NOTIFICATION_EVENT                      0x20
#define RTC_CONSTPERIOD_EVENT                      0x40
// #define MINUTES_EVENT                               0x80

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

#define BLE_UART_RXD_IND      P5_bit.no3
#define BLE_UART_RXD_IND_MODE         PM5_bit.no3

#define BLE_STATUS_1          P12_bit.no1
#define BLE_NO_CONNECT        P12_bit.no1
#define BLE_NO_CONNECT_MODE   PM12_bit.no1
#define BLE_NO_CONNECT_MODE_PULL_UP PU12_bit.no1

#define BLE_RESET         P5_bit.no2
#define BLE_RESET_MODE         PM5_bit.no2//PM5_bit.no2

#define UART1_TXD_MODE         PM5_bit.no0
#define UART0_TXD_MODE         PM3_bit.no7
#define UART1_TXD             P5_bit.no0

#define IS_LORA_PROGRAMMING  (P0_bit.no7)
#define IN_FACTORY  !(P0_bit.no6)// P3_bit.no2

#define LORA_READY  P3_bit.no2// P3_bit.no2
#define LORA_READY_MODE   PM3_bit.no2
#define LORA_RESET  P3_bit.no5//P3_bit.no5
#define LORA_RESET_MODE   PM3_bit.no5
#define LORA_STA    P7_bit.no0
#define LORA_STA_MODE   PM7_bit.no0
#define LORA_POW_CNT    P7_bit.no1

#define MAX_ADC_BUF 8

#define PIN_MODE_AS_INPUT 1U
#define PIN_MODE_AS_OUTPUT 0U
#define PIN_LEVEL_AS_LOW 0
#define PIN_LEVEL_AS_HIGH 1

#define P_STATUS     P0_bit.no2//P0_bit.no2
#define P_STATUS_MODE   PM0_bit.no2

#define SWITCH_LORA  P0_bit.no6//P0_bit.no6

#define P_TEST       P0_bit.no1//P0_bit.no1


#define POWER_OFF  1
#define POWER_ON  0

#define BLE_POW_BYPASS_MODE  PM5_bit.no2
#define BLE_POW_BYPASS     P5_bit.no2
#define BLE_POW_CNT_MODE  PM0_bit.no4
#define BLE_POW_CNT     P0_bit.no4


//#define LORA_POW_CNT    P0_bit.no6 //v2

#define EPROM_POW_CNT   P0_bit.no4//P0_bit.no4

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
#define R_PFDL_SAM_FDL_FRQ      32      /* Sets the frequency (32 MHz)             */
#define R_PFDL_SAM_FDL_VOL      0x00    /* Sets the voltage mode (full-speed mode) */

/* Control macro for sample program */
#define R_PFDL_SAM_WDT_RESET()  WDTE = 0xAC /* Watchdog timer reset                */

#define MAX_SENSOR_FETCH_TIMES 10
#define MAX_BLE_DATA_LENGTH 160
#define BLE_CMD_LENGTH 2
/*factory setting value*/
#define F_READY           0x01 
#define F_ADC_READY       0x02
#define F_BLE_READY       0x04
#define F_DSADC_READY     0x08
#define F_EEPROM_READY    0x10
#define F_LORA_READY      0x20


/*hardwaresetting bytes define */
#define F_NO_BLE          0x01 
#define F_NO_EEPROM       0x02 
#define F_NO_LORA         0x04 
#define F_NO_PT100        0x08 
#define F_NO_K_SENSOR     0x10 

#define I2C_ERROR 0x01
#define I2C_RECEIVED_END 0x02
#define I2C_SEND_END 0x04
#define EEPROM_DATA_SIZE 2 //unit:(bytes)
#define EEPROM_READ_BLOCK_SIZE 100 //unit:(bytes)
#define TOTAL_RECORD_QTY 43200 //unit:(QTY)
#define LORA_ERROR_BYTE 4
#define FCNT_START 121
#define MAX_LORA_SENDING_PROCESS_TIMES 3



#define BLE_TOTAL_FACTORY_SETTING_COMMAND_NUMBER 7
#define BLE_FACTORY_SETTING_FINISH 0xFF
#define BLE_SHUT_DOWN 0xA5

#define BLE_TEMPERATURE_OFFSET 100
#define PT100_TEMPERATURE_OFFSET_BASE 0
#define SENSOR_FETCH_TIMES 4
#define eepromIndexStorageAddressinEEPROM 90000


#define lora_programming_mode 1
#define factory_test_mode 2
#define factory_mode 3
#define    normal_mode 4

extern uint8_t sendToLora[20];

extern uint8_t mode;
extern uint32_t Rpt100;
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
extern int32_t VM0; // uV
extern int32_t VM1; // uV
extern int32_t VM2; // uV
extern int32_t VM3; // uV
// extern double Vm0 ;
// extern double Vm1 ;
// extern double Vm2 ;
// extern double Vm3 ;
extern uint8_t F_Done;
extern uint8_t LORA_F_Done;
extern uint8_t EEPROM_F_Done;
extern uint8_t PT100_F_Done;
extern uint8_t PCB_TEMPERATURE_F_Done;
extern uint8_t BLE_F_Done;
extern uint8_t setBleDeviceNameCommand[];
extern uint16_t K;
extern uint8_t low_power;
/* End user code. Do not edit comment generated here */
#endif
