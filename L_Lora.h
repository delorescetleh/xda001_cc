#include "r_cg_userdefine.h"
#define LORA_PROCESS_START 12
#define LORA_PROCESS_END 1
#define LORA_PROCESS_TIMEOUT 50
extern uint8_t lora_process;
extern uint8_t lora_process_timeout_counter;
// extern int lora;
extern void L_Lora_procedure(void);
extern uint16_t convertTemperatureFormat(int dsadc_temperature);
