// #include "L_normal.h"
// #include "r_cg_userdefine.h"

// void normal_process(void){
//     dataFlashStart();
//     dataFlashRead((pfdl_u08 *)&board,0);
//     dataFlashEnd();
//     if(L_BLE_INIT()){
//         R_INTC1_Start();
//         ble_shutdown_process = 0;
//     }else{
//         ble_shutdown_process = BLE_SHUTDOWN_START;
//         R_IT8Bit0_Channel1_Start();
//     }

//     R_RTC_Start();
//     while (1)
//     {
//         if(low_power){
//             L_LORA_STOP();
//             LORA_RESET = PIN_LEVEL_AS_LOW;
//             L_BLE_STOP();
//             BLE_RESET = PIN_LEVEL_AS_LOW;
//             R_PGA_DSAD_Stop();
//             L_EEPROM_STOP();
//             P_STATUS = PIN_LEVEL_AS_HIGH;
//             break;
//         }
//         if(events)
//         {
//             if(events & BLE_TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel1 , 200mS
//             {
//               events &= ~BLE_TIMER_PERIODIC_EVENT;
//               if (ble_shutdown_process)
//               {
//                 L_BLE_shutdown_procedure();
//               }
//               if (ble_connect_process)
//               {
//                 L_BLE_connect_procedure();
//               }
//             if((!ble_connect_process)&&(!ble_shutdown_process))
//             {
//                 R_IT8Bit0_Channel1_Stop();
//             }
//             }
//             if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 400mS
//             {
//                 events &= ~TIMER_PERIODIC_EVENT;
//                 if (pt100_process)
//                 {
//                     L_PT100_Procedure();
//                 }
//                 if (pcb_temperature_process)
//                 {
//                     L_PCB_TEMP_procedure();
//                 }
//                 if (lora_process)
//                 {
//                     L_Lora_procedure();
//                 }
//                 if (eeprom_process)
//                 {
//                     L_EEPROM_procedure();
//                 }
//                 if ((!pt100_process) && (!pcb_temperature_process) && (!lora_process))
//                 {
//                     R_IT8Bit0_Channel0_Stop();
//                 }
//             }
//         }
//         if ( (!pt100_process) && (!pcb_temperature_process) && (!lora_process)&& (!eeprom_process)&&(!ble_connect_process)&&(!ble_shutdown_process))
//         {
//             goToSleep();
//         }
//     }
// }