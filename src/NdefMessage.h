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
};

#endif