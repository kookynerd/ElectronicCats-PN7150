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

class NdefMessage {
 private:
  static unsigned char *content;
  static unsigned short contentSize;
  static void update(unsigned char *message, unsigned short messageSize);
  void getNextRecord();

 public:
  NdefMessage();
  void begin();
  static unsigned char *getContent();
  static unsigned short getContentSize();
  static void setContent(unsigned char *content, unsigned short contentSize);
  NdefRecord_t getRecord();
  bool isEmpty();
  bool isNotEmpty();
  bool hasRecord();
};

#endif