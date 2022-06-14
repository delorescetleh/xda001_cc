#include "L_useful.h"

// uint32_t timeOutBaseLine = 0;

// uint8_t PCB_Temperature_when_in_Factory = 0;

// uint8_t getPCB_Temperature_when_in_Factory(){
//     return PCB_Temperature_when_in_Factory;
// }

// void getFactroySetting(uint8_t *hardWareSetting_ptr, uint8_t *factorySetting_ptr, uint8_t *dubReadBuffer_ptr){
//     pfdl_status_t dtyFdlResult;
//     dataFlashStart();
//     dtyFdlResult=dataFlashRead(dubReadBuffer_ptr,0);
//     *hardWareSetting_ptr = *(dubReadBuffer_ptr + F_HAREWARE_SETTING_BYTE);
//     *factorySetting_ptr = *(dubReadBuffer_ptr + F_FACTORY_SETTING_BYTE);
//     if( *hardWareSetting_ptr==0xff){
//         *hardWareSetting_ptr = 0;
//         *factorySetting_ptr = 0; 
//     }
//     // value of PCB_Temperature_when_in_Factory, from 20 degC to 40 degC encoded by 0-255
//     PCB_Temperature_when_in_Factory=(uint16_t)(((*(dubReadBuffer_ptr + F_PCB_TEMPERATURE_BYTE))*0.7843+200));
//     dataFlashEnd();
// }

// void r_pfdl_samFdlEnd( void )
// {
//     /* Data flash library end processing */
//     PFDL_Close();
// }
// pfdl_status_t r_pfdl_samFdlStart( void )
// {
//     pfdl_status_t       dtyFdlResult;
//     pfdl_descriptor_t   dtyDescriptor;
    
//     /* Inputs the initial values */
//     dtyDescriptor.fx_MHz_u08            = R_PFDL_SAM_FDL_FRQ;  /* Sets the frequency    */
//     dtyDescriptor.wide_voltage_mode_u08 = R_PFDL_SAM_FDL_VOL;  /* Sets the voltage mode */
    
//     /* Executes the PFDL initialization function */
//     dtyFdlResult = PFDL_Open( &dtyDescriptor );
    
//     return dtyFdlResult;
// }
// pfdl_status_t waitDataFlashProcess( void ){
//     pfdl_status_t dtyFdlResult;
//     /* Waiting for command to end */
//     while (dtyFdlResult == PFDL_BUSY)
//     {
//         NOP();
//         NOP();
//         dtyFdlResult = PFDL_Handler();
//     }
//     return dtyFdlResult;
// }

// extern pfdl_status_t dataFlashWrite( pfdl_u08 *dubWriteBuffer_ptr, pfdl_u16 duhWriteAddress)
// {
//     pfdl_request_t  dtyRequester;       /* PFDL control variable (requester) */
//     pfdl_status_t       dtyFdlResult;
//     pfdl_descriptor_t   dtyDescriptor;
//     // pfdl_u08        dubWriteBuffer[ DATA_FLASH_SIZE ] = {0,1,2,3,4,5,6,7,8,9};
//     /* Inputs the initial values */
//     dtyDescriptor.fx_MHz_u08            = R_PFDL_SAM_FDL_FRQ;  /* Sets the frequency    */
//     dtyDescriptor.wide_voltage_mode_u08 = R_PFDL_SAM_FDL_VOL;  /* Sets the voltage mode */
    
//     /* Executes the PFDL initialization function */
//     dtyFdlResult = PFDL_Open( &dtyDescriptor );
//     /* Requester initialization processing */
//     dtyRequester.index_u16     = 0;
//     dtyRequester.data_pu08     = dubWriteBuffer_ptr;
//     dtyRequester.bytecount_u16 = 0;
//     dtyRequester.command_enu   = PFDL_CMD_READ_BYTES; /* Initializes with PFDL_CMD_READ_BYTES(= 0) */
    
//     /* If pre-erase is to be performed, perform blank check of the entire target block and then erase the block. */
//     if( dtyFdlResult == PFDL_OK )
//     {
//         /* Performs blank check process for the entire block. Sets the blank check command */
//         dtyRequester.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;
        
//         /* Sets the start address and execution range to 400H (1024) bytes */
//         dtyRequester.index_u16     = DATA_FLASH_TARGET_BLOCK * DATA_FLASH_BLOCK_SIZE;
//         dtyRequester.bytecount_u16 = DATA_FLASH_BLOCK_SIZE;
        
