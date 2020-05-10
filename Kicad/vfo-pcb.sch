EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector:Conn_01x19_Female J4
U 1 1 5D7BA50D
P 2950 3200
F 0 "J4" H 2978 3226 50  0000 L CNN
F 1 "ESP32L" H 2978 3135 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 2950 3200 50  0001 C CNN
F 3 "~" H 2950 3200 50  0001 C CNN
	1    2950 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x19_Female J5
U 1 1 5D7BB4BD
P 3700 3200
F 0 "J5" H 3592 4285 50  0000 C CNN
F 1 "ESP32R" H 3592 4194 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 3700 3200 50  0001 C CNN
F 3 "~" H 3700 3200 50  0001 C CNN
	1    3700 3200
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x14_Female J7
U 1 1 5D7C3C46
P 5500 3750
F 0 "J7" H 5528 3726 50  0000 L CNN
F 1 "ILI9341" H 5528 3635 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 5500 3750 50  0001 C CNN
F 3 "~" H 5500 3750 50  0001 C CNN
	1    5500 3750
	1    0    0    -1  
$EndComp
Text GLabel 5300 2250 0    50   Input ~ 0
V33
Text GLabel 5300 2350 0    50   Input ~ 0
GND
Text GLabel 5300 2450 0    50   Input ~ 0
SDA_1
Text GLabel 5300 2550 0    50   Input ~ 0
SCL_1
Text GLabel 5300 2650 0    50   Input ~ 0
I1_1
Text GLabel 5300 2750 0    50   Input ~ 0
I2_1
Text GLabel 5300 2850 0    50   Input ~ 0
I3_1
Text GLabel 5300 3150 0    50   Input ~ 0
Vcc
Text GLabel 5300 3250 0    50   Input ~ 0
GND
Text GLabel 5300 3350 0    50   Input ~ 0
CS
Text GLabel 5300 3550 0    50   Input ~ 0
DC
Text GLabel 5300 3650 0    50   Input ~ 0
SDI
Text GLabel 5300 3750 0    50   Input ~ 0
SCK
Text GLabel 5300 3850 0    50   Input ~ 0
LED
Text GLabel 5300 3950 0    50   Input ~ 0
SDO
Text GLabel 5300 4150 0    50   Input ~ 0
T_CS
Text GLabel 5300 4450 0    50   Input ~ 0
T_IRQ
Text GLabel 2750 2700 0    50   Input ~ 0
DC
Text GLabel 2750 4100 0    50   Input ~ 0
GND
Text GLabel 3900 2300 2    50   Input ~ 0
Vcc
$Comp
L Device:R R1
U 1 1 5D7C757E
P 4400 1600
F 0 "R1" H 4470 1646 50  0000 L CNN
F 1 "2.2k" H 4470 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4330 1600 50  0001 C CNN
F 3 "~" H 4400 1600 50  0001 C CNN
	1    4400 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5D7C7AAA
P 4700 1600
F 0 "R2" H 4770 1646 50  0000 L CNN
F 1 "2.2k" H 4770 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4630 1600 50  0001 C CNN
F 3 "~" H 4700 1600 50  0001 C CNN
	1    4700 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1450 4700 1450
Text GLabel 4550 1450 1    50   Input ~ 0
V33
Text GLabel 2750 3900 0    50   Input ~ 0
SCL_1
Text GLabel 4400 1750 3    50   Input ~ 0
SCL_1
Text GLabel 4700 1750 3    50   Input ~ 0
SDA_1
$Comp
L Connector:Screw_Terminal_01x02 J6
U 1 1 5D7C9147
P 1700 3350
F 0 "J6" H 1780 3342 50  0000 L CNN
F 1 "Rotery Encoder1" H 1780 3251 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1700 3350 50  0001 C CNN
F 3 "~" H 1700 3350 50  0001 C CNN
	1    1700 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J8
U 1 1 5D7C972F
P 1700 3850
F 0 "J8" H 1780 3842 50  0000 L CNN
F 1 "Rotery Encoder2" H 1780 3751 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1700 3850 50  0001 C CNN
F 3 "~" H 1700 3850 50  0001 C CNN
	1    1700 3850
	1    0    0    -1  
