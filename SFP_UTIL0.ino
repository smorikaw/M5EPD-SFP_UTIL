//
// M5 paper 960x540 dot screen
// 0 to 58 top menu
// 6 to 499 detail view
// 500 o 879 HEX view 379H
//
// PORT A I2C/ PORT B DAC/ADC / PORT C UART
//
#include <M5EPD.h>
#include <Wire.h>
#include "qsfp.h"
#include "sff-common.h"
#include "binaryttf.h"

unsigned char EEPROM_DATA[512];
unsigned char EEPROM_DATA2[512];
// serial number
char SN[32];
char fnP[512][64] = {"", "/C90AW42595.bin","", "/C90AW41867.bin", "/C90AW41865.bin", "/C90AW41733.bin"};
int DIRmax=0,DIRtop=0;
int lY = 0;
int menuP = 1; // 0:load file , 1:read I2C , 2:save file
int fileP = 0;
int dataSize = 0;
const int menuW = 540/6;
const int pitch = 30;
const int HEXpitch = 22;
const uint8_t WR_PASS[] = {0x00, 0x00, 0x00, 0x00};
#define I2CADD 0x50;

char temStr[10];
float tem;
rtc_time_t RTCtime;
rtc_date_t RTCDate;

M5EPD_Canvas canvas(&M5.EPD);
M5EPD_Canvas DTLcanvas(&M5.EPD);
M5EPD_Canvas HEXcanvas(&M5.EPD);
M5EPD_Canvas MENUcanvas(&M5.EPD);
M5EPD_Canvas filePcanvas(&M5.EPD);
//---------------------------------------------------------------
// save fle name set from vender Serial number
void setSN(int offset, int len){
      int i = 0;
      SN[i] = '/';
      while(EEPROM_DATA[i+offset] != 0x20){
        SN[i+1] = EEPROM_DATA[offset+i];
        i++;
        if(i>16) break;
      }
      SN[i+1] = '.';
      SN[i+2] = 'b';
      SN[i+3] = 'i';
      SN[i+4] = 'n';                  
      SN[i+5] = 0x00;
}
//================================== dir display and file select ===========================================
void drawfileP(){
  filePcanvas.fillCanvas(0);
  filePcanvas.setTextSize(48);
  filePcanvas.drawString(">" , 0, (fileP-1)*pitch);
  filePcanvas.pushCanvas(2,60,UPDATE_MODE_DU);
}
void upfileP(){
 fileP++;
 if(fileP >  15  ) ShowDirPageUp();
 drawfileP();
}
void downfileP(){
 fileP--;
 if(fileP < 1 ) ShowDirPageDown();
 drawfileP();
}
void ShowDirList(){
  int i, locY=0, DIRlen;
      if((DIRtop+15)< DIRmax) {DIRlen =15; }else{ DIRlen = DIRmax - DIRtop;}
     DTLcanvas.fillCanvas(0);
     DTLcanvas.setTextSize(32);
     DTLcanvas.setTextColor(15,0);
   for(i=0; i < DIRlen ; i++){
         DTLcanvas.drawString(fnP[i+DIRtop+1], 10, locY++ * pitch);
   }
   DTLcanvas.pushCanvas(20,60,UPDATE_MODE_DU);
}
void ShowDirPageUp(){

  if((DIRtop+15) < DIRmax){
    DIRtop += 15;
    fileP = 1;
    ShowDirList();
  }
}
void ShowDirPageDown(){

  if(DIRtop>15){
    DIRtop -= 15;
    fileP = 15;
    ShowDirList();
    }
}

