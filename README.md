# VFO-ESP32-Si5351 v2.43 beta
In this version the GUI is changed to support touch using simple buttons at the bottom of the screen.
It took some time to have them also supported by using the rotary encoder.

Changes:
- TFT library has been changed to TFT_eSPI, download the library in Arduine IDE and config the User_Setup.h to your configuration or use the one in this repository for the PCB in this project
- LGVL library is used for GUI. Install in Arduino IDE LVGL library (not Arduino LVGL) copy the lv_conf.h to you library directory to config lvgl library or create your own config file to your liking.
- I use now Visual Studio Community 2019 with Visual Micro to compile the project. But Arduino IDE is also supported.
- source has been moved to vfo_esp32_si5351 directory
- vfo1 and vfo2 support
- gui supported wifi configuration
- gui supported si5351 configuration
- libraries are moved to library directory 
- source code includes a visual studio project file
- TX GUI with power and SWR measurement
- GUI supported AD8307 calibration
- Bfo adjustment, A button select bfo adjustment and use the optical encoder to change frequency.


Notes
- The si5351 library is customized for esp32 support and can be found in the library directory
- Libraries used
   -  Acebutton https://github.com/bxparks/AceButton
   -  ESP32Encoder https://github.com/madhephaestus/ESP32Encoder
   -  lvgl https://lvgl.io/

New GUI with buttons at the bottom   
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/lvgl_gui.jpg)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/lvgl_gui2.jpg)

# VFO-ESP32-Si5351 v2.41 beta
update-- added Power and SWR gauges
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/swr_pwr.jpg)
# VFO-ESP32-Si5351 v2.4 beta
Version 2.33 source code is moved to the main branch, as I am now working on a new version of the GUI based on LVGL userinterface.
This because the old userinterface became to complex to maintain if I want to add wifi management and other setup pages.
The software is not yet complete, however enough to test.

Gui supported Wifi settings
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/wifi_gui.jpg)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/wifi_gui2.jpg)
Gui supported si5351 calibration
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/si5351_gui.jpg)

# VFO-ESP32-Si5351 v2.33 (main branch)
Updated network functions. Added telnet support for debugging, CAT interface on port 1003 support.
CAT interface will send frequency updates when the VFO is tuned. This is used by RF-AMP and later Antenna tunner to follow the frequencies
See PWRSWR repository for the rf-amp esp32 controler with swr meter functionality. I use same VFO PCB also for rf-amp controler. 

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/trxnet.jpg)

# VFO-ESP32-Si5351 v2.32
Updated the ota software to support Arduino IDE interface, recently I have dropped the Arduino IDE for Visual Studio 2019 which gives a bit more software development support
The main changes for this version is a Network CAT support the idea is from TRXNET (TRX-Manager) unfortunally TRX-Manager is discontinued. But the idea of a CAT interface on tcp/ip is nice. I use this interface to let the VFO to communicate with other ESP32 controlers to automatic send commands to my rf-amp. Also I want to use it to communicate with my automatic antenna tuner. Will be added to github later.

# VFO-ESP32-Si5351 v2.31
Some PCB updates, added output capacitors and changed the resistor pi network on the output to simple voltage divider.
The pi network caused a higher current in the buffer transistor which causes the board to radiate some BFO frequency.
This was picked up by my IF and caused the AGC to react. It seems to be better to lower the current in the buffer transistor.
The output voltage can be regulated by adjusting the resitor divider.

# VFO-ESP32-Si5351 v2.3
I added a new version of the pcb to support Wifi and uses a buck converter for powersupply.
I am still testing if the buck converter does not generate to much noise.
Source code is adjusted to use new GPIO ports so ADC1 is only used
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/VFO-PCB4.jpg)

# VFO-ESP32-Si5351 v2.2
Now my transceiver is in a box, it is more easy to update software using wifi. This release support this. The only problem is that some analog ports will not support both ADC and wifi, in the next release I will update the PCB and rearange the analog inputs to user ADC1 only.

# VFO-ESP32-Si5351 v2.1
The PCB has a connector for analog I/O this is intended to support facilities like SWR meter, Voltage and current meter. In the src directory I have put a version of the software that support SWR measurement and CAT interface. Also some simple calibration menu's are added. But this is still in development. The code for SWR comes from Loftur E. Jónasson - TF3LJ / VE2LJX but was intended for Teensy. I made a ESP32 implementation of it and with support for ILI9341 adafruit library. 
I will update (hope fully) this extension the comming months to have full calibration capability for both si5351 and the AD8307 log detectors. A pcb for the SWR meter is also developed this I will put in a seperate repository.
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/vfo7.jpg)

# VFO-ESP32-Si5351 v2.0
The new version v 2.0 uses 2x si5351 on the pcb with buffer transistors to minimize crosstalk. The old version had about 52 db  crosstalk suppression with the new version this is much improved. I cannot hear any injected spurs any more. But will add some measurements later.
The SI5351 library is updated to support multiple si5351's.

If you are using diode double balanced mixers than a square wave is no problem. But if you want to use a jfet balanced mixer than you need to filter the output of the vfo. For this I developed a filter PCB which is controled by the vfo.

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/vfo4.jpg)

![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/VFO-PCB1.jpg)

Spectrum of two seperate si5351 with buffer. (12.5 Mhz and 8.998 Mhz)
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/DS0049.PNG)

VFO build in into QRP tranceiver with hycas IF and bandpassfilters managed by esp32
The VFO uses a optical encoder for frequency adjustment and mechanical rotary for bandswitch and option selection.
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/IMG_20200825_223043.jpg)

Bottom side with transmitter amplifier and low-pass filter section also managed by esp32.
![VFO](https://github.com/paulh002/VFO-ESP32-Si5351/blob/ESP32-VFO-with-2-Si5351/IMG_20200902_211710.jpg)


# VFO-ESP32-Si5351 v 1.0
VFO ESP32 Si5351 (old master branch)

VFO using ESP32 and ILI9341 display, based on development of T.Uebo / JF3HZB.

Input is optical encoder (360 pulses ZSP3806 Encoder for frequency adjustment) and rotary encoder  for bandswitch.
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
