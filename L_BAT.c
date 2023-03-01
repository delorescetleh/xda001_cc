#include "L_BAT.h"
#define BAT_MOVING_AVERAGE_TIMES 32
struct battery_struct *battery;
enum ADC_MODE adc_mode = ADC_STANDBY;
enum POWER_MODE power_mode = POWER_NORMAL;
uint16_t moving_average_times = BAT_MOVING_AVERAGE_TIMES;
float vbat = 0;
uint16_t _ADC10 = 0;
uint8_t battery_fetch_finish = 0;
enum battery_process_t battery_process = BATTERY_PROCESS_END;
void BATTERY_PROCESS(void)
{
  if(adc_mode==BATTERY_VOLTAGE_FETCH)
  {
    _ADC10+= ADC10;
    _ADC10 = _ADC10>>1;
    if(moving_average_times)
    {
            moving_average_times--;
            R_ADC_Start();
    }else{
            battery_fetch_finish = 1;
            vbat=(float)_ADC10* VBAT_ADC_RAW_CONVERTION_RATE;
            R_ADC_Stop();
            R_IT_Stop();
    }  
  }
}
void BATTERY_VOLTAGE_FETCH_START(void)
{
    adc_mode = BATTERY_VOLTAGE_FETCH;
    R_IT_Create();

    R_ADC_Create();
    R_ADC_Set_OperationOn();
    R_ADC_Start();
    R_IT_Start();
}

void BATTERY_VOLTAGE_FETCH_STOP(void)
{
    adc_mode=ADC_STANDBY;
    BAT_ADC_ON = PIN_LEVEL_AS_HIGH;
    // BAT_ADC_ON_MODE = PIN_MODE_AS_INPUT;
    R_ADC_Stop();
    R_ADC_Set_OperationOff();
    ADCEN = 0U; 
    R_IT_Stop();
    ITMC = _0000_IT_OPERATION_DISABLE;
    TMKAMK = 0U;    /* disable INTIT interrupt */
    TMKAIF = 0U;    /* clear INTIT interrupt flag */
    TMKAEN = 0U;    /* enables input clock supply */
}


void battery_procedure_init(struct battery_struct *_battery)
{
    battery_process = BATTERY_PROCESS_START;
    battery=_battery;
    battery->fetch_finish = 0;
    battery_fetch_finish = 0;
    BAT_ADC_ON_MODE = PIN_MODE_AS_OUTPUT;
    BAT_ADC_ON = PIN_LEVEL_AS_LOW;
    LORA_POW_CNT_MODE = PIN_MODE_AS_OUTPUT;
    LORA_POW_CNT = PIN_LEVEL_AS_HIGH;
}

void battery_procedure(void)
{
    switch (battery_process)
    {
    case BATTERY_PROCESS_START:

    BATTERY_VOLTAGE_FETCH_START();
    moving_average_times = BAT_MOVING_AVERAGE_TIMES;
    battery_fetch_finish = 0;
    vbat = 0;
    battery_process = POWER_OFF_BATTERY_FETCH;
    break;
    case POWER_OFF_BATTERY_FETCH: 

        if(battery_fetch_finish){
            battery_fetch_finish = 0;
            battery_process = SAVE_BAT_DATA;
        }
        break;

    case SAVE_BAT_DATA :
        if (vbat < 3)
        {
            power_mode = POWER_SAVING;
        }
        battery->Vbat=vbat;
        battery->fetch_finish = 1;
        BATTERY_VOLTAGE_FETCH_STOP();
        LORA_POW_CNT_MODE = PIN_MODE_AS_OUTPUT;
        LORA_POW_CNT = PIN_LEVEL_AS_LOW;
        battery_process = BATTERY_PROCESS_END;
    break;
    case BATTERY_PROCESS_END:
        break;
    }
}
void L_BAT_STOP(void)
{
    BATTERY_VOLTAGE_FETCH_STOP();
}



