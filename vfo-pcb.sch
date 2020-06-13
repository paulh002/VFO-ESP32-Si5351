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
L Connector:Conn_01x19_Female J6
U 1 1 5D7BA50D
P 2250 3200
F 0 "J6" H 2278 3226 50  0000 L CNN
F 1 "ESP32L" H 2278 3135 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 2250 3200 50  0001 C CNN
F 3 "~" H 2250 3200 50  0001 C CNN
	1    2250 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x19_Female J7
U 1 1 5D7BB4BD
P 3000 3200
F 0 "J7" H 2892 4285 50  0000 C CNN
F 1 "ESP32R" H 2892 4194 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x19_P2.54mm_Vertical" H 3000 3200 50  0001 C CNN
F 3 "~" H 3000 3200 50  0001 C CNN
	1    3000 3200
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x14_Female J8
U 1 1 5D7C3C46
P 3450 6000
F 0 "J8" H 3478 5976 50  0000 L CNN
F 1 "ILI9341" H 3478 5885 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x14_P2.54mm_Vertical" H 3450 6000 50  0001 C CNN
F 3 "~" H 3450 6000 50  0001 C CNN
	1    3450 6000
	1    0    0    -1  
$EndComp
Text GLabel 8700 800  1    50   Input ~ 0
V33a
Text GLabel 8300 2250 0    50   Input ~ 0
SDA_1
Text GLabel 3250 5400 0    50   Input ~ 0
Vcc
Text GLabel 3250 5500 0    50   Input ~ 0
GND
Text GLabel 3250 5600 0    50   Input ~ 0
CS
Text GLabel 3250 5800 0    50   Input ~ 0
DC
Text GLabel 3250 5900 0    50   Input ~ 0
SDI
Text GLabel 3250 6000 0    50   Input ~ 0
SCK
Text GLabel 3250 6100 0    50   Input ~ 0
LED
Text GLabel 3250 6200 0    50   Input ~ 0
SDO
Text GLabel 3250 6400 0    50   Input ~ 0
T_CS
Text GLabel 3250 6700 0    50   Input ~ 0
T_IRQ
Text GLabel 2050 2700 0    50   Input ~ 0
DC
Text GLabel 2050 4100 0    50   Input ~ 0
GND
Text GLabel 3200 2300 2    50   Input ~ 0
Vcc
$Comp
L Device:R R4
U 1 1 5D7C757E
P 7300 2400
F 0 "R4" H 7370 2446 50  0000 L CNN
F 1 "2.2k" H 7370 2355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7230 2400 50  0001 C CNN
F 3 "~" H 7300 2400 50  0001 C CNN
	1    7300 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5D7C7AAA
P 7600 2400
F 0 "R5" H 7670 2446 50  0000 L CNN
F 1 "2.2k" H 7670 2355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7530 2400 50  0001 C CNN
F 3 "~" H 7600 2400 50  0001 C CNN
	1    7600 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 2250 7600 2250
Text GLabel 2050 3900 0    50   Input ~ 0
SCL_1
Text GLabel 7300 2550 3    50   Input ~ 0
SCL_1
Text GLabel 7600 2550 3    50   Input ~ 0
SDA_1
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 5D7C9147
P 1000 3350
F 0 "J1" H 1080 3342 50  0000 L CNN
F 1 "Rotery Encoder1" H 1080 3251 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1000 3350 50  0001 C CNN
F 3 "~" H 1000 3350 50  0001 C CNN
	1    1000 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 5D7C972F
P 1000 3850
F 0 "J2" H 1080 3842 50  0000 L CNN
F 1 "Rotery Encoder2" H 1080 3751 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1000 3850 50  0001 C CNN
F 3 "~" H 1000 3850 50  0001 C CNN
	1    1000 3850
	1    0    0    -1  
$EndComp
Text GLabel 800  3450 0    50   Input ~ 0
GND
Text GLabel 800  3350 0    50   Input ~ 0
Vcc
Text GLabel 800  3950 0    50   Input ~ 0
R1
Text GLabel 800  3850 0    50   Input ~ 0
R2
$Comp
L Device:R R1
U 1 1 5D7CB031
P 900 4400
F 0 "R1" H 970 4446 50  0000 L CNN
F 1 "2.2k" H 970 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 830 4400 50  0001 C CNN
F 3 "~" H 900 4400 50  0001 C CNN
	1    900  4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5D7CBC44
P 1250 4400
F 0 "R2" H 1320 4446 50  0000 L CNN
F 1 "2.2k" H 1320 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1180 4400 50  0001 C CNN
F 3 "~" H 1250 4400 50  0001 C CNN
	1    1250 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  4250 1250 4250
Text GLabel 1250 4550 3    50   Input ~ 0
R2
Text GLabel 900  4550 3    50   Input ~ 0
R1
Text GLabel 1050 4250 1    50   Input ~ 0
V33
$Comp
L Device:R R6
U 1 1 5D7CD148
P 3000 1200
F 0 "R6" H 3070 1246 50  0000 L CNN
F 1 "150" H 3070 1155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2930 1200 50  0001 C CNN
F 3 "~" H 3000 1200 50  0001 C CNN
	1    3000 1200
	1    0    0    -1  
