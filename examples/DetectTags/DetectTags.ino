/**
 * Example detect tags and show their unique ID
 * Authors:
 *        Salvador Mendoza - @Netxing - salmg.net
 *        For Electronic Cats - electroniccats.com
 *
 *  March 2020
 *
 * This code is beerware; if you see me (or any other collaborator
 * member) at the local, and you've found our code helpful,
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 */

#include "Electroniccats_PN7150.h"
#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // creates a global NFC device interface object, attached to pins 7 (IRQ) and 8 (VEN) and using the default I2C address 0x28
RfIntf_t RfInterface;                                            // Intarface to save data for multiple tags

void PrintBuf(const byte* data, const uint32_t numBytes) {  // Print hex data buffer in format
  uint32_t szPos;
  for (szPos = 0; szPos < numBytes; szPos++) {
    Serial.print(F("0x"));
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      Serial.print(F("0"));
    Serial.print(data[szPos] & 0xff, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1)) {
      Serial.print(F(" "));
    }
  }
  Serial.println();
}

String getHexRepresentation(const byte* data, const uint32_t numBytes) {
  String hexString;
  for (uint32_t szPos = 0; szPos < numBytes; szPos++) {
    hexString += "0x";
    if (data[szPos] <= 0xF)
      hexString += "0";
    hexString += String(data[szPos] & 0xFF, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1)) {
      hexString += " ";
    }
  }
  return hexString;
}

void displayCardInfo(RfIntf_t RfIntf) {  // Funtion in charge to show the card/s in te field
  char tmp[16];
  int index = 0;
  static String sensRes;
  static String sensResLen;
  static String nfcID;

  while (1) {
    switch (nfc.remoteDevice.getProtocol()) {  // Indetify card protocol
      case nfc.protocol.T1T:
      case nfc.protocol.T2T:
      case nfc.protocol.T3T:
      case nfc.protocol.ISODEP:
        Serial.print(" - POLL MODE: Remote activated tag type: ");
        Serial.println(nfc.remoteDevice.getProtocol());
        break;
      case nfc.protocol.ISO15693:
        Serial.println(" - POLL MODE: Remote ISO15693 card activated");
        break;
      case nfc.protocol.MIFARE:
        Serial.println(" - POLL MODE: Remote MIFARE card activated");
        break;
      default:
        Serial.println(" - POLL MODE: Undetermined target");
        return;
    }

    switch (nfc.remoteDevice.getModeTech()) {  // Indetify card technology
      case (nfc.modeTech.POLL | nfc.tech.PASSIVE_NFCA):
        Serial.print("\tSENS_RES = ");
        Serial.println(getHexRepresentation(nfc.remoteDevice.getAPPSensRes(), nfc.remoteDevice.getAPPSensResLen()));
        Serial.println("\tSENS_RES Length = " + String(nfc.remoteDevice.getAPPSensResLen()) + " bytes");

        Serial.print("\tNFCID = ");
        nfcID = getHexRepresentation(nfc.remoteDevice.getAPPID(), nfc.remoteDevice.getAPPIDLen());
        Serial.println(nfcID);
        Serial.println("\tNFCID Length = " + String(nfc.remoteDevice.getAPPIDLen()) + " bytes");

        // if (RfIntf.Info.NFC_APP.SelResLen != 0) {
        if (nfc.remoteDevice.getAPPSelResLen() != 0) {
          Serial.print("\tSEL_RES = ");
          Serial.println(getHexRepresentation(nfc.remoteDevice.getAPPSelRes(), nfc.remoteDevice.getAPPSelResLen()));
          Serial.println("\tSEL_RES Length; = " + String(nfc.remoteDevice.getAPPSelResLen()) + " bytes");
        }
        break;

      case (MODE_POLL | TECH_PASSIVE_NFCB):
        if (RfIntf.Info.NFC_BPP.SensResLen != 0) {
          Serial.print("\tSENS_RES = ");
          PrintBuf(RfIntf.Info.NFC_BPP.SensRes, RfIntf.Info.NFC_BPP.SensResLen);
        }
        break;

      case (MODE_POLL | TECH_PASSIVE_NFCF):
        Serial.print("\tBitrate = ");
        Serial.println((RfIntf.Info.NFC_FPP.BitRate == 1) ? "212" : "424");

        if (RfIntf.Info.NFC_FPP.SensResLen != 0) {
          Serial.print("\tSENS_RES = ");
          PrintBuf(RfIntf.Info.NFC_FPP.SensRes, RfIntf.Info.NFC_FPP.SensResLen);
        }
        break;

      case (MODE_POLL | TECH_PASSIVE_15693):
        Serial.print("\tID = ");
        PrintBuf(RfIntf.Info.NFC_VPP.ID, sizeof(RfIntf.Info.NFC_VPP.ID));

        Serial.print("\ntAFI = ");
        Serial.println(RfIntf.Info.NFC_VPP.AFI);

        Serial.print("\tDSFID = ");
        Serial.println(RfIntf.Info.NFC_VPP.DSFID, HEX);
        break;

      default:
        break;
    }
    if (RfIntf.MoreTags) {  // It will try to identify more NFC cards if they are the same technology
      if (nfc.readerActivateNext(&RfIntf) == NFC_ERROR) break;
    } else
      break;
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Detect NFC tags with PN7150");

  Serial.println("Initializing...");
  if (nfc.connectNCI()) {  // Wake up the board
    Serial.println("Error while setting up the mode, check connections!");
    while (1)
      ;
  }

  if (nfc.configureSettings()) {
    Serial.println("The Configure Settings is failed!");
    while (1)
      ;
  }

  // Read/Write mode as default
  if (nfc.configMode()) {  // Set up the configuration mode
    Serial.println("The Configure Mode is failed!!");
    while (1)
      ;
  }
  nfc.startDiscovery();  // NCI Discovery mode
  Serial.println("Waiting for an Card ...");
}

void loop() {
  // TODO: invert the logic to make it more readable
  if (!nfc.waitForDiscoveryNotification()) {  // Waiting to detect cards
    displayCardInfo(RfInterface);

    // It can detect multiple cards at the same time if they use the same protocol
    if (nfc.remoteDevice.hasMoreTags()) {
      nfc.activateNextTagDiscovery();
      Serial.println("Multiple cards are detected!");
    }
    Serial.println("Remove the Card");
    nfc.waitForTagRemoval();
    Serial.println("Card removed!");
  }

  Serial.println("Restarting...");
  nfc.reset();
  Serial.println("Waiting for a Card...");
  delay(500);
}
