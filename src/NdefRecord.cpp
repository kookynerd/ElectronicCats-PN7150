/**
 * Library to manage the NDEF record
 * Authors:
 *        Francisco Torres - Electronic Cats - electroniccats.com
 *
 *  August 2023
 *
 * This code is beerware; if you see me (or any other collaborator
 * member) at the local, and you've found our code helpful,
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 */

#include "NdefRecord.h"

NdefRecord::NdefRecord() {
  this->_type = UNSUPPORTED_NDEF_RECORD;
  this->headerFlags = 0;
  this->payload = NULL;
  this->payloadSize = 0;
  this->typeLength = 0;
  this->recordType = 0;
  this->status = 0;
  this->languageCode = 0;
  this->newString = "null";
}

void NdefRecord::create(NdefRecord_t record) {
  this->_type = record.recordType;
  this->payload = record.recordPayload;
  this->payloadSize = record.recordPayloadSize;
}

String NdefRecord::getHexRepresentation(const byte *data, const uint32_t dataSize) {
  String hexString;

  if (dataSize == 0) {
    hexString = newString;
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
  return this->_type;
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
  String text = newString;

  if (getType() == WELL_KNOWN_SIMPLE_TEXT) {
    text = reinterpret_cast<const char *>(&payload[payload[0] + 1]);
  }

  payload[payloadSize] = save;

  return text;
}

String NdefRecord::getBluetoothName() {
  String bluetoothName = newString;

  if (getType() != MEDIA_HANDOVER_BT) {
    return bluetoothName;
  } else {
    bluetoothName = "";
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
  String bluetoothAddress = newString;

  if (getType() != MEDIA_HANDOVER_BT) {
    return bluetoothAddress;
  } else {
    bluetoothAddress = "";
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
  String ssid = newString;

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

String NdefRecord::getWiFiAuthenticationType() {
  String authenticationType = newString;
  unsigned char index = 0;

  if (getType() != MEDIA_HANDOVER_WIFI) {
    return authenticationType;
  }

  if ((getPayload()[index] == 0x10) && (getPayload()[index + 1] == 0x0e)) {
    index += 4;
  }

  while (index < getPayloadSize()) {
    if (getPayload()[index] == 0x10) {
      if (getPayload()[index + 1] == 0x03) {
        authenticationType = ndef_helper_WifiAuth(getPayload()[index + 5]);
      }
      index += 4 + getPayload()[index + 3];
    }
  }

  return authenticationType;
}

String NdefRecord::getWiFiEncryptionType() {
  String encryptionType = newString;
  unsigned char index = 0;

  if (getType() != MEDIA_HANDOVER_WIFI) {
    return encryptionType;
  }

  if ((getPayload()[index] == 0x10) && (getPayload()[index + 1] == 0x0e)) {
    index += 4;
  }

  while (index < getPayloadSize()) {
    if (getPayload()[index] == 0x10) {
      if (getPayload()[index + 1] == 0x0f) {
        encryptionType = ndef_helper_WifiEnc(getPayload()[index + 5]);
      }
      index += 4 + getPayload()[index + 3];
    }
  }

  return encryptionType;
}

String NdefRecord::getWiFiPassword() {
  String networkKey = newString;
  unsigned char index = 0;

  if (getType() != MEDIA_HANDOVER_WIFI) {
    return networkKey;
  }

  if ((getPayload()[index] == 0x10) && (getPayload()[index + 1] == 0x0e)) {
    index += 4;
  }

  while (index < getPayloadSize()) {
    if (getPayload()[index] == 0x10) {
      if (getPayload()[index + 1] == 0x27) {
        networkKey = reinterpret_cast<const char *>(&getPayload()[index + 4]);
      }
      index += 4 + getPayload()[index + 3];
    }
  }

  return networkKey;
}

String NdefRecord::getVCardContent() {
  String content = newString;

  if (getType() != MEDIA_VCARD) {
    return content;
  }

  content = reinterpret_cast<const char *>(getPayload());

  return content;
}

String NdefRecord::getUri() {
  String uri = newString;

  if (getType() != WELL_KNOWN_SIMPLE_URI) {
    return uri;
  }

  unsigned char save = payload[payloadSize];
  payload[payloadSize] = '\0';
  uri = reinterpret_cast<const char *>(ndef_helper_UriHead(payload[0]), &payload[1]);
  payload[payloadSize] = save;

  return uri;
}

void NdefRecord::setPayload(unsigned char *payload) {
  this->payload = payload;
}

void NdefRecord::setHeaderFlags(uint8_t headerFlags) {
  this->headerFlags = headerFlags;
}

void NdefRecord::setTypeLength(uint8_t typeLength) {
  this->typeLength = typeLength;
}

void NdefRecord::setRecordType(uint8_t recordType) {
  this->recordType = recordType;
}

void NdefRecord::setStatus(uint8_t status) {
  this->status = status;
}

void NdefRecord::setLanguageCode(unsigned char *languageCode) {
  this->languageCode = languageCode;
  Serial.println("Language code: " + String((char *)this->languageCode));
  Serial.println("Language code: " + getHexRepresentation(this->languageCode, 2)); // Language code: 65:6E
}

void NdefRecord::setPayloadSize(uint8_t payloadSize) {
  this->payloadSize = payloadSize;
}

const char *NdefRecord::getContent() {
  char *recordContent = new char[getPayloadSize()];
  Serial.println("Language code: " + getHexRepresentation(this->languageCode, 2)); // Language code: 010:00

  recordContent[0] = headerFlags;
  recordContent[1] = typeLength;
  recordContent[2] = payloadSize;
  recordContent[3] = recordType;
  recordContent[4] = status;
  recordContent[5] = languageCode[0];
  recordContent[6] = languageCode[1];

  Serial.println("Header flags: " + String(recordContent[0], HEX));
  Serial.print("Language code: ");
  Serial.print(languageCode[0], HEX);
  Serial.println(languageCode[1], HEX);
  Serial.println("Language code: " + String(recordContent[5]) + String(recordContent[6]));

  for (int i = 0; i < getPayloadSize(); i++) {
    recordContent[i + 7] = payload[i];
  }

  return recordContent;
}

unsigned short NdefRecord::getContentSize() {
  return getPayloadSize() + 4; // 5 bytes for header, type length, payload length, record type, status, and 2 bytes for language code
}