$EndComp
Text GLabel 3000 1050 1    50   Input ~ 0
Vcc
Text GLabel 3000 1350 3    50   Input ~ 0
LED
Text GLabel 5100 2950 0    50   Input ~ 0
GND
Text GLabel 5100 2850 0    50   Input ~ 0
Vcc
Text GLabel 5100 2650 0    50   Input ~ 0
SCLK_2
Text GLabel 5100 2550 0    50   Input ~ 0
RCLK_2
Text GLabel 3200 2700 2    50   Input ~ 0
SER_2
Text GLabel 3200 3300 2    50   Input ~ 0
RCLK_2
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 5D7CFA25
P 4550 4150
F 0 "SW1" H 4550 4517 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 4550 4426 50  0000 C CNN
F 2 "Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm" H 4400 4310 50  0001 C CNN
F 3 "~" H 4550 4410 50  0001 C CNN
	1    4550 4150
	1    0    0    -1  
$EndComp
Text GLabel 4250 4150 0    50   Input ~ 0
GND
Text GLabel 4250 4050 0    50   Input ~ 0
R1_2
Text GLabel 4250 4250 0    50   Input ~ 0
R2_2
Text GLabel 3200 3900 2    50   Input ~ 0
R1_2a
Text GLabel 4850 4250 2    50   Input ~ 0
R3_2
Text GLabel 3200 3200 2    50   Input ~ 0
R3_2a
Text GLabel 4850 4050 2    50   Input ~ 0
GND
Text GLabel 3200 3500 2    50   Input ~ 0
A1P
Text GLabel 3200 3400 2    50   Input ~ 0
A2P
$Comp
L Connector:Conn_01x07_Female J9
U 1 1 5D7CDE44
P 5300 2650
F 0 "J9" H 5328 2676 50  0000 L CNN
F 1 "Conn_01x07_Female" H 5328 2585 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 5300 2650 50  0001 C CNN
F 3 "~" H 5300 2650 50  0001 C CNN
	1    5300 2650
	1    0    0    -1  
$EndComp
Text GLabel 3200 2900 2    50   Input ~ 0
SCLK_2
Text GLabel 3200 2800 2    50   Input ~ 0
GND
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 5D7C375F
P 1500 6650
F 0 "J4" H 1580 6642 50  0000 L CNN
F 1 "RxTx" H 1580 6551 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1500 6650 50  0001 C CNN
F 3 "~" H 1500 6650 50  0001 C CNN
	1    1500 6650
	1    0    0    -1  
$EndComp
Text GLabel 1300 6650 0    50   Input ~ 0
RXTX
$Comp
L Device:C C2
U 1 1 5DDA518E
P 5900 1900
F 0 "C2" H 5785 1854 50  0000 R CNN
F 1 "100n" H 5785 1945 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5938 1750 50  0001 C CNN
F 3 "~" H 5900 1900 50  0001 C CNN
	1    5900 1900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5DDA5ACC
P 5900 2050
F 0 "#PWR02" H 5900 1800 50  0001 C CNN
F 1 "GND" H 5905 1877 50  0000 C CNN
F 2 "" H 5900 2050 50  0001 C CNN
F 3 "" H 5900 2050 50  0001 C CNN
	1    5900 2050
	1    0    0    -1  
$EndComp
Text GLabel 5900 1750 1    50   Input ~ 0
V12V
$Comp
L Device:CP C1
U 1 1 5D40F4A0
P 5350 1900
F 0 "C1" H 5235 1854 50  0000 R CNN
F 1 "100u" H 5235 1945 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 5388 1750 50  0001 C CNN
F 3 "~" H 5350 1900 50  0001 C CNN
	1    5350 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2050 5900 2050
Connection ~ 5900 2050
Wire Wire Line
	5350 1750 5900 1750
$Comp
L Device:C C6
U 1 1 5D487CC1
P 6950 1900
F 0 "C6" H 6835 1854 50  0000 R CNN
F 1 "100n" H 6835 1945 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6988 1750 50  0001 C CNN
F 3 "~" H 6950 1900 50  0001 C CNN
	1    6950 1900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5D487CCB
P 6950 2050
F 0 "#PWR06" H 6950 1800 50  0001 C CNN
F 1 "GND" H 6955 1877 50  0000 C CNN
F 2 "" H 6950 2050 50  0001 C CNN
F 3 "" H 6950 2050 50  0001 C CNN
	1    6950 2050
	1    0    0    -1  
$EndComp
Text GLabel 6950 1750 1    50   Input ~ 0
Vcc
$Comp
L Connector:Conn_Coaxial J13
U 1 1 5D271B44
P 10300 4250
F 0 "J13" H 10400 4225 50  0000 L CNN
F 1 "Link-out 2-0" H 10400 4134 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10300 4250 50  0001 C CNN
F 3 " ~" H 10300 4250 50  0001 C CNN
	1    10300 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5D27217A
P 10850 5250
F 0 "#PWR016" H 10850 5000 50  0001 C CNN
F 1 "GND" H 10855 5077 50  0000 C CNN
F 2 "" H 10850 5250 50  0001 C CNN
F 3 "" H 10850 5250 50  0001 C CNN
	1    10850 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 5D5D4DE1
P 6700 1900
F 0 "C4" H 6585 1854 50  0000 R CNN
F 1 "100u" H 6585 1945 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 6738 1750 50  0001 C CNN
F 3 "~" H 6700 1900 50  0001 C CNN
	1    6700 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 1750 6950 1750
Wire Wire Line
	6700 2050 6950 2050
