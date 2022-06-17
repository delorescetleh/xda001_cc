/**********************************************************************************************************************
    Program Name    : Sample program for Renesas Flash Driver (RFD RL78 Type01) (Data Flash)
    
    File Name       : sample_data_initsct.c
    Program Version : V1.00.00
    Device(s)       : RL78/G23 microcontroller
    Description     : Sample program for copying the sections for Data Flash Control. 
**********************************************************************************************************************/

/**********************************************************************************************************************
    DISCLAIMER
    This software is supplied by Renesas Electronics Corporation and is only intended for use with
    Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
    Corporation and is protected under all applicable laws, including copyright laws.
    THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
    TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
    ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
    BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    Renesas reserves the right, without notice, to make changes to this software and to discontinue the
    availability of this software. By using this software, you agree to the additional terms and conditions
    found by accessing the following link:
    http://www.renesas.com/disclaimer
    
    Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "sample_data_initsct.h"

/**********************************************************************************************************************
 * Function name : Sample_Data_INITSCT
 *********************************************************************************************************************/
/**
 *  Sample function for copying the sections for data flash control.
 *  
 *  @param[in]     -
 *  @return            -
 */
/*********************************************************************************************************************/


#define R_RFD_DSEC_MAX 1 /* The number of the data sections to copy */

const struct r_rfd_dsec_t {
        char __far *d_rom_sectop; /* Copy source section top address */
        char __far *d_rom_secend; /* Copy source section end address+1 */
        char __near *d_ram_sectop; /* Copy destination section top address */
} r_rfd_dsec_table[R_RFD_DSEC_MAX] = {
        {__sectop("RFD_DATA_n"),
         __secend("RFD_DATA_n"),
         (char __near *)__sectop("RFD_DATA_nR")}};

R_RFD_FAR_FUNC void Sample_Data_INITSCT(void)
{
        unsigned int i;
        char __far *rom_p;
        char __near *ram_p;

        for (i = 0; i < R_RFD_DSEC_MAX; i++) {
                rom_p = r_rfd_dsec_table[i].d_rom_sectop;
                ram_p = r_rfd_dsec_table[i].d_ram_sectop;
                for ( ; rom_p != r_rfd_dsec_table[i].d_rom_secend; rom_p++, ram_p++) {
                        *ram_p = *rom_p;
                }
        }
}
