/**
 * Example to send NDEF messages
 *
 * Note: If you know how to create custom NDEF messages, you can use the NDEFSendRawMessage example, otherwise, use this example.
 *
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

#include <Electroniccats_PN7150.h>

#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
void messageSentCallback();

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // Creates a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
NdefMessage message;

const char payload[] = {
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
    0x09,  // Length of the Network Name attribute
    // 't', 'e', 's', 't', 's',
    'B', 'o', 'm', 'b', 'e', 'r', 'c', 'a', 't',
    0x10,
    0x03,
    0x00,
    0x02,  // Length of the Authentication Type attribute
    0x00,
    0x01,  // Value for Authentication type
    0x10,
    0x0F,
    0x00,
    0x02,  // Length of the Encryption Type attribute
    0x00,
    0x01,  // Value for Encryption type
    0x10,
    0x27,
    0x00,
    0x04,  // Length of the Network Key attribute
    'p', 'a', 's', 's'};

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  // message.addTextRecord("Hello");                          // English by default
  // message.addTextRecord("world", "en");                    // English explicitly, the library only supports two letter language codes (ISO 639-1) by now
  // message.addTextRecord("Hola mundo!", "es");              // Spanish explicitly, check a language code table at https://www.science.co.il/language/Locale-codes.php
  // message.addTextRecord("Bonjour le monde!", "fr");        // French explicitly
  // message.addUriRecord("google.com");                      // No prefix explicitly
  // message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  message.addMimeMediaRecord("application/vnd.wfa.wsc", payload, sizeof(payload));
  // message.addUriRecord("tel:1234567890");
  // message.addUriRecord("mailto:deimoshallgmail.com");  // TODO: check @ problem
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
