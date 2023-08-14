#include "RemoteDevice.h"

unsigned char RemoteDevice::getInterface() const {
	return this->remoteDeviceStruct.interface;
}

unsigned char RemoteDevice::getProtocol() const {
  return this->remoteDeviceStruct.protocol;
}

unsigned char RemoteDevice::getModeTech() const {
	return this->remoteDeviceStruct.modeTech;
}

bool RemoteDevice::hasMoreTags() const {
  return this->remoteDeviceStruct.moreTagsAvailable;
}

// TODO: validate memory access
unsigned char RemoteDevice::getVPPID(int position) const {
	return this->remoteDeviceStruct.info.nfcVPP.id[position];
}

void RemoteDevice::setInterface(unsigned char interface) {
  this->remoteDeviceStruct.interface = interface;
}

void RemoteDevice::setProtocol(unsigned char protocol) {
	this->remoteDeviceStruct.protocol = protocol;
}

void RemoteDevice::setModeTech(unsigned char modeTech) {
	this->remoteDeviceStruct.modeTech = modeTech;
}

void RemoteDevice::setMoreTagsAvailable(bool moreTags) {
	this->remoteDeviceStruct.moreTagsAvailable = moreTags;
}

void RemoteDevice::setInfo(RfIntf_t *pRfIntf, uint8_t *pBuf) {
	uint8_t i, temp;

  switch (pRfIntf->ModeTech) {
    case (MODE_POLL | TECH_PASSIVE_NFCA):
      memcpy(pRfIntf->Info.NFC_APP.SensRes, &pBuf[0], 2);
			memcpy(remoteDeviceStruct.info.nfcAPP.sensRes, &pBuf[0], 2);
      temp = 2;
      pRfIntf->Info.NFC_APP.NfcIdLen = pBuf[temp];
      temp++;
      memcpy(pRfIntf->Info.NFC_APP.NfcId, &pBuf[3], pRfIntf->Info.NFC_APP.NfcIdLen);
      temp += pBuf[2];
      pRfIntf->Info.NFC_APP.SelResLen = pBuf[temp];
      temp++;

      if (pRfIntf->Info.NFC_APP.SelResLen == 1)
        pRfIntf->Info.NFC_APP.SelRes[0] = pBuf[temp];

      temp += 4;
      if (pBuf[temp] != 0) {
        temp++;
        pRfIntf->Info.NFC_APP.RatsLen = pBuf[temp];
        memcpy(pRfIntf->Info.NFC_APP.Rats, &pBuf[temp + 1], pBuf[temp]);
      } else {
        pRfIntf->Info.NFC_APP.RatsLen = 0;
      }
      break;

    case (MODE_POLL | TECH_PASSIVE_NFCB):
      pRfIntf->Info.NFC_BPP.SensResLen = pBuf[0];
      memcpy(pRfIntf->Info.NFC_BPP.SensRes, &pBuf[1], pRfIntf->Info.NFC_BPP.SensResLen);
      temp = pBuf[0] + 4;
      if (pBuf[temp] != 0) {
        temp++;
        pRfIntf->Info.NFC_BPP.AttribResLen = pBuf[temp];
        memcpy(pRfIntf->Info.NFC_BPP.AttribRes, &pBuf[temp + 1], pBuf[temp]);
      } else {
        pRfIntf->Info.NFC_BPP.AttribResLen = 0;
      }
      break;

    case (MODE_POLL | TECH_PASSIVE_NFCF):
      pRfIntf->Info.NFC_FPP.BitRate = pBuf[0];
      pRfIntf->Info.NFC_FPP.SensResLen = pBuf[1];
      memcpy(pRfIntf->Info.NFC_FPP.SensRes, &pBuf[2], pRfIntf->Info.NFC_FPP.SensResLen);
      break;

    case (MODE_POLL | TECH_PASSIVE_15693):
      pRfIntf->Info.NFC_VPP.AFI = pBuf[0];
      pRfIntf->Info.NFC_VPP.DSFID = pBuf[1];

      for (i = 0; i < 8; i++)
        pRfIntf->Info.NFC_VPP.ID[7 - i] = pBuf[2 + i];

      break;

    default:
      break;
  }
}