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
* File Name    : r_cg_sau_user.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 2022/6/25
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_uart0_interrupt_send(vect=INTST0)
#pragma interrupt r_uart0_interrupt_receive(vect=INTSR0)
#pragma interrupt r_uart1_interrupt_send(vect=INTST1)
#pragma interrupt r_uart1_interrupt_receive(vect=INTSR1)
/* Start user code for pragma. Do not edit comment generated here */
# pragma address (receivedFromBle=0xFFC00U)
# pragma address (receivedFromLora=0xFF700U)
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_uart1_tx_address;         /* uart1 send buffer address */
extern volatile uint16_t  g_uart1_tx_count;            /* uart1 send data number */
extern volatile uint8_t * gp_uart1_rx_address;         /* uart1 receive buffer address */
extern volatile uint16_t  g_uart1_rx_count;            /* uart1 receive data number */
extern volatile uint16_t  g_uart1_rx_length;           /* uart1 receive data length */
/* Start user code for global. Do not edit comment generated here */
uint8_t portP3Status=0;
uint8_t sendToLora[20];
uint8_t receivedFromLora[20];
uint8_t LoraReceivedEnd;
uint8_t maxLoraReceiveLength=18;

uint8_t LORA_ID[4];
uint8_t receivedFromBle[160];
uint8_t sendToBle[160] = {0};
uint8_t BleReceivedEnd = 0;
uint8_t setBleDeviceNameCommand[] = {'S','N',',','B','L','E','-','1','2','3','4','\r'};
uint8_t APP_SET_LORA_INTERVAL[] = {0xA1,0x02};
uint8_t APP_READ_EEPROM[] = {0xA2,0x02};
uint8_t APP_SHUT_DOWN_BLE[] = {0xA3,0x02};
uint8_t APP_SET_TEMP_CALIBRARTION[] = {0xA4,0x02};
uint8_t APP_GET_ECHO[] = {0xA5,0x02};
uint8_t *appParam;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}
/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    LoraReceivedEnd = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_receive
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR03 & 0x0007U);
    SIR03 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart1_callback_error(err_type);
    }
    
    rx_data = RXD1;

    if (g_uart1_rx_length > g_uart1_rx_count)
    {
        *gp_uart1_rx_address = rx_data;
        gp_uart1_rx_address++;
        g_uart1_rx_count++;

        if (g_uart1_rx_length == g_uart1_rx_count)
        {
            r_uart1_callback_receiveend();
        }
    }
    else
    {
        r_uart1_callback_softwareoverrun(rx_data);
    }
}
/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_send
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_send(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
    else
    {
        r_uart1_callback_sendend();
    }
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_receiveend
* Description  : This function is a callback function when UART1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_softwareoverrun
* Description  : This function is a callback function when UART1 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_sendend
* Description  : This function is a callback function when UART1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart1_callback_error
* Description  : This function is a callback function when UART1 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
uint8_t checkLoraMessage(void){
    int i=5 ,j=4;
    R_DTCD8_Stop();
    while (1)
    	{
        	if(receivedFromLora[i] == '}'){
                while (j)
                {
                    i--;
                    setBleDeviceNameCommand[j+6]=(receivedFromLora[i]);
                    j--;
                }
                return 1;
        }
        i++;
        if(i>20){
            break;
        }
    }
    R_DTCD8_Start();
    return 0;
}
uint8_t doSendLoraData(uint16_t temp, uint16_t pcbTemp)
{
    sendToLora[0] = '{';
    sendToLora[1] = (uint8_t)((temp/100) + 0x30);
    sendToLora[2] = (uint8_t)(((temp%100)/10)+ 0x30);
    sendToLora[3] = (uint8_t)((temp%10)  + 0x30);
    sendToLora[4] = (uint8_t)((pcbTemp/100) + 0x30);
    sendToLora[5] = (uint8_t)(((pcbTemp%100)/10)+ 0x30);
    sendToLora[6] = (uint8_t)((pcbTemp%10)  + 0x30);
    sendToLora[7] = '}';
    R_UART0_Send(sendToLora, 8);
    return 1;
}
void L_LORA_STOP(void){
    LORA_POW_CNT = POWER_OFF;
    R_UART0_Stop();
    LORA_READY_MODE = PIN_MODE_AS_INPUT;
    UART0_TXD_MODE = PIN_MODE_AS_INPUT;
}
uint8_t L_LORA_INIT(void){
    memclr(receivedFromLora, maxLoraReceiveLength);
    LORA_RESET_MODE = PIN_MODE_AS_OUTPUT;
    LORA_READY_MODE = PIN_MODE_AS_OUTPUT;
    R_DTC_Create();
    R_UART0_Create();
    R_DTCD8_Start();
    R_UART0_Start();
    LORA_RESET_MODE = PIN_MODE_AS_OUTPUT;
    LORA_READY_MODE = PIN_MODE_AS_OUTPUT;
    LORA_READY = PIN_LEVEL_AS_HIGH;

    LORA_RESET = PIN_LEVEL_AS_LOW;
    LORA_POW_CNT = PIN_LEVEL_AS_LOW;
    delayInMs(10);
    LORA_RESET = PIN_LEVEL_AS_HIGH;
    
    return 0;
}


static void doBleTask_AppGetEcho(void){
    uint8_t bleAck[4] = {0xa5, 0x02, 0x00, 0x00};// ble ack to app
    bleAck[2] = *appParam;
    bleAck[3] = *(appParam+1);
    memcpy(sendToBle, bleAck, 4);
    R_UART1_Send(sendToBle, 4);
}

static void doBleTask_SetLoraInterval(void){
    loraProcessIntervalTime = *appParam;
    // Save into DataFlash
    dataFlashStart();
    dataFlashRead(dubReadBuffer,0);
    *(dubReadBuffer+F_LORA_INTV_BYTE)=*appParam;
    memcpy(dubWriteBuffer, dubReadBuffer, DATA_FLASH_SIZE);
    dataFlashWrite(dubWriteBuffer,0);
    dataFlashEnd();
    //set ble ack to app
    sendToBle[0] = 0xA1;
    sendToBle[1] = 0x01;
    sendToBle[2] = 0x55;
    R_UART1_Send(sendToBle,(uint8_t) 3);
    lora_rtc_counter = 0;
    // resetLoRaCounter();// reset relative parameter
}

static void doBleTask_ShutDownBle(void){
    R_INTC1_Stop();
    bleProcess = 1; // count down to shut down BLE
    bleShutDownProcess = 100;
}

void checkAppCommand(void) {
    uint8_t offset;
    if (offset=memcmp(receivedFromBle,APP_SET_LORA_INTERVAL,2,160)){
        if (offset<158){
            appParam = receivedFromBle + offset + 1;
            doBleTask_SetLoraInterval();
        }
        memclr(receivedFromBle, 160);
        reset_DTC10();
    } else if (offset=memcmp(receivedFromBle,APP_READ_EEPROM,2,160))
    {
        if (offset<158){
            appParam = receivedFromBle + offset + 1;
            doEepromReadRecords();
        }
        memclr(receivedFromBle, 160);
        reset_DTC10();
    } else if (offset=memcmp(receivedFromBle,APP_SHUT_DOWN_BLE,2,160))
    {
        if (offset<158){
	    appParam = receivedFromBle + offset + 1;
            doBleTask_ShutDownBle();
        }
        memclr(receivedFromBle, 160);
        reset_DTC10();
    } else if (offset=memcmp(receivedFromBle,APP_SET_TEMP_CALIBRARTION,2,160))
    {
        if (offset<158){
            appParam = receivedFromBle + offset + 1;
        }
        memclr(receivedFromBle, 160);
        reset_DTC10();
    }else if (offset=memcmp(receivedFromBle,APP_GET_ECHO,2,160))
    {
        if (offset<158){
            appParam = receivedFromBle + offset + 1;
            doBleTask_AppGetEcho();
        }
        memclr(receivedFromBle, 160);
        reset_DTC10();
    }
}
void L_BLE_STOP(void){
    R_UART1_Stop();
    UART1_TXD_MODE = PIN_MODE_AS_INPUT;
    BLE_UART_RXD_IND = PIN_LEVEL_AS_HIGH;
}
uint8_t L_BLE_INIT(void){
    memclr(receivedFromBle, MAX_BLE_DATA_LENGTH);
    R_DTCD10_Start();
    R_UART1_Create();
    R_UART1_Start();
    BLE_RESET = PIN_LEVEL_AS_LOW;
    BLE_POW_CNT = POWER_ON;
    delayInMs(2);
    BLE_RESET = PIN_LEVEL_AS_HIGH;

    delayInMs(500);
    return memcmp(receivedFromBle, (uint8_t *)("%REBOOT%"),(uint8_t) 8, MAX_BLE_DATA_LENGTH);
}


void L_BLE_RESTART_FROM_STOP_MODE(void){
    // R_DTCD10_Start();
    R_UART1_Create();
    R_UART1_Start();
    BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
}

uint8_t L_BLE_SEND_COMMAND(char *command,uint8_t comandLength,char *expectAck,uint8_t ackLength ){
    memclr(receivedFromBle, reset_DTC10());
    R_UART1_Receive(receivedFromBle,ackLength);
    R_UART1_Send((uint8_t *)command, comandLength);
    delayInMs(500);
    return memcmp(receivedFromBle, (uint8_t *)expectAck, ackLength, MAX_BLE_DATA_LENGTH);
}

uint8_t L_BLE_FACTORY_MODE_SETTING(void){
    if (L_BLE_SEND_COMMAND("$$$", 3, "CMD>", 4))
    {
        if (L_BLE_SEND_COMMAND("SS,40\r", 6, "AOK", 3))
        {
            if (L_BLE_SEND_COMMAND((char *)setBleDeviceNameCommand, 12, "AOK", 3))
            {
                if (L_BLE_SEND_COMMAND("SW,0B,07\r", 9, "AOK", 3))
                {
                    if (L_BLE_SEND_COMMAND("SW,0A,04\r", 9, "AOK", 3))
                    {
                        if (L_BLE_SEND_COMMAND("SO,1\r", 5, "AOK", 3))
                        {
                            if (L_BLE_SEND_COMMAND("R,1\r", 4, "Rebooting", 9))
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void set_TXD1_as_Input_Mode(void){
    UART1_TXD_MODE = PIN_MODE_AS_INPUT;
}

void set_TXD0_as_Input_Mode(void){
    UART0_TXD_MODE = PIN_MODE_AS_INPUT;
}
/* End user code. Do not edit comment generated here */
