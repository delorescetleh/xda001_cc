# XDA001_V3 Project - Branch Master

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
3. ver 0.0.1.b: merge DTC branch success (Add ADC to fetch mcu temperature and RTC to trigger ADC , ADC trigger DTC, Data Store in 0xFF900 16 bytes (8 times * 2 bytes) per second, power consumption in 0.55mA)
4. ver 0.0.1.c: improve pcb temperature raw data fetch. DTC could switch ADC mode between internal volt and internal temperature each one sec. Keep STOP mode in 0.5mA 
5. ver 0.0.1.d: add get_pt100_result function to have PT100 value
6. ver 0.0.1.e: implement BLE factory mode setting function, UART1 work success(no use DTC)
7. ver 0.0.1.f: BLE normal mode success to connect app and add "echo function"(A5 02 xx xx , will return A502xxxx) 
8. ver 0.0.1.g: add FDL module for dataflash access
9. ver 0.0.1.h: eeprom read write success, could test by ble app command A202xxxx will return all record data