void ShowDir(){
 int locY = 0,i  = 1, DIRlen;
 File file;
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(32);
 file = SD.open("/", FILE_READ);
 if(file == false){
        DTLcanvas.drawString("SD read  fail !!", 10, locY++ * pitch);
 }
 while(true){
  File entry = file.openNextFile();
  if(! entry){
    break;
  }else{
    if(entry.isDirectory()){
//      DTLcanvas.drawString(entry.name(), 10, locY++ * pitch);
//    directory file is not display
    }else{
      sprintf(fnP[i++],"%s", entry.name());
     }
   }
 }
   DIRmax = i;
   DIRtop = 0;
    ShowDirList();
   fileP = 1;
   drawfileP();
   delay(500);
}
//==============================================================================
int FileRead(char *fn){
  int i,j, size, locY = 1,offset = 0;
  byte c;
  char buf[33];
  String s;
  File file;

fileP = 0;
// open error checkを加える
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(32);
  file = SD.open(fn, FILE_READ);
  if(file == false){
      DTLcanvas.drawString("try open " + String(fn), 10, locY++ * pitch);
      DTLcanvas.drawString("file read fail !!", 10, locY++ * pitch);
  }else{
  dataSize = file.size();
  if(dataSize == 128) offset = 128;
  if(dataSize > 256 ) dataSize = 256;
  for (i=0 ; i < dataSize ; i++){
    EEPROM_DATA[i + offset] = file.read();
  }
      DTLcanvas.drawString("try open " + String(fn), 10, locY++ * pitch);
      DTLcanvas.drawString("file read success !!", 10, locY++ * pitch);
      locY++;
      DTLcanvas.setTextSize(28);
      DTLcanvas.drawString("     >>> ASCII DUMP <<<", 20, locY++ * pitch);
    for(i=0 ; i < 8 ; i++){
      s = "";
      for(j = 0; j < 32 ; j++){
        c = EEPROM_DATA[i*32+j];
        if(c > 0x20 && c < 0x7f){
          sprintf(buf, "%c",c);
        }else {
          sprintf(buf, ".",c);
        }
        s.concat(buf);
    }
        DTLcanvas.drawString(s , 20, locY++ * pitch);
  }

  }
  file.close();

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);

        dispHEXdump();

  return 0;
}
int FileRead2(char *fn){
  int i,j, size, locY = 1,offset = 0;
  byte c;
  char buf[33];
  String s;
  File file;

// open error checkを加える
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(32);
  file = SD.open(fn, FILE_READ);
  if(file == false){
      DTLcanvas.drawString("try open " + String(fn), 10, locY++ * pitch);
      DTLcanvas.drawString("file read fail !!", 10, locY++ * pitch);
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
  }else{
  dataSize = file.size();
  if(dataSize == 128) offset = 128;
  if(dataSize > 256 ) dataSize = 256;
  for (i=0 ; i < dataSize ; i++){
    EEPROM_DATA2[i + offset] = file.read();
  }
        dispHEXDIFFdump();
 
  }
  file.close();
  fileP = 0;   // back main menu
  return 0;
}

int FileWrite(char *fn){
  
  int locY = 0;
  File file;
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(32);
  if(fn[0] == 0x00){
        DTLcanvas.drawString("file name not set", 10, locY++ * pitch);
  }else{
  if(SD.exists(fn)){
    DTLcanvas.drawString(String(fn) + " exists!", 10, locY++ * pitch);
  }
  file =SD.open(fn, FILE_WRITE);  // over write mode
  DTLcanvas.drawString("try write " + String(fn), 10, locY++ * pitch);
  file.write(EEPROM_DATA, dataSize);
  DTLcanvas.drawString("file write success !!", 10, locY++ * pitch);
  file.close();

    }
  DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
}
// ==============================================
//   bool result[0x80];
//  M5.I2C.scanID(&result[0]);
//  bool readByte(uint8_t address, uint8_t subAddress,uint8_t *result);
//  bool readBytes(uint8_t address, uint8_t count,uint8_t * dest);
//  bool readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
//  
// I2C / write EEPROM data
//
int WriteI2C(){
  int i;
  // write password 1st
  Wire1.beginTransmission(0x51);
  Wire1.write(0x7b);
  Wire1.write(0x00);
  Wire1.write(0x00);
  Wire1.write(0x00);
  Wire1.write(0x00);
  Wire1.endTransmission();
  delay(5);
  for(i=0; i < 0xff;i++){
    Wire1.beginTransmission(0x50);
    Wire1.write(i);
    Wire1.write(EEPROM_DATA[i]);
    Wire1.endTransmission();
    delay(5);
  }
  
}
//
//    I2C / PORT A G25/G32
//
int ReadI2C(){
  int i, cc1=0,cc2=0,locY=0;
  int cc_base, cc_ext;
  char buf[512];

      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
      DTLcanvas.drawString("Try read I2C/PORT A", 10, locY++ * pitch);
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);

      SN[0] = 0x00;       // clear filename   
