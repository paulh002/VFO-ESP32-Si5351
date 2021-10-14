EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR010
U 1 1 60FB6BE4
P 1750 2700
F 0 "#PWR010" H 1750 2450 50  0001 C CNN
F 1 "GND" H 1755 2527 50  0000 C CNN
F 2 "" H 1750 2700 50  0001 C CNN
F 3 "" H 1750 2700 50  0001 C CNN
	1    1750 2700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 60FB6BEA
P 1750 2600
F 0 "#PWR09" H 1750 2350 50  0001 C CNN
F 1 "GND" H 1755 2427 50  0000 C CNN
F 2 "" H 1750 2600 50  0001 C CNN
F 3 "" H 1750 2600 50  0001 C CNN
	1    1750 2600
	0    1    1    0   
$EndComp
Text Notes 2600 3050 0    50   ~ 0
TMS
Text Notes 2600 2950 0    50   ~ 0
TCK
Text Notes 2600 2850 0    50   ~ 0
TDO
Text Notes 2600 2750 0    50   ~ 0
TDI
Text GLabel 2250 2800 2    50   Input ~ 0
GPIO15
Text GLabel 2250 2700 2    50   Input ~ 0
GPIO12
Text GLabel 2250 2900 2    50   Input ~ 0
GPIO13
Text GLabel 2250 3000 2    50   Input ~ 0
GPIO14
Text GLabel 1750 3000 0    50   Input ~ 0
V33
$Comp
L power:GND #PWR011
U 1 1 60FB6BF9
P 1750 2800
F 0 "#PWR011" H 1750 2550 50  0001 C CNN
F 1 "GND" H 1755 2627 50  0000 C CNN
F 2 "" H 1750 2800 50  0001 C CNN
F 3 "" H 1750 2800 50  0001 C CNN
	1    1750 2800
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J3
U 1 1 60FB6BFF
P 1950 2800
F 0 "J3" H 2000 3217 50  0000 C CNN
F 1 "Jtag" H 2000 3126 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 1950 2800 50  0001 C CNN
F 3 "~" H 1950 2800 50  0001 C CNN
	1    1950 2800
	1    0    0    -1  
$EndComp
Text GLabel 7050 1150 2    50   Input ~ 0
DC
Text GLabel 10850 4550 2    50   Input ~ 0
R1_2a
Text GLabel 10850 4350 2    50   Input ~ 0
R3_2a
Text GLabel 7050 3050 2    50   Input ~ 0
A1P
Text GLabel 7050 3150 2    50   Input ~ 0
A2P
Text GLabel 7050 1450 2    50   Input ~ 0
CS
Text GLabel 7050 2150 2    50   Input ~ 0
SCK
Text GLabel 5850 1150 0    50   Input ~ 0
A4P
Text GLabel 7050 2750 2    50   Input ~ 0
RXTX
Text GLabel 7050 2050 2    50   Input ~ 0
R1
Text GLabel 7050 2950 2    50   Input ~ 0
SDO
Text GLabel 10850 4450 2    50   Input ~ 0
R2_2a
Text GLabel 5850 1250 0    50   Input ~ 0
A3P
$Comp
L Device:CP C?
U 1 1 60FC082B
P 5450 1100
AR Path="/60FB3B0E/60FC082B" Ref="C?"  Part="1" 
AR Path="/5515D395/60FC082B" Ref="C?"  Part="1" 
AR Path="/60FC082B" Ref="C1"  Part="1" 
F 0 "C1" H 5335 1054 50  0000 R CNN
F 1 "10u" H 5335 1145 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5488 950 50  0001 C CNN
F 3 "~" H 5450 1100 50  0001 C CNN
	1    5450 1100
	1    0    0    -1  
$EndComp
Text GLabel 7050 1850 2    50   Input ~ 0
GPIO15
Text GLabel 7050 1550 2    50   Input ~ 0
GPIO12
Text GLabel 7050 1750 2    50   Input ~ 0
GPIO14
Text GLabel 7050 1650 2    50   Input ~ 0
GPIO13
Text Notes 5150 1750 0    50   ~ 0
TMS
Text Notes 5150 1650 0    50   ~ 0
TCK
Text Notes 5150 1550 0    50   ~ 0
TDO
Text Notes 5150 1450 0    50   ~ 0
TDI
$Comp
L power:GND #PWR?
U 1 1 60FC083C
P 5450 1250
AR Path="/60FB3B0E/60FC083C" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FC083C" Ref="#PWR?"  Part="1" 
AR Path="/60FC083C" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 5450 1000 50  0001 C CNN
F 1 "GND" H 5455 1077 50  0000 C CNN
F 2 "" H 5450 1250 50  0001 C CNN
F 3 "" H 5450 1250 50  0001 C CNN
	1    5450 1250
	1    0    0    -1  
$EndComp
$Comp
L RF_Module:ESP32-WROOM-32D U?
U 1 1 60FC0842
P 6450 2150
AR Path="/60FB3B0E/60FC0842" Ref="U?"  Part="1" 
AR Path="/5515D395/60FC0842" Ref="U?"  Part="1" 
AR Path="/60FC0842" Ref="U2"  Part="1" 
F 0 "U2" H 6450 3731 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 6450 3640 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 6450 650 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 6150 2200 50  0001 C CNN
	1    6450 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FC0848
P 6450 3550
AR Path="/60FB3B0E/60FC0848" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FC0848" Ref="#PWR?"  Part="1" 
AR Path="/60FC0848" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 6450 3300 50  0001 C CNN
F 1 "GND" H 6455 3377 50  0000 C CNN
F 2 "" H 6450 3550 50  0001 C CNN
F 3 "" H 6450 3550 50  0001 C CNN
	1    6450 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 950  5850 950 
Text GLabel 6450 750  1    50   Input ~ 0
V33
$Comp
L Device:R R?
U 1 1 60FC0850
P 5450 800
AR Path="/60FB3B0E/60FC0850" Ref="R?"  Part="1" 
AR Path="/5515D395/60FC0850" Ref="R?"  Part="1" 
AR Path="/60FC0850" Ref="R2"  Part="1" 
F 0 "R2" H 5520 846 50  0000 L CNN
F 1 "1k" H 5520 755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5380 800 50  0001 C CNN
F 3 "~" H 5450 800 50  0001 C CNN
	1    5450 800 
	1    0    0    -1  
$EndComp
Connection ~ 5450 950 
Text GLabel 5450 650  1    50   Input ~ 0
V33
Text GLabel 7050 1050 2    50   Input ~ 0
TXD0
Text GLabel 7050 1250 2    50   Input ~ 0
RXD0
Text GLabel 5100 950  1    50   Input ~ 0
EN
Text GLabel 7050 2250 2    50   Input ~ 0
GPIO19
Text Notes 5650 1100 0    50   ~ 0
36\n
Text Notes 5650 1400 0    50   ~ 0
39
Text GLabel 900  5150 2    50   Input ~ 0
GND
Text GLabel 900  5050 2    50   Input ~ 0
Vcc
Text GLabel 900  5250 2    50   Input ~ 0
R1
Text GLabel 900  5350 2    50   Input ~ 0
R2
Text GLabel 900  5450 2    50   Input ~ 0
RXTX
$Comp
L Device:C C?
U 1 1 60FCDA71
P 900 6100
AR Path="/60FB3B0E/60FCDA71" Ref="C?"  Part="1" 
AR Path="/5515D395/60FCDA71" Ref="C?"  Part="1" 
AR Path="/60FCDA71" Ref="C16"  Part="1" 
F 0 "C16" H 785 6054 50  0000 R CNN
F 1 "100n" H 785 6145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 938 5950 50  0001 C CNN
F 3 "~" H 900 6100 50  0001 C CNN
	1    900  6100
	1    0    0    -1  
