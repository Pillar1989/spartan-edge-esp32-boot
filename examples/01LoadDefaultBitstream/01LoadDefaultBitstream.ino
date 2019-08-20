/*
 * 01LoadDefaultBitstream
 *  
 * loading the default Bitstream
 *
 * The MIT License (MIT)
 * Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the library:
#include <spartan-edge-esp32-boot.h>

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

// the bitstream name which we loading
#define LOADING_DEFAULT_FIEE "/overlay/default.bit"

// the setup routine runs once when you press reset:
void setup() {
  // initialization 
  esp32Cla.begin();

  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(LOADING_DEFAULT_FIEE);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("run");
  delay(3000);
}
