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
* File Name    : r_cg_iica_user.c
* Version      : Code Generator for RL78/H1D V1.00.02.01 [25 Nov 2020]
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for IICA module.
* Creation Date: 2022/6/16
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_iica.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_iica0_interrupt(vect=INTIICA0)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
uint8_t iic_err_cnt = 0;
uint8_t iic_tx_end_cnt = 0;
uint8_t iic_rx_end_cnt = 0;
uint16_t iic_int_cnt = 0;


uint8_t iic_status = 0;
uint8_t iic_bytes[256] = {0};
uint8_t iic_device_sel = 0;

// uint32_t eepromReadingCounter = 0;
uint16_t storeIndex = TOTAL_RECORD_QTY;
uint16_t storeCounter = 0;
uint8_t eepromRandomReadSuccess = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_iica0_interrupt(void)
{
    if (0x80U == (IICS0 & _80_IICA_MASTER_DEVICE))
    {
        iica0_masterhandler();
    }
}
/***********************************************************************************************************************
* Function Name: iica0_masterhandler
* Description  : This function is IICA0 master handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_masterhandler(void)
{
    /* Control for communication */
    if ((0U == IICBSY0) && (g_iica0_tx_cnt != 0U))
    {
        r_iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if (0U == (g_iica0_master_status_flag & _80_IICA_ADDRESS_COMPLETE))
        {
            if (1U == ACKD0)
            {
                g_iica0_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;

                if (1U == TRC0)
                {
                    WTIM0 = 1U;     /* interrupt request is generated at the ninth clock's falling edge */

                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1U;     /* enable acknowledgment */
                    WTIM0 = 0U;     /* interrupt request is generated at the eighth clock's falling edge */
                    WREL0 = 1U;     /* cancel wait */
                }
            }
            else
            {
                r_iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;

                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        ACKE0 = 0U;     /* disable acknowledgment */
                        WTIM0 = 1U;     /* interrupt request is generated at the ninth clock's falling edge */
                        WREL0 = 1U;     /* cancel wait */
                    }
                    else
                    {
                        WREL0 = 1U;     /* cancel wait */
                    }
                }
                else
                {
                    r_iica0_callback_master_receiveend();
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_error
* Description  : This function is a callback function when IICA0 master error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_receiveend
* Description  : This function is a callback function when IICA0 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_receiveend(void)
{
    SPT0 = 1U;      /* stop condition is generated */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_iica0_callback_master_sendend
* Description  : This function is a callback function when IICA0 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_master_sendend(void)
{
    SPT0 = 1U;      /* stop condition is generated */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

uint8_t eepromRandomRead(uint8_t adr, uint8_t * const buf, uint16_t num, uint8_t wait){
    uint16_t c = 0;
    R_IICA0_Master_Send(adr,buf, 2, wait);
    while (TRC0){
        c++;
        if (c>65500){
            return 0;
        }
    }
    STT0 = 1U; 
    R_IICA0_Master_Receive(adr , buf, num, wait);
    return 1;
}
void L_EEPROM_INIT(void){
    EPROM_POW_CNT = 0;
    R_IICA0_Create();
    delayInMs(1);
}
void L_EEPROM_STOP(void){
    R_IICA0_Stop();
    EPROM_POW_CNT = 1;
}
extern void doEepromReadRecords(void){
    uint32_t index = storeIndex;
    uint8_t maxReadQty = EEPROM_READ_BLOCK_SIZE/EEPROM_DATA_SIZE;
    // uint32_t readIndex = storeIndex;
    uint8_t dataByteLength = 0;
    uint32_t i2cAccessIndex;
    uint32_t restQty = storeCounter;
    uint8_t sendDataToBle[160]={0};
    L_EEPROM_INIT();

    sendDataToBle[0] = (uint8_t)(storeCounter >> 8);
    sendDataToBle[1] = (uint8_t)storeCounter;
    R_UART1_Send(sendDataToBle, 2);
    delayInMs(10);
    while (restQty)
    {
        if (BLE_NO_CONNECT){
            break;
        }
        if ((index+maxReadQty)>TOTAL_RECORD_QTY){
            if (storeCounter==TOTAL_RECORD_QTY){
                restQty = storeCounter - (TOTAL_RECORD_QTY - index);
                index = 0;
            } else {
                restQty = 0;
            }
            i2cAccessIndex = index * EEPROM_DATA_SIZE;
            dataByteLength =(uint8_t)(TOTAL_RECORD_QTY-index)*EEPROM_DATA_SIZE;
        }
        else
        {
            i2cAccessIndex = index * EEPROM_DATA_SIZE;
            dataByteLength = (maxReadQty)*EEPROM_DATA_SIZE;
            index = index + maxReadQty;
            restQty = restQty - maxReadQty;
        }
        
        setEepromAccessAddress(i2cAccessIndex);
        eepromRandomReadSuccess=eepromRandomRead(iic_device_sel, &iic_bytes[0], (uint16_t)dataByteLength+2, 0x7f);
        delayInMs(10);
        memcpy(sendDataToBle,&iic_bytes[2],dataByteLength);
        R_UART1_Send(sendDataToBle,dataByteLength);
	delayInMs(10);
        if (!eepromRandomReadSuccess){
            break;
        }
    }
    L_EEPROM_STOP();
    delayInMs(10);
}

void doEepromWriteRecords(void){
    uint16_t data = 0; // getTemperatureDataForEeprom();
    uint32_t i2cAccessIndex = ((uint32_t)(storeIndex))*EEPROM_DATA_SIZE;
    setEepromAccessAddress(i2cAccessIndex);
    L_EEPROM_INIT();
    delayInMs(2);
    iic_bytes[2] = (uint8_t) (data>>8);
    iic_bytes[3] = (uint8_t) (data>>0);
    R_IICA0_Master_Send(iic_device_sel, iic_bytes, EEPROM_DATA_SIZE+2, 0x7f);
    // R_IICA0_Stop();
    // R_IICA0_Create();
    setEepromAccessAddress(i2cAccessIndex);
    delayInMs(10);
    eepromRandomReadSuccess=eepromRandomRead(iic_device_sel, &iic_bytes[0], 2, 0x7f);
    storeIndex--;
    if (storeIndex>TOTAL_RECORD_QTY){
        storeIndex = TOTAL_RECORD_QTY;
    }
// save to eeprom
    setEepromAccessAddress(eepromIndexStorageAddressinEEPROM);
    iic_bytes[2] = (uint8_t) (storeIndex>>8);
    iic_bytes[3] = (uint8_t) (storeIndex>>0);
    R_IICA0_Master_Send(iic_device_sel, iic_bytes, EEPROM_DATA_SIZE+2, 0x7f);
    L_EEPROM_STOP();
    
    storeCounter++;
    if (storeCounter > TOTAL_RECORD_QTY)
    {
        storeCounter = TOTAL_RECORD_QTY;
    }
}
void setEepromAccessAddress(uint32_t i2cAccessIndex){

    if (i2cAccessIndex < 0x10000)
    {
        iic_device_sel = EEPROM_SLAVE_ADDR_B0;
        iic_bytes[0] = (uint8_t)(i2cAccessIndex >> 8); // Data Addr HI
        iic_bytes[1] = (uint8_t)(i2cAccessIndex >> 0); // Data Addr LO
    }
    else
    {
        iic_device_sel = EEPROM_SLAVE_ADDR_B1;
        iic_bytes[0] = (uint8_t)((i2cAccessIndex&0x0000FFFF)>>8); // Data Addr HI
        iic_bytes[1] = (uint8_t)((i2cAccessIndex&0x0000FFFF)>>0); // Data Addr LO
    }

}

uint16_t getStoreIndexFromEEPROM(void){
    uint16_t result;
    setEepromAccessAddress(eepromIndexStorageAddressinEEPROM);
    eepromRandomRead(iic_device_sel, &iic_bytes[0], 2, 0x7f);
    result = (iic_bytes[0] << 8) || (iic_bytes[1] << 0);
    return result;
}


void clearEeprom(void){
    uint32_t i2cAccessIndex = ((uint32_t)(storeIndex))*EEPROM_DATA_SIZE;
    const uint8_t PAGE_SIZE=200;
    uint16_t index=0,i=0;
    
    L_EEPROM_INIT();
    delayInMs(1);

    i2cAccessIndex = 0;
    setEepromAccessAddress(i2cAccessIndex);
    eepromRandomReadSuccess=eepromRandomRead(iic_device_sel, &iic_bytes[0], 200, 0x7f);

    for (i = 0; i <= 200;i++){
        iic_bytes[i] = 0xFF;
    }

    for (index = 0; index < 500; index++)
    {
        i2cAccessIndex = index * PAGE_SIZE;
        setEepromAccessAddress(i2cAccessIndex);
        R_IICA0_Master_Send(iic_device_sel, iic_bytes, PAGE_SIZE + 2, 0x7f);
        delayInMs(10);
    }

    i2cAccessIndex = 0;
    setEepromAccessAddress(i2cAccessIndex);
    eepromRandomReadSuccess=eepromRandomRead(iic_device_sel, &iic_bytes[0], 200, 0x7f);
    
    L_EEPROM_STOP();
}

/* End user code. Do not edit comment generated here */