Connection ~ 6950 2050
$Comp
L Connector:Conn_Coaxial J14
U 1 1 5D59CD1A
P 10850 5050
F 0 "J14" H 10950 5025 50  0000 L CNN
F 1 "Link-out 2-1" H 10950 4934 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10850 5050 50  0001 C CNN
F 3 " ~" H 10850 5050 50  0001 C CNN
	1    10850 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5D59D11C
P 10300 4450
F 0 "#PWR015" H 10300 4200 50  0001 C CNN
F 1 "GND" H 10305 4277 50  0000 C CNN
F 2 "" H 10300 4450 50  0001 C CNN
F 3 "" H 10300 4450 50  0001 C CNN
	1    10300 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J10
U 1 1 5D5A5258
P 10500 1150
F 0 "J10" H 10600 1125 50  0000 L CNN
F 1 "Link-out 1-0" H 10600 1034 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10500 1150 50  0001 C CNN
F 3 " ~" H 10500 1150 50  0001 C CNN
	1    10500 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5D5A5262
P 10750 2200
F 0 "#PWR013" H 10750 1950 50  0001 C CNN
F 1 "GND" H 10755 2027 50  0000 C CNN
F 2 "" H 10750 2200 50  0001 C CNN
F 3 "" H 10750 2200 50  0001 C CNN
	1    10750 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5D5A5276
P 10500 1350
F 0 "#PWR012" H 10500 1100 50  0001 C CNN
F 1 "GND" H 10350 1300 50  0000 C CNN
F 2 "" H 10500 1350 50  0001 C CNN
F 3 "" H 10500 1350 50  0001 C CNN
	1    10500 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J11
U 1 1 5D7E888A
P 10750 2000
F 0 "J11" H 10850 1975 50  0000 L CNN
F 1 "Link-out 1-1" H 10850 1884 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10750 2000 50  0001 C CNN
F 3 " ~" H 10750 2000 50  0001 C CNN
	1    10750 2000
	1    0    0    -1  
$EndComp
Text GLabel 5100 2350 0    50   Input ~ 0
V12V
Text GLabel 2050 3200 0    50   Input ~ 0
CS
Text GLabel 2050 3300 0    50   Input ~ 0
SCK
Text GLabel 2050 4000 0    50   Input ~ 0
SDI
Text GLabel 2050 3500 0    50   Input ~ 0
GND
Text GLabel 2050 3600 0    50   Input ~ 0
SDA_1
Connection ~ 1250 4250
$Comp
L Device:R R3
U 1 1 5D86398D
P 1950 4400
F 0 "R3" H 2020 4446 50  0000 L CNN
F 1 "2.2k" H 2020 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 1880 4400 50  0001 C CNN
F 3 "~" H 1950 4400 50  0001 C CNN
	1    1950 4400
	1    0    0    -1  
$EndComp
Text GLabel 1950 4550 3    50   Input ~ 0
RXTX
$Comp
L Device:R R7
U 1 1 5D88A14D
P 5350 4400
F 0 "R7" H 5420 4446 50  0000 L CNN
F 1 "10k" H 5420 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5280 4400 50  0001 C CNN
F 3 "~" H 5350 4400 50  0001 C CNN
	1    5350 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5D88A157
P 5650 4400
F 0 "R9" H 5720 4446 50  0000 L CNN
F 1 "10k" H 5720 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5580 4400 50  0001 C CNN
F 3 "~" H 5650 4400 50  0001 C CNN
	1    5650 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 4250 5650 4250
Text GLabel 5500 4250 1    50   Input ~ 0
V33
Text GLabel 5350 4550 3    50   Input ~ 0
R1_2
Text GLabel 5650 4550 3    50   Input ~ 0
R2_2
$Comp
L Device:R R11
U 1 1 5D88BE39
P 5950 4400
F 0 "R11" H 6020 4446 50  0000 L CNN
F 1 "10k" H 6020 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5880 4400 50  0001 C CNN
F 3 "~" H 5950 4400 50  0001 C CNN
	1    5950 4400
	1    0    0    -1  
$EndComp
Text GLabel 5950 4550 3    50   Input ~ 0
R3_2
Wire Wire Line
	5650 4250 5950 4250
Connection ~ 5650 4250
$Comp
L Connector:Screw_Terminal_01x02 J5
U 1 1 5D894359
P 1500 7100
F 0 "J5" H 1580 7092 50  0000 L CNN
F 1 "V12" H 1580 7001 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 1500 7100 50  0001 C CNN
F 3 "~" H 1500 7100 50  0001 C CNN
	1    1500 7100
	1    0    0    -1  
$EndComp
Text GLabel 1300 7100 0    50   Input ~ 0
V12V
Text GLabel 1300 7200 0    50   Input ~ 0
GND
$Comp
L Regulator_Linear:L7805 U1
U 1 1 5D8DFFB7
P 6350 1250
F 0 "U1" H 6350 1492 50  0000 C CNN
F 1 "L7805" H 6350 1401 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 6375 1100 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 6350 1200 50  0001 C CNN
	1    6350 1250
	1    0    0    -1  
$EndComp
Text GLabel 6050 1250 1    50   Input ~ 0
V12V
Text GLabel 6650 1250 1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR03
U 1 1 5D8E1771
P 6350 1550
F 0 "#PWR03" H 6350 1300 50  0001 C CNN
F 1 "GND" H 6355 1377 50  0000 C CNN
F 2 "" H 6350 1550 50  0001 C CNN
F 3 "" H 6350 1550 50  0001 C CNN
	1    6350 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5D987FC7
