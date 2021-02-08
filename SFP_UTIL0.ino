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
// serial number
char SN[32];
char fnP[128][128] = {"", "/C90AW42595.bin","", "/C90AW41867.bin", "/C90AW41865.bin", "/C90AW41733.bin"};
int DIRmax=0;
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
void ShowDir(){
 int locY = 0,i  = 1;
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
      DTLcanvas.drawString(entry.name(), 10, locY++ * pitch);
//      fnP[i++] = entry.name();
      sprintf(fnP[i++],"%s", entry.name());
      if(i > 16 ) break;
     }
   }
 }
   DIRmax = i;
   DTLcanvas.pushCanvas(20,60,UPDATE_MODE_DU);
   fileP = 1;
   drawfileP();
   delay(500);
}
int FileRead(char *fn){
  int i, size, locY = 1,offset = 0;
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
  }
  file.close();

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      dispHEXdump();
  return 0;
}
int FileWrite(char *fn){
  
  int locY = 0;
  File file;
  DTLcanvas.fillCanvas(0);
  DTLcanvas.setTextSize(32);
  if(SD.exists(fn)){
    DTLcanvas.drawString(String(fn) + " exists!", 10, locY++ * pitch);
  }
  file =SD.open(fn, FILE_WRITE);
  file.write(EEPROM_DATA, dataSize);
  file.close();
  DTLcanvas.drawString("try write " + String(fn), 10, locY++ * pitch);
  DTLcanvas.drawString("file write success !!", 10, locY++ * pitch);
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
  char buf[512];

//  Wire.beginTransmission(0x50);
//  Wire.write(0x00);
//  Wire.endTransmission();
//  Wire.requestFrom(0x50, 256);
// Wire.available();
//  i=0;
// while(Wire.available()){
//   EEPROM_DATA[i++] = Wire.read();
// }
for(i=0; i < 0xff; i++){
  Wire1.beginTransmission(0x50);
  Wire1.write(i);
  Wire1.endTransmission();
  Wire1.requestFrom(0x50, 1);
    delay(5);                // IMPORTANT !!!
  EEPROM_DATA[i] = Wire1.read();
}
// M5.I2C.readBytes(0x50, 0xff, EEPROM_DATA);

 dataSize = i;
      DTLcanvas.fillCanvas(0);
      DTLcanvas.setTextSize(32);
      if(check_CC()){
      DTLcanvas.drawString("I2C read success !!", 10, locY++ * pitch);
      }else{
      DTLcanvas.drawString("I2C read fail !!", 10, locY++ * pitch);
      }
      cc1 = calc_CC1();
      cc2 = calc_CC2();

      sprintf(buf,"CC base=%02x  CALC=%04x",EEPROM_DATA[SFF8636_CC_BASE_OFFSET], cc1);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);
      sprintf(buf,"CC ext =%02x  CALC=%04x",EEPROM_DATA[SFF8636_CC_EXT_OFFSET], cc2);
      DTLcanvas.drawString(String(buf), 10, locY++ * pitch);

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
      dispHEXdump();
      delay(1000);
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
      
      HEXcanvas.pushCanvas(0,520,UPDATE_MODE_DU);
}
//
// check checksum
//
int calc_CC1(){
  int i,cc1 = 0;
    for (i=0x80; i < 0xbf ; i++){
    cc1 = cc1 + EEPROM_DATA[i]; 
  }
return cc1;
}
int calc_CC2(){
  int i,cc2 = 0;
    for (i=0xc0; i < 0xdf ; i++){
    cc2 = cc2 + EEPROM_DATA[i]; 
  }
return cc2;
}
// ---------------------------------------------------
bool check_CC (){
  int cc1 = 0; // 0xbf
  int cc2 = 0; // 0xdf
  cc1 = calc_CC1();
  cc2 = calc_CC2();

  cc1 = cc1 & 0xff;
  if(cc1 != EEPROM_DATA[SFF8636_CC_BASE_OFFSET]) return false;
  cc2 = cc2 & 0xff;
  if(cc2 != EEPROM_DATA[SFF8636_CC_EXT_OFFSET]) return false;
  return true;
}
// =====================================================================
//
//    main function , ETHTOOL stype decoder
//
// =====================================================================
int DispETHTOOL(){
  int cc1 = 0;
  int cc2 = 0;
  int i,j;
  int locY = 0;
 String s = "dummy";
 char buf[512];

  if(!check_CC()){
    cc1 = calc_CC1();
    cc2 = calc_CC2();
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
      sprintf(buf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+1],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+2],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+3],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+4],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+5],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+6],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+7],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+8],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+9],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+10],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+11],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+12],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+13],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+14],
                                 EEPROM_DATA[SFF8636_VENDOR_NAME_START_OFFSET+15]);
      DTLcanvas.drawString("Vendor NAME: " + String(buf), 10, locY++ * pitch);
      
