#include "r_cg_userdefine.h"
#define PCB_TEMPERATURE_PROCESS_START 5
#define PCB_TEMPERATURE_PROCESS_END 1
#define PCB_TEMPERATURE_PROCESS_TIMEOUT 10

extern uint8_t pcb_temperature_process;
extern uint8_t pcb_temperature_process_timeout_counter;
extern int pcb_temperature;
extern void L_PCB_TEMP_procedure(void);