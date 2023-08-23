#ifndef NdefRecord_H
#define NdefRecord_H

#include <Arduino.h>

#include "ndef_helper.h"

class NdefRecord {
 private:
  NdefRecordType_e type;
  unsigned char *payload;
  unsigned short payloadSize;

 public:
  NdefRecord();
	void create(NdefRecord_t record);
	NdefRecordType_e getType();
	unsigned char getPayload();
	unsigned short getPayloadSize();
	String getText();
};

#endif