/**
 * Example to create a custom NDEF message and send it using the PN7150.
 *
 * Note: If you know how to create custom NDEF messages, you can use this example, otherwise, use the NDEFSendMessage example.
 *
 * Authors:
 *        Salvador Mendoza - @Netxing - salmg.net
 *        Francisco Torres - Electronic Cats - electroniccats.com
 *
 * December 2023
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

// Three records, "Hello", "world" and Uri "https://www.electroniccats.com"
// const char ndefMessage[] = {0x91,                                                                                       // MB/ME/CF/1/IL/TNF
//                             0x01,                                                                                       // Type length (1 byte)
//                             0x08,                                                                                       // Payload length
//                             'T',                                                                                        // Type -> 'T' for text, 'U' for URI
//                             0x02,                                                                                       // Status
//                             'e', 'n',                                                                                   // Language
//                             'H', 'e', 'l', 'l', 'o',                                                                    // Message Payload
//                             0x11,                                                                                       // MB/ME/CF/1/IL/TNF
//                             0x01,                                                                                       // Type length (1 byte)
//                             0x08,                                                                                       // Payload length
//                             'T',                                                                                        // Type -> 'T' for text, 'U' for URI
//                             0x02,                                                                                       // Status
//                             'e', 'n',                                                                                   // Language
//                             'w', 'o', 'r', 'l', 'd',                                                                    // Message Payload
//                             0x51,                                                                                       // MB/ME/CF/1/IL/TNF
//                             0x01,                                                                                       // Type length (1 byte)
//                             0x13,                                                                                       // Payload length
//                             'U',                                                                                        // Type -> 'T' for text, 'U' for URI
//                             0x02,                                                                                       // Status
//                             'e', 'l', 'e', 'c', 't', 'r', 'o', 'n', 'i', 'c', 'c', 'a', 't', 's', '.', 'c', 'o', 'm'};  // Message Payload

// One uri record "https://www.electroniccats.com"
// const char ndefMessage[] = {0xD1,
//                             0x01,
//                             0x13,
//                             'U',
//                             0x02,
//                             'e', 'l', 'e', 'c', 't', 'r', 'o', 'n', 'i', 'c', 'c', 'a', 't', 's', '.', 'c', 'o', 'm'};

// const char ndefMessage[] = {0xD1, 0x02, 0x1A, 'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', '/', 'v', 'n', 'd', '.', 'w', 'f', 'a', '.', 'w', 's', 'c', 0x01, 0x00, 0x00, 0x00, 0x04, 't', 'e', 's', 't', 0x00, 0x00, 0x00, 0x01, 0x00, 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
// One media record of type "application/vnd.wfa.wsc", with network name = test, Authentication type = OPEN, Encription type = WEP, Password = password
// D2 17 29 61 70 70 6C 69 63 61 74 69 6F 6E 2F 76 6E 64 2E 77 66 61 2E 77 73 63 10 0E 00 36 10 26 00 01 01 10 45 00 04 74 65 73 74 10 03 00 02 00 01 10 0F 00 02 00 01 10 27 00 08 70 61 73 73 77 6F 72 64
// const char ndefMessage[] = {0xD2,
//                             0x17,
//                             0x29,
//                             0x61,
//                             0x70,
//                             0x70,
//                             0x6C,
//                             0x69,
//                             0x63,
//                             0x61,
//                             0x74,
//                             0x69,
//                             0x6F,
//                             0x6E,
//                             0x2F,
//                             0x76,
//                             0x6E,
//                             0x64,
//                             0x2E,
//                             0x77,
//                             0x66,
//                             0x61,
//                             0x2E,
//                             0x77,
//                             0x73,
//                             0x63,
//                             0x10,
//                             0x0E,
//                             0x00,
//                             0x36,
//                             0x10,
//                             0x26,
//                             0x00,
//                             0x01,
//                             0x01,
//                             0x10,
//                             0x45,
//                             0x00,
//                             0x04,
//                             0x74,
//                             0x65,
//                             0x73,
//                             0x74,
//                             0x10,
//                             0x03,
//                             0x00,
//                             0x02,
//                             0x00,
//                             0x01,
//                             0x10,
//                             0x0F,
//                             0x00,
//                             0x02,
//                             0x00,
//                             0x01,
//                             0x10,
//                             0x27,
//                             0x00,
//                             0x08,
//                             0x70,
//                             0x61,
//                             0x73,
//                             0x73,
//                             0x77,
//                             0x6F,
//                             0x72,
//                             0x64};
// D2:17:29:61:61:70:70:6C:69:63:61:74:69:6F:6E:2F:76:6E:64:2E:77:66:61:2E:77:73:63:10:0E:00:36:10:26:00:01:01:10:45:00:04:74:65:73:74:10:03:00:02:00:01:10:0F:00:02:00:01:10:27:00:08:70:61:73:73:77:6F:72:64
const char ndefMessage[] = {
    0xD2,
    0X17,
    0x29,
    'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', '/', 'v', 'n', 'd', '.', 'w', 'f', 'a', '.', 'w', 's', 'c',
    0x10,
    0x0E,
    0x00,
    0x36,
    0x10,
    0x26,
    0x00,
    0x01,
    0x01,
    0x10,
    0x45,
    0x00,
    0x04,
    't', 'e', 's', 't',
    0x10,
    0x03,
    0x00,
    0x02,
    0x00,
    0x01,
    0x10,
    0x0F,
    0x00,
    0x02,
    0x00,
    0x01,
    0x10,
    0x27,
    0x00,
    0x08,
    'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  message.setContent(ndefMessage, sizeof(ndefMessage));
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