$EndComp
Text GLabel 1500 3450 0    50   Input ~ 0
GND
Text GLabel 1500 3350 0    50   Input ~ 0
Vcc
Text GLabel 1500 3950 0    50   Input ~ 0
R1
Text GLabel 1500 3850 0    50   Input ~ 0
R2
$Comp
L Device:R R7
U 1 1 5D7CB031
P 1600 4400
F 0 "R7" H 1670 4446 50  0000 L CNN
F 1 "2.2k" H 1670 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1530 4400 50  0001 C CNN
F 3 "~" H 1600 4400 50  0001 C CNN
	1    1600 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5D7CBC44
P 1950 4400
F 0 "R8" H 2020 4446 50  0000 L CNN
F 1 "2.2k" H 2020 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1880 4400 50  0001 C CNN
F 3 "~" H 1950 4400 50  0001 C CNN
	1    1950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4250 1950 4250
Text GLabel 1950 4550 3    50   Input ~ 0
R2
Text GLabel 1600 4550 3    50   Input ~ 0
R1
Text GLabel 1750 4250 1    50   Input ~ 0
V33
$Comp
L Device:R R3
U 1 1 5D7CD148
P 5400 1600
F 0 "R3" H 5470 1646 50  0000 L CNN
F 1 "150" H 5470 1555 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5330 1600 50  0001 C CNN
F 3 "~" H 5400 1600 50  0001 C CNN
	1    5400 1600
	1    0    0    -1  
$EndComp
Text GLabel 5400 1450 1    50   Input ~ 0
Vcc
Text GLabel 5400 1750 3    50   Input ~ 0
LED
Text GLabel 7250 2800 0    50   Input ~ 0
GND
Text GLabel 7250 2700 0    50   Input ~ 0
Vcc
Text GLabel 7250 2500 0    50   Input ~ 0
SCLK_2
Text GLabel 7250 2400 0    50   Input ~ 0
RCLK_2
Text GLabel 3900 2700 2    50   Input ~ 0
SER_2
Text GLabel 3900 3300 2    50   Input ~ 0
RCLK_2
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 5D7CFA25
P 6700 4000
F 0 "SW1" H 6700 4367 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 6700 4276 50  0000 C CNN
F 2 "Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm" H 6550 4160 50  0001 C CNN
F 3 "~" H 6700 4260 50  0001 C CNN
	1    6700 4000
	1    0    0    -1  
$EndComp
Text GLabel 6400 4000 0    50   Input ~ 0
GND
Text GLabel 6400 3900 0    50   Input ~ 0
R1_2
Text GLabel 6400 4100 0    50   Input ~ 0
R2_2
Text GLabel 3900 3900 2    50   Input ~ 0
R1_2a
Text GLabel 7000 4100 2    50   Input ~ 0
R3_2
Text GLabel 3900 3200 2    50   Input ~ 0
R3_2a
Text GLabel 7000 3900 2    50   Input ~ 0
GND
Text GLabel 3900 3500 2    50   Input ~ 0
A1P
Text GLabel 3900 3400 2    50   Input ~ 0
A2P
$Comp
L Connector:Conn_01x07_Female J2
U 1 1 5D7CDE44
P 7450 2500
F 0 "J2" H 7478 2526 50  0000 L CNN
F 1 "Conn_01x07_Female" H 7478 2435 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 7450 2500 50  0001 C CNN
F 3 "~" H 7450 2500 50  0001 C CNN
	1    7450 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x07_Female J3
U 1 1 5D7C2654
P 5500 2550
F 0 "J3" H 5528 2576 50  0000 L CNN
F 1 "Si5351" H 5528 2485 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 5500 2550 50  0001 C CNN
F 3 "~" H 5500 2550 50  0001 C CNN
	1    5500 2550
	1    0    0    -1  