for(i=0; i < 0xff; i++){
  Wire1.beginTransmission(0x50);
  Wire1.write(i);
  Wire1.endTransmission();
  delay(5);
  Wire1.requestFrom(0x50, 1);
    delay(5);                // IMPORTANT !!!
  EEPROM_DATA[i] = Wire1.read();
}

 dataSize = i;

 // check start from 0x00

  if ((EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP) ||
      (EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP_PLUS) ||
      (EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP28)){

      if(check_8636CC()){
        DTLcanvas.drawString("I2C read success !!", 10, locY++ * pitch);
        DTLcanvas.drawString("QSFP/QSFP+/QSFP28", 10, locY++ * pitch);
      }else{
        DTLcanvas.drawString("I2C read fail !!", 10, locY++ * pitch);
      }
      cc1 = calc_8636CC1();
      cc2 = calc_8636CC2();
      cc_base = EEPROM_DATA[SFF8636_CC_BASE_OFFSET];
      cc_ext = EEPROM_DATA[SFF8636_CC_EXT_OFFSET];

   } else {
 // if SFP/SFP+(SFF-8472)
      if(check_8079CC()){
        DTLcanvas.drawString("I2C read success !!", 10, locY++ * pitch);
        DTLcanvas.drawString("GBIC/SFP/SFP+", 10, locY++ * pitch);
      }else{
        DTLcanvas.drawString("I2C read fail !!", 10, locY++ * pitch);
      }
      cc1 = calc_8079CC1();
      cc2 = calc_8079CC2();
      cc_base = EEPROM_DATA[0x3f];
      cc_ext = EEPROM_DATA[0x5f];
}
      sprintf(buf,"CC base=%02x  CALC=%04x",cc_base, cc1);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      sprintf(buf,"CC ext =%02x  CALC=%04x",cc_ext, cc2);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      dispHEXdump();
      delay(1000);
}
//
// HEX DIFF dump
//
void dispHEXDIFFdump(){
      int i,j, addr, xf;
      int     locY=2;
      String s;
      char buf[512];
      File file;
      byte a,b;

      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextColor(15,0);
      DTLcanvas.drawString(" diff "+ String(fnP[fileP + DIRtop]) , 40 , 10);
      DTLcanvas.setTextSize(22);

      DTLcanvas.drawString("   0 1  2 3  4 5  6 7  8 9  A B  C D  E F" , 4, locY++ * HEXpitch);
      // hex dump loop
  for(i=0 ; i < 16 ; i++){
    s = String(i,HEX);
    s.toUpperCase();
    s.concat(":");
    DTLcanvas.setTextColor(15,0);
    DTLcanvas.drawString(s , 4, locY * HEXpitch);
 
    xf=0;
    for(j = 0; j < 16 ; j++){
         addr = i*16+j;
         a = EEPROM_DATA[addr];
         b = EEPROM_DATA2[addr];
        if(a == b){
            DTLcanvas.setTextColor(15,0);
        }else {
            DTLcanvas.setTextColor(0,15);
        }
        sprintf(buf, "%02x",b);

        DTLcanvas.drawString(String(buf) , 4+ 26 + j *26+ xf*13, locY * HEXpitch);
        if(j % 2) xf++;
    }
    locY++;

  }
      
        DTLcanvas.pushCanvas(0,60,UPDATE_MODE_GLR16);  // 450ms long delay
}