// 
      sprintf(buf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+1],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+2],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+3],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+4],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+5],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+6],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+7],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+8],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+9],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+10],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+11],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+12],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+13],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+14],
                                 EEPROM_DATA[SFF8636_VENDOR_PN_START_OFFSET+15]);
     DTLcanvas.drawString("Vendor PN  : " + String(buf), 10, locY++ * pitch);
      
// 
// REV
      sprintf(buf,"%c%c",EEPROM_DATA[SFF8636_VENDOR_REV_START_OFFSET],
                         EEPROM_DATA[SFF8636_VENDOR_REV_START_OFFSET+1]);
      DTLcanvas.drawString("Vendor REV : " + String(buf), 10, locY++ * pitch);
// 
      sprintf(buf,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+1],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+2],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+3],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+4],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+5],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+6],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+7],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+8],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+9],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+10],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+11],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+12],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+13],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+14],
                                 EEPROM_DATA[SFF8636_VENDOR_SN_START_OFFSET+15]);
      DTLcanvas.drawString("Vendor SN  : " + String(buf), 10, locY++ * pitch);
      // create file name for save data, omit space
      i = 0;
      SN[i] = '/';
      while(buf[i] != 0x20){
        SN[i+1] = buf[i];
        i++;
        if(i>16) break;
      }
      SN[i+1] = '.';
      SN[i+2] = 'b';
      SN[i+3] = 'i';
      SN[i+4] = 'n';                  
      SN[i+5] = 0x00;
//      SN = String(buf);
//      strncpy(buf, &EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET], 6);
      sprintf(buf,"%c%c%c%c%c%c%c%c",EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+1],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+2],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+3],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+4],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+5],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+6],
                                 EEPROM_DATA[SFF8636_DATE_YEAR_OFFSET+7]);
      DTLcanvas.drawString("Vendor DATE: " + String(buf), 10, locY++ * pitch);
// OUI
      sprintf(buf,"%02x:%02x:%02x",EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET],
                                   EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET+1],
                                   EEPROM_DATA[SFF8636_VENDOR_OUI_OFFSET+2]);
      DTLcanvas.drawString("Vendor OUI : " + String(buf), 10, locY++ * pitch);
      DTLcanvas.setTextSize(2);
      DTLcanvas.drawString("REV Compli : " + sff_revision(SFF8636_REV_COMPLIANCE_OFFSET), 10, locY++ * pitch);

      DTLcanvas.pushCanvas(0,60,UPDATE_MODE_DU);
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
      DTLcanvas.drawString("   2021/02/04 ver 0.1" , 10, locY++ * pitch);
      DTLcanvas.drawString("             by JN1OLJ" , 10, locY++ * pitch);
      DTLcanvas.drawString("" , 10, locY++ * pitch);
      DTLcanvas.drawString("refer ethtool 5.10" , 10, locY++ * pitch);
      DTLcanvas.drawString("      SFF-8472" , 10, locY++ * pitch);
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
      DTLcanvas.drawString("Temperatura:" + String(temStr) + "*C", 10, locY++ * pitch);
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
void drawfileP(){
  filePcanvas.fillCanvas(0);
  filePcanvas.setTextSize(48);
  filePcanvas.drawString(">" , 0, (fileP-1)*pitch);
  filePcanvas.pushCanvas(2,60,UPDATE_MODE_DU);
}
void upfileP(){
 fileP++;
 if(fileP >  (DIRmax-1) ) fileP = (DIRmax-1);
 drawfileP();
}
void downfileP(){
 fileP--;
 if(fileP < 1 ) fileP = 1;
 drawfileP();
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
  MENUcanvas.drawString("^" , 50+ menuP* 80, 0);
  MENUcanvas.pushCanvas(0,940,UPDATE_MODE_DU);
}
void upMenu(){
 menuP++;
 if(menuP >  6 ) menuP = 6;
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
  if(menuP == 4) dummy_read(EEPROM_DATA);;  // dummy data dislay for debug

  if(menuP == 5 ) DispINFO();
  if(menuP == 6 ) GoSleep();
}
// ========== basic system function ============================================
void setup() {

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
    canvas.drawString(" load   read   save   disp  read   info  PWR" , 10, 900);
    canvas.drawString(" file   I2C    file  decode dummy        off" , 10, 920);
    canvas.drawFastHLine(0, 898, 540, 0);
    canvas.drawFastVLine(90, 898, 50, 0);
    canvas.drawFastVLine(170, 898, 50, 0);
    canvas.drawFastVLine(250, 898, 50, 0);
    canvas.drawFastVLine(330, 898, 50, 0);
    canvas.drawFastVLine(410, 898, 50, 0);
    canvas.drawFastVLine(490, 898, 50, 0);
    canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
    
    drawMenu();
}
////////////////////////////////////////////////////////////
/// main loop pickup user operations
///////////////////////////////////////////////////////////
void loop() {
  if(fileP){
    if( M5.BtnL.wasPressed()) downfileP();
    if( M5.BtnP.wasPressed()) FileRead(fnP[fileP]);
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
