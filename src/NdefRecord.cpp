#include "NdefRecord.h"

NdefRecord::NdefRecord() {
  type = UNSUPPORTED_NDEF_RECORD;
  payload = NULL;
  payloadSize = 0;
}

void NdefRecord::create(NdefRecord_t record) {
  this->type = record.recordType;
  this->payload = record.recordPayload;
  this->payloadSize = record.recordPayloadSize;
}

bool NdefRecord::isEmpty() {
  return this->payload == NULL;
}

bool NdefRecord::isNotEmpty() {
  return this->payload != NULL;
}

NdefRecordType_e NdefRecord::getType() {
  return this->type;
}

unsigned char *NdefRecord::getPayload() {
  return this->payload;
}

unsigned short NdefRecord::getPayloadSize() {
  return this->payloadSize;
}

String NdefRecord::getText() {
  unsigned char save = payload[payloadSize];
  payload[payloadSize] = '\0';
  String text = "";

  if (getType() == WELL_KNOWN_SIMPLE_TEXT) {
    text = reinterpret_cast<const char *>(&payload[payload[0] + 1]);
  }

  payload[payloadSize] = save;

  return text;
}

String NdefRecord::getBluetoothName() {
  String bluetoothName = "";
  
  for (unsigned int i = 10; i < payloadSize; i++) {
    if (payload[i] == 0x04) {
      break;
    }
    bluetoothName += (char)payload[i];
  }

  return bluetoothName;
}
