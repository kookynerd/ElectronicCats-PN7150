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

/* POLL passive type A camelCase */
struct RfIntfInfoAppCC_t {
  unsigned char sensRes[2];
  unsigned char nfcIdLen;
  unsigned char nfcId[10];
  unsigned char selResLen;
  unsigned char selRes[1];
  unsigned char ratsLen;
  unsigned char rats[20];
};

/* POLL passive type B */
struct RfIntf_info_BPP_t {
  unsigned char SensResLen;
  unsigned char SensRes[12];
  unsigned char AttribResLen;
  unsigned char AttribRes[17];
};

/* POLL passive type B camelCase */
struct RfIntfInfoBppCC_t {
  unsigned char sensResLen;
  unsigned char sensRes[12];
  unsigned char attribResLen;
  unsigned char attribRes[17];
};

/* POLL passive type F */
struct RfIntf_info_FPP_t {
  unsigned char BitRate;
  unsigned char SensResLen;
  unsigned char SensRes[18];
};

/* POLL passive type F camelCase */
struct RfIntfInfoFppCC_t {
  unsigned char bitRate;
  unsigned char sensResLen;
  unsigned char sensRes[18];
};

/* POLL passive type ISO15693 */
struct RfIntf_info_VPP_t {
  unsigned char AFI;
  unsigned char DSFID;
  unsigned char ID[8];
};

/* POLL passive type ISO15693 camelCase */
struct RfIntfInfoVppCC_t {
  unsigned char afi;
  unsigned char dsfid;
  unsigned char id[8];
};

typedef union {
  RfIntf_info_APP_t NFC_APP;
  RfIntf_info_BPP_t NFC_BPP;
  RfIntf_info_FPP_t NFC_FPP;
  RfIntf_info_VPP_t NFC_VPP;
} RfIntf_Info_t;

typedef union {
  RfIntfInfoAppCC_t nfcAPP;
  RfIntfInfoBppCC_t nfcBPP;
  RfIntfInfoFppCC_t nfcFPP;
  RfIntfInfoVppCC_t nfcVPP;
} RfIntfInfoCC_t;

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

// Definition of discovered remote device properties using camelCase
struct RfIntfCC_t {
  unsigned char interface;
  unsigned char protocol;
  unsigned char modeTech;
  bool moreTags;
  RfIntfInfoCC_t info;
};

class RemoteDevice {
 public:
  RfIntfCC_t remoteDevice;
};

#endif