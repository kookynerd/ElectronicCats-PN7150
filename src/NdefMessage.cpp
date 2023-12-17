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

#ifdef DEBUG3
  Serial.println("Content size: " + String(contentSize));
  Serial.println(NdefMessage::getHexRepresentation((byte *)content, (uint32_t)contentSize));
#endif
  NdefMessage::updateHeaderFlags();
#ifdef DEBUG3
  Serial.println(NdefMessage::getHexRepresentation((byte *)content, (uint32_t)contentSize));
#endif
  T4T_NDEF_EMU_SetMsg(content, contentSize);
}

void NdefMessage::updateHeaderFlags() {
  uint8_t headersPositions[recordCounter];
  uint8_t recordCounterAux = 0;
#ifdef DEBUG3
  Serial.println("Header positions:");
#endif
  for (uint8_t i = 0; i < contentSize; i++) {
    if (isHeaderByte(content[i])) {  // New record found
#ifdef DEBUG3
      Serial.println("\t" + String(i) + ": " + String(content[i], HEX) + ",");
#endif
      headersPositions[recordCounterAux] = i;
      recordCounterAux++;
    }
  }
#ifdef DEBUG3
  Serial.println();
#endif

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

bool NdefMessage::isHeaderByte(unsigned char byte) {
  return (byte == NDEF_HEADER_FLAGS_SINGLE_RECORD) || (byte == NDEF_HEADER_FLAGS_FIRST_RECORD) || (byte == NDEF_HEADER_FLAGS_NEXT_RECORD) || (byte == NDEF_HEADER_FLAGS_LAST_RECORD);
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

#ifdef DEBUG3
  Serial.println("Record size: " + String(record.getContentSize()));
#endif

  NdefMessage::newContent = new unsigned char[contentSize + record.getContentSize()];
  memcpy(newContent, content, contentSize);
  memcpy(newContent + contentSize, record.getContent(), record.getContentSize());
  setContent((const char *)newContent, contentSize + record.getContentSize());
}

void NdefMessage::addTextRecord(String text, String languageCode) {
  NdefRecord record;
  record.setHeaderFlags(NDEF_HEADER_FLAGS_SINGLE_RECORD);
  record.setTypeLength(NDEF_TYPE_LENGTH);
  record.setPayloadSize(text.length() + 3);  // 3 = status + language code length
  record.setRecordType(NDEF_TEXT_RECORD_TYPE);
  record.setStatus(NDEF_STATUS);
  record.setLanguageCode(languageCode);
  record.setPayload(text);

  addRecord(record);
}

void NdefMessage::addTextRecord(String text) {
  addTextRecord(text, NDEF_DEFAULT_LANGUAGE_CODE);
}

void NdefMessage::addUriRecord(String uri) {
  uint8_t statusLength = 1;
  NdefRecord record;
  record.setHeaderFlags(NDEF_HEADER_FLAGS_SINGLE_RECORD);
  record.setTypeLength(NDEF_TYPE_LENGTH);
  record.setRecordType(NDEF_URI_RECORD_TYPE);

  if (uri.startsWith("http://www.")) {
    record.setStatus(NDEF_URI_HTTP_WWWDOT);
    record.setPayload(uri.substring(11).c_str());
    record.setPayloadSize(uri.length() - 11 + statusLength);
  } else if (uri.startsWith("https://www.")) {
    record.setStatus(NDEF_URI_HTTPS_WWWDOT);
    record.setPayload(uri.substring(12).c_str());
    record.setPayloadSize(uri.length() - 12 + statusLength);
  } else if (uri.startsWith("http://")) {
    record.setStatus(NDEF_URI_HTTP);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("https://")) {
    record.setStatus(NDEF_URI_HTTPS);
    record.setPayload(uri.substring(8).c_str());
    record.setPayloadSize(uri.length() - 8 + statusLength);
  } else if (uri.startsWith("tel:")) {
    record.setStatus(NDEF_URI_TEL);
    record.setPayload(uri.substring(4).c_str());
    record.setPayloadSize(uri.length() - 4 + statusLength);
  } else if (uri.startsWith("mailto:")) {
    record.setStatus(NDEF_URI_MAILTO);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("ftp://anonymous:anonymous@")) {
    record.setStatus(NDEF_URI_FTP_ANONIMOUS);
    record.setPayload(uri.substring(26).c_str());
    record.setPayloadSize(uri.length() - 26 + statusLength);
  } else if (uri.startsWith("ftp://ftp.")) {
    record.setStatus(NDEF_URI_FTP_FTPDOT);
    record.setPayload(uri.substring(9).c_str());
    record.setPayloadSize(uri.length() - 9 + statusLength);
  } else if (uri.startsWith("ftps://")) {
    record.setStatus(NDEF_URI_FTPS);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("sftp://")) {
    record.setStatus(NDEF_URI_SFTP);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("smb://")) {
    record.setStatus(NDEF_URI_SMB);
    record.setPayload(uri.substring(6).c_str());
    record.setPayloadSize(uri.length() - 6 + statusLength);
  } else if (uri.startsWith("nfs://")) {
    record.setStatus(NDEF_URI_NFS);
    record.setPayload(uri.substring(6).c_str());
    record.setPayloadSize(uri.length() - 6 + statusLength);
  } else if (uri.startsWith("ftp://")) {
    record.setStatus(NDEF_URI_FTP);
    record.setPayload(uri.substring(6).c_str());
    record.setPayloadSize(uri.length() - 6 + statusLength);
  } else if (uri.startsWith("dav://")) {
    record.setStatus(NDEF_URI_DAV);
    record.setPayload(uri.substring(6).c_str());
    record.setPayloadSize(uri.length() - 6 + statusLength);
  } else if (uri.startsWith("news:")) {
    record.setStatus(NDEF_URI_NEWS);
    record.setPayload(uri.substring(5).c_str());
    record.setPayloadSize(uri.length() - 5 + statusLength);
  } else if (uri.startsWith("telnet://")) {
    record.setStatus(NDEF_URI_TELNET);
    record.setPayload(uri.substring(9).c_str());
    record.setPayloadSize(uri.length() - 9 + statusLength);
  } else if (uri.startsWith("imap:")) {
    record.setStatus(NDEF_URI_IMAP);
    record.setPayload(uri.substring(5).c_str());
    record.setPayloadSize(uri.length() - 5 + statusLength);
  } else if (uri.startsWith("rtsp://")) {
    record.setStatus(NDEF_URI_RTSP);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("urn:")) {
    record.setStatus(NDEF_URI_URN);
    record.setPayload(uri.substring(4).c_str());
    record.setPayloadSize(uri.length() - 4 + statusLength);
  } else if (uri.startsWith("pop:")) {
    record.setStatus(NDEF_URI_POP);
    record.setPayload(uri.substring(4).c_str());
    record.setPayloadSize(uri.length() - 4 + statusLength);
  } else if (uri.startsWith("sip:")) {
    record.setStatus(NDEF_URI_SIP);
    record.setPayload(uri.substring(4).c_str());
    record.setPayloadSize(uri.length() - 4 + statusLength);
  } else if (uri.startsWith("sips:")) {
    record.setStatus(NDEF_URI_SIPS);
    record.setPayload(uri.substring(5).c_str());
    record.setPayloadSize(uri.length() - 5 + statusLength);
  } else if (uri.startsWith("tftp:")) {
    record.setStatus(NDEF_URI_TFTP);
    record.setPayload(uri.substring(5).c_str());
    record.setPayloadSize(uri.length() - 5 + statusLength);
  } else if (uri.startsWith("btspp://")) {
    record.setStatus(NDEF_URI_BTSPP);
    record.setPayload(uri.substring(8).c_str());
    record.setPayloadSize(uri.length() - 8 + statusLength);
  } else if (uri.startsWith("btl2cap://")) {
    record.setStatus(NDEF_URI_BTL2CAP);
    record.setPayload(uri.substring(10).c_str());
    record.setPayloadSize(uri.length() - 10 + statusLength);
  } else if (uri.startsWith("btgoep://")) {
    record.setStatus(NDEF_URI_BTGOEP);
    record.setPayload(uri.substring(9).c_str());
    record.setPayloadSize(uri.length() - 9 + statusLength);
  } else if (uri.startsWith("tcpobex://")) {
    record.setStatus(NDEF_URI_TCPOBEX);
    record.setPayload(uri.substring(10).c_str());
    record.setPayloadSize(uri.length() - 10 + statusLength);
  } else if (uri.startsWith("irdaobex://")) {
    record.setStatus(NDEF_URI_IRDAOBEX);
    record.setPayload(uri.substring(11).c_str());
    record.setPayloadSize(uri.length() - 11 + statusLength);
  } else if (uri.startsWith("file://")) {
    record.setStatus(NDEF_URI_FILE);
    record.setPayload(uri.substring(7).c_str());
    record.setPayloadSize(uri.length() - 7 + statusLength);
  } else if (uri.startsWith("urn:epc:id:")) {
    record.setStatus(NDEF_URI_URN_EPC_ID);
    record.setPayload(uri.substring(11).c_str());
    record.setPayloadSize(uri.length() - 11 + statusLength);
  } else if (uri.startsWith("urn:epc:tag:")) {
    record.setStatus(NDEF_URI_URN_EPC_TAG);
    record.setPayload(uri.substring(12).c_str());
    record.setPayloadSize(uri.length() - 12 + statusLength);
  } else if (uri.startsWith("urn:epc:pat:")) {
    record.setStatus(NDEF_URI_URN_EPC_PAT);
    record.setPayload(uri.substring(12).c_str());
    record.setPayloadSize(uri.length() - 12 + statusLength);
  } else if (uri.startsWith("urn:epc:raw:")) {
    record.setStatus(NDEF_URI_URN_EPC_RAW);
    record.setPayload(uri.substring(12).c_str());
    record.setPayloadSize(uri.length() - 12 + statusLength);
  } else if (uri.startsWith("urn:epc:")) {
    record.setStatus(NDEF_URI_URN_EPC);
    record.setPayload(uri.substring(8).c_str());
    record.setPayloadSize(uri.length() - 8 + statusLength);
  } else if (uri.startsWith("urn:nfc:")) {
    record.setStatus(NDEF_URI_URN_NFC);
    record.setPayload(uri.substring(8).c_str());
    record.setPayloadSize(uri.length() - 8 + statusLength);
  } else {
    record.setStatus(NDEF_URI_NO_PREFIX);
    record.setPayload(uri);
    record.setPayloadSize(uri.length() + statusLength);
  }

  addRecord(record);
}
