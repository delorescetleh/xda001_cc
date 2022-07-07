#include "L_PT100.h"
#include "r_cg_userdefine.h"

double pt100_line_voltage=0; // mv
int dsadc_temperature=0;
uint16_t convertTemperatureFormat(void);
void prepareDataToLora(void);
extern uint8_t pt100_process=PT100_PROCESS_START;
extern uint8_t pt100_process_timeout_counter=0;

void L_PT100_Procedure(void)
{
    pt100_process_timeout_counter++;
    if (pt100_process_timeout_counter > PT100_PROCESS_TIMEOUT)
    {
        pt100_process = PT100_PROCESS_END;
    }
    switch (pt100_process)
    {
        case PT100_PROCESS_START:
        //     analogProcessDone = 0;
            clr_dsadc_buf();
            L_EEPROM_INIT();
            R_PGA_DSAD_Create();
            pt100_process--;
            break;
        case 14:
            R_PGA_DSAD_Start();
            pt100_process--;
            break;
        case 13:
            if ((dsadc_ready))
            {
                dsadc_ready = 0;
                L_get_pt100_result(&dsadc_temperature);
                Record_Data = convertTemperatureFormat();
                pt100_process--;
            }
            break;
        case 12:
            prepareDataToLora();
            L_PGA_STOP();
        //     Record_Temperature = DSADC_Temperature + boardOffset(&board[F_DSADC_TEMPERATURE_SENSOR_OFFSET]);
        //     Record_Data = (uint16_t)((Record_Temperature) / 5 + 100); // Record Temperature as 0~999 (as -50degC to 450 degC)
        //     if (Record_Data >= 1000)
        //     {
        //         Record_Data = 0; // means record value will become 0, send to Lora "000" mean ERR
        //     }
        //     pt100_process--;
        //     break;
        // case 11:
        //     EEPROM_F_Done=doEepromWriteRecords((uint16_t)Record_Data);
        //     pt100_process--;
        //     break;
        // case 10:
        //     L_EEPROM_STOP();
        //     pt100_processTimeOutCounter = 0;
        //     pt100_process=1;
        //     break;
        case 1:
            pt100_process_timeout_counter = 0;
            pt100_process--;
            break;
        default:
            if (pt100_process)
            pt100_process--;
            break;
    }
}
uint16_t convertTemperatureFormat(void)
{
    return (dsadc_temperature/5+100);
}
