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
unsigned char *NdefMessage::newContent;
unsigned short NdefMessage::newContentSize;
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

void NdefMessage::addContent(const char *record, unsigned short recordSize) {
  unsigned short newContentSize = contentSize + recordSize;
  unsigned char *newContent = (unsigned char *)malloc(newContentSize);
  memcpy(newContent, content, contentSize);
  memcpy(newContent + contentSize, record, recordSize);
  setContent((const char *)newContent, newContentSize);
  free(newContent);
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
  if (recordCounter > MAX_NDEF_RECORDS) {
    return false;
  }

  // records[recordCounter] = record;

  // Update header flags
  if (recordCounter == 1) {
    // NdefMessage::content[0] = NDEF_HEADER_FLAGS_FIRST_RECORD;
    // records[recordCounter].setHeaderFlags(NDEF_HEADER_FLAGS_NEXT_RECORD);
    NdefMessage::content[0] = NDEF_HEADER_FLAGS_FIRST_RECORD;
    record.setHeaderFlags(NDEF_HEADER_FLAGS_NEXT_RECORD);
  } else if (recordCounter == 2) {
    NdefMessage::content[contentSize - record.getContentSize()] = 0x11;
    record.setHeaderFlags(NDEF_HEADER_FLAGS_NEXT_RECORD);
  }

  NdefMessage::newContent = new unsigned char[contentSize + record.getContentSize()];
  memcpy(newContent, content, contentSize);
  memcpy(newContent + contentSize, record.getContent(), record.getContentSize());
  setContent((const char *)newContent, contentSize + record.getContentSize());

  recordCounter++;

  // addContent(record.getContent(), record.getContentSize());  // Doesn't work
  // setContent(record.getContent(), record.getContentSize());  // Works

  return true;
}

bool NdefMessage::addTextRecord(String text) {
  NdefRecord record;
  record.setHeaderFlags(NDEF_HEADER_FLAGS_SINGLE_RECORD);
  record.setTypeLength(NDEF_TYPE_LENGTH);
  record.setPayloadSize(text.length() + 3);
  record.setRecordType(NDEF_TEXT_RECORD_TYPE);
  record.setStatus(NDEF_STATUS);
  record.setLanguageCode(NDEF_DEFAULT_LANGUAGE_CODE);
  record.setPayload(text);

  Serial.println("Payload size: " + String(record.getPayloadSize()));
  unsigned char *payload = record.getPayload();
  Serial.println("Payload: " + String((char *)payload));

  return addRecord(record);
}
