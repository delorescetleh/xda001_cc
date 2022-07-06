#include "L_PCB_TEMP.h"
#include "r_cg_userdefine.h"

extern int pcb_temperature=0;
extern uint8_t pcb_temperature_process=PCB_TEMPERATURE_PROCESS_START;
extern uint8_t pcb_temperature_process_timeout_counter=0;

void L_PCB_TEMP_procedure(void)
{
    pcb_temperature_process_timeout_counter++;
    if (pcb_temperature_process_timeout_counter > PCB_TEMPERATURE_PROCESS_TIMEOUT)
    {
       pcb_temperature_process = PCB_TEMPERATURE_PROCESS_END;
    }
    switch (pcb_temperature_process)
    {
        case PCB_TEMPERATURE_PROCESS_START:
            init_pcb_temperature();
            R_ADC_Create();
            R_ADC_Start();
            pcb_temperature_process--;
            break;
        case PCB_TEMPERATURE_PROCESS_END:
            get_pcb_temperature(&pcb_temperature);
            R_ADC_Stop();
            ADCEN = 0U;
            pcb_temperature_process--;
        break;    
        default:
            pcb_temperature_process--;
        break;
    }
}


