String sff8472_ID_name(int offset){
  String s;
  switch (EEPROM_DATA[offset]){
  case 0x00: s="unspecified"; break;
  case 0x01: s="GBIC";        break;
  case 0x02: s="SFF";         break;
  case 0x03: s="SFP/SFP+/SFP28";    break;
  default: s="Unallocated";
}
  if(EEPROM_DATA[offset] > 0x80) s = "Vender specific";
  return s;
}
String getSTRn(int offset, int len){
  int i;
  char buf[32];
  for(i=0; i < len; i++){
    buf[i] = EEPROM_DATA[offset+i];
  }
  buf[len] = 0x00;
  return String(buf);
}
//
// check checksum
//
int calc_8079CC1(){
  int i,cc1 = 0;
    for (i=0x00; i < 0x3f ; i++){
    cc1 = cc1 + EEPROM_DATA[i]; 
  }
return cc1;
}
int calc_8079CC2(){
  int i,cc2 = 0;
    for (i=0x40; i < 0x5f ; i++){
    cc2 = cc2 + EEPROM_DATA[i]; 
  }
return cc2;
}
bool check_8079CC (){
  int cc1 = 0; // 0x3f
  int cc2 = 0; // 0x5f
  cc1 = calc_8079CC1();
  cc2 = calc_8079CC2();

  cc1 = cc1 & 0xff;
  if(cc1 != EEPROM_DATA[0x3f]) return false;
  cc2 = cc2 & 0xff;
  if(cc2 != EEPROM_DATA[0x5f]) return false;
  return true;
}
/*
static void sff8079_show_identifier(const __u8 *id)
{
  sff8024_show_identifier(id, 0);
}
*/

String  sff8079_ext_identifier()
{
  String s;
  
  if (EEPROM_DATA[1] == 0x00)
    s ="GBIC not specified";
  else if (EEPROM_DATA[1] == 0x04)
    s = "GBIC/SFP";
  else if (EEPROM_DATA[1] <= 0x07)
    s = "GBIC";
  else
    s = "unknown";

    return s;
}

