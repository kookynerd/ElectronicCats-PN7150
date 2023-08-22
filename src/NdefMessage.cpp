#include "NdefMessage.h"

unsigned char *NdefMessage::content;
unsigned short NdefMessage::contentSize;

NdefMessage::NdefMessage() {
  content = NULL;
  contentSize = 0;
}

void NdefMessage::begin() {
  registerUpdateNdefMessageCallback(NdefMessage::update);
}

void NdefMessage::update(unsigned char *message, unsigned short messageSize) {
  Serial.println("Updating message...");
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

void NdefMessage::setContent(unsigned char *content, unsigned short contentSize) {
  content = content;
  contentSize = contentSize;
}