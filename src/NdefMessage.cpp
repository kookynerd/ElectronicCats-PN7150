/**
 * Library to manage the NDEF message
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

#include "NdefMessage.h"

unsigned char *NdefMessage::content;
unsigned short NdefMessage::contentSize;
uint8_t NdefMessage::recordCounter;
String NdefMessage::newString;

NdefMessage::NdefMessage() {
  content = NULL;
  contentSize = 0;
  recordCounter = 0;
  newString = "null";
}

void NdefMessage::begin() {
  registerUpdateNdefMessageCallback(NdefMessage::update);
}

String NdefMessage::getHexRepresentation(const byte *data, const uint32_t dataSize) {
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

void NdefMessage::update(unsigned char *message, unsigned short messageSize) {
  if (content != NULL) {
    free(content);
  }
  content = (unsigned char *)malloc(messageSize);
  memcpy(content, message, messageSize);
  contentSize = messageSize;
}

unsigned char *NdefMessage::getContent() {
  return content;
}

unsigned short NdefMessage::getContentSize() {
  return contentSize;
}

void NdefMessage::setContent(const char *content, unsigned short contentSize) {
  NdefMessage::content = (unsigned char *)content;
  NdefMessage::contentSize = contentSize;

  Serial.println(NdefMessage::getHexRepresentation((byte *)content, (uint32_t)contentSize));
  T4T_NDEF_EMU_SetMsg(content, contentSize);
}

NdefRecord_t NdefMessage::getRecord() {
  NdefRecord_t ndefRecord = DetectNdefRecordType(content);

  if (NdefMessage::isEmpty()) {
    return ndefRecord;
  }

  NdefMessage::getNextRecord();
  return ndefRecord;
}

void NdefMessage::getNextRecord() {
  content = GetNextRecord(content);
}

bool NdefMessage::isEmpty() {
  return NdefMessage::getContent() == NULL;
}

bool NdefMessage::isNotEmpty() {
  return NdefMessage::getContent() != NULL;
}

bool NdefMessage::hasRecord() {
  return NdefMessage::isNotEmpty();
}

bool NdefMessage::addRecord(NdefRecord record) {
  if (recordCounter < MAX_NDEF_RECORDS) {
    records[recordCounter] = record;
    recordCounter++;

    setContent(record.getContent(), record.getContentSize());

    return true;
  }
  return false;
}

bool NdefMessage::addTextRecord(String text) {
  NdefRecord record;
  record.setHeaderFlags(0xD1);
  record.setTypeLength(0x01);
  record.setPayloadSize(text.length() + 3);
  record.setRecordType(0x54);
  record.setStatus(0x02);
  record.setLanguageCode((unsigned char *)"en");
  record.setPayload((unsigned char *)text.c_str());

  Serial.println("Payload size: " + String(record.getPayloadSize()));
  unsigned char *payload = record.getPayload();
  Serial.println("Payload: " + String((char *)payload));

  return addRecord(record);
}