//         /* Command execution process */
//         dtyFdlResult = PFDL_Execute( &dtyRequester );
        
//         /* Waiting for command to end */
//         while( dtyFdlResult == PFDL_BUSY )
//         {
//             /* Perform any desired process (background operation) */
//             NOP();
//             NOP();
            
//             /* End confirmation process */
//             dtyFdlResult = PFDL_Handler();
//         }
        
//         /* If the target block is not in the blank state, executes the erase command */
//         if( dtyFdlResult == PFDL_ERR_MARGIN )
//         {
//             /* Performs erase process for the target block. Sets the erase command */
//             dtyRequester.command_enu = PFDL_CMD_ERASE_BLOCK;
            
//             /* Sets the block number of the target block */
//             dtyRequester.index_u16   = DATA_FLASH_TARGET_BLOCK;
            
//             /* Command execution process */
//             dtyFdlResult = PFDL_Execute( &dtyRequester );
            
//             /* Waiting for command to end */
//             while( dtyFdlResult == PFDL_BUSY )
//             {
//                 /* Perform any desired process (background operation) */
//                 NOP();
//                 NOP();
                
//                 /* End confirmation process */
//                 dtyFdlResult = PFDL_Handler();
//             }
//         }
//     }
    
//     /* Data writing process */
//     if( dtyFdlResult == PFDL_OK )
//     {
//         /* Variable definition for writing */
//         pfdl_u16    duhWriteAddress;
        
//         /* Before writing, performs blank check for the target area. */
//         /* Sets the blank check command                              */
//         dtyRequester.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;
//         /* Sets the write data size                                  */
//         dtyRequester.bytecount_u16 = DATA_FLASH_SIZE;
        
//         /* Write check loop (performs blank check and searches for an area which can be written to) */
//         for( duhWriteAddress  = DATA_FLASH_TARGET_BLOCK * DATA_FLASH_BLOCK_SIZE         ;
//              duhWriteAddress  < ( DATA_FLASH_TARGET_BLOCK + 1 ) * DATA_FLASH_BLOCK_SIZE ;
//              duhWriteAddress += DATA_FLASH_SIZE )
//         {
//             /* Sets the start address for writing */
//             dtyRequester.index_u16 = duhWriteAddress;
            
//             /* Command execution process */
//             dtyFdlResult = PFDL_Execute( &dtyRequester );
            
//             /* Waiting for command to end */
//             while( dtyFdlResult == PFDL_BUSY )
//             {
//                 /* Perform any desired process (background operation) */
//                 NOP();
//                 NOP();
                
//                 /* End confirmation process */
//                 dtyFdlResult = PFDL_Handler();
//             }
            
//             /* If in the blank state, performs writing */
//             if( dtyFdlResult == PFDL_OK )
//             {
//                 /* Sets the write command (write data and buffer have already been set) */
//                 dtyRequester.command_enu = PFDL_CMD_WRITE_BYTES;
                
//                 /* Command execution process */
//                 dtyFdlResult = PFDL_Execute( &dtyRequester );
                
//                 /* Waiting for command to end */
//                 while( dtyFdlResult == PFDL_BUSY )
//                 {
//                     /* Perform any desired process (background operation) */
//                     NOP();
//                     NOP();
                    
//                     /* End confirmation process and trigger process for each 1-byte writing */
//                     dtyFdlResult = PFDL_Handler();
//                 }
                
//                 /* If writing has been completed normally, executes the internal verification process */
//                 if( dtyFdlResult == PFDL_OK )
//                 {
//                     /* Sets the internal verification command */
//                     dtyRequester.command_enu = PFDL_CMD_IVERIFY_BYTES;
                    
//                     /* Command execution process */
//                     dtyFdlResult = PFDL_Execute( &dtyRequester );
                    
//                     /* Waiting for command to end */
//                     while( dtyFdlResult == PFDL_BUSY )
//                     {
//                         /* Perform any desired process (background operation) */
//                         NOP();
//                         NOP();
                        
//                         /* End confirmation process */
//                         dtyFdlResult = PFDL_Handler();
//                     }
//                 }
// #if R_PFDL_SAM_DIRECT_READ
// /* Confirms data by direct reading (this is possible only at byte access with accesses */
// /* to data flash memory enabled)                                                       */
// /* Note that data cannot be read correctly when data flash memory is being written to. */
//                 /* Data confirmation process */
//                 if( dtyFdlResult == PFDL_OK )
//                 {
//                     /* Variable definitions for reading */
//                     unsigned short        duh_i;        /* Loop variable definition */
//                     __near unsigned char *pubReadData;  /* Pointer for direct reading (only byte access is possible) */
                    