$EndComp
Text GLabel 3900 2900 2    50   Input ~ 0
SCLK_2
Text GLabel 3900 2800 2    50   Input ~ 0
GND
$Comp
L Connector:Screw_Terminal_01x02 J11
U 1 1 5D7C375F
P 2200 6650
F 0 "J11" H 2280 6642 50  0000 L CNN
F 1 "RxTx" H 2280 6551 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 2200 6650 50  0001 C CNN
F 3 "~" H 2200 6650 50  0001 C CNN
	1    2200 6650
	1    0    0    -1  
$EndComp
Text GLabel 2000 6650 0    50   Input ~ 0
RXTX
$Comp
L Device:C C2
U 1 1 5DDA518E
P 8050 1750
F 0 "C2" H 7935 1704 50  0000 R CNN
F 1 "100n" H 7935 1795 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8088 1600 50  0001 C CNN
F 3 "~" H 8050 1750 50  0001 C CNN
	1    8050 1750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5DDA5ACC
P 8050 1900
F 0 "#PWR01" H 8050 1650 50  0001 C CNN
F 1 "GND" H 8055 1727 50  0000 C CNN
F 2 "" H 8050 1900 50  0001 C CNN
F 3 "" H 8050 1900 50  0001 C CNN
	1    8050 1900
	1    0    0    -1  
$EndComp
Text GLabel 8050 1600 1    50   Input ~ 0
V12V
$Comp
L Device:CP C1
U 1 1 5D40F4A0
P 7500 1750
F 0 "C1" H 7385 1704 50  0000 R CNN
F 1 "100u" H 7385 1795 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 7538 1600 50  0001 C CNN
F 3 "~" H 7500 1750 50  0001 C CNN
	1    7500 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 1900 8050 1900
Connection ~ 8050 1900
Wire Wire Line
	7500 1600 8050 1600
$Comp
L Device:C C4
U 1 1 5D487CC1
P 9100 1750
F 0 "C4" H 8985 1704 50  0000 R CNN
F 1 "100n" H 8985 1795 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9138 1600 50  0001 C CNN
F 3 "~" H 9100 1750 50  0001 C CNN
	1    9100 1750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5D487CCB
P 9100 1900
F 0 "#PWR02" H 9100 1650 50  0001 C CNN
F 1 "GND" H 9105 1727 50  0000 C CNN
F 2 "" H 9100 1900 50  0001 C CNN
F 3 "" H 9100 1900 50  0001 C CNN
	1    9100 1900
	1    0    0    -1  
$EndComp
Text GLabel 9100 1600 1    50   Input ~ 0
Vcc
$Comp
L Connector:Conn_Coaxial J19
U 1 1 5D271B44
P 5900 5950
F 0 "J19" H 6000 5925 50  0000 L CNN
F 1 "Link-out 2b" H 6000 5834 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 5900 5950 50  0001 C CNN
F 3 " ~" H 5900 5950 50  0001 C CNN
	1    5900 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5D27217A
P 5900 6550
F 0 "#PWR011" H 5900 6300 50  0001 C CNN
F 1 "GND" H 5905 6377 50  0000 C CNN
F 2 "" H 5900 6550 50  0001 C CNN
F 3 "" H 5900 6550 50  0001 C CNN
	1    5900 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 5D5D4DE1
P 8850 1750
F 0 "C3" H 8735 1704 50  0000 R CNN
F 1 "100u" H 8735 1795 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 8888 1600 50  0001 C CNN
F 3 "~" H 8850 1750 50  0001 C CNN
	1    8850 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 1600 9100 1600
Wire Wire Line
	8850 1900 9100 1900
Connection ~ 9100 1900
$Comp
L Connector:Conn_Coaxial J17
U 1 1 5D59CD1A
P 5900 6350
F 0 "J17" H 6000 6325 50  0000 L CNN
F 1 "Link-out 2a" H 6000 6234 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 5900 6350 50  0001 C CNN
F 3 " ~" H 5900 6350 50  0001 C CNN
	1    5900 6350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5D59D11C
