#include "r_cg_userdefine.h"

#define MAX_LORA_RECEIVE 10

// extern void L_Lora_procedure(void);
// void L_Lora_procedure_init(void);
// void resetLoRaCounter(void);
// uint8_t countDownLoRaCounter(void);


#ifndef _LORA_DEF_H
#define _LORA_DEF_H

void L_LORA_INIT(void);
void LORA_PROCESS(void);
void lora_procedure(void);
void lora_procedure_init(float *battery,float *pt100);
#endif