#include "r_cg_userdefine.h"
#ifndef _DSADC_DEF_H
#define _DSADC_DEF_H
typedef struct dsadc_struct
{
    uint8_t fetch_finish;
    float pt100_temperature;
    float pcb_temperature;
} dsadc_data_t;
void _convert_differential_value_as_uv_G64(int32_t *value);
void _convert_signal_end_value_as_uv(int32_t *value);
void _convert_differential_value_as_uv(int32_t *value, uint8_t g);
void DSADC_PROCESS(void);
void dsadc_procedure_init(struct dsadc_struct *_dsadc);
void dsadc_procedure(void);
void L_PT100_STOP(void);
void DSADC_PROCESS_TEST(void);
#endif