P 5900 6150
F 0 "#PWR09" H 5900 5900 50  0001 C CNN
F 1 "GND" H 5905 5977 50  0000 C CNN
F 2 "" H 5900 6150 50  0001 C CNN
F 3 "" H 5900 6150 50  0001 C CNN
	1    5900 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5950 5700 6150
$Comp
L Connector:Conn_Coaxial J13
U 1 1 5D5A5258
P 5800 4800
F 0 "J13" H 5900 4775 50  0000 L CNN
F 1 "Link-out 1b" H 5900 4684 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 5800 4800 50  0001 C CNN
F 3 " ~" H 5800 4800 50  0001 C CNN
	1    5800 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5D5A5262
P 5800 5400
F 0 "#PWR08" H 5800 5150 50  0001 C CNN
F 1 "GND" H 5805 5227 50  0000 C CNN
F 2 "" H 5800 5400 50  0001 C CNN
F 3 "" H 5800 5400 50  0001 C CNN
	1    5800 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5D5A5276
P 5800 5000
F 0 "#PWR06" H 5800 4750 50  0001 C CNN
F 1 "GND" H 5650 4950 50  0000 C CNN
F 2 "" H 5800 5000 50  0001 C CNN
F 3 "" H 5800 5000 50  0001 C CNN
	1    5800 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4800 5600 5000
$Comp
L Connector:Conn_Coaxial J10
U 1 1 5D7E888A
P 5800 5200
F 0 "J10" H 5900 5175 50  0000 L CNN
F 1 "Link-out 1a" H 5900 5084 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 5800 5200 50  0001 C CNN
F 3 " ~" H 5800 5200 50  0001 C CNN
	1    5800 5200
	1    0    0    -1  
$EndComp
Text GLabel 7250 2200 0    50   Input ~ 0
V12V
Text GLabel 2750 3200 0    50   Input ~ 0
CS
Text GLabel 2750 3300 0    50   Input ~ 0
SCK
Text GLabel 2750 4000 0    50   Input ~ 0
SDI
Text GLabel 2750 3500 0    50   Input ~ 0
GND
Text GLabel 2750 3600 0    50   Input ~ 0
SDA_1
Connection ~ 1950 4250
$Comp
L Device:R R10
U 1 1 5D86398D
P 2650 4400
F 0 "R10" H 2720 4446 50  0000 L CNN
F 1 "2.2k" H 2720 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2580 4400 50  0001 C CNN
F 3 "~" H 2650 4400 50  0001 C CNN
	1    2650 4400
	1    0    0    -1  
$EndComp
Text GLabel 2650 4550 3    50   Input ~ 0
RXTX
$Comp
L Device:R R4
U 1 1 5D88A14D
P 7500 4250
F 0 "R4" H 7570 4296 50  0000 L CNN
F 1 "10k" H 7570 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7430 4250 50  0001 C CNN
F 3 "~" H 7500 4250 50  0001 C CNN
	1    7500 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5D88A157
P 7800 4250
F 0 "R5" H 7870 4296 50  0000 L CNN
F 1 "10k" H 7870 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7730 4250 50  0001 C CNN
F 3 "~" H 7800 4250 50  0001 C CNN
	1    7800 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4100 7800 4100
Text GLabel 7650 4100 1    50   Input ~ 0
V33
Text GLabel 7500 4400 3    50   Input ~ 0
R1_2
Text GLabel 7800 4400 3    50   Input ~ 0
R2_2
$Comp
L Device:R R6
U 1 1 5D88BE39
P 8100 4250
F 0 "R6" H 8170 4296 50  0000 L CNN
F 1 "10k" H 8170 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 8030 4250 50  0001 C CNN
F 3 "~" H 8100 4250 50  0001 C CNN
	1    8100 4250
	1    0    0    -1  
$EndComp
Text GLabel 8100 4400 3    50   Input ~ 0
R3_2
Wire Wire Line
	7800 4100 8100 4100
Connection ~ 7800 4100
$Comp
L Connector:Screw_Terminal_01x02 J14
U 1 1 5D894359
P 2200 7100
F 0 "J14" H 2280 7092 50  0000 L CNN
F 1 "V12" H 2280 7001 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 2200 7100 50  0001 C CNN
F 3 "~" H 2200 7100 50  0001 C CNN
	1    2200 7100
	1    0    0    -1  
