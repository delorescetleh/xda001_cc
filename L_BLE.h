#include "r_cg_userdefine.h"

#define BLE_SHUTDOWN_PROCESS_START 15
#define BLE_SHUTDOWN_PROCESS_END 1
#define BLE_SHUTDOWN_PROCESS_TIMEOUT 10


#define BLE_F_PROCESS_START 15
#define BLE_F_PROCESS_END 1
#define BLE_F_PROCESS_TIMEOUT 10


#define BLE_PROCESS_START 15
#define BLE_PROCESS_END 1
#define BLE_PROCESS_TIMEOUT 250

extern uint8_t ble_process;
extern uint8_t ble_process_timeout_counter;
extern int ble;
extern void L_BLE_procedure(void);
extern void L_BLE_F_procedure(void);
extern void L_BLE_shutdown_procedure(void);