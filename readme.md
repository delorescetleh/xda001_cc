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

## BLE APP FUNCTION
1. SET LORA INTERVAL TIME : A1 02 mm 00
2. READ EEPROM DATA : A2 02 00 00
3. TURN BLE OFF : A3 02 00 00
4. SET TEMPERATURE FOR OFFSET : A4 02 TT tt
5. ECHO : A5 02 ec ho
6. AUTO TEMPERATURE CALIBRATION : A6 02 00 00

### SET LORA INTERVAL TIME : A1 02 mm 00
Device use "mm" value as minutes unit to set interval time. 
"mm" value range: 1~254 minutes 
ACK : A1 02 01 00
### READ EEPROM DATA : A2 02 00 00
Device return the record data from now to oldest data sequently.
ACK : A2 mm nn DD DD DD DD...
mm nn : Data Length 
### TURN BLE OFF : A3 02 00 00
Device turn off after receive this comment in 4 sec.
ACK : A3 02 00 00
### SET TEMPERATURE FOR OFFSET : A4 02 TT tt
Device base on the target temperature(TT tt) set the offset temperature. 
TT: Hex integer of Celsius 
tt : 2 digital of decimal point 
example: 
31.02 degC => TT: 0x1F , tt:0x02
ACK : A4 02 00 00
### ECHO : A5 02 XX xx
Device will return XX xx back to app
ACK: A5 02 XX xx  
### AUTO TEMPERATURE CALIBRATION : A6 02 00 00
Device will use internal temperature sensor value as the target temperature to make the temperature offset. 
ACK: A6 02 00 00
Auction : internal temperature may have +-3 degC gap between outside environment temperature. 

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
10. ver 0.0.1.i: ble function work fine in A102,A202, command, sleep mode , 0.2mA, some mistake in dsadc and pcb process.  
11. ver 0.0.1.j: ble function work fine in A102,A202, command, sleep mode , 0.2mA, fix dsadc and pcb temp bug 
12. ver 0.0.1.k: sleep mode 0.2mA, BLE off 0.15mA, Battery workable in 3V. BLE function "A1,A2,A3,A5" Done.
13. ver 0.0.1.l: sleep mode 150uA, BLE off 86uA, BLE function "A1,A2,A3,A5" Done.
14. ver 0.0.1.m: sleep mode 150uA, BLE function "A1,A2,A3,A4,A5" Done.
15. ver 0.0.1.n: sleep mode 150uA, BLE function "A1,A2,A3,A4,A5,A6" Done.
16. ver 0.0.3.a: sleep mode tested 85uA, need remove R85. "A1,A2,A3,A4,A5,A6" Done. "PT100 under test"
   