//
// HEX dump
//
void dispHEXdump(){
      int i,j;
      int     locY=0;
      String s;
      char buf[512];
      HEXcanvas.fillCanvas(0);
      HEXcanvas.setTextSize(22);
      HEXcanvas.setTextColor(15);
      HEXcanvas.drawString("   0 1  2 3  4 5  6 7  8 9  A B  C D  E F" , 4, locY++);
      // hex dump loop
  for(i=0 ; i < 16 ; i++){
    s = String(i,HEX);
    s.toUpperCase();
    s.concat(":");
    for(j = 0; j < 16 ; j++){
 //     s.concat(String(EEPROM_DATA[i*16+j],HEX));
        sprintf(buf, "%02x",EEPROM_DATA[i*16+j]);
        s.concat(buf);
        if(j % 2) s.concat(" ");
    }
                HEXcanvas.drawString(s , 4, locY++ * HEXpitch);
  }
      
 //     HEXcanvas.pushCanvas(0,520,UPDATE_MODE_DU);   // 250ms delay
        HEXcanvas.pushCanvas(0,520,UPDATE_MODE_GLR16);  // 450ms long delay
}
//
// ASCII dump
//
void dispASCIIdump(){
      int i,j;
      byte c;
      int     locY=0;
      String s;
      char buf[4];
      HEXcanvas.fillCanvas(0);
      HEXcanvas.setTextSize(22);
      HEXcanvas.setTextColor(15);
//      HEXcanvas.drawString("   0 1  2 3  4 5  6 7  8 9  A B  C D  E F" , 4, locY++);
      // hex dump loop
  for(i=0 ; i < 8 ; i++){
    s = String(i*2,HEX);
    s.toUpperCase();
    s.concat(":");
    for(j = 0; j < 32 ; j++){
 //     s.concat(String(EEPROM_DATA[i*16+j],HEX));
        c = EEPROM_DATA[i*32+j];
        if(c > 0x20 && c < 0x7f){
          sprintf(buf, "%c",c);
        }else {
//          sprintf(buf, "%02x",c);
            sprintf(buf, ".",c);
        }
        s.concat(buf);
//        if(j % 2) s.concat(" ");
    }
                HEXcanvas.drawString(s , 4, locY++ * pitch);
  }
      
      HEXcanvas.pushCanvas(0,520,UPDATE_MODE_DU);
}
//==========================
// ASCII display 16x16
//==========================
void dispASCII(){
  int i,j;
  int locY;
  String s;
  byte c;
  char buf[4];
      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
//      DTLcanvas.drawString("     >>> ASCII DUMP <<<", 20, locY++ * pitch);
    for(i=0 ; i < 16 ; i++){
      s = "";
      for(j = 0; j < 16 ; j++){
        c = EEPROM_DATA[i*16+j];
        if(c > 0x20 && c < 0x7f){
          sprintf(buf, "%c",c);
        }else {
          sprintf(buf, ".",c);
        }
        s.concat(buf);
    }
        DTLcanvas.drawString(s , 20, locY++ * pitch);
  }
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
}
// =====================================================================
//
//    main function , ETHTOOL stype decoder
//
// =====================================================================
int Disp8079ETHTOOL(){
  int i,j,cc1,cc2;
  int locY = 0;
 String s = "dummy";
 char buf[512];

  if(!check_8079CC()){
    cc1 = calc_8079CC1();
    cc2 = calc_8079CC2();
      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
      DTLcanvas.drawString("check sum fail !!" , 10, locY++ * pitch);
      sprintf(buf,"CC base=%02x  CALC=%04x",EEPROM_DATA[0x3f], cc1);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      sprintf(buf,"CC ext =%02x  CALC=%04x",EEPROM_DATA[0x9f], cc2);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      dispHEXdump();
      delay(1000);
      return false;
  }
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(28);

  DTLcanvas.drawString("ID         : " + sff8472_ID_name(0x00), 10, locY++ * pitch);

      s = sff8079_E_transceiver();
      DTLcanvas.drawString("TRN type   : " + s, 10, locY++ * pitch);
  if(EEPROM_DATA[0x04] || EEPROM_DATA[0x05]){
      DTLcanvas.drawString("SONET      : " + sff8472_SONET_transceiver(), 10, locY++ * pitch);
}
  if(EEPROM_DATA[0x07]){
      DTLcanvas.drawString("FC Type: " + sff8472_FC_transceiver(), 10, locY++ * pitch);
}
  if(EEPROM_DATA[0x0a]){
      DTLcanvas.drawString("FC Speed   : " + sff8472_FC_speed(), 10, locY++ * pitch);
}
// 8 bit 0 to 3 SFP+ Cable technology
  if(EEPROM_DATA[0x08] & 0x0f){
      DTLcanvas.drawString("Cable technology: " + sff8472_cable_tech(), 10, locY++ * pitch);
}
//
// show connector type
//
      s = sff8024_connector(0x02);
      DTLcanvas.drawString("Connector  : " + s, 10, locY++ * pitch);

//  DTLcanvas.drawString("ID         : " + sff8079_rate_identifier(), 10, locY++ * pitch);
// 12 BR extention is 66 250MB base
if(EEPROM_DATA[0x0c]){
      if(EEPROM_DATA[0x0c] == 0xff){
         sprintf(buf,"%6.2fGbps",float(EEPROM_DATA[0x42])*0.25);
      }else {
         sprintf(buf,"%dMbps",EEPROM_DATA[0x42]*100);
      }
  DTLcanvas.drawString("BR,Nominal : " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[0x0e]){
  sprintf(buf,"%dkm",EEPROM_DATA[0x0e]);
  DTLcanvas.drawString("Length(SMF): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[0x0f]){
  sprintf(buf,"%dm",EEPROM_DATA[0x0f] * 100);
  DTLcanvas.drawString("Length(SMF): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[0x10]){
  sprintf(buf,"%dm",EEPROM_DATA[0x10]*10);
  DTLcanvas.drawString("Length(OM2): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[0x11]){
  sprintf(buf,"%dm",EEPROM_DATA[0x11]);
  DTLcanvas.drawString("Length(OM1): " + String(buf), 10, locY++ * pitch);
}
// 18 OM4 or copper
if(EEPROM_DATA[0x12]){
  if(EEPROM_DATA[0x02] == 0x22){
    // RJ45
     sprintf(buf,"%dm",EEPROM_DATA[0x12]);
     DTLcanvas.drawString("Length(UTP): " + String(buf), 10, locY++ * pitch);

  } else {
    sprintf(buf,"%dm",EEPROM_DATA[0x12]*10);
     DTLcanvas.drawString("Length(OM4): " + String(buf), 10, locY++ * pitch);
  }
}
if(EEPROM_DATA[0x13]){
  sprintf(buf,"%dm",EEPROM_DATA[0x13]*10);
  DTLcanvas.drawString("Length(OM3): " + String(buf), 10, locY++ * pitch);
}
//
      DTLcanvas.drawString("Vendor NAME: " + getSTRn(0x14,16), 10, locY++ * pitch);
// 36(0x24) extend TRN type ?
// OUI
      sprintf(buf,"%02x:%02x:%02x",EEPROM_DATA[0x25],
                                   EEPROM_DATA[0x25+1],
                                   EEPROM_DATA[0x25+2]);
     DTLcanvas.drawString("Vendor OUI : " + String(buf), 10, locY++ * pitch);      
// 
     DTLcanvas.drawString("Vendor PN  : " + getSTRn(0x28,16), 10, locY++ * pitch);
// 
// REV
      DTLcanvas.drawString("Vendor REV : " + getSTRn(0x38,4), 10, locY++ * pitch);
// 60-61 wavelength
// 62 Unallocated
// 63 CC base
// 64-65 options
// 66 BR MAX
// 67 BR MIN

      DTLcanvas.drawString("Vendor SN  : " + getSTRn(0x44,16), 10, locY++ * pitch);
      // create file name for save data, omit space
      setSN(0x44, 16);
      
      DTLcanvas.drawString("Vendor DATE: " + getSTRn(0x54,8), 10, locY++ * pitch);
 // 92 diag type
 // 94 rev comp

      DTLcanvas.setTextSize(20);
      DTLcanvas.drawString("REV Compli : " + sff_revision(0x5e), 10, locY++ * pitch);
// 95 CC ext

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_GLR16);
}

int Disp8636ETHTOOL(){
  int cc1 = 0;
  int cc2 = 0;
  int i,j;
  int locY = 0;
 String s = "dummy";
 char buf[512];

  if(!check_8636CC()){
    cc1 = calc_8636CC1();
    cc2 = calc_8636CC2();
      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
      DTLcanvas.drawString("check sum fail !!" , 10, locY++ * pitch);
      sprintf(buf,"CC base=%02x  CALC=%04x",EEPROM_DATA[SFF8636_CC_BASE_OFFSET], cc1);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      sprintf(buf,"CC ext =%02x  CALC=%04x",EEPROM_DATA[SFF8636_CC_EXT_OFFSET], cc2);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      dispHEXdump();
      delay(1000);
      return false;
  }
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(28);

//    DTLcanvas.setTextSize(24);
//  if (id[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP_DD) {
//    qsfp_dd_show_all(id);
//    return;
//  }
  DTLcanvas.drawString("ID         : " + IDname(), 10, locY++ * pitch);
  DTLcanvas.drawString("Power class: " + sff_powerclass(), 10, locY++ * pitch);
// if extend type
      s = sff_ext_type(SFF8636_OPTION_1_OFFSET);
      DTLcanvas.drawString("TRN type   : " + s, 10, locY++ * pitch);
//
// show connector type
//
      s = sff8024_connector(SFF8636_CTOR_OFFSET);
      DTLcanvas.drawString("Connector  : " + s, 10, locY++ * pitch);
      if(EEPROM_DATA[SFF8636_BR_NOMINAL_OFFSET] == 0xff){
         sprintf(buf,"%6.2fGbps",float(EEPROM_DATA[0xde])*0.25);
      }else {
         sprintf(buf,"%dMbps",EEPROM_DATA[SFF8636_BR_NOMINAL_OFFSET]*100);
      }
  DTLcanvas.drawString("BR, Nominal: " + String(buf), 10, locY++ * pitch);
if(EEPROM_DATA[SFF8636_SM_LEN_OFFSET]){
  sprintf(buf,"%dkm",EEPROM_DATA[SFF8636_SM_LEN_OFFSET]);
  DTLcanvas.drawString("Length(SMF): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[SFF8636_OM3_LEN_OFFSET]){
  sprintf(buf,"%dm",EEPROM_DATA[SFF8636_OM3_LEN_OFFSET]);
  DTLcanvas.drawString("Length(OM3): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[SFF8636_OM2_LEN_OFFSET]){
  sprintf(buf,"%dm",EEPROM_DATA[SFF8636_OM2_LEN_OFFSET]);
  DTLcanvas.drawString("Length(OM2): " + String(buf), 10, locY++ * pitch);
}
if(EEPROM_DATA[SFF8636_CBL_LEN_OFFSET]){
  sprintf(buf,"%dm",EEPROM_DATA[SFF8636_CBL_LEN_OFFSET]);
  DTLcanvas.drawString("Length(UTP): " + String(buf), 10, locY++ * pitch);
}
  DTLcanvas.drawString("Technology : " + sff8636_tx_tech(), 10, locY++ * pitch);
//
      DTLcanvas.drawString("Vendor NAME: " + getSTRn(SFF8636_VENDOR_NAME_START_OFFSET,16), 10, locY++ * pitch);   
// 
     DTLcanvas.drawString("Vendor PN  : " + getSTRn(SFF8636_VENDOR_PN_START_OFFSET,16), 10, locY++ * pitch);
      
// 
// REV
      sprintf(buf,"%c%c",EEPROM_DATA[SFF8636_VENDOR_REV_START_OFFSET],
                         EEPROM_DATA[SFF8636_VENDOR_REV_START_OFFSET+1]);
      DTLcanvas.drawString("Vendor REV : " + String(buf), 10, locY++ * pitch);
// 
      DTLcanvas.drawString("Vendor SN  : " + getSTRn(SFF8636_VENDOR_SN_START_OFFSET,16), 10, locY++ * pitch);
      // create file name for save data, omit space
      setSN(SFF8636_VENDOR_SN_START_OFFSET,16);

      DTLcanvas.drawString("Vendor DATE: " + getSTRn(SFF8636_DATE_YEAR_OFFSET,8), 10, locY++ * pitch);
// OUI
      sprintf(buf,"%02x:%02x:%02x",EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET],
                                   EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET+1],
                                   EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET+2]);
      DTLcanvas.drawString("Vendor OUI : " + String(buf), 10, locY++ * pitch);
      DTLcanvas.setTextSize(20);
      DTLcanvas.drawString("REV Compli : " + sff_revision(SFF8636_REV_COMPLIANCE_OFFSET), 10, locY++ * pitch);

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_GLR16);
}
int DispETHTOOL(){
    if ((EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP) ||
        (EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP_PLUS) ||
        (EEPROM_DATA[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP28)){
      Disp8636ETHTOOL();
    } else {
      Disp8079ETHTOOL();
    }
//
// HEX dump
//
     dispHEXdump();
     delay(500);

}
//
// info / help page
//
void DispINFO(){
    int pitch = 34;
  int locY = 0;
   char buf[512];

  DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
      DTLcanvas.setTextColor(15);
      DTLcanvas.setTextDatum(TC_DATUM);
      DTLcanvas.drawString("SFP/QSFP EEPROM decoder" , 10, locY++ * pitch);
      DTLcanvas.drawString("   2021/02/24 ver 1.1" , 10, locY++ * pitch);
      DTLcanvas.drawString("             by JN1OLJ" , 10, locY++ * pitch);
      DTLcanvas.drawString("" , 10, locY++ * pitch);
      DTLcanvas.drawString("refer ethtool 5.10" , 10, locY++ * pitch);
      DTLcanvas.drawString("      SFF-8472 Rev 12.3" , 10, locY++ * pitch);
      DTLcanvas.drawString("      SFF-8636 Rev 2.1" , 10, locY++ * pitch);
      DTLcanvas.drawString("      SFF-8024 Rev 4.8" , 10, locY++ * pitch);
      locY++;
      sprintf(buf,"BatteryVoltage %06d",M5.getBatteryVoltage());
      DTLcanvas.drawString(String(buf) , 10, locY++ * pitch);
      
    M5.RTC.getTime(&RTCtime);
    M5.RTC.getDate(&RTCDate);
    sprintf(buf,"RTC Date %04d/%02d/%02d %02d:%02d:%02d",
                        RTCDate.year,RTCDate.mon,RTCDate.day,
                        RTCtime.hour,RTCtime.min,RTCtime.sec);
      DTLcanvas.drawString(String(buf) , 10, locY++ * pitch);
                        
      M5.SHT30.UpdateData();
      tem = M5.SHT30.GetTemperature();
      dtostrf(tem, 2, 2 , temStr);
      DTLcanvas.drawString("Temperatura:" + String(temStr) + " C", 10, locY++ * pitch);
      tem = M5.SHT30.GetTemperature();
      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      delay(500);
}

void ButtonTest(char* str)
{
//    canvas.fillCanvas(0);
    canvas.drawString(str, 100, lY);
    lY = lY + 30;
    canvas.pushCanvas(100,300,UPDATE_MODE_DU);
    delay(500);
}
//
// sample data load and ntp test
//
void menu4(){

//  sync_with_ntp();
  dispASCII();
}
void GoSleep(){
    MENUcanvas.fillCanvas(0);
    MENUcanvas.setTextSize(20);
    MENUcanvas.drawString("I'm going to sleep.", 50, 0);
    MENUcanvas.pushCanvas(0,940,UPDATE_MODE_DU);
    delay(1000);
    M5.shutdown();
}
void drawMenu(){
  MENUcanvas.fillCanvas(0);
  MENUcanvas.setTextSize(48);
  MENUcanvas.drawString("^" , 25+ menuP* 70, 0);
  MENUcanvas.pushCanvas(0,940,UPDATE_MODE_DU);
}
void upMenu(){
 menuP++;
 if(menuP >  7 ) menuP = 7;
 drawMenu();
}
void downMenu(){
 menuP--;
 if(menuP < 0 ) menuP = 0;
 drawMenu();
}
void doMenu()
{
  if(menuP == 0 ) ShowDir();
//  if(menuP == 0 ) FileRead("/C90AW42595.bin");
  if(menuP == 1 ) ReadI2C();
  if(menuP == 2 ) FileWrite(SN);
  if(menuP == 3) DispETHTOOL();  // 
  if(menuP == 4) menu4();  // ASCII data dislay for debug
  if(menuP == 5 ) ShowDir();
  if(menuP == 6 ) DispINFO();
  if(menuP == 7 ) GoSleep();
}
// ========== basic system function ============================================
void setup() {
// void M5EPD::begin(bool touchEnable, bool SDEnable, bool SerialEnable, bool BatteryADCEnable, bool I2CEnable)
    M5.begin();
    M5.RTC.begin();
//    Wire.begin(21,22);     // for I2C master
    Wire1.begin(25,32); // port A
    SD.begin();       // for SD card file read/write
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    
    canvas.createCanvas(540, 960);
    DTLcanvas.createCanvas(540, pitch*15);
//   loadFont is effeted all canvas
//
//    DTLcanvas.loadFont(binaryttf, sizeof(binaryttf)); // Load font files from binary data
//    DTLcanvas.loadFont("/ipaexg.ttf", SD); // Load font files from SD Card
//    DTLcanvas.loadFont("/CascadiaCode.ttf", SD); // Load font files from SD Card
//    DTLcanvas.loadFont("/fonts/GenShinGothic-Monospace-Bold.ttf", SD); // Load font files from SD Card
    DTLcanvas.loadFont("/fonts/MonospaceTypewriter.ttf", SD); // Load font files from SD Card
//      DTLcanvas.loadFont("/fonts/Envy Code R.ttf", SD); // Load font files from SD Card
    DTLcanvas.createRender(28);

    HEXcanvas.createCanvas(540, HEXpitch*17);
    HEXcanvas.createRender(22,256);
    MENUcanvas.createCanvas(540,20);
    MENUcanvas.createRender(48);
    filePcanvas.createCanvas(20,500);
    canvas.createRender(32);
    canvas.setTextSize(32);
    canvas.setTextColor(15);
//    canvas.setTextDatum(ML_DATUM); // Middle left
    canvas.drawString("ETHTOOL style EEPROM decoder" , 10, 20);
    canvas.drawFastHLine(0, 50, 540, 15);
    canvas.drawFastHLine(0, 52, 540, 15);

// draw menu grid
    canvas.fillRect(0, 900, 540, 40, 15);  // black back
    canvas.createRender(20);
    canvas.setTextSize(20);
    canvas.setTextColor(0);                // white color
//    canvas.setTextDatum(ML_DATUM); // Middle left
    canvas.drawString(" LOAD  READ  SAVE  DISP  DISP  LOAD  INFO  PWR" , 0, 902);
    canvas.drawString(" file  I2C   file decode ASCII DIFF        off" , 0, 921);
    canvas.drawFastHLine(0, 898, 540, 0);
    canvas.drawFastVLine(70, 898, 50, 0);
    canvas.drawFastVLine(70+1, 898, 50, 0);
    canvas.drawFastVLine(70*2, 898, 50, 0);
    canvas.drawFastVLine(70*2+1, 898, 50, 0);
    canvas.drawFastVLine(70*3, 898, 50, 0);
    canvas.drawFastVLine(70*3+1, 898, 50, 0);
    canvas.drawFastVLine(70*4, 898, 50, 0);
    canvas.drawFastVLine(70*4+1, 898, 50, 0);
    canvas.drawFastVLine(70*5, 898, 50, 0);
    canvas.drawFastVLine(70*5+1, 898, 50, 0);
    canvas.drawFastVLine(70*6, 898, 50, 0);
    canvas.drawFastVLine(70*6+1, 898, 50, 0);
    canvas.drawFastVLine(70*7, 898, 50, 0);
    canvas.drawFastVLine(70*7+1, 898, 50, 0);
    canvas.pushCanvas(0,0,UPDATE_MODE_GLR16);
    
    drawMenu();

}
////////////////////////////////////////////////////////////
/// main loop pickup user operations
///////////////////////////////////////////////////////////
void loop() {
  if(fileP){
    if( M5.BtnL.wasPressed()) downfileP();
    if( M5.BtnP.wasPressed()) if (menuP == 5) {FileRead2(fnP[fileP + DIRtop]);}else{FileRead(fnP[fileP + DIRtop]);}
    if( M5.BtnR.wasPressed()) upfileP();

  }else{
    if( M5.BtnL.wasPressed()) upMenu();
    if( M5.BtnP.wasPressed()) doMenu();
    if( M5.BtnR.wasPressed()) downMenu();
  }
    M5.update();  // check bottun
    delay(200);  // 200ms delay
    // M5.Power.lightSleep(30);   // sleep sedonds
}