P 6500 4950
F 0 "C3" H 6385 4904 50  0000 R CNN
F 1 "10n" H 6385 4995 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6538 4800 50  0001 C CNN
F 3 "~" H 6500 4950 50  0001 C CNN
	1    6500 4950
	-1   0    0    1   
$EndComp
$Comp
L Device:C C5
U 1 1 5D9887F8
P 6900 4950
F 0 "C5" H 6785 4904 50  0000 R CNN
F 1 "10n" H 6785 4995 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6938 4800 50  0001 C CNN
F 3 "~" H 6900 4950 50  0001 C CNN
	1    6900 4950
	-1   0    0    1   
$EndComp
$Comp
L Device:C C7
U 1 1 5D988B2D
P 7300 4950
F 0 "C7" H 7185 4904 50  0000 R CNN
F 1 "10n" H 7185 4995 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7338 4800 50  0001 C CNN
F 3 "~" H 7300 4950 50  0001 C CNN
	1    7300 4950
	-1   0    0    1   
$EndComp
Text GLabel 5650 5450 3    50   Input ~ 0
R2_2
Text GLabel 5350 5450 3    50   Input ~ 0
R1_2
$Comp
L Device:R R12
U 1 1 5D988F6A
P 6050 5300
F 0 "R12" H 6120 5346 50  0000 L CNN
F 1 "10k" H 6120 5255 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5980 5300 50  0001 C CNN
F 3 "~" H 6050 5300 50  0001 C CNN
	1    6050 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5D98969C
P 5650 5300
F 0 "R10" H 5720 5346 50  0000 L CNN
F 1 "10k" H 5720 5255 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5580 5300 50  0001 C CNN
F 3 "~" H 5650 5300 50  0001 C CNN
	1    5650 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5D989930
P 5350 5300
F 0 "R8" H 5420 5346 50  0000 L CNN
F 1 "10k" H 5420 5255 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5280 5300 50  0001 C CNN
F 3 "~" H 5350 5300 50  0001 C CNN
	1    5350 5300
	1    0    0    -1  
$EndComp
Text GLabel 6050 5450 3    50   Input ~ 0
R3_2
Text GLabel 5350 5150 1    50   Input ~ 0
R1_2a
Text GLabel 5650 5150 1    50   Input ~ 0
R2_2a
Text GLabel 6050 5150 1    50   Input ~ 0
R3_2a
$Comp
L power:GND #PWR07
U 1 1 5D989F63
P 7300 5100
F 0 "#PWR07" H 7300 4850 50  0001 C CNN
F 1 "GND" H 7305 4927 50  0000 C CNN
F 2 "" H 7300 5100 50  0001 C CNN
F 3 "" H 7300 5100 50  0001 C CNN
	1    7300 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5D98A28F
P 6900 5100
F 0 "#PWR05" H 6900 4850 50  0001 C CNN
F 1 "GND" H 6905 4927 50  0000 C CNN
F 2 "" H 6900 5100 50  0001 C CNN
F 3 "" H 6900 5100 50  0001 C CNN
	1    6900 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5D98A4F7
P 6500 5100
F 0 "#PWR04" H 6500 4850 50  0001 C CNN
F 1 "GND" H 6505 4927 50  0000 C CNN
F 2 "" H 6500 5100 50  0001 C CNN
F 3 "" H 6500 5100 50  0001 C CNN
	1    6500 5100
	1    0    0    -1  
$EndComp
Text GLabel 7300 4800 1    50   Input ~ 0
R3_2a
Text GLabel 6900 4800 1    50   Input ~ 0
R2_2a
Text GLabel 6500 4800 1    50   Input ~ 0
R1_2a
Text GLabel 2050 2900 0    50   Input ~ 0
A3P
Text GLabel 3200 3000 2    50   Input ~ 0
A4P
$Comp
L Connector:Conn_01x07_Female J3
U 1 1 5DCA62E3
P 1300 5400
F 0 "J3" H 1328 5426 50  0000 L CNN
F 1 "Analog in 3-6" H 1328 5335 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 1300 5400 50  0001 C CNN
F 3 "~" H 1300 5400 50  0001 C CNN
	1    1300 5400
	1    0    0    -1  
$EndComp
Text GLabel 1100 5500 0    50   Input ~ 0
A4P
Text GLabel 1100 5400 0    50   Input ~ 0
A3P
Text GLabel 1100 5300 0    50   Input ~ 0
A2P
$Comp
L power:GND #PWR01
U 1 1 5DCB9564
P 1100 5100
F 0 "#PWR01" H 1100 4850 50  0001 C CNN
F 1 "GND" H 1105 4927 50  0000 C CNN
F 2 "" H 1100 5100 50  0001 C CNN
F 3 "" H 1100 5100 50  0001 C CNN
	1    1100 5100
	0    1    1    0   
