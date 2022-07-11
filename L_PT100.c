#include "L_PT100.h"
#include "r_cg_userdefine.h"
extern uint8_t dsadc_ready=0;
double pt100_line_voltage=0; // mv
int dsadc_temperature=0;
int dsadc_temperature_with_offset = 0;
uint16_t convertTemperatureFormat(void);
void prepareDataToLora(void);
extern uint8_t pt100_process=PT100_PROCESS_START;
extern uint8_t pt100_process_timeout_counter= PT100_PROCESS_TIMEOUT;
extern uint32_t Rpt100 = 0;
void L_PT100_Procedure(void)
{
    pt100_process_timeout_counter--;
    if (!pt100_process_timeout_counter)
    {
        pt100_process = PT100_PROCESS_END;
    }
    switch (pt100_process)
    {
        case PT100_PROCESS_START:
            clr_dsadc_buf();
            L_EEPROM_INIT();
            R_PGA_DSAD_Create();
            R_AMP_Create();
            R_AMP1_Start();
            R_AMP2_Start();
            R_AMP_Set_PowerOn();
            pt100_process--;
            break;
        case 14:
            R_PGA_DSAD_Start();
            pt100_process--;
            break;
        case 13:
            if (!pcb_temperature_process)
            {
                pt100_process--;
            }
            break;    
        case 12:
            if (dsadc_ready)
            {
                dsadc_ready = 0;
                pt100_process--;
            }
            break;
        case 11:
            L_get_pt100_result(&dsadc_temperature);
            dsadc_temperature_with_offset = dsadc_temperature + boardOffset();
            Record_Data = convertTemperatureFormat();
            pt100_process--;
            break;
        case 10:
            prepareDataToLora();
            L_PGA_STOP();
            R_AMP1_Stop();
            R_AMP2_Stop();
            pt100_process=PT100_PROCESS_END;
            break;
        case PT100_PROCESS_END:
            if(mode==factory_mode)
            {
                if(pt100_process_timeout_counter)
                {
                    PT100_F_Done = 1;
                }
                else
                {
                    PT100_F_Done = 0;
                }
            }
            pt100_process_timeout_counter = PT100_PROCESS_TIMEOUT;
            pt100_process--;
            break;
        default:
            if (pt100_process)
            pt100_process--;
            break;
    }
}
uint16_t convertTemperatureFormat(void)
{
    return (dsadc_temperature_with_offset/5+100);
}
int16_t boardOffset(void){
    return (int16_t)(((int16_t) board[DSADC_TEMPERATURE_SENSOR_OFFSET+1]<<8)|board[DSADC_TEMPERATURE_SENSOR_OFFSET]);
}