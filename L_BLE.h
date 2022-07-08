#include "r_cg_userdefine.h"

#define BLE_PROCESS_START 15
#define BLE_PROCESS_END 1
#define BLE_PROCESS_TIMEOUT 250


#define BLE_CONNECT_START 10
#define BLE_CONNECT_END 1
#define BLE_CONNECT_PROCESS_TIMEOUT 250

#define BLE_SHUTDOWN_START 20
#define BLE_SHUTDOWN_END 1
#define BLE_SHUTDOWN_PROCESS_TIMEOUT 250
extern uint8_t ble_connect_process_timeout_counter;
extern uint8_t ble_connect_process;
extern uint8_t ble_shutdown_process;
extern uint8_t ble_process;
extern uint8_t ble_process_timeout_counter;
extern int ble;
extern void L_BLE_F_procedure(void);
// extern void L_BLE_F_procedure(void);
extern void L_BLE_shutdown_procedure(void);
extern void L_BLE_connect_procedure(void);