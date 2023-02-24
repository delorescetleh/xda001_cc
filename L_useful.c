#include "L_useful.h"
extern uint8_t board[DATA_FLASH_SIZE] = {0};

void r_pfdl_samFdlEnd( void )
{
    /* Data flash library end processing */
    PFDL_Close();
}
pfdl_status_t r_pfdl_samFdlStart( void )
{
    pfdl_status_t       dtyFdlResult;
    pfdl_descriptor_t   dtyDescriptor;
    
    /* Inputs the initial values */
    dtyDescriptor.fx_MHz_u08            = R_PFDL_SAM_FDL_FRQ;  /* Sets the frequency    */
    dtyDescriptor.wide_voltage_mode_u08 = R_PFDL_SAM_FDL_VOL;  /* Sets the voltage mode */
    
    /* Executes the PFDL initialization function */
    dtyFdlResult = PFDL_Open( &dtyDescriptor );
    
    return dtyFdlResult;
}
pfdl_status_t waitDataFlashProcess(pfdl_status_t dtyFdlResult ){
    /* Waiting for command to end */
    while (dtyFdlResult == PFDL_BUSY)
    {
        NOP();
        NOP();
        dtyFdlResult = PFDL_Handler();
    }
    return dtyFdlResult;
}

extern pfdl_status_t dataFlashWrite( pfdl_u08 *dubWriteBuffer_ptr, pfdl_u16 duhWriteAddress)
{
    pfdl_request_t  dtyRequester;       /* PFDL control variable (requester) */
    pfdl_status_t       dtyFdlResult;
    pfdl_descriptor_t   dtyDescriptor;
    // pfdl_u08        dubWriteBuffer[ DATA_FLASH_SIZE ] = {0,1,2,3,4,5,6,7,8,9};
    /* Inputs the initial values */
    dtyDescriptor.fx_MHz_u08            = R_PFDL_SAM_FDL_FRQ;  /* Sets the frequency    */
    dtyDescriptor.wide_voltage_mode_u08 = R_PFDL_SAM_FDL_VOL;  /* Sets the voltage mode */
    
    /* Executes the PFDL initialization function */
    dtyFdlResult = PFDL_Open( &dtyDescriptor );
    /* Requester initialization processing */
    dtyRequester.index_u16     = 0;
    dtyRequester.data_pu08     = dubWriteBuffer_ptr;
    dtyRequester.bytecount_u16 = 0;
    dtyRequester.command_enu   = PFDL_CMD_READ_BYTES; /* Initializes with PFDL_CMD_READ_BYTES(= 0) */
    
    /* If pre-erase is to be performed, perform blank check of the entire target block and then erase the block. */
    if( dtyFdlResult == PFDL_OK )
    {
        /* Performs blank check process for the entire block. Sets the blank check command */
        dtyRequester.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;
        
        /* Sets the start address and execution range to 400H (1024) bytes */
        dtyRequester.index_u16     = DATA_FLASH_TARGET_BLOCK * DATA_FLASH_BLOCK_SIZE;
        dtyRequester.bytecount_u16 = DATA_FLASH_BLOCK_SIZE;
        
        /* Command execution process */
        dtyFdlResult = PFDL_Execute( &dtyRequester );
        
        /* Waiting for command to end */
        while( dtyFdlResult == PFDL_BUSY )
        {
            /* Perform any desired process (background operation) */
            NOP();
            NOP();
            
            /* End confirmation process */
            dtyFdlResult = PFDL_Handler();
        }
        
        /* If the target block is not in the blank state, executes the erase command */
        if( dtyFdlResult == PFDL_ERR_MARGIN )
        {
            /* Performs erase process for the target block. Sets the erase command */
            dtyRequester.command_enu = PFDL_CMD_ERASE_BLOCK;
            
            /* Sets the block number of the target block */
            dtyRequester.index_u16   = DATA_FLASH_TARGET_BLOCK;
            
            /* Command execution process */
            dtyFdlResult = PFDL_Execute( &dtyRequester );
            
            /* Waiting for command to end */
            while( dtyFdlResult == PFDL_BUSY )
            {
                /* Perform any desired process (background operation) */
                NOP();
                NOP();
                
                /* End confirmation process */
                dtyFdlResult = PFDL_Handler();
            }
        }
    }
    
    /* Data writing process */
    if( dtyFdlResult == PFDL_OK )
    {
        /* Variable definition for writing */
        pfdl_u16    duhWriteAddress;
        
        /* Before writing, performs blank check for the target area. */
        /* Sets the blank check command                              */
        dtyRequester.command_enu   = PFDL_CMD_BLANKCHECK_BYTES;
        /* Sets the write data size                                  */
        dtyRequester.bytecount_u16 = DATA_FLASH_SIZE;
        
        /* Write check loop (performs blank check and searches for an area which can be written to) */
        for( duhWriteAddress  = DATA_FLASH_TARGET_BLOCK * DATA_FLASH_BLOCK_SIZE         ;
             duhWriteAddress  < ( DATA_FLASH_TARGET_BLOCK + 1 ) * DATA_FLASH_BLOCK_SIZE ;
             duhWriteAddress += DATA_FLASH_SIZE )
        {
            /* Sets the start address for writing */
            dtyRequester.index_u16 = duhWriteAddress;
            
            /* Command execution process */
            dtyFdlResult = PFDL_Execute( &dtyRequester );
            
            /* Waiting for command to end */
            while( dtyFdlResult == PFDL_BUSY )
            {
                /* Perform any desired process (background operation) */
                NOP();
                NOP();
                
                /* End confirmation process */
                dtyFdlResult = PFDL_Handler();
            }
            
            /* If in the blank state, performs writing */
            if( dtyFdlResult == PFDL_OK )
            {
                /* Sets the write command (write data and buffer have already been set) */
                dtyRequester.command_enu = PFDL_CMD_WRITE_BYTES;
                
                /* Command execution process */
                dtyFdlResult = PFDL_Execute( &dtyRequester );
                
                /* Waiting for command to end */
                while( dtyFdlResult == PFDL_BUSY )
                {
                    /* Perform any desired process (background operation) */
                    NOP();
                    NOP();
                    
                    /* End confirmation process and trigger process for each 1-byte writing */
                    dtyFdlResult = PFDL_Handler();
                }
                
                /* If writing has been completed normally, executes the internal verification process */
                if( dtyFdlResult == PFDL_OK )
                {
                    /* Sets the internal verification command */
                    dtyRequester.command_enu = PFDL_CMD_IVERIFY_BYTES;
                    
                    /* Command execution process */
                    dtyFdlResult = PFDL_Execute( &dtyRequester );
                    
                    /* Waiting for command to end */
                    while( dtyFdlResult == PFDL_BUSY )
                    {
                        /* Perform any desired process (background operation) */
                        NOP();
                        NOP();
                        
                        /* End confirmation process */
                        dtyFdlResult = PFDL_Handler();
                    }
                }

/* Confirms data by direct reading (this is possible only at byte access with accesses */
/* to data flash memory enabled)                                                       */
/* Note that data cannot be read correctly when data flash memory is being written to. */
                /* Data confirmation process */
                if( dtyFdlResult == PFDL_OK )
                {
                    /* Variable definitions for reading */
                    unsigned short        duh_i;        /* Loop variable definition */
                    __near unsigned char *pubReadData;  /* Pointer for direct reading (only byte access is possible) */
                    
                    pubReadData = (__near unsigned char *) duhWriteAddress + DATA_FLASH_DREAD_OFSET;
                    
                    /* Compares read data and write data */
                    for( duh_i = 0 ; duh_i < DATA_FLASH_SIZE ; duh_i++ )
                    {
                        if( *(dubWriteBuffer_ptr+ duh_i ) != pubReadData[ duh_i ] )
                        {
                            /* Error is generated when read data and write data do not match. */
                            dtyFdlResult = PFDL_ERR_PARAMETER;
                            break;
                        }
                    }
                }

                break;
            }
            /* Processing is aborted when an unsolvable error has occurred. */
            else if( dtyFdlResult != PFDL_ERR_MARGIN ) {
                break;
            }
        }
    
    }
    return dtyFdlResult;
}
extern pfdl_status_t dataFlashRead( pfdl_u08 *dubReadBuffer_ptr,pfdl_u16 duhReadAddress)
{
    pfdl_request_t dtyRequester;
    /* Requester initialization processing */
    dtyRequester.data_pu08   = dubReadBuffer_ptr;
    dtyRequester.bytecount_u16 = DATA_FLASH_SIZE;
    dtyRequester.command_enu   = PFDL_CMD_READ_BYTES;
    dtyRequester.index_u16   = duhReadAddress;
    return PFDL_Execute( &dtyRequester );
}
extern pfdl_status_t dataFlashStart(void){
    /* FDL initialization processing */
    return r_pfdl_samFdlStart();
}
extern void dataFlashEnd(void){
    r_pfdl_samFdlEnd();
}

extern void memcpy(uint8_t *target, uint8_t *source, uint8_t length)
{
  while (length)
  {
    *(target) = *(source);
    length--;
    target++;
    source++;
  }
}

extern void delayInMs(uint32_t ms){
	const int16_t ONE_MS_BASE = 3000;
    int16_t one_ms = ONE_MS_BASE;
		while(ms!=0){
		while(one_ms!=0)
		{
				one_ms--;
		}
		one_ms = ONE_MS_BASE;
		ms--;
		}
}
// Success : final compare offset , Fail: 0
void replace_0_as_1_in_buffer(uint8_t *buffer,int length)
{
		uint8_t i = 0;
		for (i = 0; i < length; i++)
		{
		if(*(buffer+i)==0)
			*(buffer+i) = 1;
		}
}
extern void DataFlashWrite(void){
    dataFlashStart();
    dataFlashWrite((pfdl_u08 *)&board[0], 0);
    dataFlashEnd();
}