$EndComp
Text GLabel 2000 7100 0    50   Input ~ 0
V12V
Text GLabel 2000 7200 0    50   Input ~ 0
GND
Text GLabel 6750 5850 1    50   Input ~ 0
I1_1
Text GLabel 3700 6250 1    50   Input ~ 0
I2_1
$Comp
L Regulator_Linear:L7805 U3
U 1 1 5D8DFFB7
P 8500 1100
F 0 "U3" H 8500 1342 50  0000 C CNN
F 1 "L7805" H 8500 1251 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8525 950 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 8500 1050 50  0001 C CNN
	1    8500 1100
	1    0    0    -1  
$EndComp
Text GLabel 8200 1100 1    50   Input ~ 0
V12V
Text GLabel 8800 1100 1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR07
U 1 1 5D8E1771
P 8500 1400
F 0 "#PWR07" H 8500 1150 50  0001 C CNN
F 1 "GND" H 8505 1227 50  0000 C CNN
F 2 "" H 8500 1400 50  0001 C CNN
F 3 "" H 8500 1400 50  0001 C CNN
	1    8500 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5D987FC7
P 8650 4800
F 0 "C5" H 8535 4754 50  0000 R CNN
F 1 "10n" H 8535 4845 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8688 4650 50  0001 C CNN
F 3 "~" H 8650 4800 50  0001 C CNN
	1    8650 4800
	-1   0    0    1   
$EndComp
$Comp
L Device:C C6
U 1 1 5D9887F8
P 9050 4800
F 0 "C6" H 8935 4754 50  0000 R CNN
F 1 "10n" H 8935 4845 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9088 4650 50  0001 C CNN
F 3 "~" H 9050 4800 50  0001 C CNN
	1    9050 4800
	-1   0    0    1   
$EndComp
$Comp
L Device:C C7
U 1 1 5D988B2D
P 9450 4800
F 0 "C7" H 9335 4754 50  0000 R CNN
F 1 "10n" H 9335 4845 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9488 4650 50  0001 C CNN
F 3 "~" H 9450 4800 50  0001 C CNN
	1    9450 4800
	-1   0    0    1   
$EndComp
Text GLabel 7800 5300 3    50   Input ~ 0
R2_2
Text GLabel 7500 5300 3    50   Input ~ 0
R1_2
$Comp
L Device:R R13
U 1 1 5D988F6A
P 8200 5150
F 0 "R13" H 8270 5196 50  0000 L CNN
F 1 "10k" H 8270 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 8130 5150 50  0001 C CNN
F 3 "~" H 8200 5150 50  0001 C CNN
	1    8200 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5D98969C
P 7800 5150
F 0 "R12" H 7870 5196 50  0000 L CNN
F 1 "10k" H 7870 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7730 5150 50  0001 C CNN
F 3 "~" H 7800 5150 50  0001 C CNN
	1    7800 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5D989930
P 7500 5150
F 0 "R11" H 7570 5196 50  0000 L CNN
F 1 "10k" H 7570 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7430 5150 50  0001 C CNN
F 3 "~" H 7500 5150 50  0001 C CNN
	1    7500 5150
	1    0    0    -1  
$EndComp
Text GLabel 8200 5300 3    50   Input ~ 0
R3_2
Text GLabel 7500 5000 1    50   Input ~ 0
R1_2a
Text GLabel 7800 5000 1    50   Input ~ 0
R2_2a
Text GLabel 8200 5000 1    50   Input ~ 0
R3_2a
$Comp
L power:GND #PWR013
U 1 1 5D989F63
P 9450 4950
F 0 "#PWR013" H 9450 4700 50  0001 C CNN
F 1 "GND" H 9455 4777 50  0000 C CNN
F 2 "" H 9450 4950 50  0001 C CNN
F 3 "" H 9450 4950 50  0001 C CNN
	1    9450 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5D98A28F
