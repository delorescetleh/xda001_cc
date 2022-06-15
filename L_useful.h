#include "r_cg_macrodriver.h"
// #include "r_cg_cgc.h"
// #include "r_cg_port.h"
// #include "r_cg_it8bit.h"
// #include "r_cg_it.h"
// #include "r_cg_pga_dsad.h"
// #include "r_cg_sau.h"
// #include "r_cg_iica.h"
// #include "r_cg_intp.h"
// #include "user_macro.h"
// #include "..\FDL\CA78K0R_110\lib\pfdl.h"
// #include "FDL\CA78K0R_110\lib\pfdl_types.h"
// #include "cg_src\r_cg_it.h"
#include "r_cg_userdefine.h"
#include "r_cg_dtc.h"

// extern void getFactroySetting(uint8_t *hardWareSetting_ptr, uint8_t *factorySetting_ptr, uint8_t *dubReadBuffer_ptr);

// /* Sample functions for target board */
// void    r_pfdl_samTargetInit( void );    /* Target board initialization processing */
// void    r_pfdl_samTargetEnd ( void );    /* Target board end processing            */
// /* Sample functions for writing program */
// pfdl_status_t r_pfdl_samFdlStart ( void  );  /* pfdl initialization processing */
// void          r_pfdl_samFdlEnd   ( void );  /* pfdl end processing            */
// extern void dataFlashStart(void);
// extern pfdl_status_t dataFlashRead(pfdl_u08 *dubReadBuffer_ptr, pfdl_u16 duhReadAddress);
// extern pfdl_status_t dataFlashWrite(pfdl_u08 *dubWriteBuffer_ptr, pfdl_u16 duhWriteAddress);
// extern void dataFlashEnd(void);

// uint8_t getPCB_Temperature_when_in_Factory();
extern uint8_t memcpy(uint8_t *target, uint8_t *source, uint8_t length);
// uint8_t timeOut(uint32_t timeOutMSeconds);
// void setTimeOut(uint32_t userSetTimeOutBase);

// pfdl_status_t waitDataFlashProcess(void);
extern void delayInMs(uint32_t ms);
extern uint8_t memcmp(uint8_t *target, uint8_t *source, uint8_t length, uint8_t maxLength);
extern void memclr(uint8_t *target, uint8_t clearlegnth);