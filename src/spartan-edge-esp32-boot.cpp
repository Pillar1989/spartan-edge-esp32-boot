#include <spartan-edge-esp32-boot.h>
#include "SD_MMC.h"

// initialization
void spartan_edge_esp32_boot::begin(void) {
  // initialize serial communication at 115200 bits per second: 
  Serial.begin(115200); 
  
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
  pinMode(XFPGA_INTB_PIN, OUTPUT | PULLUP);
  digitalWrite(XFPGA_INTB_PIN, HIGH);
  pinMode(XFPGA_PROGRAM_PIN, OUTPUT);
  digitalWrite(XFPGA_PROGRAM_PIN, HIGH);
  delay(50);

  // FPGA configuration start sign
  digitalWrite(XFPGA_INTB_PIN, LOW);
  digitalWrite(XFPGA_PROGRAM_PIN, LOW);
  delay(50);
  digitalWrite(XFPGA_INTB_PIN, HIGH);
  digitalWrite(XFPGA_PROGRAM_PIN, HIGH);
}

// loading the FPGA LOGIC
int spartan_edge_esp32_boot::xlibsSstream(const char* path) {
  unsigned char byte_buff[1024];
  int byte_len;
  unsigned char byte;
  int i = 0;
  int j = 0;
	
  // open the file
  File file = SD_MMC.open(path);
  if(!file) {
    Serial.println("Failed to open");
    Serial.println("please check the file exists ");
    return -1;
  }
   
  // seek 100 byte
  int ret = file.seek(100, SeekSet);
  if(!ret) {
    Serial.println("Failed to seek");
    return -1;
  }

  /* put pins down for Configuration */
  pinMode(XFPGA_CCLK_PIN, OUTPUT);
  pinMode(XFPGA_DIN_PIN, OUTPUT);
  digitalWrite(XFPGA_CCLK_PIN, LOW);
  digitalWrite(XFPGA_DIN_PIN, LOW);
  
  /* read data form bitstream */
  byte_len = file.read(byte_buff, READ_SIZE);
   /*
   * loading the bitstream
   * If you want to know the details,you can Refer to the following documentation
   * https://www.xilinx.com/support/documentation/user_guides/ug470_7Series_Config.pdf
   */
  while ((byte_len != 0)&&(byte_len != -1)) {
    for (i = 0;i < byte_len;i++) {
      byte = byte_buff[i];

      for(j = 0;j < 8;j++) {
        digitalWrite(XFPGA_CCLK_PIN, LOW);
        if ((byte&0x80) == 0x80)
          digitalWrite(XFPGA_DIN_PIN, HIGH);
        else
          digitalWrite(XFPGA_DIN_PIN, LOW);
        byte = byte << 1;
        digitalWrite(XFPGA_CCLK_PIN, HIGH);
      }
    }
    byte_len = file.read(byte_buff, READ_SIZE);
  }
  digitalWrite(XFPGA_CCLK_PIN, LOW); 
	
  if(byte_len == -1) Serial.println("read error");
	
  file.close();
	
  // check the result
  pinMode(XFPGA_DONE_PIN, INPUT);
  if(0 == digitalRead(XFPGA_DONE_PIN)) {
    Serial.println("FPGA Configuration Failed");
  }
  else {
    Serial.println("FPGA Configuration success");
  }
}
