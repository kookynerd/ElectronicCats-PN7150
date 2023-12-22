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

  // message.addTextRecord("Hello");                          // English by default
  // message.addTextRecord("world", "en");                    // English explicitly, the library only supports two letter language codes (ISO 639-1) by now
  // message.addTextRecord("Hola mundo!", "es");              // Spanish explicitly, check a language code table at https://www.science.co.il/language/Locale-codes.php
  // message.addTextRecord("Bonjour le monde!", "fr");        // French explicitly
  // message.addUriRecord("google.com");                      // No prefix explicitly
  // message.addUriRecord("https://www.electroniccats.com");  // https://www. prefix explicitly, the library can handle all the prefixes listed at TODO: add link to prefixes table

  message.addUriRecord("google.com");
  message.addUriRecord("http://www.test1.com");
  message.addUriRecord("https://www.test2.com");
  message.addUriRecord("tel:123456789");
  message.addUriRecord("mailto:deimoshallgmail.com");  // TODO: check @ problem
  message.addUriRecord("ftp://anonymous:anonymous@test5");
  message.addUriRecord("ftp://ftp.test6");
  // message.addUriRecord("ftps://test7");
  // message.addUriRecord("sftp://test8");
  // message.addUriRecord("smb://test9");
  // message.addUriRecord("nfs://test10");
  // message.addUriRecord("ftp://test11");
  // message.addUriRecord("dav://test12");
  // message.addUriRecord("news:test13");
  // message.addUriRecord("telnet://test14");
  // message.addUriRecord("imap://test15");
  // message.addUriRecord("rtsp://test16");
  message.addUriRecord("urn:test17");
  // message.addUriRecord("pop:test18");
  // message.addUriRecord("sip:test19");
  // message.addUriRecord("sips:test20");
  // message.addUriRecord("tftp://test21");
  // message.addUriRecord("btspp://test22");
  // message.addUriRecord("btl2cap://test23");
  // message.addUriRecord("btgoep://test24");
  // message.addUriRecord("tcpobex://test25");
  // message.addUriRecord("irdaobex://test26");
  // message.addUriRecord("file://test27");

  // message.addUriRecord("urn:epc:id:sgtin:0614141.107346.1");

  message.addUriRecord("urn:epc:id:test28");
  message.addUriRecord("urn:epc:tag:test29");
  message.addUriRecord("urn:epc:pat:sgtin96:3.0614141.812345.6789");
  message.addUriRecord("urn:epc:raw:test30");
  message.addUriRecord("urn:epc:test31");
  message.addUriRecord("urn:nfc:test32");
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
