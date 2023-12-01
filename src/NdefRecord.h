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

#ifndef NdefRecord_H
#define NdefRecord_H

#include <Arduino.h>

#include "ndef_helper.h"

class NdefRecord {
 private:
  NdefRecord_t content;
  NdefRecordType_e _type;
  unsigned char *payload;
  unsigned short payloadSize;
  String getHexRepresentation(const byte *data, const uint32_t dataSize);
  String newString;

 public:
  NdefRecord();
  RecordType type;
  void create(NdefRecord_t record);
  bool isEmpty();
  bool isNotEmpty();
  NdefRecordType_e getType();
  unsigned char *getPayload();
  unsigned short getPayloadSize();
  String getText();
  String getBluetoothName();
  String getBluetoothAddress();
  String getWiFiSSID();
  String getWiFiAuthenticationType();
  String getWiFiEncryptionType();
  String getWiFiPassword();
  String getVCardContent();
  String getUri();
};

#endif