$EndComp
Text GLabel 900  5950 0    50   Input ~ 0
RXTX
$Comp
L power:GND #PWR?
U 1 1 60FCDA78
P 900 6250
AR Path="/60FB3B0E/60FCDA78" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FCDA78" Ref="#PWR?"  Part="1" 
AR Path="/60FCDA78" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 900 6000 50  0001 C CNN
F 1 "GND" H 905 6077 50  0000 C CNN
F 2 "" H 900 6250 50  0001 C CNN
F 3 "" H 900 6250 50  0001 C CNN
	1    900  6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FCDA7E
P 900 5550
AR Path="/60FB3B0E/60FCDA7E" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FCDA7E" Ref="#PWR?"  Part="1" 
AR Path="/60FCDA7E" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 900 5300 50  0001 C CNN
F 1 "GND" H 905 5377 50  0000 C CNN
F 2 "" H 900 5550 50  0001 C CNN
F 3 "" H 900 5550 50  0001 C CNN
	1    900  5550
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x07_Male J?
U 1 1 60FCDA88
P 700 5350
AR Path="/60FB3B0E/60FCDA88" Ref="J?"  Part="1" 
AR Path="/5515D395/60FCDA88" Ref="J?"  Part="1" 
AR Path="/60FCDA88" Ref="J8"  Part="1" 
F 0 "J8" H 808 5831 50  0000 C CNN
F 1 "Encoders" H 808 5740 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 700 5350 50  0001 C CNN
F 3 "~" H 700 5350 50  0001 C CNN
	1    700  5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60FE34E4
P 3800 5250
AR Path="/60FB3B0E/60FE34E4" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE34E4" Ref="R?"  Part="1" 
AR Path="/60FE34E4" Ref="R7"  Part="1" 
F 0 "R7" H 3870 5296 50  0000 L CNN
F 1 "10k" H 3870 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3730 5250 50  0001 C CNN
F 3 "~" H 3800 5250 50  0001 C CNN
	1    3800 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60FE34EA
P 4100 5250
AR Path="/60FB3B0E/60FE34EA" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE34EA" Ref="R?"  Part="1" 
AR Path="/60FE34EA" Ref="R8"  Part="1" 
F 0 "R8" H 4170 5296 50  0000 L CNN
F 1 "10k" H 4170 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 5250 50  0001 C CNN
F 3 "~" H 4100 5250 50  0001 C CNN
	1    4100 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 5100 4100 5100
Text GLabel 4100 5400 3    50   Input ~ 0
R2_2
$Comp
L Device:R R?
U 1 1 60FE34F2
P 4400 5250
AR Path="/60FB3B0E/60FE34F2" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE34F2" Ref="R?"  Part="1" 
AR Path="/60FE34F2" Ref="R9"  Part="1" 
F 0 "R9" H 4470 5296 50  0000 L CNN
F 1 "10k" H 4470 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 5250 50  0001 C CNN
F 3 "~" H 4400 5250 50  0001 C CNN
	1    4400 5250
	1    0    0    -1  
$EndComp
Text GLabel 4400 5400 3    50   Input ~ 0
R3_2
Connection ~ 4100 5100
$Comp
L Device:C C?
U 1 1 60FE34FA
P 3800 7000
AR Path="/60FB3B0E/60FE34FA" Ref="C?"  Part="1" 
AR Path="/5515D395/60FE34FA" Ref="C?"  Part="1" 
AR Path="/60FE34FA" Ref="C10"  Part="1" 
F 0 "C10" H 3685 6954 50  0000 R CNN
F 1 "100n" H 3685 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3838 6850 50  0001 C CNN
F 3 "~" H 3800 7000 50  0001 C CNN
	1    3800 7000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 60FE3500
P 4100 7000
AR Path="/60FB3B0E/60FE3500" Ref="C?"  Part="1" 
AR Path="/5515D395/60FE3500" Ref="C?"  Part="1" 
AR Path="/60FE3500" Ref="C11"  Part="1" 
F 0 "C11" H 3985 6954 50  0000 R CNN
F 1 "100n" H 3985 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4138 6850 50  0001 C CNN
F 3 "~" H 4100 7000 50  0001 C CNN
	1    4100 7000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 60FE3506
P 4400 7000
AR Path="/60FB3B0E/60FE3506" Ref="C?"  Part="1" 
AR Path="/5515D395/60FE3506" Ref="C?"  Part="1" 
AR Path="/60FE3506" Ref="C12"  Part="1" 
F 0 "C12" H 4285 6954 50  0000 R CNN
F 1 "100n" H 4285 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4438 6850 50  0001 C CNN
F 3 "~" H 4400 7000 50  0001 C CNN
	1    4400 7000
	-1   0    0    1   
$EndComp
Text GLabel 4100 5950 1    50   Input ~ 0
R2_2
$Comp
L Device:R R?
U 1 1 60FE350D
P 4400 6100
AR Path="/60FB3B0E/60FE350D" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE350D" Ref="R?"  Part="1" 
AR Path="/60FE350D" Ref="R18"  Part="1" 
F 0 "R18" H 4470 6146 50  0000 L CNN
F 1 "10k" H 4470 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4330 6100 50  0001 C CNN
F 3 "~" H 4400 6100 50  0001 C CNN
	1    4400 6100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60FE3513
P 4100 6100
AR Path="/60FB3B0E/60FE3513" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE3513" Ref="R?"  Part="1" 
AR Path="/60FE3513" Ref="R17"  Part="1" 
F 0 "R17" H 4170 6146 50  0000 L CNN
F 1 "10k" H 4170 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 6100 50  0001 C CNN
F 3 "~" H 4100 6100 50  0001 C CNN
	1    4100 6100
	-1   0    0    1   
$EndComp
Text GLabel 4400 5950 1    50   Input ~ 0
R3_2
Text GLabel 4100 6250 3    50   Input ~ 0
R2_2a
Text GLabel 4400 6250 3    50   Input ~ 0
R3_2a
$Comp
L power:GND #PWR?
U 1 1 60FE351C
P 4100 7150
AR Path="/60FB3B0E/60FE351C" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FE351C" Ref="#PWR?"  Part="1" 
AR Path="/60FE351C" Ref="#PWR023"  Part="1" 
F 0 "#PWR023" H 4100 6900 50  0001 C CNN
F 1 "GND" H 4105 6977 50  0000 C CNN
F 2 "" H 4100 7150 50  0001 C CNN
F 3 "" H 4100 7150 50  0001 C CNN
	1    4100 7150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FE3522
P 3800 7150
AR Path="/60FB3B0E/60FE3522" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FE3522" Ref="#PWR?"  Part="1" 
AR Path="/60FE3522" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 3800 6900 50  0001 C CNN
F 1 "GND" H 3805 6977 50  0000 C CNN
F 2 "" H 3800 7150 50  0001 C CNN
F 3 "" H 3800 7150 50  0001 C CNN
	1    3800 7150
	1    0    0    -1  