//                     pubReadData = (__near unsigned char *) duhWriteAddress + DATA_FLASH_DREAD_OFSET;
                    
//                     /* Compares read data and write data */
//                     for( duh_i = 0 ; duh_i < DATA_FLASH_SIZE ; duh_i++ )
//                     {
//                         if( *(dubWriteBuffer_ptr+ duh_i ) != pubReadData[ duh_i ] )
//                         {
//                             /* Error is generated when read data and write data do not match. */
//                             dtyFdlResult = PFDL_ERR_PARAMETER;
//                             break;
//                         }
//                     }
//                 }
// #else
// /* Uses the reading function for data flash libraries */
//                 /* Data confirmation process */
//                 if( dtyFdlResult == PFDL_OK )
//                 {
//                     /* Variable definitions for reading */
//                     pfdl_u16    duh_i;      /* Loop variable definition */
//                     pfdl_u08    dubReadBuffer [ DATA_FLASH_SIZE ];
//                                             /* Read data input buffer */
                    
//                     /* Sets the read command */
//                     dtyRequester.command_enu = PFDL_CMD_READ_BYTES;
                    
//                     /* Sets the read start address */
//                     dtyRequester.index_u16   = duhWriteAddress;
                    
//                     /* Sets the address for the read data input buffer */
//                     dtyRequester.data_pu08   = dubReadBuffer;
                    
//                     /* Command execution process */
//                     dtyFdlResult = PFDL_Execute( &dtyRequester );
                    
//                     /* Compares read data and write data */
//                     for( duh_i = 0 ; duh_i < DATA_FLASH_SIZE ; duh_i++ )
//                     {
//                         if( dubWriteBuffer_ptr[ duh_i ] != dubReadBuffer[ duh_i ] )
//                         {
//                             /* Error is generated when read data and write data do not match. */
//                             dtyFdlResult = PFDL_ERR_PARAMETER;
//                             break;
//                         }
//                     }
//                 }
// #endif
//                 break;
//             }
//             /* Processing is aborted when an unsolvable error has occurred. */
//             else if( dtyFdlResult != PFDL_ERR_MARGIN ) {
//                 break;
//             }
//         }
    
//     }
//     return dtyFdlResult;
// }
// extern pfdl_status_t dataFlashRead( pfdl_u08 *dubReadBuffer_ptr,pfdl_u16 duhReadAddress)
// {
//     pfdl_request_t dtyRequester;
//     /* Requester initialization processing */
//     dtyRequester.data_pu08   = dubReadBuffer_ptr;
//     dtyRequester.bytecount_u16 = DATA_FLASH_SIZE;
//     dtyRequester.command_enu   = PFDL_CMD_READ_BYTES;
//     dtyRequester.index_u16   = duhReadAddress;
//     return PFDL_Execute( &dtyRequester );
// }
// extern void dataFlashStart(void){
//     pfdl_status_t dtyFdlResult;
//     /* FDL initialization processing */
//     dtyFdlResult=r_pfdl_samFdlStart();
// }
// extern void dataFlashEnd(void){
//     r_pfdl_samFdlEnd();
// }
extern uint8_t memcpy(uint8_t *target, uint8_t *source, uint8_t length)
{
  while (length)
  {
    *(target) = *(source);
    length--;
    target++;
    source++;
  }
  return 1;
}

extern void delayInMs(uint32_t ms){
    const uint16_t ONE_MS_BASE = 800;
    ms = ms * ONE_MS_BASE;
    while(ms--);
}
// Success :1 , Fail: 0
uint8_t memcmp(uint8_t *target,uint8_t *source,uint8_t length,uint8_t maxLength){
    uint8_t i=0;
    while(i<maxLength){
        if(*(target)==*(source)){
            break;
        }
        if(i>(maxLength-length)){
            return 0;
        }
        target++;
	    i++;
    }
    for (i = 0; i < length; i++)
    {
        if(*(target+i)!=*(source+i)){
            return 0;
        }
    }
    return 1;
}
// void setTimeOut(uint32_t userSetTimeOutBase){
//     timeOutBaseLine = userSetTimeOutBase;
// }

// uint8_t timeOut(uint32_t timeOutMSeconds){
//   //  if( (millis()-timeOutBaseLine)>timeOutMSeconds){
//   //      return 1;
//   //  }
//    return 0;
// }