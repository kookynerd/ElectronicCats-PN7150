#ifndef NdefRecord_H
#define NdefRecord_H

#include <Arduino.h>

#include "ndef_helper.h"

class NdefRecord {
 private:
	NdefRecord_t content;
  NdefRecordType_e type;
  unsigned char *payload;
  unsigned short payloadSize;
  String getHexRepresentation(const byte *data, const uint32_t dataSize);

 public:
  NdefRecord();
	void create(NdefRecord_t record);
	bool isEmpty();
	bool isNotEmpty();
	NdefRecordType_e getType();
	unsigned char *getPayload();
	unsigned short getPayloadSize();
	String getText();
	String getBluetoothName();
	String getBluetoothAddress();
	String getWiFiSSID();
	String getWiFiAuthenticationType();  // TODO: not working properly
};

#endif