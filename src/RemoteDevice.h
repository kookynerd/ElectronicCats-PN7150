#ifndef RemoteDevice_H
#define RemoteDevice_H

#include "Arduino.h"

/*
 * Definition of discovered remote device properties information
 */

/* POLL passive type A */
struct RfIntf_info_APP_t {
  unsigned char SensRes[2];
  unsigned char NfcIdLen;
  unsigned char NfcId[10];
  unsigned char SelResLen;
  unsigned char SelRes[1];
  unsigned char RatsLen;
  unsigned char Rats[20];
};

/* POLL passive type B */
struct RfIntf_info_BPP_t {
  unsigned char SensResLen;
  unsigned char SensRes[12];
  unsigned char AttribResLen;
  unsigned char AttribRes[17];
};

/* POLL passive type F */
struct RfIntf_info_FPP_t {
  unsigned char BitRate;
  unsigned char SensResLen;
  unsigned char SensRes[18];
};

/* POLL passive type ISO15693 */
struct RfIntf_info_VPP_t {
  unsigned char AFI;
  unsigned char DSFID;
  unsigned char ID[8];
};

typedef union {
  RfIntf_info_APP_t NFC_APP;
  RfIntf_info_BPP_t NFC_BPP;
  RfIntf_info_FPP_t NFC_FPP;
  RfIntf_info_VPP_t NFC_VPP;
} RfIntf_Info_t;

/*
 * Definition of discovered remote device properties
 */
struct RfIntf_t {
  unsigned char Interface;
  unsigned char Protocol;
  unsigned char ModeTech;
  bool MoreTags;
  RfIntf_Info_t Info;
};

class RemoteDevice {
 protected:
  RfIntf_t *remoteDevice;
};

#endif