#include "L_BLE.h"
#define BLE_PROCESS_TIMEOUT_COUNT 30
uint8_t ble_received_end = 0;
uint8_t ble_process_timeout_counter = BLE_PROCESS_TIMEOUT_COUNT;
uint8_t ble_process_timer_counter=2;
enum ble_process_t ble_process = BLE_PROCESS_END;

extern uint16_t lora_rtc_counter;
enum ble_process_t ble_check_command(void);

enum ble_process_t ble_check_command(void)
{
    enum ble_process_t result;
    const uint8_t bleAck_a1[] = {0xa1, 0x01, 0x55};// lora change interval time
    const uint8_t bleAck_a2[] = {0xa2, 0x01, 0x55};// send data
    const uint8_t bleAck_a3[] = {0xa3, 0x01, 0x55};// ble go to sleep
    const uint8_t bleAck_a4[] = {0xa4, 0x01, 0x55};// temperature calibrartion
    const uint8_t bleAck_e1[] = {0xe1, 0x01, 0x55};// error
    switch (receivedFromBle[0])
    {
    case 0xA1:
        memcpy(sendToBle,bleAck_a1,3);
        R_UART1_Send(sendToBle, 3);
        result = BLE_CHANGE_LORA_FETCH_TIME;
        break;
    case 0xA2:
        memcpy(sendToBle,bleAck_a2,3);
        R_UART1_Send(sendToBle, 3);
        result = BLE_SEND_DATA_TO_PHONE;
        break;
    case 0xA3:
        memcpy(sendToBle,bleAck_a3,3);
        R_UART1_Send(sendToBle, 3);
        result = BLE_POWER_OFF;
        break;            
    case 0xA4:
        memcpy(sendToBle,bleAck_a4,3);
        R_UART1_Send(sendToBle, 3);
        result = BLE_TEMPERATURE_OFFSET;
        break;
        default:
        memcpy(sendToBle,bleAck_e1,3);
        R_UART1_Send(sendToBle, 3);
        result = BLE_BINARY_MODE_EXIT;
        break;
    }
    // reset_eeprom_index();
    // reset_ble_connect_process_timeout_counter();
    return result;
}


void ble_procedure_init(void)
{
    ble_process = BLE_PROCESS_START;
    ble_process_timeout_counter=BLE_PROCESS_TIMEOUT_COUNT;
    R_SAU0_Create();
    R_UART1_Create();
    R_UART1_Start();
}

void ble_procedure(void){
    int16_t user_Temperature = 0;
    int16_t diff = 0;
    if(!ble_process_timeout_counter)
    {
    	ble_process_timeout_counter=BLE_PROCESS_TIMEOUT_COUNT;
        ble_process = BLE_BINARY_MODE_EXIT;
    }
    switch (ble_process)
    {
        case BLE_PROCESS_START                       :
            if(BLE_RTS==PIN_LEVEL_AS_LOW)
            {
                memset(receivedFromBle, 0, BLE_BUFFER_SIZE);
		        ble_received_end=0;
                R_UART1_Receive(receivedFromBle, 4);
                R_UART1_Send((uint8_t *)"AT+BINREQACK\r", 14);
                ble_process = BLE_CHECK_COMMAND;
            }
            break;
        case BLE_SET_NAME                            :
            break;
        case BLE_CHECK_COMMAND                        :
            if(ble_received_end)
            {
                ble_received_end = 0;
                receivedFromBle[BLE_BUFFER_SIZE-1] = 0;
                ble_process = ble_check_command();
            }
            break;
        case BLE_CHANGE_LORA_FETCH_TIME                :// A1020x00
            lora_rtc_counter = 0;
            if(receivedFromBle[3]==0){
                receivedFromBle[3] = 1;
            }
            lora_countdown_sec = receivedFromBle[3]*LORA_CYCLE_TIME_BASE;
            board[LORA_INTV] = receivedFromBle[3];
            DataFlashWrite();
            memset(receivedFromBle, 0, BLE_BUFFER_SIZE);
            R_UART1_Receive(receivedFromBle, 4);
            ble_process = BLE_CHECK_COMMAND;
            break;
        case BLE_SEND_DATA_TO_PHONE                    :// A2020000
            ble_process_timeout_counter=BLE_PROCESS_TIMEOUT_COUNT;
            memset(receivedFromBle, 0, BLE_BUFFER_SIZE);
            R_UART1_Receive(receivedFromBle, 4);
            ble_process = BLE_CHECK_COMMAND;
            break;
        case BLE_TEMPERATURE_OFFSET                    :// A402xxxx
            memset(receivedFromBle, 0, BLE_BUFFER_SIZE);
            user_Temperature = receivedFromBle[3]*10 + receivedFromBle[4];
            diff =  user_Temperature-pt100_temperature;
            board[DSADC_TEMPERATURE_SENSOR_OFFSET + 1] = diff >> 8;
            board[DSADC_TEMPERATURE_SENSOR_OFFSET] = diff;
            DataFlashWrite();

            ble_process_timeout_counter=BLE_PROCESS_TIMEOUT_COUNT;
            R_UART1_Receive(receivedFromBle, 4);
            ble_process = BLE_CHECK_COMMAND;
            break;
        case BLE_BINARY_MODE_EXIT                      :// A3020000
            R_UART1_Send((uint8_t *)"+++", 3);
            memset(receivedFromBle, 0, BLE_BUFFER_SIZE);
            R_UART1_Receive(receivedFromBle, 4);
            ble_process = BLE_GOTO_SLEEP;
            break;
        case BLE_POWER_OFF                             :
            R_UART1_Stop();
            UART1_TXD_MODE=PIN_MODE_AS_INPUT;
            BLE_CTS_MODE = PIN_MODE_AS_INPUT;
            BLE_POW_CNT = PIN_LEVEL_AS_LOW;
            semaphore = 0;
            ble_process = BLE_PROCESS_END;
            break;    
        case BLE_GOTO_SLEEP                           :
            R_UART1_Stop();
            UART1_TXD_MODE=PIN_MODE_AS_INPUT;
            BLE_CTS_MODE = PIN_MODE_AS_INPUT;
            semaphore = 0;
            R_INTC0_Start();
            ble_process = BLE_PROCESS_END;
            break;     
        case BLE_PROCESS_END                        :
            break;
        }
}