P 9050 4950
F 0 "#PWR012" H 9050 4700 50  0001 C CNN
F 1 "GND" H 9055 4777 50  0000 C CNN
F 2 "" H 9050 4950 50  0001 C CNN
F 3 "" H 9050 4950 50  0001 C CNN
	1    9050 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5D98A4F7
P 8650 4950
F 0 "#PWR010" H 8650 4700 50  0001 C CNN
F 1 "GND" H 8655 4777 50  0000 C CNN
F 2 "" H 8650 4950 50  0001 C CNN
F 3 "" H 8650 4950 50  0001 C CNN
	1    8650 4950
	1    0    0    -1  
$EndComp
Text GLabel 9450 4650 1    50   Input ~ 0
R3_2a
Text GLabel 9050 4650 1    50   Input ~ 0
R2_2a
Text GLabel 8650 4650 1    50   Input ~ 0
R1_2a
Wire Wire Line
	3550 6250 3700 6250
Text GLabel 2750 2900 0    50   Input ~ 0
A3P
Text GLabel 3900 3000 2    50   Input ~ 0
A4P
$Comp
L Connector:Conn_01x07_Female J9
U 1 1 5DCA62E3
P 2000 5400
F 0 "J9" H 2028 5426 50  0000 L CNN
F 1 "Analog in 3-6" H 2028 5335 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 2000 5400 50  0001 C CNN
F 3 "~" H 2000 5400 50  0001 C CNN
	1    2000 5400
	1    0    0    -1  
$EndComp
Text GLabel 1800 5500 0    50   Input ~ 0
A4P
Text GLabel 1800 5400 0    50   Input ~ 0
A3P
Text GLabel 1800 5300 0    50   Input ~ 0
A2P
$Comp
L power:GND #PWR018
U 1 1 5DCB9564
P 1800 5100
F 0 "#PWR018" H 1800 4850 50  0001 C CNN
F 1 "GND" H 1805 4927 50  0000 C CNN
F 2 "" H 1800 5100 50  0001 C CNN
F 3 "" H 1800 5100 50  0001 C CNN
	1    1800 5100
	0    1    1    0   
$EndComp
Text GLabel 3450 5100 1    50   Input ~ 0
I3_1
Wire Wire Line
	6750 5850 6950 5850
$Comp
L Connector:Conn_Coaxial J18
U 1 1 5DEE94D2
P 8000 5850
F 0 "J18" H 8100 5825 50  0000 L CNN
F 1 "Link-out 3" H 8100 5734 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 8000 5850 50  0001 C CNN
F 3 " ~" H 8000 5850 50  0001 C CNN
	1    8000 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5DEE9C31
P 8000 6050
F 0 "#PWR020" H 8000 5800 50  0001 C CNN
F 1 "GND" H 8005 5877 50  0000 C CNN
F 2 "" H 8000 6050 50  0001 C CNN
F 3 "" H 8000 6050 50  0001 C CNN
	1    8000 6050
	1    0    0    -1  
$EndComp
Text GLabel 3900 3800 2    50   Input ~ 0
RXTX
Text GLabel 3900 3700 2    50   Input ~ 0
R1
Text GLabel 3900 3600 2    50   Input ~ 0
R2
Text Notes 6050 4250 0    50   ~ 0
19
Text Notes 7000 4250 0    50   ~ 0
26
Text Notes 2450 6650 0    50   ~ 0
39
Text Notes 2450 6750 0    50   ~ 0
36
Text GLabel 3900 4100 2    50   Input ~ 0
V33
Text GLabel 7250 2300 0    50   Input ~ 0
Dout
Text GLabel 8600 2950 0    50   Input ~ 0
Vcc
Text GLabel 9400 3550 3    50   Input ~ 0
Dout
Text GLabel 9000 2450 1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR05
U 1 1 5D29F33D
P 9000 3750
F 0 "#PWR05" H 9000 3500 50  0001 C CNN
F 1 "GND" H 9005 3577 50  0000 C CNN
F 2 "" H 9000 3750 50  0001 C CNN
F 3 "" H 9000 3750 50  0001 C CNN
	1    9000 3750
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC595 U2
U 1 1 5D256E65
P 9000 3050
F 0 "U2" H 9000 3831 50  0000 C CNN
F 1 "74HC595" H 9000 3740 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_LongPads" H 9000 3050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 9000 3050 50  0001 C CNN
	1    9000 3050
	1    0    0    -1  
