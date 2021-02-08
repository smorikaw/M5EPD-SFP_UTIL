//
// extend transeiver type name case
//
String sff_ext_type(int offset){
  String s;

  switch(EEPROM_DATA[offset]){

  case SFF8636_ETHERNET_UNSPECIFIED: s = "UNSPECIFIED"; break;
  case SFF8636_ETHERNET_100G_AOC: s = "100G AOC"; break;
  case SFF8636_ETHERNET_100G_SR4: s = "100G SR4"; break;
  case SFF8636_ETHERNET_100G_LR4: s = "100G LR4"; break;
  case SFF8636_ETHERNET_100G_ER4: s = "100G ER4"; break;
  case SFF8636_ETHERNET_100G_SR10: s ="100G SR10"; break;
  case SFF8636_ETHERNET_100G_CWDM4_FEC: s = "100G CWDM4"; break;
  case SFF8636_ETHERNET_100G_PSM4: s = "100G PSM4"; break;
  case SFF8636_ETHERNET_100G_ACC: s = "100G ACC"; break;
  case SFF8636_ETHERNET_100G_CWDM4_NO_FEC: s = "100G CWDM4/noFEC"; break;
  case SFF8636_ETHERNET_100G_RSVD1: s = "100G WSV"; break;
  case SFF8636_ETHERNET_100G_CR4: s = "100G CR4"; break;
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
  case SFF8636_ETHERNET_100G_AOC2: s = "100G AOC2"; break;
  case SFF8636_ETHERNET_100G_ACC2: s = "100G AOC2"; break;
  
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
  case SFF8636_ETHERNET_200G_CR4: s = "200G CR4"; break;
  case SFF8636_ETHERNET_200G_SR4: s = "200G SR4"; break;
  case SFF8636_ETHERNET_200G_DR4: s = "200G DR4"; break;
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
// ===============================================================
String identName(int offset){
  String s;
    switch (EEPROM_DATA[offset]) {
  case SFF8024_ID_UNKNOWN:
    s = "(no module present, unknown, or unspecified)";
    break;
  case SFF8024_ID_GBIC:
    s = "(GBIC)";
    break;
  case SFF8024_ID_SOLDERED_MODULE:
   s = "(module soldered to motherboard)";
    break;
  case SFF8024_ID_SFP:
    s = "(SFP)";
    break;
  case SFF8024_ID_300_PIN_XBI:
    s = "(300 pin XBI)";
    break;
  case SFF8024_ID_XENPAK:
    s = "(XENPAK)";
    break;
  case SFF8024_ID_XFP:
    s = "(XFP)";
    break;
  case SFF8024_ID_XFF:
    s = "(XFF)";
    break;
  case SFF8024_ID_XFP_E:
    s = "(XFP-E)";
    break;
  case SFF8024_ID_XPAK:
    s = "(XPAK)";
    break;
  case SFF8024_ID_X2:
    s = "(X2)";
    break;
  case SFF8024_ID_DWDM_SFP:
    s = "(DWDM-SFP)";
    break;
  case SFF8024_ID_QSFP:
    s = "(QSFP)";
    break;
  case SFF8024_ID_QSFP_PLUS:
    s = "(QSFP+)";
    break;
  case SFF8024_ID_CXP:
    s = "(CXP)";
    break;
  case SFF8024_ID_HD4X:
    s = " (Shielded Mini Multilane HD 4X)";
    break;
  case SFF8024_ID_HD8X:
    s = " (Shielded Mini Multilane HD 8X)";
    break;
  case SFF8024_ID_QSFP28:
    s = " QSFP28)";
    break;
  case SFF8024_ID_CXP2:
    s = "(CXP2/CXP28)";
    break;
  case SFF8024_ID_CDFP:
    s = "(CDFP Style 1/Style 2)";
    break;
  case SFF8024_ID_HD4X_FANOUT:
    s = "(Shielded Mini Multilane HD 4X Fanout Cable)";
    break;
  case SFF8024_ID_HD8X_FANOUT:
    s = "(Shielded Mini Multilane HD 8X Fanout Cable)";
    break;
  case SFF8024_ID_CDFP_S3:
    s = "(CDFP Style 3)";
    break;
  case SFF8024_ID_MICRO_QSFP:
    s = "(microQSFP)";
    break;
  case SFF8024_ID_QSFP_DD:
    s = "(QSFP-DD Double Density 8X Pluggable Transceiver (INF-8628))";
    break;
  default:
    s = "(reserved or unknown)";
    break;
  }
  return s;
}

// ===============================================================
String sff_revision(int offset)
{
  String s;
  switch (EEPROM_DATA[offset]) {
  case SFF8636_REV_UNSPECIFIED:
    s = "Revision not specified";
    break;
  case SFF8636_REV_8436_48:
    s = "SFF-8436 Rev 4.8 or earlier";
    break;
  case SFF8636_REV_8436_8636:
    s = "SFF-8436 Rev 4.8 or earlier";
    break;
  case SFF8636_REV_8636_13:
    s = "SFF-8636 Rev 1.3 or earlier";
    break;
  case SFF8636_REV_8636_14:
    s = "SFF-8636 Rev 1.4";
    break;
  case SFF8636_REV_8636_15:
    s = "SFF-8636 Rev 1.5";
    break;
  case SFF8636_REV_8636_20:
    s = "SFF-8636 Rev 2.0";
    break;
  case SFF8636_REV_8636_27:
    s = "SFF-8636 Rev 2.5/2.6/2.7";
    break;
  default:
    s = "Unallocated";
    break;
  }
  return s;
}
// ===============================================================
String sff8024_connector(int offset)
{
  String s;
  
  switch (EEPROM_DATA[offset]) {
  case  SFF8024_CTOR_UNKNOWN:
    s = "unspecified";
    break;
  case SFF8024_CTOR_SC:
    s = "SC";
    break;
  case SFF8024_CTOR_FC_STYLE_1:
    s = "FC Style 1 copper";
    break;
  case SFF8024_CTOR_FC_STYLE_2:
    s = "FC Style 2 copper";
    break;
  case SFF8024_CTOR_BNC_TNC:
    s = "BNC/TNC";
    break;
  case SFF8024_CTOR_FC_COAX:
    s = "FC coaxial headers";
    break;
  case SFF8024_CTOR_FIBER_JACK:
    s = "FibreJack";
    break;
  case SFF8024_CTOR_LC:
    s = "LC";
    break;
  case SFF8024_CTOR_MT_RJ:
    s = "MT-RJ";
    break;
  case SFF8024_CTOR_MU:
    s = "MU";
    break;
  case SFF8024_CTOR_SG:
    s = "SG";
    break;
  case SFF8024_CTOR_OPT_PT:
    s = "Optical pigtail";
    break;
  case SFF8024_CTOR_MPO:
    s = "MPO 1x12";
    break;
  case SFF8024_CTOR_MPO_2:
    s = "MPO 2x16";
    break;
  case SFF8024_CTOR_HSDC_II:
    s = "HSSDC II";
    break;
  case SFF8024_CTOR_COPPER_PT:
    s = "Copper pigtail";
    break;
  case SFF8024_CTOR_RJ45:
    s = "RJ45";
    break;
  case SFF8024_CTOR_NO_SEPARABLE:
    s = "No separable connector";
    break;
  case SFF8024_CTOR_MXC_2x16:
    s = "MXC 2x16";
    break;
  case SFF8024_CTOR_CS_OPTICAL:
    s = "CS optical connector";
    break;
  case SFF8024_CTOR_CS_OPTICAL_MINI:
    s = "Mini CS optical connector";
    break;
  case SFF8024_CTOR_MPO_2X12:
    s = "MPO 2x12";
    break;
  case SFF8024_CTOR_MPO_1X16:
    s = "MPO 1x16";
    break;
  default:
    s = "unknown";
    break;
  }
  return s;
}
String  sff8636_tx_tech()
{
  String s;
//  printf("\t%-41s : 0x%02x", "Transmitter technology",
//    (id[SFF8636_DEVICE_TECH_OFFSET] & SFF8636_TRANS_TECH_MASK));

  switch (EEPROM_DATA[SFF8636_DEVICE_TECH_OFFSET] & SFF8636_TRANS_TECH_MASK) {
  case SFF8636_TRANS_850_VCSEL:
    s = "850 nm VCSEL";
    break;
  case SFF8636_TRANS_1310_VCSEL:
    s = "1310 nm VCSEL";
    break;
  case SFF8636_TRANS_1550_VCSEL:
    s = "1550 nm VCSEL";
    break;
  case SFF8636_TRANS_1310_FP:
    s = "1310 nm FP";
    break;
  case SFF8636_TRANS_1310_DFB:
    s = "1310 nm DFB";
    break;
  case SFF8636_TRANS_1550_DFB:
    s = "1550 nm DFB";
    break;
  case SFF8636_TRANS_1310_EML:
    s = "1310 nm EML";
    break;
  case SFF8636_TRANS_1550_EML:
    s = "1550 nm EML";
    break;
  case SFF8636_TRANS_OTHERS:
    s = "Others/Undefined";
    break;
  case SFF8636_TRANS_1490_DFB:
    s = "1490 nm DFB";
    break;
  case SFF8636_TRANS_COPPER_PAS_UNEQUAL:
    s = "Copper cable unequalized";
    break;
  case SFF8636_TRANS_COPPER_PAS_EQUAL:
    s = "Copper cable passive equalized";
    break;
  case SFF8636_TRANS_COPPER_LNR_FAR_EQUAL:
    s = "Copper cable, near and far end limiting active equalizers";
    break;
  case SFF8636_TRANS_COPPER_FAR_EQUAL:
    s = "Copper cable, far end limiting active equalizers";
    break;
  case SFF8636_TRANS_COPPER_NEAR_EQUAL:
    s = "Copper cable, near end limiting active equalizers";
    break;
  case SFF8636_TRANS_COPPER_LNR_EQUAL:
    s = "Copper cable, linear active equalizers";
    break;
  }
  return s;
}

// static void sff8636_show_ext_identifier(const __u8 *id)
String sff_powerclass(){

  String s;

//  printf("\t%-41s : 0x%02x\n", "Extended identifier",
//      id[SFF8636_EXT_ID_OFFSET]);

//  static const char *pfx =
//    "\tExtended identifier description           :";

  switch (EEPROM_DATA[SFF8636_EXT_ID_OFFSET] & SFF8636_EXT_ID_PWR_CLASS_MASK) {
  case SFF8636_EXT_ID_PWR_CLASS_1:
    s = "1.5W max";
    break;
  case SFF8636_EXT_ID_PWR_CLASS_2:
    s = "2.0W max";
    break;
  case SFF8636_EXT_ID_PWR_CLASS_3:
    s = "2.5W max";
    break;
  case SFF8636_EXT_ID_PWR_CLASS_4:
    s = "3.5W max";
    break;
  }

//  if (id[SFF8636_EXT_ID_OFFSET] & SFF8636_EXT_ID_CDR_TX_MASK)
//    printf("%s CDR present in TX,", pfx);
//  else
//    printf("%s No CDR in TX,", pfx);

//  if (id[SFF8636_EXT_ID_OFFSET] & SFF8636_EXT_ID_CDR_RX_MASK)
//    printf(" CDR present in RX\n");
//  else
//    printf(" No CDR in RX\n");

  switch (EEPROM_DATA[SFF8636_EXT_ID_OFFSET] & SFF8636_EXT_ID_EPWR_CLASS_MASK) {
  case SFF8636_EXT_ID_PWR_CLASS_LEGACY:
//    printf("%s", pfx);
    break;
  case SFF8636_EXT_ID_PWR_CLASS_5:
    s = "4.0W max";
    break;
  case SFF8636_EXT_ID_PWR_CLASS_6:
    s = "4.5W max";
    break;
  case SFF8636_EXT_ID_PWR_CLASS_7:
    s = "5.0W max";
    break;
  }
  if(EEPROM_DATA[SFF8636_EXT_ID_OFFSET] & 0x20) s = "over 5.0W";
//  if (id[SFF8636_PWR_MODE_OFFSET] & SFF8636_HIGH_PWR_ENABLE)
//    printf(" High Power Class (> 3.5 W) enabled\n");
//  else
//    printf(" High Power Class (> 3.5 W) not enabled\n");
 return s;
}

//  if ((id[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP) ||
//    (id[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP_PLUS) ||
//    (id[SFF8636_ID_OFFSET] == SFF8024_ID_QSFP28)) 
String IDname(){
  String s;
  switch (EEPROM_DATA[SFF8636_ID_OFFSET]){
    case SFF8024_ID_GBIC: s = "GBIC"; break;
    case SFF8024_ID_SFP: s = "SFP"; break;
    case SFF8024_ID_DWDM_SFP: s="DWDM SFP"; break;
    case SFF8024_ID_QSFP : s = "QSFP"; break;
    case SFF8024_ID_QSFP_PLUS: s = "QSFP+"; break;
    case SFF8024_ID_QSFP28: s = "QSFP28"; break;
    case SFF8024_ID_QSFP_DD: s ="QSFP-DD"; break;
    default : s = "unkown";
  }
  return s;
}