// void L_BLE_shutdown_procedure_init(void){
//     ble_shutdown_process = BLE_SHUTDOWN_START;
//     ble_shutdown_process_timeout_counter = BLE_SHUTDOWN_PROCESS_TIMEOUT;
// }

// void reset_ble_connect_process_timeout_counter(void){
//     ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;
// }

// void L_BLE_F_procedure(void)
// {
//     ble_F_process_timeout_counter--;
//     if (!ble_F_process_timeout_counter){
//         ble_F_process = BLE_F_PROCESS_END;
//     }

//     switch (ble_F_process)
//     {
//     case BLE_F_PROCESS_START:
//         R_UART1_Create();
//         R_UART1_Start();
//         ble_F_process--;
//         break;
//     case 14:
//         L_BLE_INIT();
//         BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
//         ble_F_process--;
//         break;
//     case 12:
//         if (L_BLE_SEND_COMMAND("$$$", 3, "CMD>", 4))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 11:
//         if (L_BLE_SEND_COMMAND("SS,40\r", 6, "AOK", 3))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 10:
//         if (!lora_process)
//         {
//             ble_F_process--;
//         }
//          break;
//     case 9:
//         if (L_BLE_SEND_COMMAND((char *)setBleDeviceNameCommand, 12, "AOK", 3))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 8:
//         if (L_BLE_SEND_COMMAND("SW,0B,07\r", 9, "AOK", 3))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 7:
//         if (L_BLE_SEND_COMMAND("SW,0A,04\r", 9, "AOK", 3))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 6:
//         if (L_BLE_SEND_COMMAND("SO,1\r", 5, "AOK", 3))
//         {
//             ble_F_process--;
//         }
//         break;
//     case 5:
//         if (L_BLE_SEND_COMMAND("R,1\r", 4, "Rebooting", 9))
//         {
//             ble_F_process--;
//         }
//         break;

//     case BLE_F_PROCESS_END:
//         if (ble_F_process_timeout_counter)
//         {
//             BLE_F_Done = 1;
//         }
//         else
//         {
//             BLE_F_Done = 0;
//         }
//         ble_F_process_timeout_counter=BLE_F_PROCESS_TIMEOUT;
//         L_BLE_STOP();
//         ble_F_process--;
//         break;
//     default:
//     if(ble_F_process)
//         ble_F_process--;
//         break;
//     }
// }

// void L_BLE_shutdown_procedure(void){
//     switch (ble_shutdown_process)
//     {
//     case BLE_SHUTDOWN_START:
//         // R_INTC1_Stop();
//         ble_connect_process = BLE_CONNECT_END;
//         ble_shutdown_process--;
//         break;
//     case 10:
//         L_BLE_STOP();
//         ble_shutdown_process--;
//         break;
//     case BLE_SHUTDOWN_END:
//         BLE_RESET_MODE = PIN_MODE_AS_OUTPUT;
//         BLE_RESET = PIN_LEVEL_AS_LOW;
//         BLE_POW_CNT = POWER_OFF;
//         delayInMs(2);
//         BLE_RESET_MODE = PIN_MODE_AS_INPUT;
//         BLE_UART_RXD_IND_MODE = PIN_MODE_AS_INPUT;
//         ble_shutdown_process--;
//     default:
//     if(ble_shutdown_process)
//         ble_shutdown_process--;
//         break;
//     }
// }

// extern void L_BLE_connect_procedure(void)
// {
//     ble_connect_process_timeout_counter--;
//     if (!ble_connect_process_timeout_counter){
//         ble_connect_process = BLE_CONNECT_END;
//     }
//     switch (ble_connect_process)
//     {
//     case BLE_CONNECT_START:
//         R_DTCD10_Start();
//         R_UART1_Create();
//         R_UART1_Start();
//         BLE_UART_RXD_IND = PIN_LEVEL_AS_LOW;
//         ble_connect_process--;
//         break;
//     case 3:
//         if (BLE_NO_CONNECT){
//             ble_connect_process--;
//         }else{
//             checkAppCommand();
//         }
//         break;
//     case BLE_CONNECT_END:
//         L_BLE_STOP();
//         ble_connect_process_timeout_counter = BLE_CONNECT_PROCESS_TIMEOUT;
//         ble_connect_process--;
//         break;
//     default:
//     if(ble_connect_process)
//         ble_connect_process--;
//         break;
//     }
// }