$EndComp
Text GLabel 4400 6850 1    50   Input ~ 0
R3_2a
Text GLabel 4100 6850 1    50   Input ~ 0
R2_2a
Text GLabel 3800 6850 1    50   Input ~ 0
R1_2a
$Comp
L power:GND #PWR?
U 1 1 60FE352B
P 4400 7150
AR Path="/60FB3B0E/60FE352B" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FE352B" Ref="#PWR?"  Part="1" 
AR Path="/60FE352B" Ref="#PWR024"  Part="1" 
F 0 "#PWR024" H 4400 6900 50  0001 C CNN
F 1 "GND" H 4405 6977 50  0000 C CNN
F 2 "" H 4400 7150 50  0001 C CNN
F 3 "" H 4400 7150 50  0001 C CNN
	1    4400 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60FE3531
P 3800 6100
AR Path="/60FB3B0E/60FE3531" Ref="R?"  Part="1" 
AR Path="/5515D395/60FE3531" Ref="R?"  Part="1" 
AR Path="/60FE3531" Ref="R16"  Part="1" 
F 0 "R16" H 3870 6146 50  0000 L CNN
F 1 "10k" H 3870 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3730 6100 50  0001 C CNN
F 3 "~" H 3800 6100 50  0001 C CNN
	1    3800 6100
	-1   0    0    1   
$EndComp
Text GLabel 3800 6250 3    50   Input ~ 0
R1_2a
Text GLabel 3800 5950 1    50   Input ~ 0
R1_2
Text GLabel 3800 5400 3    50   Input ~ 0
R1_2
Wire Wire Line
	4100 5100 4400 5100
Text GLabel 3950 5100 1    50   Input ~ 0
V33
$Comp
L Device:C C?
U 1 1 60FFEBCB
P 1950 7000
AR Path="/60FB3B0E/60FFEBCB" Ref="C?"  Part="1" 
AR Path="/5515D395/60FFEBCB" Ref="C?"  Part="1" 
AR Path="/60FFEBCB" Ref="C19"  Part="1" 
F 0 "C19" H 1835 6954 50  0000 R CNN
F 1 "100n" H 1835 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1988 6850 50  0001 C CNN
F 3 "~" H 1950 7000 50  0001 C CNN
	1    1950 7000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 60FFEBD1
P 2350 7000
AR Path="/60FB3B0E/60FFEBD1" Ref="C?"  Part="1" 
AR Path="/5515D395/60FFEBD1" Ref="C?"  Part="1" 
AR Path="/60FFEBD1" Ref="C20"  Part="1" 
F 0 "C20" H 2235 6954 50  0000 R CNN
F 1 "100n" H 2235 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2388 6850 50  0001 C CNN
F 3 "~" H 2350 7000 50  0001 C CNN
	1    2350 7000
	-1   0    0    1   
$EndComp
Connection ~ 2350 7150
Text GLabel 1950 6850 1    50   Input ~ 0
A1P
Text GLabel 2350 6850 1    50   Input ~ 0
A2P
$Comp
L Device:C C?
U 1 1 60FFEBDA
P 1600 7000
AR Path="/60FB3B0E/60FFEBDA" Ref="C?"  Part="1" 
AR Path="/5515D395/60FFEBDA" Ref="C?"  Part="1" 
AR Path="/60FFEBDA" Ref="C18"  Part="1" 
F 0 "C18" H 1485 6954 50  0000 R CNN
F 1 "100n" H 1485 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1638 6850 50  0001 C CNN
F 3 "~" H 1600 7000 50  0001 C CNN
	1    1600 7000
	-1   0    0    1   
$EndComp
Text GLabel 1600 6850 1    50   Input ~ 0
A3P
$Comp
L Device:C C?
U 1 1 60FFEBE1
P 1200 7000
AR Path="/60FB3B0E/60FFEBE1" Ref="C?"  Part="1" 
AR Path="/5515D395/60FFEBE1" Ref="C?"  Part="1" 
AR Path="/60FFEBE1" Ref="C17"  Part="1" 
F 0 "C17" H 1085 6954 50  0000 R CNN
F 1 "100n" H 1085 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1238 6850 50  0001 C CNN
F 3 "~" H 1200 7000 50  0001 C CNN
	1    1200 7000
	-1   0    0    1   
$EndComp
Text GLabel 1200 6850 1    50   Input ~ 0
A4P
Connection ~ 1950 7150
Connection ~ 1600 7150
Wire Wire Line
	1600 7150 1200 7150
Wire Wire Line
	1950 7150 1600 7150
Wire Wire Line
	1950 7150 2350 7150
Wire Wire Line
	2350 7150 2650 7150
$Comp
L Device:R R?
U 1 1 61005843
P 2250 5150
AR Path="/60FB3B0E/61005843" Ref="R?"  Part="1" 
AR Path="/5515D395/61005843" Ref="R?"  Part="1" 
AR Path="/61005843" Ref="R22"  Part="1" 
F 0 "R22" H 2320 5196 50  0000 L CNN
F 1 "2.2k" H 2320 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2180 5150 50  0001 C CNN
F 3 "~" H 2250 5150 50  0001 C CNN
	1    2250 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61005849
P 2600 5150
AR Path="/60FB3B0E/61005849" Ref="R?"  Part="1" 
AR Path="/5515D395/61005849" Ref="R?"  Part="1" 
AR Path="/61005849" Ref="R23"  Part="1" 
F 0 "R23" H 2670 5196 50  0000 L CNN
F 1 "2.2k" H 2670 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2530 5150 50  0001 C CNN
F 3 "~" H 2600 5150 50  0001 C CNN
	1    2600 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 5000 2600 5000
Text GLabel 2600 5300 3    50   Input ~ 0
R2
Text GLabel 2250 5300 3    50   Input ~ 0
R1
Text Notes 2800 6200 0    50   ~ 0
36\n39
Text GLabel 2600 5000 1    50   Input ~ 0
V33
$Comp
L power:GND #PWR?
U 1 1 6100B4F9
P 1900 1800
AR Path="/60FB3B0E/6100B4F9" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/6100B4F9" Ref="#PWR?"  Part="1" 
AR Path="/6100B4F9" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 1900 1550 50  0001 C CNN
F 1 "GND" H 1905 1627 50  0000 C CNN
F 2 "" H 1900 1800 50  0001 C CNN
F 3 "" H 1900 1800 50  0001 C CNN
	1    1900 1800
	1    0    0    -1  
$EndComp
Connection ~ 1900 1800
$Comp
L Device:C C?
U 1 1 6100B500
P 2300 1650
AR Path="/60FB3B0E/6100B500" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B500" Ref="C?"  Part="1" 
AR Path="/6100B500" Ref="C5"  Part="1" 
F 0 "C5" H 2185 1604 50  0000 R CNN
F 1 "100n" H 2185 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2338 1500 50  0001 C CNN
F 3 "~" H 2300 1650 50  0001 C CNN
	1    2300 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	1900 1800 2050 1800
$Comp
L Device:C C?
U 1 1 6100B507
P 2700 1650
AR Path="/60FB3B0E/6100B507" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B507" Ref="C?"  Part="1" 
AR Path="/6100B507" Ref="C6"  Part="1" 
F 0 "C6" H 2585 1604 50  0000 R CNN
F 1 "100n" H 2585 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2738 1500 50  0001 C CNN
F 3 "~" H 2700 1650 50  0001 C CNN
	1    2700 1650
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 6100B50D
P 3100 1650
AR Path="/60FB3B0E/6100B50D" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B50D" Ref="C?"  Part="1" 
AR Path="/6100B50D" Ref="C7"  Part="1" 
F 0 "C7" H 2985 1604 50  0000 R CNN
F 1 "100n" H 2985 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3138 1500 50  0001 C CNN
F 3 "~" H 3100 1650 50  0001 C CNN
	1    3100 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	2300 1500 2700 1500
Connection ~ 2300 1500
Wire Wire Line
	3100 1500 2700 1500
