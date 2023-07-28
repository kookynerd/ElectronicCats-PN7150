#include <Electroniccats_PN7150.h>

#include "ndef_helper.h"

#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
void checkReaders();
void p2pMode();
void resetMode();
void ndefPush_Cb(unsigned char *pNdefRecord, unsigned short NdefRecordSize);
void displayDeviceInfo();

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // Creates a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
RfIntf_t RfInterface;                                            // Interface to save data for multiple tags

uint8_t mode = 2;  // modes: 1 = Reader/ Writer, 2 = Emulation, 3 = Peer to peer P2P

unsigned char STATUSOK[] = {0x90, 0x00}, Cmd[256], CmdSize;

const char NDEF_MESSAGE[] = {0xD1,                      // MB/ME/CF/1/IL/TNF
                             0x01,                      // Type length (1 byte for "T")
                             0x08,                      // Payload length
                             'U',                       // Type -> 'T' for text, 'U' for URI
                             0x02,                      // Status
                             'e', 'n',                  // Language
                             'H', 'e', 'l', 'l', 'o'};  // Message Payload

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Send NDEF Message with PN7150");

  if (T4T_NDEF_EMU_SetMessage((unsigned char *)NDEF_MESSAGE, sizeof(NDEF_MESSAGE), (void *)*ndefPush_Cb)) {
    Serial.println("Set message ok\r\n");
  } else {
    Serial.println("Set message error\r\n");
  }

  if (RW_NDEF_SetMessage((unsigned char *)NDEF_MESSAGE, sizeof(NDEF_MESSAGE), (void *)*ndefPush_Cb)) {
    Serial.println("Set message ok\r\n");
  } else {
    Serial.println("Set message error\r\n");
  }

  // if (P2P_NDEF_SetMessage((unsigned char *)NDEF_MESSAGE, sizeof(NDEF_MESSAGE), (void *)*ndefPush_Cb)) {
  // 	Serial.println("Set message ok\r\n");
  // } else {
  // 	Serial.println("Set message error\r\n");
  // }

  // P2P_NDEF_RegisterPullCallback((void *)*ndefPush_Cb);

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
  Serial.println("Waiting for an NDEF device...");
}

void loop() {
  checkReaders();
  // p2pMode();
}

void checkReaders() {
  Serial.print(".");
  if (nfc.CardModeReceive(Cmd, &CmdSize) == 0) {  // Data in buffer?
    if ((CmdSize >= 2) && (Cmd[0] == 0x00)) {     // Expect at least two bytes
      if (Cmd[1] == 0xA4) {
        Serial.println("Reader detected!");
        // nfc.ReadNdef(RfInterface);
        nfc.ProcessCardMode(RfInterface);
        displayDeviceInfo();
      }
      nfc.CardModeSend(STATUSOK, sizeof(STATUSOK));
    }
  }
}

void p2pMode() {
  Serial.print(".");

  if (!nfc.WaitForDiscoveryNotification(&RfInterface, 1000)) {  // Waiting to detect
    Serial.println();
    displayDeviceInfo();

    if (RfInterface.Interface == INTF_NFCDEP || RfInterface.Interface == INTF_ISODEP) {
      if ((RfInterface.ModeTech & MODE_LISTEN) == MODE_LISTEN) {
        Serial.println(" - P2P TARGET MODE: Activated from remote Initiator");
      } else {
        Serial.println(" - P2P INITIATOR MODE: Remote Target activated");
      }

      /* Process with SNEP for NDEF exchange */
      nfc.ProcessP2pMode(RfInterface);
      Serial.println("Peer lost!");
    } else {
      Serial.println("Wrong discovery!");
    }

    // Wait for removal
    nfc.ProcessReaderMode(RfInterface, PRESENCE_CHECK);
    Serial.println("Device removed!");
    resetMode();
  }
  delay(500);
}

void resetMode() {  // Reset the configuration mode after each reading
  Serial.println("\nRe-initializing...");
  // nfc.StopDiscovery();
  nfc.ConfigMode(mode);
  nfc.StartDiscovery(mode);
}

void ndefPush_Cb(unsigned char *pNdefRecord, unsigned short NdefRecordSize) {
  Serial.println("--- NDEF Record sent");
}

void displayDeviceInfo() {
  Serial.print("RfInterface: ");
  switch (RfInterface.Interface) {
    case INTF_ISODEP:
      Serial.println("ISO-DEP");
      break;
    case INTF_NFCDEP:
      Serial.println("NFC-DEP");
      break;
    case INTF_TAGCMD:
      Serial.println("TAG");
      break;
    case INTF_FRAME:
      Serial.println("FRAME");
      break;
    case INTF_UNDETERMINED:
      Serial.println("UNDETERMINED");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  Serial.print("Mode: ");
  switch (RfInterface.ModeTech) {
    case MODE_POLL:
      Serial.println("POLL");
      break;
    case MODE_LISTEN:
      Serial.println("LISTEN");
      break;
    case MODE_MASK:
      Serial.println("MASK");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  Serial.println();
}