$EndComp
Text GLabel 8600 2650 0    50   Input ~ 0
SER_2
Text GLabel 8600 3150 0    50   Input ~ 0
RCLK_2
Text GLabel 8600 2850 0    50   Input ~ 0
SCLK_2
$Comp
L power:GND #PWR03
U 1 1 5D1C62FB
P 8600 3250
F 0 "#PWR03" H 8600 3000 50  0001 C CNN
F 1 "GND" H 8605 3077 50  0000 C CNN
F 2 "" H 8600 3250 50  0001 C CNN
F 3 "" H 8600 3250 50  0001 C CNN
	1    8600 3250
	1    0    0    -1  
$EndComp
$Comp
L RF_Switch:AS179-92LF U4
U 1 1 5E28BAA7
P 4400 5200
F 0 "U4" H 4400 5625 50  0000 C CNN
F 1 "AS179-92LF" H 4400 5534 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 4500 5200 50  0001 C CNN
F 3 "http://www.skyworksinc.com/uploads/documents/AS179_92LF_200176H.pdf" H 4500 5200 50  0001 C CNN
	1    4400 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5E28E3FA
P 4500 5600
F 0 "#PWR0102" H 4500 5350 50  0001 C CNN
F 1 "GND" H 4505 5427 50  0000 C CNN
F 2 "" H 4500 5600 50  0001 C CNN
F 3 "" H 4500 5600 50  0001 C CNN
	1    4500 5600
	1    0    0    -1  
$EndComp
Text GLabel 9400 2650 2    50   Input ~ 0
Qa
Text GLabel 9400 2750 2    50   Input ~ 0
Qb
Text GLabel 4000 5300 0    50   Input ~ 0
Qa
Text GLabel 4000 5400 0    50   Input ~ 0
Qb
$Comp
L RF_Switch:AS179-92LF U1
U 1 1 5E2943B1
P 4400 6350
F 0 "U1" H 4400 6775 50  0000 C CNN
F 1 "AS179-92LF" H 4400 6684 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-363_SC-70-6" H 4500 6350 50  0001 C CNN
F 3 "http://www.skyworksinc.com/uploads/documents/AS179_92LF_200176H.pdf" H 4500 6350 50  0001 C CNN
	1    4400 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 6250 4100 6250
$Comp
L power:GND #PWR0103
U 1 1 5E295347
P 4500 6750
F 0 "#PWR0103" H 4500 6500 50  0001 C CNN
F 1 "GND" H 4505 6577 50  0000 C CNN
F 2 "" H 4500 6750 50  0001 C CNN
F 3 "" H 4500 6750 50  0001 C CNN
	1    4500 6750
	1    0    0    -1  
$EndComp
Text GLabel 4000 6450 0    50   Input ~ 0
Qa
Text GLabel 4000 6550 0    50   Input ~ 0
Qb
Text GLabel 1800 5200 0    50   Input ~ 0
A1P
$Comp
L Device:C C8
U 1 1 5E2AB95F
P 3800 5100
F 0 "C8" H 3685 5054 50  0000 R CNN
F 1 "100n" H 3685 5145 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3838 4950 50  0001 C CNN
F 3 "~" H 3800 5100 50  0001 C CNN
	1    3800 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3950 5100 4000 5100
$Comp
L Device:C C10
U 1 1 5E2ADEDB
P 5150 5000
F 0 "C10" H 5035 4954 50  0000 R CNN
F 1 "100n" H 5035 5045 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5188 4850 50  0001 C CNN
F 3 "~" H 5150 5000 50  0001 C CNN
	1    5150 5000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 5000 5600 5000
Wire Wire Line
	5000 5000 4800 5000
