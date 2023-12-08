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

#ifndef NdefMessage_H
#define NdefMessage_H

#include <Arduino.h>

#include "RW_NDEF.h"
#include "ndef_helper.h"
#include "NdefRecord.h"
#include "T4T_NDEF_emu.h"

#define MAX_NDEF_RECORDS 4

class NdefMessage : NdefRecord {
 private:
  NdefRecord records[MAX_NDEF_RECORDS];
  static uint8_t recordCounter;
  static unsigned char *content;
  static unsigned short contentSize;
  static void update(unsigned char *message, unsigned short messageSize);
  void getNextRecord();
  static String getHexRepresentation(const byte *data, const uint32_t dataSize);
  static String newString;

 public:
  NdefMessage();
  void begin();
  static unsigned char *getContent();
  static unsigned short getContentSize();
  static void setContent(const char *content, unsigned short contentSize);
  NdefRecord_t getRecord();
  bool isEmpty();
  bool isNotEmpty();
  bool hasRecord();
  bool addRecord(NdefRecord record);
  bool addTextRecord(String text);
  bool addTextRecord(String text, String languageCode);
  bool addUriRecord(String uri);
};

#endif