static void sff8079_show_connector(const __u8 *id)
{
  sff8024_show_connector(id, 2);
}
String sff8472_cable_tech(){
    String s;
  switch(EEPROM_DATA[0x08] & 0x0f){
    case 0x08: s = "Active Cable"; break;
    case 0x04: s = "Passive Cable"; break;
    case 0x02: s = "Unallocated"; break;
    case 0x01: s = "Unallocated"; break;
  }
  return s;
}
String sff8472_FC_transceiver(){
  String s = "";
  switch(EEPROM_DATA[0x07]){
    case 0x80: s = "(V)"; 
    case 0x40: s = "(S)";
    case 0x20: s = "(I)";
    case 0x10: s = "(L)";
    case 0x08: s = "(M)"; 
    case 0x04: s += "(SA)"; break;
    case 0x02: s += "(LC)"; break;
    case 0x01: s += "(EL)"; break;
  }
  switch(EEPROM_DATA[0x08]){
    case 0x80: s += "(EL)";  break;
    case 0x40: s += "(SN)"; break;
    case 0x20: s = "(SL)";  break;
    case 0x10: s = "(LL)";  break;
  }
    switch(EEPROM_DATA[0x09]){
    case 0x80: s += "Twin Axlial Pair(TW)";  break;
    case 0x40: s += "Twisted Pair(TP)"; break;
    case 0x20: s += "Miniature Coax(MI)";  break;
    case 0x10: s += "Video Coax(TV)";  break;
    case 0x08: s += "Multimode 62.6um(M6)";  break;
    case 0x04: s += "Multimode 50um(M5)";  break;
    case 0x01: s += "Single mode";  break;
  }
  return s;
}
String sff8472_FC_speed(){
 String s;
  switch(EEPROM_DATA[0x0a]){
    case 0x80: s = "1200 MBytes/s";break;
    case 0x40: s = "800MBytes/s";  break;
    case 0x20: s = "1600Mbytes/s"; break;
    case 0x10: s = "400MBytes/s";  break;
    case 0x08: s = "3200MBytes/s"; break;
    case 0x04: s = "200MBytes/s";  break;
    case 0x02: s = "extend";       break;
    case 0x01: s = "100MBytes/s";  break;
  }
  return s;
}
String sff8472_SONET_transceiver(){
  String s;
  switch(EEPROM_DATA[0x04]){
    case 0x80: s = "ESCON MMF";  break;
    case 0x40: s = "ESCON SMF"; break;
    case 0x20: s = "OC-192 SR";  break;
    case 0x10: s = "SONET bit 1";  break;
    case 0x08: s = "SONET bit 2"; break;
    case 0x04: s = "OC-48 LR"; break;
    case 0x02: s = "OC-48 IR"; break;
    case 0x01: s = "OC-48 SR"; break;
  }
  switch(EEPROM_DATA[0x05]){
    case 0x80: s = "Unallocated";  break;
    case 0x40: s = "OC-12 LR"; break;
    case 0x20: s = "OC-12 IR";  break;
    case 0x10: s = "OC-12 SR";  break;
    case 0x08: s = "Unallocated"; break;
    case 0x04: s = "OC-3 LR"; break;
    case 0x02: s = "OC-3 IR"; break;
    case 0x01: s = "OC-3 SR"; break;
  }
  
  return s;
}
//
// extend transeiver type name case
//
String SFP_ext_type(int offset){
  String s;

  switch(EEPROM_DATA[offset]){

  case SFF8636_ETHERNET_UNSPECIFIED: s = "UNSPECIFIED"; break;
  case SFF8636_ETHERNET_100G_AOC: s = "25G AOC(FEC)"; break;
  case SFF8636_ETHERNET_100G_SR4: s = "25G SR"; break;
  case SFF8636_ETHERNET_100G_LR4: s = "25G LR"; break;
  case SFF8636_ETHERNET_100G_ER4: s = "25G ER"; break;
  case SFF8636_ETHERNET_100G_SR10: s ="100G SR10"; break;
  case SFF8636_ETHERNET_100G_CWDM4_FEC: s = "100G CWDM4"; break;
  case SFF8636_ETHERNET_100G_PSM4: s = "100G PSM4"; break;
  case SFF8636_ETHERNET_100G_ACC: s = "25G ACC"; break;
  case SFF8636_ETHERNET_100G_CWDM4_NO_FEC: s = "100G CWDM4/noFEC"; break;
  case SFF8636_ETHERNET_100G_RSVD1: s = "100G WSV"; break;
  case SFF8636_ETHERNET_100G_CR4: s = "25G CR-CA-L"; break;
  case SFF8636_ETHERNET_25G_CR_CA_S: s = "25G CR-CA-S"; break;
  case SFF8636_ETHERNET_25G_CR_CA_N: s = "25G CR-CA-N"; break;
  case SFF8636_ETHERNET_40G_ER4: s = "40G ER4"; break;
  case SFF8636_ETHERNET_4X10_SR: s = "4x10G SR"; break;
  case SFF8636_ETHERNET_40G_PSM4: s = "40G PSM4"; break;
  case SFF8636_ETHERNET_G959_P1I1_2D1: s = "G959 PlTl 2D1"; break;
  case SFF8636_ETHERNET_G959_P1S1_2D2: s = "G959 PlSl 2D2"; break;
  case SFF8636_ETHERNET_G959_P1L1_2D2: s = "G959 PlLl 2D2"; break;
  case SFF8636_ETHERNET_10GT_SFI: s = "10G T SFI"; break;
  case SFF8636_ETHERNET_100G_CLR4: s = "100G CLR4"; break;
  case SFF8636_ETHERNET_100G_AOC2: s = "25G AOC"; break;
  case SFF8636_ETHERNET_100G_ACC2: s = "25G AOC"; break;
  
  case SFF8636_ETHERNET_100GE_DWDM2: s = "100G DWDM2"; break;
  case SFF8636_ETHERNET_100G_1550NM_WDM: s = "100G 1550nm WDM"; break;
  case SFF8636_ETHERNET_10G_BASET_SR: s = "10G T SR"; break;
  case SFF8636_ETHERNET_5G_BASET: s = "5G T"; break;
  case SFF8636_ETHERNET_2HALFG_BASET: s = "2.5G T"; break;
  case SFF8636_ETHERNET_40G_SWDM4: s = "40G SWDM4"; break;
  case SFF8636_ETHERNET_100G_SWDM4: s = "100G SWDM4"; break;
  case SFF8636_ETHERNET_100G_PAM4_BIDI: s = "100G PAM4 BiDi"; break;
  case SFF8636_ETHERNET_4WDM10_MSA: s = "100G 4WDM-10"; break;
  case SFF8636_ETHERNET_4WDM20_MSA: s = "100G 4WDM-20"; break;
  case SFF8636_ETHERNET_4WDM40_MSA: s = "100G 3WDM-40"; break;
  case SFF8636_ETHERNET_100G_DR: s = "100G DR"; break;
  case SFF8636_ETHERNET_100G_FR_NOFEC: s = "100G FR/noFEC"; break;
  case SFF8636_ETHERNET_100G_LR_NOFEC: s = "100G LR/noFEC"; break;
/*  28h-2Fh reserved */
  case SFF8636_ETHERNET_200G_ACC1: s = "200G ACC1"; break;
  case SFF8636_ETHERNET_200G_AOC1: s = "200G AOC1"; break;
  case SFF8636_ETHERNET_200G_ACC2: s = "200G ACC2"; break; 
  case SFF8636_ETHERNET_200G_A0C2: s = "200G AOC2"; break;
/*  34h-3Fh reserved */
  case SFF8636_ETHERNET_200G_CR4: s = "50G CR"; break;
  case SFF8636_ETHERNET_200G_SR4: s = "50G SR"; break;
  case SFF8636_ETHERNET_200G_DR4: s = "50G FR"; break;
  case SFF8636_ETHERNET_200G_FR4: s = "200G FR4"; break;
  case SFF8636_ETHERNET_200G_PSM4: s = "200G PSM4"; break;
  case SFF8636_ETHERNET_50G_LR: s = "50G LR"; break;
  case SFF8636_ETHERNET_200G_LR4: s = "200G LR4"; break;
/*  47h-4Fh reserved */
  case SFF8636_ETHERNET_64G_EA: s = "64G EA"; break;
  case SFF8636_ETHERNET_64G_SW: s = "64G SW"; break;
  case SFF8636_ETHERNET_64G_LW: s = "64G LW"; break;
  case SFF8636_ETHERNET_128FC_EA: s = "128FC EA"; break;
  case SFF8636_ETHERNET_128FC_SW: s = "128FC SW"; break;
  case SFF8636_ETHERNET_128FC_LW: s = "128FC LW"; break;
 default: s = "unkown " + String(EEPROM_DATA[SFF8636_OPTION_1_OFFSET], HEX);
}
 return s;
}
String sff8079_E_transceiver(){
  String s;
  if(EEPROM_DATA[36]){
    // extend SFF-8024 table 4-4
    s = SFP_ext_type(36);
  }else{
  switch(EEPROM_DATA[0x03]){
    case 0x80: s = "10G ER";  break;
    case 0x40: s = "10G LRM"; break;
    case 0x20: s = "10G LR";  break;
    case 0x10: s = "10G SR";  break;
    case 0x08: s = "IB 1X SX"; break;
    case 0x04: s = "IB 1X LX"; break;
    case 0x02: s = "IB 1X Copper Active"; break;
    case 0x01: s = "IB 1X Copper Passive"; break;
  }
  switch(EEPROM_DATA[0x06]){
    case 0x80: s = "10BASE-PX";  break;
    case 0x40: s = "10BASE-BX10"; break;
    case 0x20: s = "100BASE-FX";  break;
    case 0x10: s = "10BASE-LX/LX10";  break;
    case 0x08: s = "1000BASE-T"; break;
    case 0x04: s = "1000BASE-CX"; break;
    case 0x02: s = "1000BASE-LX"; break;
    case 0x01: s = "1000BASE-SX"; break;
  }
  }
  return s;
}
String sff8079_transceiver()
{
 String s = "";

  if (EEPROM_DATA[3] & (1 << 7))
    s = "10G ER";
  if (EEPROM_DATA[3] & (1 << 6))
    s = "10G LRM";
  if (EEPROM_DATA[3] & (1 << 5))
    s = "10G LR";
  if (EEPROM_DATA[3] & (1 << 4))
    s = "10G SR";
  /* Infiniband Compliance Codes */
  if (EEPROM_DATA[3] & (1 << 3))
    s = "IB 1X SX";
  if (EEPROM_DATA[3] & (1 << 2))
    s = "IB 1X LX";
  if (EEPROM_DATA[3] & (1 << 1))
    s = "IB 1X Copper Active";
  if (EEPROM_DATA[3] & (1 << 0))
    s = "IB 1X Copper Passive";
  /* ESCON Compliance Codes */
  if (EEPROM_DATA[4] & (1 << 7))
    s = "ESCON MMF,1310nm LED";
  if (EEPROM_DATA[4] & (1 << 6))
    s = "ESCON SMF,1310nm Laser";
  /* SONET Compliance Codes */
  if (EEPROM_DATA[4] & (1 << 5))
    s = "SONET OC-192 SR";
  if (EEPROM_DATA[4] & (1 << 4))
    s = "SONET reach specifier bit 1";
  if (EEPROM_DATA[4] & (1 << 3))
    s = "SONET reach specifier bit 2";
  if (EEPROM_DATA[4] & (1 << 2))
    s = "SONET OC-48 LR";
  if (EEPROM_DATA[4] & (1 << 1))
    s = "SONET OC-48 IR";
  if (EEPROM_DATA[4] & (1 << 0))
    s = "SONET OC-48 SR";
  if (EEPROM_DATA[5] & (1 << 6))
    s = "SONET OC-12 LR";
  if (EEPROM_DATA[5] & (1 << 5))
    s = "SONET OC-12 IR";
  if (EEPROM_DATA[5] & (1 << 4))
    s = "SONET OC-12 SR";
  if (EEPROM_DATA[5] & (1 << 2))
    s = "SONET OC-3 LR";
  if (EEPROM_DATA[5] & (1 << 1))
    s = "SONET OC-3 IR";
  if (EEPROM_DATA[5] & (1 << 0))
    s = "SONET OC-3 SR";
  /* Ethernet Compliance Codes */
  if (EEPROM_DATA[6] & (1 << 7))
    s = "10 BASE-PX";
  if (EEPROM_DATA[6] & (1 << 6))
    s = "10 BASE-BX10";
  if (EEPROM_DATA[6] & (1 << 5))
    s = "100BASE-FX";
  if (EEPROM_DATA[6] & (1 << 4))
    s = "100BASE-LX/LX10";
  if (EEPROM_DATA[6] & (1 << 3))
    s = "1000BASE-T";
  if (EEPROM_DATA[6] & (1 << 2))
    s = "1000BASE-CX";
  if (EEPROM_DATA[6] & (1 << 1))
    s = "1000BASE-LX";
  if (EEPROM_DATA[6] & (1 << 0))
    s = "1000BASE-SX";
  /* Fibre Channel link length */
  if (EEPROM_DATA[7] & (1 << 7))
    s = "FC LR";
  if (EEPROM_DATA[7] & (1 << 6))
    s = "FC SR";
  if (EEPROM_DATA[7] & (1 << 5))
    s = "FC IR";
  if (EEPROM_DATA[7] & (1 << 4))
    s = "FC long distance";
  if (EEPROM_DATA[7] & (1 << 3))
    s = "FC medium distance";
  /* Fibre Channel transmitter technology */
  if (EEPROM_DATA[7] & (1 << 2))
    s = "FC Shortwave laser, linear Rx (SA)";
  if (EEPROM_DATA[7] & (1 << 1))
    s = "FC Longwave laser (LC)";
  if (EEPROM_DATA[7] & (1 << 0))
    s = "FC Electrical inter-enclosure (EL)";
  if (EEPROM_DATA[8] & (1 << 7))
    s = "FC Electrical intra-enclosure (EL)";
  if (EEPROM_DATA[8] & (1 << 6))
    s = "FC Shortwave laser w/o OFC (SN)";
  if (EEPROM_DATA[8] & (1 << 5))
    s = "FC Shortwave laser with OFC (SL)";
  if (EEPROM_DATA[8] & (1 << 4))
    s = "FC Longwave laser (LL)";
  if (EEPROM_DATA[8] & (1 << 3))
    s = "Active Cable";
  if (EEPROM_DATA[8] & (1 << 2))
    s = "Passive Cable";
  if (EEPROM_DATA[8] & (1 << 1))
    s = "FC Copper FC-BaseT";
  /* Fibre Channel transmission media */
  if (EEPROM_DATA[9] & (1 << 7))
    s = "FC Twin Axial Pair (TW)";
  if (EEPROM_DATA[9] & (1 << 6))
    s = "FC Twisted Pair (TP)";
  if (EEPROM_DATA[9] & (1 << 5))
    s = "FC Miniature Coax (MI)";
  if (EEPROM_DATA[9] & (1 << 4))
    s = "FC: Video Coax (TV)";
  if (EEPROM_DATA[9] & (1 << 3))
    s = "FC Multimode, 62.5um (M6)";
  if (EEPROM_DATA[9] & (1 << 2))
    s += "FC Multimode, 50um (M5)";
  if (EEPROM_DATA[9] & (1 << 0))
    s = "FC Single Mode (SM)";
  /* Fibre Channel speed */
  if (EEPROM_DATA[10] & (1 << 7))
    s = "FC 1200 MBytes/sec";
  if (EEPROM_DATA[10] & (1 << 6))
    s = "FC 800 MBytes/sec";
  if (EEPROM_DATA[10] & (1 << 4))
    s = "FC 400 MBytes/sec";
  if (EEPROM_DATA[10] & (1 << 2))
    s = "FC 200 MBytes/sec";
  if (EEPROM_DATA[10] & (1 << 0))
    s = "FC 100 MBytes/sec";

    return s;
}
/*
static void sff8079_show_encoding(const __u8 *id)
{
  sff8024_show_encoding(id, 11, ETH_MODULE_SFF_8472);
}
*/