$EndComp
Text GLabel 3200 3800 2    50   Input ~ 0
RXTX
Text GLabel 3200 3700 2    50   Input ~ 0
R1
Text GLabel 3200 3600 2    50   Input ~ 0
R2
Text Notes 3900 4400 0    50   ~ 0
19
Text Notes 4850 4400 0    50   ~ 0
26
Text Notes 1750 6650 0    50   ~ 0
39
Text Notes 1750 6750 0    50   ~ 0
36
Text GLabel 1100 5200 0    50   Input ~ 0
A1P
Text GLabel 3250 6300 0    50   Input ~ 0
SCK
Text GLabel 3250 6500 0    50   Input ~ 0
SDI
Text GLabel 2050 2800 0    50   Input ~ 0
T_CS
Text GLabel 2050 2600 0    50   Input ~ 0
RESET
Text GLabel 3250 5700 0    50   Input ~ 0
V33a
Text GLabel 3200 3100 2    50   Input ~ 0
SDO
Text GLabel 3250 6600 0    50   Input ~ 0
SDO
Text GLabel 1300 6750 0    50   Input ~ 0
GND
Wire Wire Line
	1250 4250 1950 4250
Text GLabel 2050 3400 0    50   Input ~ 0
R2_2a
Text Notes 3900 4000 0    50   ~ 0
36
$Comp
L Oscillator:Si5351A-B-GT U2
U 1 1 5EBB0A76
P 8800 1950
F 0 "U2" H 8400 2650 50  0000 C CNN
F 1 "Si5351A-B-GT" H 8800 2650 50  0000 C CNN
F 2 "Package_SO:MSOP-10_3x3mm_P0.5mm" H 8800 1150 50  0001 C CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si5351-B.pdf" H 8450 1850 50  0001 C CNN
	1    8800 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EBB3776
P 8800 2450
F 0 "#PWR010" H 8800 2200 50  0001 C CNN
F 1 "GND" H 8805 2277 50  0000 C CNN
F 2 "" H 8800 2450 50  0001 C CNN
F 3 "" H 8800 2450 50  0001 C CNN
	1    8800 2450
	1    0    0    -1  
$EndComp
Text GLabel 8300 2150 0    50   Input ~ 0
SCL_1
$Comp
L Device:Crystal_GND2_Small Y1
U 1 1 5EBB7B66
P 7950 1750
F 0 "Y1" V 7996 1619 50  0000 R CNN
F 1 "Crystal" V 7905 1619 50  0000 R CNN
F 2 "Crystal:Crystal_SMD_TXC_7M-4Pin_3.2x2.5mm_HandSoldering" H 7950 1750 50  0001 C CNN
F 3 "~" H 7950 1750 50  0001 C CNN
	1    7950 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 1450 8300 1450
Wire Wire Line
	8300 1450 8300 1650
Wire Wire Line
	7950 2000 8300 2000
Wire Wire Line
	8300 2000 8300 1850
$Comp
L Device:C C8
U 1 1 5EBBA328
P 8550 800
F 0 "C8" H 8435 754 50  0000 R CNN
F 1 "100n" H 8435 845 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8588 650 50  0001 C CNN
F 3 "~" H 8550 800 50  0001 C CNN
	1    8550 800 
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5EBBBBC8
P 8400 800
F 0 "#PWR08" H 8400 550 50  0001 C CNN
F 1 "GND" H 8405 627 50  0000 C CNN
F 2 "" H 8400 800 50  0001 C CNN
F 3 "" H 8400 800 50  0001 C CNN
	1    8400 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 800  8700 1050
Wire Wire Line
	8700 1050 8900 1050
Wire Wire Line
	8900 1050 8900 1450
Connection ~ 8700 1050
Wire Wire Line
	8700 1050 8700 1450
Text GLabel 8550 3700 1    50   Input ~ 0
V33a
$Comp
L Oscillator:Si5351A-B-GT U3
U 1 1 5EBBE7DB
P 8650 4850
F 0 "U3" H 8250 5550 50  0000 C CNN
F 1 "Si5351A-B-GT" H 8650 5550 50  0000 C CNN
F 2 "Package_SO:MSOP-10_3x3mm_P0.5mm" H 8650 4050 50  0001 C CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si5351-B.pdf" H 8300 4750 50  0001 C CNN
	1    8650 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5EBBE7E5
P 8650 5350
F 0 "#PWR011" H 8650 5100 50  0001 C CNN
F 1 "GND" H 8655 5177 50  0000 C CNN
F 2 "" H 8650 5350 50  0001 C CNN
F 3 "" H 8650 5350 50  0001 C CNN
	1    8650 5350
	1    0    0    -1  
$EndComp
Text GLabel 8150 5050 0    50   Input ~ 0
SCL_2
$Comp
L Device:Crystal_GND2_Small Y2
U 1 1 5EBBE7F0
P 7800 4650
F 0 "Y2" V 7846 4519 50  0000 R CNN
F 1 "Crystal" V 7755 4519 50  0000 R CNN
F 2 "Crystal:Crystal_SMD_TXC_7M-4Pin_3.2x2.5mm_HandSoldering" H 7800 4650 50  0001 C CNN
F 3 "~" H 7800 4650 50  0001 C CNN
	1    7800 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 4350 8150 4350
Wire Wire Line
	8150 4350 8150 4550
Wire Wire Line
	7800 4900 8150 4900
Wire Wire Line
	8150 4900 8150 4750
$Comp
L Device:C C9
U 1 1 5EBBE800
P 8400 3700
F 0 "C9" H 8285 3654 50  0000 R CNN
F 1 "100n" H 8285 3745 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8438 3550 50  0001 C CNN
F 3 "~" H 8400 3700 50  0001 C CNN
	1    8400 3700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5EBBE80A
P 8250 3700
F 0 "#PWR09" H 8250 3450 50  0001 C CNN
F 1 "GND" H 8255 3527 50  0000 C CNN
F 2 "" H 8250 3700 50  0001 C CNN
F 3 "" H 8250 3700 50  0001 C CNN
	1    8250 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3700 8550 3950
