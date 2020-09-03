# VFO-ESP32-Si5351 v2.0
The new version v 2.0 uses 2x si5351 on the pcb with buffer transistors to minimize crosstalk. The old version had about 52 db  crosstalk suppression with the new version this is much improved. I cannot hear any injected spurs any more. But will add some measurements later.
The SI5351 library is updated to support multiple si5351's.

If you are using diode double balanced mixers than a square wave is no problem. But I want to use a jfet balanced mixer for that you need to filter the output of the vfo. For this I developed a filter PCB which is controled by the vfo.

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/VFO-PCB1.jpg)

Spectrum of two seperate si5351 with buffer. (12.5 Mhz and 8.998 Mhz)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/DS0049.PNG)

VFO build in into QRP tranceiver with hycas IF and bandpassfilters managed by esp32
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/IMG_20200825_223043.jpg)



# VFO-ESP32-Si5351 v 1.0
VFO ESP32 Si5351 (old master branch)

VFO using ESP32 and ILI9341 display, based on development of T.Uebo / JF3HZB.

Input is optical encoder (frequency adjustment) and rotary encoder  for bandswitch.
Has 74HC595 for switching a pair of as179 hf switches and a connecter for bandpass / lowpass filter board.
PCB in KICAD format available. ESP32 is a 38 pin board available on aliexpress as the SI5351 I used connectors to mount them
PCB Board also has an 7805 for power supply. Connectors are used to expose RX/TX switch, optical encoder, analog inputs for S-Meter and Powermeter.

The PCB contains a pi attenuator network on the output of the SI5351, however research done by QRPLabs suggest not to do this. Only have 100nF DC blockers. These blockers are already in the PCB for the AS179 switches but not for the 3rd output. This can easily be changed if necessary. When the resistor attenuator us removed the crosstalk is much improved. I can measure crosstalk between the outputs but have not done some experiments to see if it can be reduced.

To controle the SI5351 I updated the Etherkit library https://github.com/etherkit/Si5351Arduino
With a new I2C not using the build in I2C because it does not work with SI5351.
Also if used above 100 Mhz a lot of click noise was heard, this is fixed by moving clock 1 and 2 to PLLB and have clock 0 on PLLA. Also the PLL reset is not necessary only when first time set. (PLL resets are only necessary during start up and incase of fixed phase set between outputs.) 

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/master/VFO_PCB1.jpg)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/master/VFO_PCB2.jpg)

The crosstalk measured is about -52db Clock 0 12Mhz, Clock 1 8.998 Mhz
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/master/DS0013.PNG)
