#include "r_cg_userdefine.h"
#define LORA_PROCESS_START 15
#define LORA_PROCESS_END 1
#define LORA_PROCESS_TIMEOUT 50
#define MAX_LORA_RECEIVE 6

extern uint8_t receivedFromLora[MAX_LORA_RECEIVE];
extern uint8_t lora_process;
extern void L_Lora_procedure(void);
void L_Lora_procedure_init(void);
void resetLoRaCounter(void);
uint8_t countDownLoRaCounter(void);