Wire Wire Line
	8550 3950 8750 3950
Wire Wire Line
	8750 3950 8750 4350
Connection ~ 8550 3950
Wire Wire Line
	8550 3950 8550 4350
$Comp
L power:GND #PWR014
U 1 1 5EBFB386
P 10500 3400
F 0 "#PWR014" H 10500 3150 50  0001 C CNN
F 1 "GND" H 10505 3227 50  0000 C CNN
F 2 "" H 10500 3400 50  0001 C CNN
F 3 "" H 10500 3400 50  0001 C CNN
	1    10500 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J12
U 1 1 5EBFB390
P 10500 3200
F 0 "J12" H 10600 3175 50  0000 L CNN
F 1 "Link-out 1-2" H 10600 3084 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10500 3200 50  0001 C CNN
F 3 " ~" H 10500 3200 50  0001 C CNN
	1    10500 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 2150 10550 2000
Wire Wire Line
	10300 3000 10300 3200
$Comp
L power:GND #PWR017
U 1 1 5EC046B5
P 10600 5950
F 0 "#PWR017" H 10600 5700 50  0001 C CNN
F 1 "GND" H 10605 5777 50  0000 C CNN
F 2 "" H 10600 5950 50  0001 C CNN
F 3 "" H 10600 5950 50  0001 C CNN
	1    10600 5950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J15
U 1 1 5EC046BF
P 10600 5750
F 0 "J15" H 10700 5725 50  0000 L CNN
F 1 "Link-out 2-2" H 10700 5634 50  0000 L CNN
F 2 "Connector_Coaxial:SMA_Amphenol_132134_Vertical" H 10600 5750 50  0001 C CNN
F 3 " ~" H 10600 5750 50  0001 C CNN
	1    10600 5750
	1    0    0    -1  
$EndComp
Text GLabel 2050 3000 0    50   Input ~ 0
SDA_2
Text GLabel 8150 5150 0    50   Input ~ 0
SDA_2
Text GLabel 2050 3100 0    50   Input ~ 0
SCL_2
Wire Wire Line
	10450 5050 10650 5050
Wire Wire Line
	10050 4250 10100 4250
Wire Wire Line
	10400 5750 10450 5750
Text GLabel 5100 2450 0    50   Input ~ 0
SER_2
Text GLabel 4050 850  1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR019
U 1 1 5EC7BD44
P 4350 1150
F 0 "#PWR019" H 4350 900 50  0001 C CNN
F 1 "GND" H 4355 977 50  0000 C CNN
F 2 "" H 4350 1150 50  0001 C CNN
F 3 "" H 4350 1150 50  0001 C CNN
	1    4350 1150
	1    0    0    -1  
$EndComp
Text GLabel 4650 850  2    50   Input ~ 0
V33a
$Comp
L Device:C C17
U 1 1 5EC7C3D6
P 4000 1550
F 0 "C17" H 3885 1504 50  0000 R CNN
F 1 "100n" H 3885 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4038 1400 50  0001 C CNN
F 3 "~" H 4000 1550 50  0001 C CNN
	1    4000 1550
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5EC7C3E0
P 4000 1700
F 0 "#PWR018" H 4000 1450 50  0001 C CNN
F 1 "GND" H 4005 1527 50  0000 C CNN
F 2 "" H 4000 1700 50  0001 C CNN
F 3 "" H 4000 1700 50  0001 C CNN
	1    4000 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C16
U 1 1 5EC7C3EB
P 3450 1550
F 0 "C16" H 3335 1504 50  0001 R CNN
F 1 "10u" H 3335 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3488 1400 50  0001 C CNN
F 3 "~" H 3450 1550 50  0001 C CNN
	1    3450 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1700 4000 1700
Connection ~ 4000 1700
Wire Wire Line
	3450 1400 4000 1400
$Comp
L Regulator_Linear:AMS1117 U4
U 1 1 5EC7A0A3
P 4350 850
F 0 "U4" H 4350 1092 50  0000 C CNN
F 1 "AMS1117" H 4350 1001 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4350 1050 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4450 600 50  0001 C CNN
	1    4350 850 
	1    0    0    -1  
$EndComp
Text GLabel 4000 1400 1    50   Input ~ 0
V33a
Text GLabel 3200 4100 2    50   Input ~ 0
V33
Text GLabel 7450 2250 1    50   Input ~ 0
V33
$Comp
L Device:R R13
U 1 1 5ECA54C7
P 7300 3450
F 0 "R13" H 7370 3496 50  0000 L CNN
F 1 "2.2k" H 7370 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7230 3450 50  0001 C CNN
F 3 "~" H 7300 3450 50  0001 C CNN
	1    7300 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5ECA54D1
P 7600 3450
F 0 "R14" H 7670 3496 50  0000 L CNN
F 1 "2.2k" H 7670 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7530 3450 50  0001 C CNN
F 3 "~" H 7600 3450 50  0001 C CNN
	1    7600 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3300 7600 3300
Text GLabel 7300 3600 3    50   Input ~ 0
SCL_2
Text GLabel 7600 3600 3    50   Input ~ 0
SDA_2
Text GLabel 7450 3300 1    50   Input ~ 0
V33a
$Comp
L Transistor_BJT135:BFR92 Q1
U 1 1 5ECAE018
P 9650 4050
F 0 "Q1" H 9840 4096 50  0000 L CNN
F 1 "BFR92" H 9840 4005 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9850 3975 50  0001 L CIN
F 3 "" H 9650 4050 50  0001 L CNN
	1    9650 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5ECB269C