Connection ~ 2700 1500
Wire Wire Line
	2300 1800 2700 1800
Connection ~ 2300 1800
Wire Wire Line
	2700 1800 3100 1800
Connection ~ 2700 1800
$Comp
L Device:C C?
U 1 1 6100B51B
P 3400 1650
AR Path="/60FB3B0E/6100B51B" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B51B" Ref="C?"  Part="1" 
AR Path="/6100B51B" Ref="C8"  Part="1" 
F 0 "C8" H 3285 1604 50  0000 R CNN
F 1 "100n" H 3285 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3438 1500 50  0001 C CNN
F 3 "~" H 3400 1650 50  0001 C CNN
	1    3400 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 1500 3400 1500
Connection ~ 3100 1500
Wire Wire Line
	3400 1800 3100 1800
Connection ~ 3100 1800
Text GLabel 1900 1500 1    50   Input ~ 0
V33
$Comp
L Device:C C?
U 1 1 6100B526
P 1350 1650
AR Path="/60FB3B0E/6100B526" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B526" Ref="C?"  Part="1" 
AR Path="/6100B526" Ref="C3"  Part="1" 
F 0 "C3" H 1235 1604 50  0001 R CNN
F 1 "100n" H 1235 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1388 1500 50  0001 C CNN
F 3 "~" H 1350 1650 50  0001 C CNN
	1    1350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 1500 1700 1500
$Comp
L Regulator_Linear:LM1117-3.3 U?
U 1 1 6100B52D
P 1200 900
AR Path="/60FB3B0E/6100B52D" Ref="U?"  Part="1" 
AR Path="/5515D395/6100B52D" Ref="U?"  Part="1" 
AR Path="/6100B52D" Ref="U1"  Part="1" 
F 0 "U1" H 1200 1142 50  0000 C CNN
F 1 "LM1117-3.3" H 1200 1051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 1200 900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 1200 900 50  0001 C CNN
	1    1200 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6100B533
P 1200 1200
AR Path="/60FB3B0E/6100B533" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/6100B533" Ref="#PWR?"  Part="1" 
AR Path="/6100B533" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 1200 950 50  0001 C CNN
F 1 "GND" H 1205 1027 50  0000 C CNN
F 2 "" H 1200 1200 50  0001 C CNN
F 3 "" H 1200 1200 50  0001 C CNN
	1    1200 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 900  1700 900 
Wire Wire Line
	1700 900  1700 1500
Connection ~ 1700 1500
Wire Wire Line
	1700 1500 2050 1500
Text GLabel 900  900  1    50   Input ~ 0
Vcc
$Comp
L Device:C C?
U 1 1 6100B53E
P 850 1650
AR Path="/60FB3B0E/6100B53E" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B53E" Ref="C?"  Part="1" 
AR Path="/6100B53E" Ref="C2"  Part="1" 
F 0 "C2" H 735 1604 50  0000 R CNN
F 1 "100n" H 735 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 888 1500 50  0001 C CNN
F 3 "~" H 850 1650 50  0001 C CNN
	1    850  1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	850  1500 1350 1500
Connection ~ 1350 1500
Wire Wire Line
	850  1800 1350 1800
Connection ~ 1350 1800
Wire Wire Line
	1350 1800 1700 1800
$Comp
L Device:C C?
U 1 1 6100B549
P 1700 1650
AR Path="/60FB3B0E/6100B549" Ref="C?"  Part="1" 
AR Path="/5515D395/6100B549" Ref="C?"  Part="1" 
AR Path="/6100B549" Ref="C4"  Part="1" 
F 0 "C4" H 1585 1604 50  0001 R CNN
F 1 "10u" H 1585 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 1738 1500 50  0001 C CNN
F 3 "~" H 1700 1650 50  0001 C CNN
	1    1700 1650
	1    0    0    -1  
$EndComp
Connection ~ 1700 1800
Wire Wire Line
	1700 1800 1900 1800
$Comp
L Connector:Conn_01x07_Female J?
U 1 1 6100C975
P 7100 5150
AR Path="/60FB3B0E/6100C975" Ref="J?"  Part="1" 
AR Path="/5515D395/6100C975" Ref="J?"  Part="1" 
AR Path="/6100C975" Ref="J7"  Part="1" 
F 0 "J7" H 7128 5176 50  0000 L CNN
F 1 "Encoders" H 7128 5085 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 7100 5150 50  0001 C CNN
F 3 "~" H 7100 5150 50  0001 C CNN
	1    7100 5150
	1    0    0    -1  
$EndComp
Text GLabel 6900 4850 0    50   Input ~ 0
R1_2
Text GLabel 6900 4950 0    50   Input ~ 0
R2_2
Text GLabel 6900 5050 0    50   Input ~ 0
R3_2
Text GLabel 6900 5150 0    50   Input ~ 0
R1_3
Text GLabel 6900 5250 0    50   Input ~ 0
R2_3
Text GLabel 6900 5350 0    50   Input ~ 0
R3_3
$Comp
L power:GND #PWR?
U 1 1 61012C64
P 6900 5450
AR Path="/60FB3B0E/61012C64" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/61012C64" Ref="#PWR?"  Part="1" 
AR Path="/61012C64" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 6900 5200 50  0001 C CNN
F 1 "GND" H 6905 5277 50  0000 C CNN
F 2 "" H 6900 5450 50  0001 C CNN
F 3 "" H 6900 5450 50  0001 C CNN
	1    6900 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61013C82
P 4700 5250
AR Path="/60FB3B0E/61013C82" Ref="R?"  Part="1" 
AR Path="/5515D395/61013C82" Ref="R?"  Part="1" 
AR Path="/61013C82" Ref="R10"  Part="1" 
F 0 "R10" H 4770 5296 50  0000 L CNN
F 1 "10k" H 4770 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4630 5250 50  0001 C CNN
F 3 "~" H 4700 5250 50  0001 C CNN
	1    4700 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61014162
P 5000 5250
AR Path="/60FB3B0E/61014162" Ref="R?"  Part="1" 
AR Path="/5515D395/61014162" Ref="R?"  Part="1" 
AR Path="/61014162" Ref="R11"  Part="1" 
F 0 "R11" H 5070 5296 50  0000 L CNN
F 1 "10k" H 5070 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4930 5250 50  0001 C CNN
F 3 "~" H 5000 5250 50  0001 C CNN
	1    5000 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 5100 5000 5100
Text GLabel 5000 5400 3    50   Input ~ 0
R2_3
$Comp
L Device:R R?
U 1 1 6101416E
P 5300 5250
AR Path="/60FB3B0E/6101416E" Ref="R?"  Part="1" 
AR Path="/5515D395/6101416E" Ref="R?"  Part="1" 
AR Path="/6101416E" Ref="R12"  Part="1" 
F 0 "R12" H 5370 5296 50  0000 L CNN
F 1 "10k" H 5370 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5230 5250 50  0001 C CNN
F 3 "~" H 5300 5250 50  0001 C CNN
	1    5300 5250
	1    0    0    -1  
$EndComp
Text GLabel 5300 5400 3    50   Input ~ 0
R3_3
Connection ~ 5000 5100
$Comp
L Device:C C?
U 1 1 6101417A
P 4700 7000
AR Path="/60FB3B0E/6101417A" Ref="C?"  Part="1" 
AR Path="/5515D395/6101417A" Ref="C?"  Part="1" 
AR Path="/6101417A" Ref="C13"  Part="1" 
F 0 "C13" H 4585 6954 50  0000 R CNN
F 1 "100n" H 4585 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4738 6850 50  0001 C CNN
F 3 "~" H 4700 7000 50  0001 C CNN
	1    4700 7000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 61014184
