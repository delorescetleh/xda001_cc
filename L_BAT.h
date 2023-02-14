#include "r_cg_userdefine.h"


#ifndef _BAT_DEF_H
#define _BAT_DEF_H
typedef struct battery_struct
{
    uint8_t fetch_finish;
    float Vbat;
} battery_data_t;

void BATTERY_VOLTAGE_FETCH_START(void);
void BATTERY_VOLTAGE_FETCH_STOP(void);
void BATTERY_PROCESS(void);
void battery_procedure(void);
void battery_procedure_init(battery_data_t *_battery);
#endif
