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

String NdefRecord::getHexRepresentation(const byte *data, const uint32_t dataSize) {
  String hexString;

  if (dataSize == 0) {
    hexString = "null";
  }

  for (uint32_t index = 0; index < dataSize; index++) {
    if (data[index] <= 0xF)
      hexString += "0";
    String hexValue = String(data[index] & 0xFF, HEX);
    hexValue.toUpperCase();
    hexString += hexValue;
    if ((dataSize > 1) && (index != dataSize - 1)) {
      hexString += ":";
    }
  }
  return hexString;
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

  if (getType() != MEDIA_HANDOVER_BT) {
    return bluetoothName;
  }

  for (unsigned int i = 10; i < payloadSize; i++) {
    if (payload[i] == 0x04) {
      break;
    }
    bluetoothName += (char)payload[i];
  }

  return bluetoothName;
}

String NdefRecord::getBluetoothAddress() {
  String bluetoothAddress = "";

  if (getType() != MEDIA_HANDOVER_BT) {
    return bluetoothAddress;
  }

  for (unsigned int i = 7; i >= 2; i--) {
    bluetoothAddress += getHexRepresentation(&payload[i], 1);
    if (i > 2) {
      bluetoothAddress += ":";
    }
  }

  return bluetoothAddress;
}

String NdefRecord::getWiFiSSID() {
  String ssid = "";

  if (getType() != MEDIA_HANDOVER_WIFI) {
    return ssid;
  }

  for (unsigned int i = 0; i < payloadSize; i++) {
    if (payload[i] == 0x45) {
      ssid = reinterpret_cast<const char *>(&payload[i + 3]);
      break;
    }
  }

  return ssid;
}
