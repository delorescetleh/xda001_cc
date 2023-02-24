#include "L_Lora.h"
#define BAT_LEVEL_FULL 254
#define BAT_LEVEL_EMPTY 1
#define      LORA_PROCESS_TIMEOUT_COUNT 30
uint8_t lora_process_timeout_counter = LORA_PROCESS_TIMEOUT_COUNT;
uint8_t lora_process_timer_counter = 0;
float *lora_battery;
float *lora_pt100;
union 
{
    uint16_t whole;
    struct {
        uint8_t b0, b1;
    } byte;
} lora_pt100_send_data;
void GetPT100_DATA(void);
uint8_t receivedFromLora[MAX_LORA_RECEIVE]={0};
uint8_t sendToLora[20]={0};
uint8_t lora_init_success=0;

enum lora_process_t lora_process=LORA_PROCESS_END;

void prepareDataToLora(void);
uint8_t GetBatteryLevel(void);

void prepareDataToLora(void)
{

    sendToLora[0] = '{';
    sendToLora[1] = GetBatteryLevel();
    GetPT100_DATA();
    sendToLora[2] = lora_pt100_send_data.byte.b0;
    sendToLora[3] = lora_pt100_send_data.byte.b1;
    sendToLora[4] = '}';
}



void LORA_PROCESS(void)
{
    lora_init_success=checkLoraMessage();
}


void L_LORA_INIT(void){
    LORA_CTS = PIN_LEVEL_AS_HIGH;
    LORA_CTS_MODE = PIN_MODE_AS_OUTPUT;
    SAU0EN = 1U;    /* enables input clock supply */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0030_SAU_CK01_FCLK_3 | _0003_SAU_CK00_FCLK_3;
    R_UART0_Create();
    R_UART0_Start();
    memset(receivedFromLora, 0, MAX_LORA_RECEIVE);
    R_UART0_Receive(receivedFromLora, 6);

    LORA_RESET = PIN_LEVEL_AS_LOW;
    LORA_RESET_MODE = PIN_MODE_AS_OUTPUT;
    
    LORA_POW_CNT = PIN_LEVEL_AS_HIGH;
    LORA_POW_CNT_MODE = PIN_MODE_AS_OUTPUT;

    delayInMs(10);
    LORA_RESET = PIN_LEVEL_AS_HIGH;
    LORA_CTS = PIN_LEVEL_AS_LOW;
}









void lora_procedure(void){
if(!lora_process_timeout_counter)
{
	lora_process_timeout_counter=LORA_PROCESS_TIMEOUT_COUNT;
    lora_process_timer_counter = 5;
    lora_process = LORA_POWER_OFF;
}
    switch (lora_process)
    {
    case LORA_PROCESS_START:
        L_LORA_INIT();
        lora_process = LORA_INIT_CHECK;
        lora_process_timer_counter = 2;
        
        BUZ0 = !BUZ0;
        break;
    case LORA_INIT_CHECK:
        if((lora_init_success))
        {
            lora_init_success = 0;
            LORA_CTS = PIN_LEVEL_AS_HIGH;
            lora_process_timer_counter = 2;
            lora_process = LORA_SEND_MESSAGE;
             break;
        }
            if(!lora_process_timer_counter)
            {
                lora_process = LORA_PROCESS_START;
		        L_LORA_STOP();
            }
        
        break;     
    case LORA_SEND_MESSAGE:
        if((!LORA_RTS))
        {
            prepareDataToLora();
            doSendLoraData();
            lora_process_timer_counter = 2;
            lora_process = LORA_SEND_MESSAGE2;
             break;
        }
        if(!lora_process_timer_counter)
            {
                lora_process = LORA_INIT_CHECK;
            }
        break;
    case LORA_SEND_MESSAGE2:
        if(!lora_process_timer_counter)
        {
            lora_process = LORA_SEND_MESSAGE;
             break;
        }
        if(LORA_RTS)
        {
            lora_process_timer_counter = 5;
            lora_process = LORA_POWER_OFF;
        }
        break;     
    case LORA_POWER_OFF:
            if(!lora_process_timer_counter)
            {
                L_LORA_STOP();
		lora_process_timeout_counter=LORA_PROCESS_TIMEOUT_COUNT;
                lora_process = LORA_PROCESS_END;
            }
        break;    
    case LORA_PROCESS_END:
        /* code */
        break;
    }
}
void lora_procedure_init(float *_battery,float *_pt100)
{
    lora_process = LORA_PROCESS_START;
    lora_battery = _battery;
    lora_pt100 = _pt100;
    lora_process_timeout_counter=LORA_PROCESS_TIMEOUT_COUNT;

}


uint8_t checkLoraMessage(void){
    char *result;
    replace_0_as_1_in_buffer(receivedFromLora,MAX_LORA_RECEIVE-1);
    receivedFromLora[MAX_LORA_RECEIVE - 1] = 0;
    result=strstr((char *)receivedFromLora, "}");
    if (result!=NULL)
    {
        // R_UART0_Stop();
        R_UART0_Receive(receivedFromLora+6, 1);
        memcpy(&setBleDeviceNameCommand[7], (result-4), 1);
        return 1;
    }
    result=strstr((char *)receivedFromLora, "{");
    if (result!=NULL)
    {
        // R_UART0_Stop();
        R_UART0_Receive(receivedFromLora+6, 1);
        memcpy(&setBleDeviceNameCommand[7], (result+1), 1);
        return 1;
    }
    return 0;
}
uint8_t doSendLoraData(void)
{
    R_UART0_Start();
    R_UART0_Send(sendToLora, 17);
    return 1;
}
void L_LORA_STOP(void){
    R_UART0_Stop();
    LORA_CTS_MODE = PIN_MODE_AS_INPUT;
    UART0_TXD_MODE = PIN_MODE_AS_INPUT;
    SAU0EN = 0U;    /* disables input clock supply */
    LORA_RESET = PIN_LEVEL_AS_LOW;
    delayInMs(10);
    LORA_POW_CNT = PIN_LEVEL_AS_LOW;
    delayInMs(10);
    LORA_RESET = PIN_LEVEL_AS_HIGH;
    LORA_RESET_MODE= PIN_MODE_AS_INPUT;
    LORA_POW_CNT_MODE = PIN_MODE_AS_INPUT;
}

uint8_t GetBatteryLevel(void)
{
  const double maxBattery = 4.2;
  const double minBattery = 3.0;
  
  uint8_t batlevel = BAT_LEVEL_EMPTY + ((*lora_battery - minBattery) / (maxBattery - minBattery)) * (BAT_LEVEL_FULL - BAT_LEVEL_EMPTY);
  if (*lora_battery> 4.2)
  {
    batlevel = 255;
  }
  if (*lora_battery < 3)
  {
    batlevel = 0;
  }
  return batlevel;
}

void GetPT100_DATA(void)
{
    switch (pt100_data_fetch_result_type)
    {
        case PT100_SUCCESS:
            lora_pt100_send_data.whole = (*lora_pt100+50) * 2;
            break;
        case PT100_LINE_ERROR:
            lora_pt100_send_data.whole = 1;
            break;
        case PT100_SENSE_ERROR:
            lora_pt100_send_data.whole = 0;
            break;
    }
}