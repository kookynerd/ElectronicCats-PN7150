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
  NdefMessage::updateHeaderFlags();
  Serial.println(NdefMessage::getHexRepresentation((byte *)content, (uint32_t)contentSize));
  T4T_NDEF_EMU_SetMsg(content, contentSize);
}

void NdefMessage::updateHeaderFlags() {
  uint8_t headersPositions[recordCounter];
  uint8_t recordCounterAux = 0;
  Serial.print("Header positions:");
  for (uint8_t i = 0; i < contentSize; i++) {
    if (content[i] == NDEF_TYPE_LENGTH) {  // New record found
      Serial.print(" " + String(i - 1));
      headersPositions[recordCounterAux] = i - 1;
      recordCounterAux++;
    }
  }
  Serial.println();

  for (uint8_t i = 0; i < recordCounter; i++) {
    if (i == 0) {
      content[headersPositions[i]] = NDEF_HEADER_FLAGS_SINGLE_RECORD;
    } else if (i == 1) {
      content[headersPositions[i - 1]] = NDEF_HEADER_FLAGS_FIRST_RECORD;
      content[headersPositions[i]] = NDEF_HEADER_FLAGS_LAST_RECORD;
    } else {
      content[headersPositions[i - 1]] = NDEF_HEADER_FLAGS_NEXT_RECORD;
      content[headersPositions[i]] = NDEF_HEADER_FLAGS_LAST_RECORD;
    }
  }
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

void NdefMessage::addRecord(NdefRecord record) {
  recordCounter++;

  NdefMessage::newContent = new unsigned char[contentSize + record.getContentSize()];
  memcpy(newContent, content, contentSize);
  memcpy(newContent + contentSize, record.getContent(), record.getContentSize());
  setContent((const char *)newContent, contentSize + record.getContentSize());
}

void NdefMessage::addTextRecord(String text) {
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
