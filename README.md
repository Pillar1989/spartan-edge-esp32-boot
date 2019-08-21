# spartan-edge-esp32-boot
###  a library for Spartan Edge Accelerator Board
The purpose of this library is to load bitstream(FPGA Logic) from SDCard to the on-board FPGA(xc7s15) by the on-board ESP32.

The software development environment is [Arduino IDE](https://www.arduino.cc/en/Main/Software) with [ESP32 Boards support](https://github.com/espressif/arduino-esp32).

## Contents
- [Spartan Edge Accelerator Board](#Spartan-Edge-Accelerator-Board)
- [Arduino IDE ESP32 Boards Support](#Arduino-IDE-with-ESP32-Boards-Support)
- [Library Usage](#Library-Usage)

<br>

## Spartan Edge Accelerator Board
It can be used independently as an Arduino compatible board, or plugged into an Arduino Boards(Arduino UNO eg.) as an auxiliary accelerator board.  
![pic](extras/spartan.png)

<br>

## Arduino IDE with ESP32 Boards Support
___using Arduino IDE Boards Manager___  
Starting with 1.6.4, Arduino allows installation of third-party platform packages using Boards Manager. We have packages available for Windows, Mac OS, and Linux (32 and 64 bit).

- Install the current upstream Arduino IDE at the 1.8 level or later. The current version is at the [Arduino website](http://www.arduino.cc/en/main/software).
- Start Arduino and open Preferences window.
- Enter ```https://dl.espressif.com/dl/package_esp32_index.json``` into *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install *esp32* platform (and don't forget to select your ESP32 board from Tools > Board menu after installation).
- select ___tool->board->DOIT ESP32 DEVKIT___   


Stable release link: `https://dl.espressif.com/dl/package_esp32_index.json`  
Development release link: `https://dl.espressif.com/dl/package_esp32_dev_index.json`  
If you want more details, you can click the [link](https://github.com/espressif/arduino-esp32)

<br>

## Library Usage

1. ***Library Installation***  
  see [Installing Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries)

2. ***Prepare SDCard***  
  2.1 Format the SDCard with FAT16/FAT32 filesystem.  
  2.2 Create a top level subfolder named __overlay__ in the SDCard.  
  2.3 Put your bitstream or [sample bitstream](#Sample-bitstreams) files (must have a extend name .bit) into the folder __overlay__.  
  2.4 If you run example 01LoadDefaultBitstream, rename the bitstream file in __overlay__ to __default.bit__.  
  2.5 If you run example 02LoadConfigBitstream, put [**board_config.ini**](extras/board_config.ini) into SDCard root folder.  
  2.6 Insert the SDCard to the Spartan (Edge Accelerator) Board.  

3. ***Upload example***  
  3.1 Connect the Spartan Board through USB Type-C wire to the PC, and install USB232 driver (chip CP2102).  
  3.2 Turn the power switch (near the USB Type-C slot) to USB side to power on the board.  
  3.3 Open one of the library examples by Arduino IDE.  
  3.4 Check Board & Port setting in Arduino IDE as described in last section.  
  3.5 Press 'BOOT' Button on Sparton Board and last more than 1 seconds to force ESP32 enter Bootloader mode.  
  3.6 Press 'Upload' button in Arduino IDE to upload the example ('s compiled binary) to ESP32.  

4. ***Run example***  
  4.1 Make sure the on-board DIP-switch K5 (last one) on Slave(ON) side, which enable FPGA programing by other device(MCU).  
  4.2 Press 'RST' button on Spartan Board to startup the example.  
  4.3 After the example bootup a few seconds, the FPGA_DONE(red color) LED on the board will light on.

<br>

#### Sample bitstreams
Some bitstream/FPGA_LOGIC files we compiled are provided here, you can download them directly.  
  + [spi2gpio.bit](https://github.com/sea-s7/AlphaTest_V1.0/tree/master/spi2gpio)  
  GPIO extension controlled by SPI interface, with support of ADC/DAC/RGB-LED, see ***[source code](https://github.com/sea-s7/Demo_project/tree/master/spi2gpio)***.  

<br>

#### Library examples
  + 01LoadDefaultBitstream  
    This example will load SDCard file /overlay/default.bit to FPGA  

  + 02LoadConfigBitstream  
    This example will read a ___ini___ format file /board_config.ini in SDCard,  
    then load the bitstream spcified by the value of key ***overlay_on_boot*** to FPGA.  

