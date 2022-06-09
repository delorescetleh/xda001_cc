# XDA001_V3 Project - Branch DTC 
For test DTC function only

Test Function as below

---
1. USE RTC interrupt trigger ADC (Set in 1 sec interval)
2. ADC Trigger DTC and move ADC result to ADC_BUFFER= 0xFF900U (each RTC trigger will store 8 times ADC result )
3. System can go to stop mode and power consumption is around 0.55mA. But ADC not stop.
---


 This is a project to migrate the XDA001_V3 old version from CS+ CA IDE to CS+ CC

## Some Mission should be done  

1. Implement all function as old version (2022 0301 version)
2. Improve PGA sensor accuracy
3. Improve power consumptions 
4. add K type sensor function

## All Function as Old version 
What is old version function
1. Factory mode 
2. Normal mode

### In Factory mode 
1. Calibrartion PGA sensor parameter 
2. Set BLE mode
3. Set Lora mode
4. Test EEPROM
5. Store Board information in DataFlash 

## Hardware Information
1. Main control Unit : RL78/H1D
2. Communication Unit : Heltec Lora module(AM02) / Microchip BLE RN4871
3. Local Storage : 1Mb  EEPROM
4. Temperature Sensor : H1D Internal Temperature sensor / DSADC PGA Block unit for PT100 / K type sensor
5. System Power: 2.7 V  from Torex DCDC XCL206

### Version Log
1. ver 0.0.1: first release@20220608 
2. ver 0.0.1.a: H/W Pin assignment setting finish (I2C, UART0, UART1, GPIO, PGA) and keep in 0.2mA