$Comp
L Device:C C11
U 1 1 5E2AFCAE
P 5150 5200
F 0 "C11" H 5035 5154 50  0000 R CNN
F 1 "100n" H 5035 5245 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5188 5050 50  0001 C CNN
F 3 "~" H 5150 5200 50  0001 C CNN
	1    5150 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4800 5200 5000 5200
Wire Wire Line
	5300 5200 5600 5200
$Comp
L Device:C C9
U 1 1 5E2B481E
P 3850 6250
F 0 "C9" H 3735 6204 50  0000 R CNN
F 1 "100n" H 3735 6295 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3888 6100 50  0001 C CNN
F 3 "~" H 3850 6250 50  0001 C CNN
	1    3850 6250
	0    -1   -1   0   
$EndComp
Connection ~ 4000 6250
$Comp
L Device:C C12
U 1 1 5E2B4DDD
P 5150 6150
F 0 "C12" H 5035 6104 50  0000 R CNN
F 1 "100n" H 5035 6195 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5188 6000 50  0001 C CNN
F 3 "~" H 5150 6150 50  0001 C CNN
	1    5150 6150
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C13
U 1 1 5E2B5361
P 5150 6350
F 0 "C13" H 5035 6304 50  0000 R CNN
F 1 "100n" H 5035 6395 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5188 6200 50  0001 C CNN
F 3 "~" H 5150 6350 50  0001 C CNN
	1    5150 6350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4800 6150 5000 6150
Wire Wire Line
	5300 6150 5700 6150
Wire Wire Line
	5300 6350 5700 6350
Wire Wire Line
	4800 6350 5000 6350
Text GLabel 1900 6000 0    50   Input ~ 0
Qb
Text GLabel 1900 6150 0    50   Input ~ 0
Qa
$Comp
L Device:C C14
U 1 1 5E2B36DD
P 2050 6000
F 0 "C14" H 1935 5954 50  0000 R CNN
F 1 "100n" H 1935 6045 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2088 5850 50  0001 C CNN
F 3 "~" H 2050 6000 50  0001 C CNN
	1    2050 6000
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C15
U 1 1 5E2B3C0B
P 2050 6150
F 0 "C15" H 1935 6104 50  0000 R CNN
F 1 "100n" H 1935 6195 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2088 6000 50  0001 C CNN
F 3 "~" H 2050 6150 50  0001 C CNN
	1    2050 6150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2200 6000 2200 6100
$Comp
L power:GND #PWR04
U 1 1 5E2B506E
P 2350 6100
F 0 "#PWR04" H 2350 5850 50  0001 C CNN
F 1 "GND" H 2355 5927 50  0000 C CNN
F 2 "" H 2350 6100 50  0001 C CNN
F 3 "" H 2350 6100 50  0001 C CNN
	1    2350 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 6100 2350 6100
Connection ~ 2200 6100
Wire Wire Line
	2200 6100 2200 6150
Text GLabel 5300 4050 0    50   Input ~ 0
SCK
Text GLabel 5300 4250 0    50   Input ~ 0
SDI
Text GLabel 2750 2800 0    50   Input ~ 0
T_CS
Text GLabel 2750 2600 0    50   Input ~ 0
RESET
Text GLabel 5300 3450 0    50   Input ~ 0
V33
Text GLabel 3900 3100 2    50   Input ~ 0
SDO
Text GLabel 5300 4350 0    50   Input ~ 0
SDO
Text GLabel 2000 6750 0    50   Input ~ 0
GND
Wire Wire Line
	1950 4250 2650 4250
Text GLabel 2750 3400 0    50   Input ~ 0
R2_2a
Text Notes 6050 3850 0    50   ~ 0
36
$Comp
L Device:C C16
U 1 1 5EBF9AAC
P 7100 5850
F 0 "C16" H 6985 5804 50  0000 R CNN
F 1 "100n" H 6985 5895 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7138 5700 50  0001 C CNN
F 3 "~" H 7100 5850 50  0001 C CNN
	1    7100 5850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7250 5850 7800 5850
Wire Wire Line
	3450 5100 3650 5100
$EndSCHEMATC