P 9300 4650
F 0 "R15" V 9093 4650 50  0000 C CNN
F 1 "1k" V 9184 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9230 4650 50  0001 C CNN
F 3 "~" H 9300 4650 50  0001 C CNN
	1    9300 4650
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 5ECB312F
P 9300 4850
F 0 "R16" V 9200 5000 50  0000 C CNN
F 1 "1k" V 9184 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9230 4850 50  0001 C CNN
F 3 "~" H 9300 4850 50  0001 C CNN
	1    9300 4850
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 5ECB346A
P 9300 5050
F 0 "R17" V 9200 5200 50  0000 C CNN
F 1 "1k" V 9184 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9230 5050 50  0001 C CNN
F 3 "~" H 9300 5050 50  0001 C CNN
	1    9300 5050
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT135:BFR92 Q3
U 1 1 5ECB36EF
P 9900 4850
F 0 "Q3" H 10090 4896 50  0000 L CNN
F 1 "BFR92" H 10090 4805 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10100 4775 50  0001 L CIN
F 3 "" H 9900 4850 50  0001 L CNN
	1    9900 4850
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT135:BFR92 Q2
U 1 1 5ECB5880
P 9700 5800
F 0 "Q2" H 9890 5846 50  0000 L CNN
F 1 "BFR92" H 9890 5755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9900 5725 50  0001 L CIN
F 3 "" H 9700 5800 50  0001 L CNN
	1    9700 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4850 9700 4850
Wire Wire Line
	10150 5050 10000 5050
Wire Wire Line
	10150 6000 10150 5750
Text GLabel 9750 3850 1    50   Input ~ 0
V33a
Text GLabel 10000 4650 1    50   Input ~ 0
V33a
Text GLabel 9800 5600 1    50   Input ~ 0
V33a
$Comp
L Device:R R19
U 1 1 5ECBB1BB
P 10300 5750
F 0 "R19" H 10230 5704 50  0000 R CNN
F 1 "20" H 10230 5795 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 10230 5750 50  0001 C CNN
F 3 "~" H 10300 5750 50  0001 C CNN
	1    10300 5750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9450 4650 9450 4050
$Comp
L Device:R R18
U 1 1 5ECBE257
P 9900 4250
F 0 "R18" H 9830 4204 50  0000 R CNN
F 1 "20" H 9830 4295 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9830 4250 50  0001 C CNN
F 3 "~" H 9900 4250 50  0001 C CNN
	1    9900 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R20
U 1 1 5ECC6099
P 10300 5050
F 0 "R20" H 10230 5004 50  0000 R CNN
F 1 "20" H 10230 5095 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 10230 5050 50  0001 C CNN
F 3 "~" H 10300 5050 50  0001 C CNN
	1    10300 5050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9450 5050 9450 5800
Wire Wire Line
	9450 5800 9500 5800
$Comp
L Device:R R21
U 1 1 5ECF4769
P 9450 1750
F 0 "R21" V 9300 1900 50  0000 C CNN
F 1 "1k" V 9334 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9380 1750 50  0001 C CNN
F 3 "~" H 9450 1750 50  0001 C CNN
	1    9450 1750
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT135:BFR92 Q4
U 1 1 5ECF52A6
P 9900 950
F 0 "Q4" H 10090 996 50  0000 L CNN
F 1 "BFR92" H 10090 905 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10100 875 50  0001 L CIN
F 3 "" H 9900 950 50  0001 L CNN
	1    9900 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 950  9700 1750
$Comp
L Transistor_BJT135:BFR92 Q6
U 1 1 5ECF8BF9
P 10150 1950
F 0 "Q6" H 10340 1996 50  0000 L CNN
F 1 "BFR92" H 10340 1905 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10350 1875 50  0001 L CIN
F 3 "" H 10150 1950 50  0001 L CNN
	1    10150 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1950 9950 1950
$Comp
L Transistor_BJT135:BFR92 Q5
U 1 1 5ECFC7D5
P 9900 2800
F 0 "Q5" H 10090 2846 50  0000 L CNN
F 1 "BFR92" H 10090 2755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10100 2725 50  0001 L CIN
F 3 "" H 9900 2800 50  0001 L CNN
	1    9900 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 5ED01171
P 10150 1150
F 0 "R24" H 10080 1104 50  0000 R CNN
F 1 "20" H 10080 1195 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 10080 1150 50  0001 C CNN
F 3 "~" H 10150 1150 50  0001 C CNN
	1    10150 1150
	0    -1   -1   0   
$EndComp
Text GLabel 10000 750  1    50   Input ~ 0
V33a
Text GLabel 10250 1750 1    50   Input ~ 0
V33a
Text GLabel 10000 2600 1    50   Input ~ 0
V33a
$Comp
L Device:R R26
U 1 1 5ED01959
P 10400 2150
F 0 "R26" H 10330 2104 50  0000 R CNN
F 1 "47" H 10330 2195 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 10330 2150 50  0001 C CNN
F 3 "~" H 10400 2150 50  0001 C CNN
	1    10400 2150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R25
