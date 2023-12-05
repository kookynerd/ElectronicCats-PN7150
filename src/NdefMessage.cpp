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

NdefMessage::NdefMessage() {
  content = NULL;
  contentSize = 0;
  recordCounter = 0;
}

void NdefMessage::begin() {
  registerUpdateNdefMessageCallback(NdefMessage::update);
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
  NdefMessage::recordCounter = MAX_NDEF_RECORDS;

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
