/**
 * Example to send NDEF messages
 * Authors:
 *        Salvador Mendoza - @Netxing - salmg.net
 *        Francisco Torres - Electronic Cats - electroniccats.com
 *
 *  August 2023
 *
 * This code is beerware; if you see me (or any other collaborator
 * member) at the local, and you've found our code helpful,
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 */

#include <Electroniccats_PN7150.h>

#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
void messageSentCallback();

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // Creates a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
NdefMessage message;

// One record, "Hello"
// D1 01 08 54 02 65 6E 48 65 6C 6C 6F
// const char ndefMessage[] = {0xD1,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x08,                      // Payload length
//                             'T',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'n',                  // Language
//                             'H', 'e', 'l', 'l', 'o'};  // Message Payload

// Two records, "Hello" and "world"
// 91 01 08 54 02 65 6E 48 65 6C 6C 6F 51 01 08 54 02 65 6E 77 6F 72 6C 64
// const char ndefMessage[] = {0x91,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x08,                      // Payload length
//                             'T',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'n',                  // Language
//                             'H', 'e', 'l', 'l', 'o',   // Message Payload
//                             0x51,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x08,                      // Payload length
//                             'T',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'n',                  // Language
//                             'w', 'o', 'r', 'l', 'd'};  // Message Payload

// Three records, "Hello", "world" and Uri "https://www.electroniccats.com"
// 91 01 08 54 02 65 6E 48 65 6C 6C 6F 11 01 08 54 02 65 6E 77 6F 72 6C 64 51 01 13 55 02 65 6C 65 63 74 72 6F 6E 69 63 63 61 74 73 2E 63 6F 72
// const char ndefMessage[] = {0x91,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x08,                      // Payload length
//                             'T',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'n',                  // Language
//                             'H', 'e', 'l', 'l', 'o',   // Message Payload
//                             0x11,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x08,                      // Payload length
//                             'T',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'n',                  // Language
//                             'w', 'o', 'r', 'l', 'd',   // Message Payload
//                             0x51,                      // MB/ME/CF/1/IL/TNF
//                             0x01,                      // Type length (1 byte)
//                             0x13,                      // Payload length
//                             'U',                       // Type -> 'T' for text, 'U' for URI
//                             0x02,                      // Status
//                             'e', 'l', 'e', 'c', 't', 'r', 'o', 'n', 'i', 'c', 'c', 'a', 't', 's', '.', 'c', 'o', 'm'};  // Message Payload

uint8_t headerContent = 0xD1;
uint8_t typeLength = 0x01;
uint8_t payloadLength = 0x08;
uint8_t type = 'T';
uint8_t status = 0x02;
uint8_t language[2] = {'e', 'n'};
uint8_t payload[5] = {'H', 'e', 'l', 'l', 'o'};

const char ndefMessage[] = {headerContent, typeLength, payloadLength, type, status, language[0], language[1], payload[0], payload[1], payload[2], payload[3], payload[4]};

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  // message.setContent(ndefMessage, sizeof(ndefMessage));
  // message.addTextRecord("Hello");
  message.addTextRecord("world");
  nfc.setSendMsgCallback(messageSentCallback);

  Serial.println("Initializing...");

  if (nfc.begin()) {
    Serial.println("Error initializing PN7150");
    while (true)
      ;
  }

  // Needed to detect readers
  nfc.setEmulationMode();
  Serial.print("Waiting for an NDEF device");
}

void loop() {
  Serial.print(".");

  if (nfc.isReaderDetected()) {
    Serial.println("\nReader detected!");
    Serial.println("Sending NDEF message...");
    nfc.sendMessage();
    Serial.print("\nWaiting for an NDEF device");
  }
}

void messageSentCallback() {
  Serial.println("NDEF message sent!");
  // Do something...
}