P 5000 7000
AR Path="/60FB3B0E/61014184" Ref="C?"  Part="1" 
AR Path="/5515D395/61014184" Ref="C?"  Part="1" 
AR Path="/61014184" Ref="C14"  Part="1" 
F 0 "C14" H 4885 6954 50  0000 R CNN
F 1 "100n" H 4885 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5038 6850 50  0001 C CNN
F 3 "~" H 5000 7000 50  0001 C CNN
	1    5000 7000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 6101418E
P 5300 7000
AR Path="/60FB3B0E/6101418E" Ref="C?"  Part="1" 
AR Path="/5515D395/6101418E" Ref="C?"  Part="1" 
AR Path="/6101418E" Ref="C15"  Part="1" 
F 0 "C15" H 5185 6954 50  0000 R CNN
F 1 "100n" H 5185 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5338 6850 50  0001 C CNN
F 3 "~" H 5300 7000 50  0001 C CNN
	1    5300 7000
	-1   0    0    1   
$EndComp
Text GLabel 5000 5950 1    50   Input ~ 0
R2_3
$Comp
L Device:R R?
U 1 1 61014199
P 5300 6100
AR Path="/60FB3B0E/61014199" Ref="R?"  Part="1" 
AR Path="/5515D395/61014199" Ref="R?"  Part="1" 
AR Path="/61014199" Ref="R21"  Part="1" 
F 0 "R21" H 5370 6146 50  0000 L CNN
F 1 "10k" H 5370 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5230 6100 50  0001 C CNN
F 3 "~" H 5300 6100 50  0001 C CNN
	1    5300 6100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 610141A3
P 5000 6100
AR Path="/60FB3B0E/610141A3" Ref="R?"  Part="1" 
AR Path="/5515D395/610141A3" Ref="R?"  Part="1" 
AR Path="/610141A3" Ref="R20"  Part="1" 
F 0 "R20" H 5070 6146 50  0000 L CNN
F 1 "10k" H 5070 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4930 6100 50  0001 C CNN
F 3 "~" H 5000 6100 50  0001 C CNN
	1    5000 6100
	-1   0    0    1   
$EndComp
Text GLabel 5300 5950 1    50   Input ~ 0
R3_3
Text GLabel 5000 6250 3    50   Input ~ 0
R2_3a
Text GLabel 5300 6250 3    50   Input ~ 0
R3_3a
$Comp
L power:GND #PWR?
U 1 1 610141B0
P 5000 7150
AR Path="/60FB3B0E/610141B0" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/610141B0" Ref="#PWR?"  Part="1" 
AR Path="/610141B0" Ref="#PWR026"  Part="1" 
F 0 "#PWR026" H 5000 6900 50  0001 C CNN
F 1 "GND" H 5005 6977 50  0000 C CNN
F 2 "" H 5000 7150 50  0001 C CNN
F 3 "" H 5000 7150 50  0001 C CNN
	1    5000 7150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 610141BA
P 4700 7150
AR Path="/60FB3B0E/610141BA" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/610141BA" Ref="#PWR?"  Part="1" 
AR Path="/610141BA" Ref="#PWR025"  Part="1" 
F 0 "#PWR025" H 4700 6900 50  0001 C CNN
F 1 "GND" H 4705 6977 50  0000 C CNN
F 2 "" H 4700 7150 50  0001 C CNN
F 3 "" H 4700 7150 50  0001 C CNN
	1    4700 7150
	1    0    0    -1  
$EndComp
Text GLabel 5300 6850 1    50   Input ~ 0
R3_3a
Text GLabel 5000 6850 1    50   Input ~ 0
R2_3a
Text GLabel 4700 6850 1    50   Input ~ 0
R1_3a
$Comp
L power:GND #PWR?
U 1 1 610141C7
P 5300 7150
AR Path="/60FB3B0E/610141C7" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/610141C7" Ref="#PWR?"  Part="1" 
AR Path="/610141C7" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 5300 6900 50  0001 C CNN
F 1 "GND" H 5305 6977 50  0000 C CNN
F 2 "" H 5300 7150 50  0001 C CNN
F 3 "" H 5300 7150 50  0001 C CNN
	1    5300 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 610141D1
P 4700 6100
AR Path="/60FB3B0E/610141D1" Ref="R?"  Part="1" 
AR Path="/5515D395/610141D1" Ref="R?"  Part="1" 
AR Path="/610141D1" Ref="R19"  Part="1" 
F 0 "R19" H 4770 6146 50  0000 L CNN
F 1 "10k" H 4770 6055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4630 6100 50  0001 C CNN
F 3 "~" H 4700 6100 50  0001 C CNN
	1    4700 6100
	-1   0    0    1   
$EndComp
Text GLabel 4700 6250 3    50   Input ~ 0
R1_3a
Text GLabel 4700 5950 1    50   Input ~ 0
R1_3
Text GLabel 4700 5400 3    50   Input ~ 0
R1_3
Wire Wire Line
	5000 5100 5300 5100
Text GLabel 4850 5100 1    50   Input ~ 0
V33
Text Notes 1200 5450 0    50   ~ 0
SER 19
Text Notes 1200 5250 0    50   ~ 0
SCLK 32
Text Notes 1200 5350 0    50   ~ 0
RCLK 33
$Comp
L power:GND #PWR020
U 1 1 6103150C
P 1700 5600
F 0 "#PWR020" H 1700 5350 50  0001 C CNN
F 1 "GND" H 1705 5427 50  0000 C CNN
F 2 "" H 1700 5600 50  0001 C CNN
F 3 "" H 1700 5600 50  0001 C CNN
	1    1700 5600
	1    0    0    -1  
$EndComp
Text GLabel 1700 5500 0    50   Input ~ 0
Vcc
Text GLabel 1700 5100 0    50   Input ~ 0
A1P
Text GLabel 1700 5300 0    50   Input ~ 0
A2P
$Comp
L Connector:Conn_01x07_Male J6
U 1 1 61031515
P 1900 5300
F 0 "J6" H 1928 5326 50  0000 L CNN
F 1 "I2C" H 1928 5235 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 1900 5300 50  0001 C CNN
F 3 "~" H 1900 5300 50  0001 C CNN
	1    1900 5300
	-1   0    0    -1  
$EndComp
Text GLabel 1700 5400 0    50   Input ~ 0
GPIO19
$Comp
L Interface_Expansion:PCF8574A U3
U 1 1 61032369
P 10350 4650
F 0 "U3" H 10350 5531 50  0000 C CNN
F 1 "PCF8574A" H 10350 5440 50  0000 C CNN
F 2 "Package_SO:SOP-16_7.5x10.4mm_P1.27mm" H 10350 4650 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/PCF8574_PCF8574A.pdf" H 10350 4650 50  0001 C CNN
	1    10350 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4550 9850 4650
$Comp
L power:GND #PWR017
U 1 1 6103540F
P 10350 5350
F 0 "#PWR017" H 10350 5100 50  0001 C CNN
F 1 "GND" H 10355 5177 50  0000 C CNN
F 2 "" H 10350 5350 50  0001 C CNN
F 3 "" H 10350 5350 50  0001 C CNN
	1    10350 5350
	1    0    0    -1  
