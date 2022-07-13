#include "L_BLE.h"

extern int ble=0;
extern uint8_t ble_F_process=0;
extern uint8_t ble_F_process_timeout_counter=BLE_F_PROCESS_TIMEOUT;

extern uint8_t ble_connect_process = 0;
extern uint8_t ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;

extern uint8_t ble_shutdown_process=0;
extern uint8_t ble_shutdown_process_timeout_counter = BLE_SHUTDOWN_PROCESS_TIMEOUT;

void L_BLE_connect_procedure_init(void){
    ble_connect_process = BLE_CONNECT_START;
    ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;
}

void L_BLE_F_procedure_init(void){
    ble_F_process = BLE_F_PROCESS_START;
    ble_F_process_timeout_counter = BLE_F_PROCESS_TIMEOUT;
}

void L_BLE_shutdown_procedure_init(void){
    ble_shutdown_process = BLE_SHUTDOWN_START;
    ble_shutdown_process_timeout_counter = BLE_SHUTDOWN_PROCESS_TIMEOUT;
}

void reset_ble_connect_process_timeout_counter(void){
    ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;
}

void L_BLE_F_procedure(void)
{
    ble_F_process_timeout_counter--;
    if (!ble_F_process_timeout_counter){
        ble_F_process = BLE_F_PROCESS_END;
    }

    switch (ble_F_process)
    {
    case BLE_F_PROCESS_START:
        R_UART1_Create();
        R_UART1_Start();
        ble_F_process--;
        break;
    case 14:
        L_BLE_INIT();
        BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
        ble_F_process--;
        break;
    case 12:
        if (L_BLE_SEND_COMMAND("$$$", 3, "CMD>", 4))
        {
            ble_F_process--;
        }
        break;
    case 11:
        if (L_BLE_SEND_COMMAND("SS,40\r", 6, "AOK", 3))
        {
            ble_F_process--;
        }
        break;
    case 10:
        if (!lora_process)
        {
            ble_F_process--;
        }
         break;
    case 9:
        if (L_BLE_SEND_COMMAND((char *)setBleDeviceNameCommand, 12, "AOK", 3))
        {
            ble_F_process--;
        }
        break;
    case 8:
        if (L_BLE_SEND_COMMAND("SW,0B,07\r", 9, "AOK", 3))
        {
            ble_F_process--;
        }
        break;
    case 7:
        if (L_BLE_SEND_COMMAND("SW,0A,04\r", 9, "AOK", 3))
        {
            ble_F_process--;
        }
        break;
    case 6:
        if (L_BLE_SEND_COMMAND("SO,1\r", 5, "AOK", 3))
        {
            ble_F_process--;
        }
        break;
    case 5:
        if (L_BLE_SEND_COMMAND("R,1\r", 4, "Rebooting", 9))
        {
            ble_F_process--;
        }
        break;

    case BLE_F_PROCESS_END:
        if (ble_F_process_timeout_counter)
        {
            BLE_F_Done = 1;
        }
        else
        {
            BLE_F_Done = 0;
        }
        ble_F_process_timeout_counter=BLE_F_PROCESS_TIMEOUT;
        L_BLE_STOP();
        ble_F_process--;
        break;
    default:
    if(ble_F_process)
        ble_F_process--;
        break;
    }
}

void L_BLE_shutdown_procedure(void){
    switch (ble_shutdown_process)
    {
    case BLE_SHUTDOWN_START:
        // R_INTC1_Stop();
        ble_connect_process = BLE_CONNECT_END;
        ble_shutdown_process--;
        break;
    case 10:
        L_BLE_STOP();
        ble_shutdown_process--;
        break;
    case BLE_SHUTDOWN_END:
        BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
        BLE_RESET = PIN_LEVEL_AS_LOW;
        BLE_POW_CNT = POWER_OFF;
        delayInMs(2);
        BLE_RESET_MODE = PIN_MODE_AS_INPUT;
        BLE_UART_RXD_IND_MODE = PIN_MODE_AS_INPUT;
        ble_shutdown_process--;
    default:
    if(ble_shutdown_process)
        ble_shutdown_process--;
        break;
    }
}

extern void L_BLE_connect_procedure(void)
{
    ble_connect_process_timeout_counter--;
    if (!ble_connect_process_timeout_counter){
        ble_connect_process = BLE_CONNECT_END;
    }
    switch (ble_connect_process)
    {
    case BLE_CONNECT_START:
        R_DTCD10_Start();
        R_UART1_Create();
        R_UART1_Start();
        BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
        ble_connect_process--;
        break;
    case 3:
        if (BLE_NO_CONNECT){
            ble_connect_process--;
        }else{
            checkAppCommand();
        }
        break;
    case BLE_CONNECT_END:
        L_BLE_STOP();
        ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;
        ble_connect_process--;
        break;
    default:
    if(ble_connect_process)
        ble_connect_process--;
        break;
    }
}