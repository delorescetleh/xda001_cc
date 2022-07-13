#include "r_cg_userdefine.h"
#define PT100_PROCESS_START 15
#define PT100_PROCESS_END 1
#define PT100_PROCESS_TIMEOUT 20

#define PT100_CALIBRATION_PROCESS_START 15
#define PT100_CALIBRATION_PROCESS_END 1
#define PT100_CALIBRATION_PROCESS_TIMEOUT 20

extern uint8_t pt100_process;
extern uint8_t pt100_process_timeout_counter;

extern uint8_t pt100_calibration_process;
extern uint8_t pt100_calibration_process_timeout_counter;

extern void L_PT100_Procedure(void);
extern void L_PT100_Calibration_Procedure(void);
void L_PT100_Calibration_Procedure_init(void);
void L_PT100_Procedure_init(void);