$EndComp
Text GLabel 10350 3550 1    50   Input ~ 0
V33
Text GLabel 7050 2450 2    50   Input ~ 0
SCL1
Text GLabel 7050 2350 2    50   Input ~ 0
SDA1
Text GLabel 10850 4250 2    50   Input ~ 0
R1_3a
Text GLabel 10850 4650 2    50   Input ~ 0
R2_3a
Text GLabel 10850 4750 2    50   Input ~ 0
R3_3a
Text GLabel 9850 4350 0    50   Input ~ 0
SDA_3
Text GLabel 1700 5200 0    50   Input ~ 0
GPIO16
Text GLabel 7050 1950 2    50   Input ~ 0
GPIO16
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 612E9B4A
P 850 2250
F 0 "J2" H 930 2242 50  0000 L CNN
F 1 "5V" H 930 2151 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 850 2250 50  0001 C CNN
F 3 "~" H 850 2250 50  0001 C CNN
	1    850  2250
	1    0    0    -1  
$EndComp
Text GLabel 650  2350 3    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR?
U 1 1 612EA6FE
P 650 2250
AR Path="/60FB3B0E/612EA6FE" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/612EA6FE" Ref="#PWR?"  Part="1" 
AR Path="/612EA6FE" Ref="#PWR08"  Part="1" 
F 0 "#PWR08" H 650 2000 50  0001 C CNN
F 1 "GND" H 655 2077 50  0000 C CNN
F 2 "" H 650 2250 50  0001 C CNN
F 3 "" H 650 2250 50  0001 C CNN
	1    650  2250
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 613143AC
P 3750 1650
AR Path="/60FB3B0E/613143AC" Ref="C?"  Part="1" 
AR Path="/5515D395/613143AC" Ref="C?"  Part="1" 
AR Path="/613143AC" Ref="C9"  Part="1" 
F 0 "C9" H 3635 1604 50  0000 R CNN
F 1 "100n" H 3635 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3788 1500 50  0001 C CNN
F 3 "~" H 3750 1650 50  0001 C CNN
	1    3750 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 1500 3750 1500
Connection ~ 3400 1500
Wire Wire Line
	3400 1800 3750 1800
Connection ~ 3400 1800
$Comp
L Device:R R?
U 1 1 6147523F
P 7250 4200
AR Path="/60FB3B0E/6147523F" Ref="R?"  Part="1" 
AR Path="/5515D395/6147523F" Ref="R?"  Part="1" 
AR Path="/6147523F" Ref="R5"  Part="1" 
F 0 "R5" H 7320 4246 50  0000 L CNN
F 1 "2k2" H 7320 4155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7180 4200 50  0001 C CNN
F 3 "~" H 7250 4200 50  0001 C CNN
	1    7250 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6147525D
P 7600 4200
AR Path="/60FB3B0E/6147525D" Ref="R?"  Part="1" 
AR Path="/5515D395/6147525D" Ref="R?"  Part="1" 
AR Path="/6147525D" Ref="R6"  Part="1" 
F 0 "R6" H 7670 4246 50  0000 L CNN
F 1 "2k2" H 7670 4155 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7530 4200 50  0001 C CNN
F 3 "~" H 7600 4200 50  0001 C CNN
	1    7600 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 4050 7600 4050
Text GLabel 7600 4350 3    50   Input ~ 0
SDA1
Text GLabel 7250 4350 3    50   Input ~ 0
SCL1
Text GLabel 7450 4050 1    50   Input ~ 0
V33
Text GLabel 7050 2850 2    50   Input ~ 0
T_IRQ
Text GLabel 9850 4250 0    50   Input ~ 0
SCL_3
Text GLabel 9850 4750 0    50   Input ~ 0
V33
$Comp
L power:GND #PWR012
U 1 1 614B94D7
P 9850 4550
F 0 "#PWR012" H 9850 4300 50  0001 C CNN
F 1 "GND" H 9855 4377 50  0000 C CNN
F 2 "" H 9850 4550 50  0001 C CNN
F 3 "" H 9850 4550 50  0001 C CNN
	1    9850 4550
	0    1    1    0   
$EndComp
Connection ~ 9850 4550
$Comp
L Switch:SW_Push SW1
U 1 1 615E332E
P 4650 950
F 0 "SW1" H 4650 1235 50  0000 C CNN
F 1 "SW_Push" H 4650 1144 50  0000 C CNN
F 2 "ESP32_TouchDown_lib:TS-1088R-02526" H 4650 1150 50  0001 C CNN
F 3 "~" H 4650 1150 50  0001 C CNN
	1    4650 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 615E5DDE
P 4450 950
AR Path="/60FB3B0E/615E5DDE" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/615E5DDE" Ref="#PWR?"  Part="1" 
AR Path="/615E5DDE" Ref="#PWR01"  Part="1" 
F 0 "#PWR01" H 4450 700 50  0001 C CNN
F 1 "GND" H 4455 777 50  0000 C CNN
F 2 "" H 4450 950 50  0001 C CNN
F 3 "" H 4450 950 50  0001 C CNN
	1    4450 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 950  4850 950 
$Comp
L Connector:Conn_01x04_Male J4
U 1 1 61631E80
P 9100 5800
F 0 "J4" H 9208 6081 50  0000 C CNN
F 1 "I2C" H 9208 5990 50  0000 C CNN
F 2 "Connector_JST:JST_PH_S4B-PH-SM4-TB_1x04-1MP_P2.00mm_Horizontal" H 9100 5800 50  0001 C CNN
F 3 "~" H 9100 5800 50  0001 C CNN
	1    9100 5800
	1    0    0    -1  
$EndComp
Text GLabel 9300 5800 2    50   Input ~ 0
SDA_4
Text GLabel 9300 5700 2    50   Input ~ 0
SCL_4
Text GLabel 9300 5900 2    50   Input ~ 0
V33
$Comp
L power:GND #PWR014
U 1 1 61632DC1
P 9300 6000
F 0 "#PWR014" H 9300 5750 50  0001 C CNN
F 1 "GND" H 9305 5827 50  0000 C CNN
F 2 "" H 9300 6000 50  0001 C CNN
F 3 "" H 9300 6000 50  0001 C CNN
	1    9300 6000
	0    -1   -1   0   
$EndComp
Text GLabel 7550 950  1    50   Input ~ 0
IO0
$Comp
L power:GND #PWR?
U 1 1 615EC3AC
P 8050 950
AR Path="/60FB3B0E/615EC3AC" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/615EC3AC" Ref="#PWR?"  Part="1" 
AR Path="/615EC3AC" Ref="#PWR02"  Part="1" 
F 0 "#PWR02" H 8050 700 50  0001 C CNN
F 1 "GND" H 8055 777 50  0000 C CNN
F 2 "" H 8050 950 50  0001 C CNN
F 3 "" H 8050 950 50  0001 C CNN
	1    8050 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 615E4204
P 7850 950
F 0 "SW2" H 7850 1235 50  0000 C CNN
F 1 "SW_Push" H 7850 1144 50  0000 C CNN
F 2 "ESP32_TouchDown_lib:TS-1088R-02526" H 7850 1150 50  0001 C CNN
F 3 "~" H 7850 1150 50  0001 C CNN
	1    7850 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 950  7650 950 
Wire Wire Line
	7050 950  7300 950 
Connection ~ 7300 950 
Wire Wire Line
	7300 900  7300 950 
Text GLabel 7300 600  1    50   Input ~ 0
V33
$Comp
L Device:R R?
U 1 1 615EA608
P 7300 750
AR Path="/60FB3B0E/615EA608" Ref="R?"  Part="1" 
AR Path="/5515D395/615EA608" Ref="R?"  Part="1" 
AR Path="/615EA608" Ref="R1"  Part="1" 
F 0 "R1" H 7370 796 50  0000 L CNN
F 1 "10k" H 7370 705 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7230 750 50  0001 C CNN
F 3 "~" H 7300 750 50  0001 C CNN
	1    7300 750 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6152C09C
