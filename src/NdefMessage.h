/**
 * Library to manage the NDEF message
 * Authors:
 *        Francisco Torres - Electronic Cats - electroniccats.com
 *
 * December 2023
 *
 * This code is beerware; if you see me (or any other collaborator
 * member) at the local, and you've found our code helpful,
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 */

#ifndef NdefMessage_H
#define NdefMessage_H

#include <Arduino.h>

#include "NdefRecord.h"
#include "RW_NDEF.h"
#include "T4T_NDEF_emu.h"
#include "ndef_helper.h"

#define NDEF_HEADER_FLAGS_SINGLE_RECORD 0xD1
#define NDEF_HEADER_FLAGS_FIRST_RECORD 0x91
#define NDEF_HEADER_FLAGS_NEXT_RECORD 0x11
#define NDEF_HEADER_FLAGS_LAST_RECORD 0x51
#define NDEF_TYPE_LENGTH 0x01
#define NDEF_TEXT_RECORD_TYPE 'T'
#define NDEF_URI_RECORD_TYPE 'U'
#define NDEF_STATUS 0x02
#define NDEF_DEFAULT_LANGUAGE_CODE "en"

#define NDEF_URI_NO_PREFIX 0x00
#define NDEF_URI_HTTP_WWWDOT 0x01
#define NDEF_URI_HTTPS_WWWDOT 0x02
#define NDEF_URI_HTTP 0x03
#define NDEF_URI_HTTPS 0x04
#define NDEF_URI_TEL 0x05
#define NDEF_URI_MAILTO 0x06
#define NDEF_URI_FTP_ANONIMOUS 0x07
#define NDEF_URI_FTP_FTPDOT 0x08
#define NDEF_URI_FTPS 0x09
#define NDEF_URI_SFTP 0x0A
#define NDEF_URI_SMB 0x0B
#define NDEF_URI_NFS 0x0C
#define NDEF_URI_FTP 0x0D
#define NDEF_URI_DAV 0x0E
#define NDEF_URI_NEWS 0x0F
#define NDEF_URI_TELNET 0x10
#define NDEF_URI_IMAP 0x11
#define NDEF_URI_RTSP 0x12
#define NDEF_URI_URN 0x13
#define NDEF_URI_POP 0x14
#define NDEF_URI_SIP 0x15
#define NDEF_URI_SIPS 0x16
#define NDEF_URI_TFTP 0x17
#define NDEF_URI_BTSPP 0x18
#define NDEF_URI_BTL2CAP 0x19
#define NDEF_URI_BTGOEP 0x1A
#define NDEF_URI_TCPOBEX 0x1B
#define NDEF_URI_IRDAOBEX 0x1C
#define NDEF_URI_FILE 0x1D
#define NDEF_URI_URN_EPC_ID 0x1E
#define NDEF_URI_URN_EPC_TAG 0x1F
#define NDEF_URI_URN_EPC_PAT 0x20
#define NDEF_URI_URN_EPC_RAW 0x21
#define NDEF_URI_URN_EPC 0x22
#define NDEF_URI_URN_NFC 0x23

class NdefMessage {
 private:
  static uint8_t recordCounter;
  static unsigned char *content;
  static unsigned short contentSize;
  static unsigned char *newContent;
  static unsigned short newContentSize;
  static void update(unsigned char *message, unsigned short messageSize);
  void getNextRecord();
  static String getHexRepresentation(const byte *data, const uint32_t dataSize);
  static String newString;
  void addRecord(NdefRecord record);
  static void updateHeaderFlags();
  static bool isHeaderByte(unsigned char byte);

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
  void addTextRecord(String text);
  void addTextRecord(String text, String languageCode);
  void addUriRecord(String uri);
  void addMimeMediaRecord(String mimeType, String payload);
};

#endif