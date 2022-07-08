#include "L_normal.h"
#include "r_cg_userdefine.h"

void normal_process(void){
    dataFlashStart();
    dataFlashRead((pfdl_u08 *)&board,0);
    dataFlashEnd();
    R_INTC1_Start();
    R_UART1_Start();
    L_BLE_INIT();
    R_RTC_Start();
    while (1)
    {
        if(events)
        {
            if(events & BLE_TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel1 , 200mS
            {
              events &= ~BLE_TIMER_PERIODIC_EVENT;
              if (ble_shutdown_process)
              {
                L_BLE_shutdown_procedure();
              }
              if (ble_connect_process)
              {
                L_BLE_connect_procedure();
              }
            }
            if(events & TIMER_PERIODIC_EVENT)//R_IT8Bit0_Channel0 , 400mS
            {
                events &= ~TIMER_PERIODIC_EVENT;
                if (pt100_process)
                {
                    L_PT100_Procedure();
                    if (pt100_process==PT100_PROCESS_END)
                    {
                        lora_process = LORA_PROCESS_START;
                        lora_process_timeout_counter = 0;
                    }
                }
                if (pcb_temperature_process)
                {
                    L_PCB_TEMP_procedure();
                }
                if (lora_process)
                {
                    L_Lora_procedure();
                }
                if (eeprom_process)
                {
                    L_EEPROM_procedure();
                }
            }
        }
        if ((!pt100_process) && (!pcb_temperature_process) && (!lora_process)&& (!eeprom_process)&&(!ble_connect_process)&&(!ble_shutdown_process))
        {
          
            goToSleep();
          
        }
    }
}