P 800 7000
AR Path="/60FB3B0E/6152C09C" Ref="C?"  Part="1" 
AR Path="/5515D395/6152C09C" Ref="C?"  Part="1" 
AR Path="/6152C09C" Ref="C21"  Part="1" 
F 0 "C21" H 685 6954 50  0000 R CNN
F 1 "100n" H 685 7045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 838 6850 50  0001 C CNN
F 3 "~" H 800 7000 50  0001 C CNN
	1    800  7000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1200 7150 800  7150
Connection ~ 1200 7150
$Sheet
S 9100 1000 500  150 
U 614CC1BF
F0 "USB" 50
F1 "USB.sch" 50
$EndSheet
Text GLabel 10850 4850 2    50   Input ~ 0
T1
Text GLabel 10850 4950 2    50   Input ~ 0
T2
$Comp
L Connector:Conn_01x14_Female J1
U 1 1 614FDA1A
P 5300 2650
F 0 "J1" H 5328 2626 50  0000 L CNN
F 1 "ILI9341" H 5328 2535 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 5300 2650 50  0001 C CNN
F 3 "~" H 5300 2650 50  0001 C CNN
	1    5300 2650
	1    0    0    -1  
$EndComp
Text GLabel 5100 2150 0    50   Input ~ 0
GND
Text GLabel 5100 2250 0    50   Input ~ 0
CS
Text GLabel 5100 2450 0    50   Input ~ 0
DC
Text GLabel 5100 2550 0    50   Input ~ 0
SDI
Text GLabel 5100 2650 0    50   Input ~ 0
SCK
Text GLabel 5100 2850 0    50   Input ~ 0
SDO
Text GLabel 5100 3050 0    50   Input ~ 0
T_CS
Text GLabel 5100 3350 0    50   Input ~ 0
T_IRQ
Text GLabel 5100 2950 0    50   Input ~ 0
SCK
Text GLabel 5100 3150 0    50   Input ~ 0
SDI
Text GLabel 5100 2350 0    50   Input ~ 0
V33
Text GLabel 5100 3250 0    50   Input ~ 0
SDO
$Sheet
S 9850 1000 500  150 
U 61506CC0
F0 "si5351" 50
F1 "si5351.sch" 50
$EndSheet
Text GLabel 8850 5250 2    50   Input ~ 0
SCL_1
Text GLabel 8850 5350 2    50   Input ~ 0
SDA_1
Text GLabel 8050 3850 0    50   Input ~ 0
SCL1
Text GLabel 8050 3950 0    50   Input ~ 0
SDA1
$Comp
L power:GND #PWR?
U 1 1 6154DDE6
P 8050 5050
AR Path="/614F5619/6154DDE6" Ref="#PWR?"  Part="1" 
AR Path="/61506CC0/6154DDE6" Ref="#PWR?"  Part="1" 
AR Path="/6154DDE6" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 8050 4800 50  0001 C CNN
F 1 "GND" H 8055 4877 50  0000 C CNN
F 2 "" H 8050 5050 50  0001 C CNN
F 3 "" H 8050 5050 50  0001 C CNN
	1    8050 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6154DDEC
P 8450 5550
AR Path="/614F5619/6154DDEC" Ref="#PWR?"  Part="1" 
AR Path="/61506CC0/6154DDEC" Ref="#PWR?"  Part="1" 
AR Path="/6154DDEC" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 8450 5300 50  0001 C CNN
F 1 "GND" H 8455 5377 50  0000 C CNN
F 2 "" H 8450 5550 50  0001 C CNN
F 3 "" H 8450 5550 50  0001 C CNN
	1    8450 5550
	1    0    0    -1  
$EndComp
Text GLabel 8850 4350 2    50   Input ~ 0
SDA_3
Text GLabel 8850 4250 2    50   Input ~ 0
SCL_3
Text GLabel 8850 4650 2    50   Input ~ 0
SCL_4
Text GLabel 8850 4750 2    50   Input ~ 0
SDA_4
Text GLabel 8050 4350 0    50   Input ~ 0
V33
Connection ~ 8050 4950
Connection ~ 8050 5050
Wire Wire Line
	8050 4950 8050 5050
Wire Wire Line
	8050 4850 8050 4950
$Comp
L Interface_Expansion:TCA9548APWR U?
U 1 1 6154DDFB
P 8450 4550
AR Path="/614F5619/6154DDFB" Ref="U?"  Part="1" 
AR Path="/61506CC0/6154DDFB" Ref="U?"  Part="1" 
AR Path="/6154DDFB" Ref="U5"  Part="1" 
F 0 "U5" H 8450 5631 50  0000 C CNN
F 1 "TCA9548APWR" H 8450 5540 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x7.8mm_P0.65mm" H 8450 3550 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tca9548a.pdf" H 8500 4800 50  0001 C CNN
	1    8450 4550
	1    0    0    -1  
$EndComp
Text GLabel 8450 3650 1    50   Input ~ 0
V33
$Comp
L Device:R R?
U 1 1 6154DE02
P 9900 2800
AR Path="/614F5619/6154DE02" Ref="R?"  Part="1" 
AR Path="/61506CC0/6154DE02" Ref="R?"  Part="1" 
AR Path="/6154DE02" Ref="R24"  Part="1" 
F 0 "R24" H 9970 2846 50  0000 L CNN
F 1 "2.2k" H 9970 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9830 2800 50  0001 C CNN
F 3 "~" H 9900 2800 50  0001 C CNN
	1    9900 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6154DE08
P 10200 2800
AR Path="/614F5619/6154DE08" Ref="R?"  Part="1" 
AR Path="/61506CC0/6154DE08" Ref="R?"  Part="1" 
AR Path="/6154DE08" Ref="R32"  Part="1" 
F 0 "R32" H 10270 2846 50  0000 L CNN
F 1 "2.2k" H 10270 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10130 2800 50  0001 C CNN
F 3 "~" H 10200 2800 50  0001 C CNN
	1    10200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 2650 10200 2650
Text GLabel 9900 2950 3    50   Input ~ 0
SCL_3
Text GLabel 10200 2950 3    50   Input ~ 0
SDA_3
Text GLabel 9900 2650 1    50   Input ~ 0
V33
$Comp
L Device:R R?
U 1 1 6154DE12
P 10550 2800
AR Path="/614F5619/6154DE12" Ref="R?"  Part="1" 
AR Path="/61506CC0/6154DE12" Ref="R?"  Part="1" 
AR Path="/6154DE12" Ref="R33"  Part="1" 
F 0 "R33" H 10620 2846 50  0000 L CNN
F 1 "2.2k" H 10620 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10480 2800 50  0001 C CNN
F 3 "~" H 10550 2800 50  0001 C CNN
	1    10550 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6154DE18
P 10850 2800
AR Path="/614F5619/6154DE18" Ref="R?"  Part="1" 
AR Path="/61506CC0/6154DE18" Ref="R?"  Part="1" 
AR Path="/6154DE18" Ref="R34"  Part="1" 
F 0 "R34" H 10920 2846 50  0000 L CNN
F 1 "2.2k" H 10920 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10780 2800 50  0001 C CNN
F 3 "~" H 10850 2800 50  0001 C CNN
	1    10850 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2650 10850 2650
