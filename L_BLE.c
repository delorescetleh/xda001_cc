#include "L_BLE.h"

extern int ble=0;
extern uint8_t ble_process=BLE_PROCESS_START;
extern uint8_t ble_process_timeout_counter=BLE_PROCESS_TIMEOUT;

void L_BLE_F_procedure(void)
{
    ble_process_timeout_counter--;
    if (!ble_process_timeout_counter){
        ble_process = BLE_PROCESS_END;
    }

    switch (ble_process)
    {
    case BLE_PROCESS_START:
        R_UART1_Create();
        R_UART1_Start();
        BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
        ble_process--;
        break;
    case 14:
        L_BLE_INIT();
        ble_process--;
        break;
    case 12:
        if (L_BLE_SEND_COMMAND("$$$", 3, "CMD>", 4))
        {
            ble_process--;
        }
        break;
    case 11:
        if (L_BLE_SEND_COMMAND("SS,40\r", 6, "AOK", 3))
        {
            ble_process--;
        }
        break;
    case 10:
        if (!lora_process)
        {
            ble_process--;
        }
         break;
    case 9:
        if (L_BLE_SEND_COMMAND((char *)setBleDeviceNameCommand, 12, "AOK", 3))
        {
            ble_process--;
        }
        break;
    case 8:
        if (L_BLE_SEND_COMMAND("SW,0B,07\r", 9, "AOK", 3))
        {
            ble_process--;
        }
        break;
    case 7:
        if (L_BLE_SEND_COMMAND("SW,0A,04\r", 9, "AOK", 3))
        {
            ble_process--;
        }
        break;
    case 6:
        if (L_BLE_SEND_COMMAND("SO,1\r", 5, "AOK", 3))
        {
            ble_process--;
        }
        break;
    case 5:
        if (L_BLE_SEND_COMMAND("R,1\r", 4, "Rebooting", 9))
        {
            ble_process--;
        }
        break;
    case BLE_PROCESS_END:
        if (mode == factory_mode)
        {
                if(ble_process_timeout_counter)
                {
                    BLE_F_Done = 1;
                }
                else
                {
                    BLE_F_Done = 0;
                }
        }
        ble_process_timeout_counter=BLE_PROCESS_TIMEOUT;
        L_BLE_STOP();
        ble_process--;
        break;
    default:
    if(ble_process)
        ble_process--;
        break;
    }
}