U 1 1 5ED01EAB
P 10150 3000
F 0 "R25" H 10080 2954 50  0000 R CNN
F 1 "20" H 10080 3045 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 10080 3000 50  0001 C CNN
F 3 "~" H 10150 3000 50  0001 C CNN
	1    10150 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9600 2800 9700 2800
Wire Wire Line
	9600 2150 9600 2800
Wire Wire Line
	9700 1750 9600 1750
$Comp
L Device:R R22
U 1 1 5ED1C3C2
P 9450 1950
F 0 "R22" V 9400 2150 50  0000 C CNN
F 1 "1k" V 9334 1950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9380 1950 50  0001 C CNN
F 3 "~" H 9450 1950 50  0001 C CNN
	1    9450 1950
	0    1    1    0   
$EndComp
$Comp
L Device:R R23
U 1 1 5ED1C65B
P 9450 2150
F 0 "R23" V 9350 2350 50  0000 C CNN
F 1 "1k" V 9334 2150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 9380 2150 50  0001 C CNN
F 3 "~" H 9450 2150 50  0001 C CNN
	1    9450 2150
	0    1    1    0   
$EndComp
$Comp
L Device:C C18
U 1 1 5EBBBE68
P 8700 3700
F 0 "C18" H 8585 3654 50  0000 R CNN
F 1 "100n" H 8585 3745 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8738 3550 50  0001 C CNN
F 3 "~" H 8700 3700 50  0001 C CNN
	1    8700 3700
	0    -1   -1   0   
$EndComp
Connection ~ 8550 3700
$Comp
L power:GND #PWR026
U 1 1 5EBBC602
P 8850 3700
F 0 "#PWR026" H 8850 3450 50  0001 C CNN
F 1 "GND" H 8855 3527 50  0000 C CNN
F 2 "" H 8850 3700 50  0001 C CNN
F 3 "" H 8850 3700 50  0001 C CNN
	1    8850 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5EBBC9A9
P 8850 800
F 0 "C19" H 8735 754 50  0000 R CNN
F 1 "100n" H 8735 845 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8888 650 50  0001 C CNN
F 3 "~" H 8850 800 50  0001 C CNN
	1    8850 800 
	0    -1   -1   0   
$EndComp
Connection ~ 8700 800 
$Comp
L power:GND #PWR027
U 1 1 5EBBCBFC
P 9000 800
F 0 "#PWR027" H 9000 550 50  0001 C CNN
F 1 "GND" H 9005 627 50  0000 C CNN
F 2 "" H 9000 800 50  0001 C CNN
F 3 "" H 9000 800 50  0001 C CNN
	1    9000 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5EBE73BB
P 4400 1550
F 0 "C20" H 4285 1504 50  0000 R CNN
F 1 "100n" H 4285 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4438 1400 50  0001 C CNN
F 3 "~" H 4400 1550 50  0001 C CNN
	1    4400 1550
	-1   0    0    1   
$EndComp
Wire Wire Line
	4000 1400 4400 1400
Connection ~ 4000 1400
Wire Wire Line
	4000 1700 4400 1700
$Comp
L Device:C C21
U 1 1 5EBF7A99
P 4800 1550
F 0 "C21" H 4685 1504 50  0000 R CNN
F 1 "100n" H 4685 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4838 1400 50  0001 C CNN
F 3 "~" H 4800 1550 50  0001 C CNN
	1    4800 1550
	-1   0    0    1   
$EndComp
$Comp
L Device:C C22
U 1 1 5EBF7F62
P 5200 1550
F 0 "C22" H 5085 1504 50  0000 R CNN
F 1 "100n" H 5085 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5238 1400 50  0001 C CNN
F 3 "~" H 5200 1550 50  0001 C CNN
	1    5200 1550
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 1400 4800 1400
Connection ~ 4400 1400
Wire Wire Line
	5200 1400 4800 1400
Connection ~ 4800 1400
Wire Wire Line
	4400 1700 4800 1700
Connection ~ 4400 1700
Wire Wire Line
	4800 1700 5200 1700
Connection ~ 4800 1700
$Comp
L Device:C C23
U 1 1 5EC1185C
P 5500 1550
F 0 "C23" H 5385 1504 50  0000 R CNN
F 1 "100n" H 5385 1595 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5538 1400 50  0001 C CNN
F 3 "~" H 5500 1550 50  0001 C CNN
	1    5500 1550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 1400 5500 1400
Connection ~ 5200 1400
Wire Wire Line
	5500 1700 5200 1700
Connection ~ 5200 1700
Wire Wire Line
	9800 6000 10150 6000
Wire Wire Line
	7950 1450 7950 1650
Wire Wire Line
	7950 1850 7950 2000
$Comp
L power:GND #PWR0101
U 1 1 5EE5C0AA
P 7850 1750
F 0 "#PWR0101" H 7850 1500 50  0001 C CNN
F 1 "GND" H 7855 1577 50  0000 C CNN
F 2 "" H 7850 1750 50  0001 C CNN
F 3 "" H 7850 1750 50  0001 C CNN
	1    7850 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5EE5C4D2
P 7700 4650
F 0 "#PWR0102" H 7700 4400 50  0001 C CNN
F 1 "GND" H 7705 4477 50  0000 C CNN
F 2 "" H 7700 4650 50  0001 C CNN
F 3 "" H 7700 4650 50  0001 C CNN
	1    7700 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 4750 7800 4900
Wire Wire Line
	7800 4350 7800 4550
$EndSCHEMATC