String  sff8079_rate_identifier()
{
  String s;


  switch (EEPROM_DATA[13]) {
  case 0x00:
    s = "unspecified";
    break;
  case 0x01:
    s = "4/2/1G Rate_Select & AS0/AS1";
    break;
  case 0x02:
    s = "8/4/2G Rx Rate_Select only";
    break;
  case 0x03:
    s = "8/4/2G Independent Rx & Tx Rate_Select";
    break;
  case 0x04:
    s = "8/4/2G Tx Rate_Select only";
    break;
  default:
    s= "reserved or unknown";
    break;
  }
  return s;
}
/*
static void sff8079_show_oui(const __u8 *id)
{
  printf("\t%-41s : %02x:%02x:%02x\n", "Vendor OUI",
         id[37], id[38], id[39]);
}

static void sff8079_show_wavelength_or_copper_compliance(const __u8 *id)
{
  if (id[8] & (1 << 2)) {
    printf("\t%-41s : 0x%02x", "Passive Cu cmplnce.", id[60]);
    switch (id[60]) {
    case 0x00:
      printf(" (unspecified)");
      break;
    case 0x01:
      printf(" (SFF-8431 appendix E)");
      break;
    default:
      printf(" (unknown)");
      break;
    }
    printf(" [SFF-8472 rev10.4 only]\n");
  } else if (id[8] & (1 << 3)) {
    printf("\t%-41s : 0x%02x", "Active Cu cmplnce.", id[60]);
    switch (id[60]) {
    case 0x00:
      printf(" (unspecified)");
      break;
    case 0x01:
      printf(" (SFF-8431 appendix E)");
      break;
    case 0x04:
      printf(" (SFF-8431 limiting)");
      break;
    default:
      printf(" (unknown)");
      break;
    }
    printf(" [SFF-8472 rev10.4 only]\n");
  } else {
    printf("\t%-41s : %unm\n", "Laser wavelength",
           (id[60] << 8) | id[61]);
  }
}

static void sff8079_show_value_with_unit(const __u8 *id, unsigned int reg,
           const char *name, unsigned int mult,
           const char *unit)
{
  unsigned int val = id[reg];

  printf("\t%-41s : %u%s\n", name, val * mult, unit);
}

static void sff8079_show_ascii(const __u8 *id, unsigned int first_reg,
             unsigned int last_reg, const char *name)
{
  unsigned int reg, val;

  printf("\t%-41s : ", name);
  while (first_reg <= last_reg && id[last_reg] == ' ')
    last_reg--;
  for (reg = first_reg; reg <= last_reg; reg++) {
    val = id[reg];
    putchar(((val >= 32) && (val <= 126)) ? val : '_');
  }
  printf("\n");
}

static void sff8079_show_options(const __u8 *id)
{
  static const char *pfx =
    "\tOption                                    :";

  printf("\t%-41s : 0x%02x 0x%02x\n", "Option values", id[64], id[65]);
  if (id[65] & (1 << 1))
    printf("%s RX_LOS implemented\n", pfx);
  if (id[65] & (1 << 2))
    printf("%s RX_LOS implemented, inverted\n", pfx);
  if (id[65] & (1 << 3))
    printf("%s TX_FAULT implemented\n", pfx);
  if (id[65] & (1 << 4))
    printf("%s TX_DISABLE implemented\n", pfx);
  if (id[65] & (1 << 5))
    printf("%s RATE_SELECT implemented\n", pfx);
  if (id[65] & (1 << 6))
    printf("%s Tunable transmitter technology\n", pfx);
  if (id[65] & (1 << 7))
    printf("%s Receiver decision threshold implemented\n", pfx);
  if (id[64] & (1 << 0))
    printf("%s Linear receiver output implemented\n", pfx);
  if (id[64] & (1 << 1))
    printf("%s Power level 2 requirement\n", pfx);
  if (id[64] & (1 << 2))
    printf("%s Cooled transceiver implemented\n", pfx);
  if (id[64] & (1 << 3))
    printf("%s Retimer or CDR implemented\n", pfx);
  if (id[64] & (1 << 4))
    printf("%s Paging implemented\n", pfx);
  if (id[64] & (1 << 5))
    printf("%s Power level 3 requirement\n", pfx);
}

void sff8079_show_all(const __u8 *id)
{
  sff8079_show_identifier(id);
  if (((id[0] == 0x02) || (id[0] == 0x03)) && (id[1] == 0x04)) {
    sff8079_show_ext_identifier(id);
    sff8079_show_connector(id);
    sff8079_show_transceiver(id);
    sff8079_show_encoding(id);
    sff8079_show_value_with_unit(id, 12, "BR, Nominal", 100, "MBd");
    sff8079_show_rate_identifier(id);
    sff8079_show_value_with_unit(id, 14,
               "Length (SMF,km)", 1, "km");
    sff8079_show_value_with_unit(id, 15, "Length (SMF)", 100, "m");
    sff8079_show_value_with_unit(id, 16, "Length (50um)", 10, "m");
    sff8079_show_value_with_unit(id, 17,
               "Length (62.5um)", 10, "m");
    sff8079_show_value_with_unit(id, 18, "Length (Copper)", 1, "m");
    sff8079_show_value_with_unit(id, 19, "Length (OM3)", 10, "m");
    sff8079_show_wavelength_or_copper_compliance(id);
    sff8079_show_ascii(id, 20, 35, "Vendor name");
    sff8079_show_oui(id);
    sff8079_show_ascii(id, 40, 55, "Vendor PN");
    sff8079_show_ascii(id, 56, 59, "Vendor rev");
    sff8079_show_options(id);
    sff8079_show_value_with_unit(id, 66, "BR margin, max", 1, "%");
    sff8079_show_value_with_unit(id, 67, "BR margin, min", 1, "%");
    sff8079_show_ascii(id, 68, 83, "Vendor SN");
    sff8079_show_ascii(id, 84, 91, "Date code");
  }
}
*/
