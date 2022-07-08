#include "r_cg_userdefine.h"
#define PT100_PROCESS_START 15
#define PT100_PROCESS_END 1
#define PT100_PROCESS_TIMEOUT 20

extern uint8_t pt100_process;
extern uint8_t pt100_process_timeout_counter;
extern int dsadc_temperature;
extern void L_PT100_Procedure(void);
int16_t boardOffset(void);