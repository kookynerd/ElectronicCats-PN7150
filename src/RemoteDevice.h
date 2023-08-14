#ifndef RemoteDevice_H
#define RemoteDevice_H

#include "Arduino.h"
#include "Interface.h"
#include "ModeTech.h"
#include "Protocol.h"
#include "Technology.h"

/*
 * Definition of discovered remote device properties information
 */

/* POLL passive type A */
struct RfIntf_info_APP_t {
  unsigned char SensRes[2];
  unsigned char NfcId[10];
  unsigned char NfcIdLen;
  unsigned char SelRes[1];
  unsigned char SelResLen;
  unsigned char Rats[20];
  unsigned char RatsLen;
};

/* POLL passive type A camelCase */
struct RfIntfInfoAppCC_t {
  unsigned char sensRes[2];
  unsigned char nfcId[10];
  unsigned char nfcIdLen;
  unsigned char selRes[1];
  unsigned char selResLen;
  unsigned char rats[20];
  unsigned char ratsLen;
};

/* POLL passive type B */
struct RfIntf_info_BPP_t {
  unsigned char SensRes[12];
  unsigned char SensResLen;
  unsigned char AttribRes[17];
  unsigned char AttribResLen;
};

/* POLL passive type B camelCase */
struct RfIntfInfoBppCC_t {
  unsigned char sensRes[12];
  unsigned char sensResLen;
  unsigned char attribRes[17];
  unsigned char attribResLen;
};

/* POLL passive type F */
struct RfIntf_info_FPP_t {
  unsigned char BitRate;
  unsigned char SensRes[18];
  unsigned char SensResLen;
};

/* POLL passive type F camelCase */
struct RfIntfInfoFppCC_t {
  unsigned char bitRate;
  unsigned char sensRes[18];
  unsigned char sensResLen;
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
  bool moreTagsAvailable;
  RfIntfInfoCC_t info;
};

class RemoteDevice {
 private:
  RfIntfCC_t remoteDeviceStruct;

 public:
  unsigned char getInterface() const;
  unsigned char getProtocol() const;
  unsigned char getModeTech() const;
  bool hasMoreTags() const;
  unsigned char getAPPSensRes(int index) const;
  const unsigned char* getAPPID() const;
  unsigned char getAPPIDLen() const;
  unsigned char getAPPSelRes(int index) const;
  unsigned char getAPPSelResLen() const;
  unsigned char getAPPRats(int index) const;
  unsigned char getAPPRatsLen() const;
  const unsigned char* getVPPID() const;
  void setInterface(unsigned char interface);
  void setProtocol(unsigned char protocol);
  void setModeTech(unsigned char modeTech);
  void setMoreTagsAvailable(bool moreTags);
  void setInfo(RfIntf_t *pRfIntf, uint8_t *pBuf);
};

#endif