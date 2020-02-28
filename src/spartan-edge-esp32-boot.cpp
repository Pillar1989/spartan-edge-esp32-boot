#include <spartan-edge-esp32-boot.h>
#include "SD_MMC.h"

extern "C" {
  //external header for the pulling up of SD pins at the initialization
  #include "driver/sdmmc_host.h"
}

// initialization
void spartan_edge_esp32_boot::begin(void) {
  // initialize serial communication at 115200 bits per second: 
  Serial.begin(115200); 

  //PULL UP of SD card pins <--- preventing mounting failure due to floating state
  sdmmc_host_pullup_en(1, 4); //Slot: 1 and Bit mode: 4
  
  // ESP/SD pulled up pins list as reference:
  // ESP GPIO2        (pin 22) <---> SD CARD D0
  // ESP GPIO4        (pin 24) <---> SD CARD D1
  // ESP GPIO12/MTDI  (pin 18) <---> SD CARD D2
  // ESP GPIO13/MTCK  (pin 20) <---> SD CARD D3
  // ESP GPIO15/MTDO  (pin 21) <---> SD CARD CMD

  // Mount SD Card
  if(!SD_MMC.begin()) {
    Serial.println("Card Mount Failed,please reboot the board");
	while(1);
    return;
  }
}

// XFPGA pin Initialize
void spartan_edge_esp32_boot::xfpgaGPIOInit(void) {
  // GPIO Initialize
  pinMode(XFPGA_INTB_PIN, INPUT);
  pinMode(XFPGA_DONE_PIN, INPUT);
  pinMode(XFPGA_PROGRAM_PIN, OUTPUT);

  // FPGA configuration start sign
  digitalWrite(XFPGA_PROGRAM_PIN, LOW);
  pinMode(XFPGA_CCLK_PIN, OUTPUT);
  digitalWrite(XFPGA_CCLK_PIN, LOW);
  digitalWrite(XFPGA_PROGRAM_PIN, HIGH);

  // wait until fpga reports reset complete
  while(digitalRead(XFPGA_INTB_PIN) == 0) {}
}

// loading the FPGA LOGIC
int spartan_edge_esp32_boot::xlibsSstream(const char* path) {
  unsigned char byte_buff[1024];
  int byte_len;
  unsigned byte;
  int i = 0;
  int j = 0;
	
  // open the file
  File file = SD_MMC.open(path);
  if(!file) {
    Serial.println("Failed to open");
    Serial.println("please check the file exists ");
    return -1;
  }

  /* read data form bitstream */
  byte_len = file.read(byte_buff, READ_SIZE);

  // find the raw bits
  if(byte_buff[0] != 0xff)
  {
    // skip header
    i = ((byte_buff[0]<<8) | byte_buff[1]) + 4;

    // find the 'e' record
    while(byte_buff[i] != 0x65)
    {
        // skip the record
        i += (byte_buff[i+1]<<8 | byte_buff[i+2]) + 3;
        // exit if the next record isn't within the buffer
        if(i>= byte_len)
            return -1;
    }
    // skip the field name and bitstrem length
    i += 5;
  } // else it's already a raw bin file

  /* put pins down for Configuration */
  pinMode(XFPGA_DIN_PIN, OUTPUT);
  
   /*
   * loading the bitstream
   * If you want to know the details,you can Refer to the following documentation
   * https://www.xilinx.com/support/documentation/user_guides/ug470_7Series_Config.pdf
   */
  while ((byte_len != 0)&&(byte_len != -1)) {
    for ( ;i < byte_len;i++) {
      byte = byte_buff[i];

      for(j = 0;j < 8;j++) {
        REG_WRITE(GPIO_OUT_W1TC_REG, (1<<XFPGA_CCLK_PIN));
        REG_WRITE((byte&0x80)?GPIO_OUT_W1TS_REG:GPIO_OUT_W1TC_REG, (1<<XFPGA_DIN_PIN));
        byte = byte << 1;
        REG_WRITE(GPIO_OUT_W1TS_REG, (1<<XFPGA_CCLK_PIN));
      }
    }
    byte_len = file.read(byte_buff, READ_SIZE);
    i = 0;
  }
  digitalWrite(XFPGA_CCLK_PIN, LOW); 
	
  if(byte_len == -1) Serial.println("read error");
	
  file.close();
	
  // check the result
  if(0 == digitalRead(XFPGA_DONE_PIN)) {
    Serial.println("FPGA Configuration Failed");
  }
  else {
    Serial.println("FPGA Configuration success");
  }
}
