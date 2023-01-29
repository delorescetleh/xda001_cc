// #include "L_PCB_TEMP.h"
// // #include "r_cg_userdefine.h"

// extern int pcb_temperature=0;
// extern uint8_t pcb_temperature_process=0;
// extern uint8_t pcb_temperature_process_timeout_counter= PCB_TEMPERATURE_PROCESS_TIMEOUT;

// void L_PCB_TEMP_procedure_init(void){
//     pcb_temperature_process = PCB_TEMPERATURE_PROCESS_START;
//     pcb_temperature_process_timeout_counter = PCB_TEMPERATURE_PROCESS_TIMEOUT;
// }

// void L_PCB_TEMP_procedure(void)
// {
//     pcb_temperature_process_timeout_counter--;
//     if (!pcb_temperature_process_timeout_counter )
//     {
//        pcb_temperature_process = PCB_TEMPERATURE_PROCESS_END;
//     }
//     switch (pcb_temperature_process)
//     {
//         case PCB_TEMPERATURE_PROCESS_START:
//             init_pcb_temperature();
//             R_ADC_Create();
//             R_ADC_Start();
//             pcb_temperature_process--;
//             break;
//         case PCB_TEMPERATURE_PROCESS_END:
//             if(mode==factory_mode){
//                 if (pcb_temperature_process_timeout_counter){
//                     PCB_TEMPERATURE_F_Done = 1;
//                 }else{
//                     PCB_TEMPERATURE_F_Done = 0;
//                 }
//             }
//             pcb_temperature_process_timeout_counter= PCB_TEMPERATURE_PROCESS_TIMEOUT;
//             get_pcb_temperature(&pcb_temperature);
//             R_ADC_Stop();
//             ADCEN = 0U;
//             pcb_temperature_process--;
//         break;    
//         default:
//             pcb_temperature_process--;
//         break;
//     }
// }