Text GLabel 10550 2950 3    50   Input ~ 0
SCL_4
Text GLabel 10850 2950 3    50   Input ~ 0
SDA_4
Text GLabel 10550 2650 1    50   Input ~ 0
V33
$Comp
L Device:R R?
U 1 1 615622D0
P 9900 1900
AR Path="/614F5619/615622D0" Ref="R?"  Part="1" 
AR Path="/61506CC0/615622D0" Ref="R?"  Part="1" 
AR Path="/615622D0" Ref="R4"  Part="1" 
F 0 "R4" H 9970 1946 50  0000 L CNN
F 1 "2.2k" H 9970 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9830 1900 50  0001 C CNN
F 3 "~" H 9900 1900 50  0001 C CNN
	1    9900 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61562AB2
P 10200 1900
AR Path="/614F5619/61562AB2" Ref="R?"  Part="1" 
AR Path="/61506CC0/61562AB2" Ref="R?"  Part="1" 
AR Path="/61562AB2" Ref="R13"  Part="1" 
F 0 "R13" H 10270 1946 50  0000 L CNN
F 1 "2.2k" H 10270 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10130 1900 50  0001 C CNN
F 3 "~" H 10200 1900 50  0001 C CNN
	1    10200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 1750 10200 1750
Text GLabel 9900 2050 3    50   Input ~ 0
SCL_1
Text GLabel 10200 2050 3    50   Input ~ 0
SDA_1
Text GLabel 9900 1750 1    50   Input ~ 0
V33
Text GLabel 2450 6350 0    50   Input ~ 0
A5P
Text GLabel 2450 6450 0    50   Input ~ 0
V33
Text GLabel 2450 5950 0    50   Input ~ 0
A1P
$Comp
L power:GND #PWR?
U 1 1 61005863
P 2450 5850
AR Path="/60FB3B0E/61005863" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/61005863" Ref="#PWR?"  Part="1" 
AR Path="/61005863" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 2450 5600 50  0001 C CNN
F 1 "GND" H 2455 5677 50  0000 C CNN
F 2 "" H 2450 5850 50  0001 C CNN
F 3 "" H 2450 5850 50  0001 C CNN
	1    2450 5850
	0    1    1    0   
$EndComp
Text GLabel 2450 6050 0    50   Input ~ 0
A2P
Text GLabel 2450 6150 0    50   Input ~ 0
A3P
Text GLabel 2450 6250 0    50   Input ~ 0
A4P
$Comp
L Connector:Conn_01x07_Female J?
U 1 1 6100585A
P 2650 6150
AR Path="/60FB3B0E/6100585A" Ref="J?"  Part="1" 
AR Path="/5515D395/6100585A" Ref="J?"  Part="1" 
AR Path="/6100585A" Ref="J9"  Part="1" 
F 0 "J9" H 2678 6176 50  0000 L CNN
F 1 "Analog in 3-6" H 2678 6085 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 2650 6150 50  0001 C CNN
F 3 "~" H 2650 6150 50  0001 C CNN
	1    2650 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60FFEBC5
P 2650 7150
AR Path="/60FB3B0E/60FFEBC5" Ref="#PWR?"  Part="1" 
AR Path="/5515D395/60FFEBC5" Ref="#PWR?"  Part="1" 
AR Path="/60FFEBC5" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 2650 6900 50  0001 C CNN
F 1 "GND" H 2655 6977 50  0000 C CNN
F 2 "" H 2650 7150 50  0001 C CNN
F 3 "" H 2650 7150 50  0001 C CNN
	1    2650 7150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 6166FE99
P 4400 2200
F 0 "H1" H 4500 2246 50  0000 L CNN
F 1 " " H 4500 2155 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4400 2200 50  0001 C CNN
F 3 "~" H 4400 2200 50  0001 C CNN
	1    4400 2200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 616709A3
P 4400 2700
F 0 "H2" H 4500 2746 50  0000 L CNN
F 1 " " H 4500 2655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4400 2700 50  0001 C CNN
F 3 "~" H 4400 2700 50  0001 C CNN
	1    4400 2700
	1    0    0    -1  
$EndComp
Text GLabel 5100 2050 0    50   Input ~ 0
V33
Text GLabel 5100 2750 0    50   Input ~ 0
V33
Text GLabel 9850 5050 0    50   Input ~ 0
INT
Text GLabel 7050 1350 2    50   Input ~ 0
INT
Text GLabel 800  6850 1    50   Input ~ 0
A5P
Text GLabel 7050 2650 2    50   Input ~ 0
SDI
Text GLabel 7050 2550 2    50   Input ~ 0
R2
Text GLabel 7050 3250 2    50   Input ~ 0
A5P
$Comp
L Device:R R?
U 1 1 6175D20D
P 7800 1600
AR Path="/614F5619/6175D20D" Ref="R?"  Part="1" 
AR Path="/61506CC0/6175D20D" Ref="R?"  Part="1" 
AR Path="/6175D20D" Ref="R15"  Part="1" 
F 0 "R15" H 7870 1646 50  0000 L CNN
F 1 "10k" H 7870 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7730 1600 50  0001 C CNN
F 3 "~" H 7800 1600 50  0001 C CNN
	1    7800 1600
	1    0    0    -1  
$EndComp
Text GLabel 7800 1750 3    50   Input ~ 0
CS
Text GLabel 7800 1450 1    50   Input ~ 0
V33
$Comp
L Device:C C?
U 1 1 617A804B
P 2050 1650
AR Path="/60FB3B0E/617A804B" Ref="C?"  Part="1" 
AR Path="/5515D395/617A804B" Ref="C?"  Part="1" 
AR Path="/617A804B" Ref="C34"  Part="1" 
F 0 "C34" H 1935 1604 50  0001 R CNN
F 1 "10u" H 1935 1695 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2088 1500 50  0001 C CNN
F 3 "~" H 2050 1650 50  0001 C CNN
	1    2050 1650
	1    0    0    -1  
$EndComp
Connection ~ 2050 1500
Wire Wire Line
	2050 1500 2300 1500
Connection ~ 2050 1800
Wire Wire Line
	2050 1800 2300 1800
Text GLabel 8850 5050 2    50   Input ~ 0
SCL_2
Text GLabel 8850 5150 2    50   Input ~ 0
SDA_2
$Comp
L Device:R R?
U 1 1 6169519F
P 10550 1900
AR Path="/614F5619/6169519F" Ref="R?"  Part="1" 
AR Path="/61506CC0/6169519F" Ref="R?"  Part="1" 
AR Path="/6169519F" Ref="R47"  Part="1" 
F 0 "R47" H 10620 1946 50  0000 L CNN
F 1 "2.2k" H 10620 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10480 1900 50  0001 C CNN
F 3 "~" H 10550 1900 50  0001 C CNN
	1    10550 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 616958E1
P 10850 1900
AR Path="/614F5619/616958E1" Ref="R?"  Part="1" 
AR Path="/61506CC0/616958E1" Ref="R?"  Part="1" 
AR Path="/616958E1" Ref="R48"  Part="1" 
F 0 "R48" H 10920 1946 50  0000 L CNN
F 1 "2.2k" H 10920 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10780 1900 50  0001 C CNN
F 3 "~" H 10850 1900 50  0001 C CNN
	1    10850 1900
	1    0    0    -1  
$EndComp
Text GLabel 10550 2050 3    50   Input ~ 0
SCL_2
Text GLabel 10850 2050 3    50   Input ~ 0
SDA_2
Wire Wire Line
	10200 1750 10550 1750
Connection ~ 10200 1750
Connection ~ 10550 1750
Wire Wire Line
	10550 1750 10850 1750
$EndSCHEMATC
