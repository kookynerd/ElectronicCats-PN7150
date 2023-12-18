/**
 * Example to send NDEF messages
 *
 * Note: If you know how to create custom NDEF messages, you can use the NDEFSendRawMessage example, otherwise, use this example.
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

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  message.addTextRecord("Hello");                          // English by default
  message.addTextRecord("world", "en");                    // English explicitly, the library only supports two letter language codes (ISO 639-1) by now
  message.addTextRecord("Hola mundo!", "es");              // Spanish explicitly, check a language code table at https://www.science.co.il/language/Locale-codes.php
  message.addTextRecord("Bonjour le monde!", "fr");        // French explicitly
  message.addUriRecord("google.com");                      // No prefix explicitly
  message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  // message.addTextRecord("Hello, lorem:50 ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris. Maecenas congue ligula ac quam viverra nec consectetur ante hendrerit. Donec et mollis dolor.", "en");
  // message.addTextRecord("lorem:60 ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris. Maecenas congue ligula ac quam viverra nec consectetur ante hendrerit. Donec et mollis dolor.", "en");

  // message.addTextRecord("lorem:70 ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus. Sed sit amet ipsum mauris. Maecenas congue ligula ac quam viverra nec consectetur ante hendrerit. Donec et mollis dolor. Donec et mollis dolor.", "en");

  message.addTextRecord("Hello");                          // English by default
  message.addTextRecord("world", "en");                    // English explicitly, the library only supports two letter language codes (ISO 639-1) by now
  message.addTextRecord("Hola mundo!", "es");              // Spanish explicitly, check a language code table at https://www.science.co.il/language/Locale-codes.php
  message.addTextRecord("Bonjour le monde!", "fr");        // French explicitly
  message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  message.addUriRecord("h.com");
  message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  message.addUriRecord("h.co");
  // message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  // message.addTextRecord("Hello world, this is a test, this is a test");                          // English by default
  // message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table
  // message.addTextRecord("Hello");                          // English by default

  // message.addUriRecord("http://test2.com");
  // message.addUriRecord("https://test3.com");
  // message.addUriRecord("tel:test4");
  // message.addUriRecord("http://www.test2.com");
  // message.addUriRecord("https://www.test2.com");
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
