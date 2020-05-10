# VFO-ESP32-Si5351
VFO ESP32 Si5351

VFO using ESP32 and ILI9341 display, based on development of T.Uebo / JF3HZB.

Input is optical encoder (frequency adjustment) and rotary encoder  for bandswitch.
Has 74HC595 for switching a pair of as179 hf switches and a connecter for bandpass / lowpass filter board.
PCB in KICAD format available. ESP32 is a 38 pin board available on aliexpress as the SI5351 I used connectors to mount them
PCB Board also has an 7805 for power supply. Connectors are used to expose RX/TX switch, optical encoder, analog inputs for S-Meter and Powermeter.

The PCB contains a pi attenuator network on the output of the SI5351, however research done by QRPLabs suggest not to do this. Only have 100nF DC blockers. These blockers are already in the PCB for the AS179 switches but not for the 3rd output. This can easily be changed if necessary. I however are not yet sure if this is the right solution to crosstalk. I can measure crosstalk between the outputs but have not done some experiments to see if it can be reduced.

To controle the SI5351 I updated the Etherkit library https://github.com/etherkit/Si5351Arduino
With a new I2C not using the build in I2C because it does not work with SI5351.
Also if used above 100 Mhz a lot of click noise was heard, this is fixed by moving clock 1 and 2 to PLLB and have clock 0 on PLLA. Also the PLL reset is not necessary only when first time set. (PLL resets are only necessary during start up and incase of fixed phase set between outputs.) 

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/master/VFO_PCB1.jpg)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/master/VFO_PCB2.jpg)
