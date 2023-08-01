#include <Electroniccats_PN7150.h>

#include "ndef_helper.h"

#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
void checkReaders();
void sendMessageCallback(unsigned char *pNdefRecord, unsigned short NdefRecordSize);

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // Creates a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
RfIntf_t RfInterface;                                            // Interface to save data for multiple tags

uint8_t mode = 2;  // modes: 1 = Reader/ Writer, 2 = Emulation, 3 = Peer to peer P2P

unsigned char STATUSOK[] = {0x90, 0x00}, Cmd[256], CmdSize;

const char uri[] = "google.com";

// const char ndefMessage[] = {0xD1,                      // MB/ME/CF/1/IL/TNF
//                              0x01,                      // Type length (1 byte)
//                              0x08,                      // Payload length
//                              'U',                       // Type -> 'T' for text, 'U' for URI
//                              0x02,                      // Status
//                              'e', 'n',                  // Language
//                              'H', 'e', 'l', 'l', 'o'};  // Message Payload

const char ndefMessage[] = {0xD1,
                            0x01,
                            sizeof(uri) + 1,
                            'U',
                            0x02,
                            // 'g', 'o', 'o', 'g', 'l', 'e', '.', 'c', 'o', 'm'};
                            uri[0], uri[1], uri[2], uri[3], uri[4], uri[5], uri[6], uri[7], uri[8], uri[9], uri[10]};

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  if (T4T_NDEF_EMU_SetMessage((unsigned char *)ndefMessage, sizeof(ndefMessage), (void *)*sendMessageCallback)) {
    Serial.println("Set message success!");
  } else {
    Serial.println("Set message failed!");
  }

  Serial.println("Initializing...");

  if (nfc.begin()) {
    Serial.println("Error initializing PN7150");
    while (1)
      ;
  }

  if (nfc.connectNCI()) {  // Wake up the board
    Serial.println("Error while setting up the mode, check connections!");
    while (1)
      ;
  }

  if (nfc.ConfigureSettings()) {
    Serial.println("The Configure Settings failed!");
    while (1)
      ;
  }

  if (nfc.ConfigMode(mode)) {
    Serial.println("The Configure Mode failed!!");
    while (1)
      ;
  }

  nfc.StartDiscovery(mode);
  Serial.print("Waiting for an NDEF device");
}

void loop() {
  checkReaders();
}

void checkReaders() {
  Serial.print(".");
  if (nfc.CardModeReceive(Cmd, &CmdSize) == 0) {  // Data in buffer?
    if ((CmdSize >= 2) && (Cmd[0] == 0x00)) {     // Expect at least two bytes
      if (Cmd[1] == 0xA4) {
        Serial.println("\nReader detected!");
        nfc.ProcessCardMode(RfInterface);
      }
      nfc.CardModeSend(STATUSOK, sizeof(STATUSOK));
      Serial.print("Waiting for an NDEF device");
    }
  }
}

void sendMessageCallback(unsigned char *pNdefRecord, unsigned short NdefRecordSize) {
  Serial.println("NDEF Record sent!");
}
