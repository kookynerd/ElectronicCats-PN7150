#include "Electroniccats_PN7150.h"
#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
void displayDeviceInfo();
void customNdefCallback();
void ndefCallback(unsigned char *pNdefMessage, unsigned short NdefMessageSize);

// Create a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Detect NFC tags with PN7150");

  // Register a callback function to be called when an NDEF message is received
  RW_NDEF_RegisterPullCallback((void *)*ndefCallback);
  nfc.setSendMsgCallback(customNdefCallback);

  Serial.println("Initializing...");
  if (nfc.connectNCI()) {  // Wake up the board
    Serial.println("Error while setting up the mode, check connections!");
    while (true)
      ;
  }

  if (nfc.configureSettings()) {
    Serial.println("The Configure Settings is failed!");
    while (true)
      ;
  }

  nfc.setReaderWriterMode();

  if (nfc.configMode()) {  // Set up the configuration mode
    Serial.println("The Configure Mode is failed!!");
    while (true)
      ;
  }
  nfc.startDiscovery();  // NCI Discovery mode
  Serial.println("Waiting for a Card...");
}

void loop() {
  if (!nfc.waitForDiscoveryNotification()) {  // Wait for a card
    displayDeviceInfo();
    switch (nfc.remoteDevice.getProtocol()) {
      // Read NDEF message from NFC Forum Type 1, 2, 3, 4, 5 tags
      case nfc.protocol.T1T:
      case nfc.protocol.T2T:
      case nfc.protocol.T3T:
      case nfc.protocol.ISODEP:
      case nfc.protocol.MIFARE:
        nfc.readNdefMessage();
        break;
      case nfc.protocol.ISO15693:
      default:
        break;
    }

    // It can detect multiple cards at the same time if they are the same technology
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

void displayDeviceInfo() {
  Serial.println();
  Serial.print("Interface: ");
  switch (nfc.remoteDevice.getInterface()) {
    case nfc.interface.ISODEP:
      Serial.println("ISO-DEP");
      break;
    case nfc.interface.NFCDEP:
      Serial.println("NFC-DEP");
      break;
    case nfc.interface.TAGCMD:
      Serial.println("TAG");
      break;
    case nfc.interface.FRAME:
      Serial.println("FRAME");
      break;
    case nfc.interface.UNDETERMINED:
      Serial.println("UNDETERMINED");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  Serial.print("Protocol: ");
  switch (nfc.remoteDevice.getProtocol()) {
    case nfc.protocol.UNDETERMINED:
      Serial.println("UNDETERMINED");
      break;
    case nfc.protocol.T1T:
      Serial.println("T1T");
      break;
    case nfc.protocol.T2T:
      Serial.println("T2T");
      break;
    case nfc.protocol.T3T:
      Serial.println("T3T");
      break;
    case nfc.protocol.ISODEP:
      Serial.println("ISO-DEP");
      break;
    case nfc.protocol.NFCDEP:
      Serial.println("NFC-DEP");
      break;
    case nfc.protocol.ISO15693:
      Serial.println("ISO15693");
      break;
    case nfc.protocol.MIFARE:
      Serial.println("MIFARE");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  Serial.print("Technology: ");
  switch (nfc.remoteDevice.getModeTech()) {
    case nfc.modeTech.POLL | nfc.tech.PASSIVE_NFCA:
      Serial.println("PASSIVE NFC A");
      break;
    case nfc.modeTech.POLL | nfc.tech.PASSIVE_NFCB:
      Serial.println("PASSIVE NFC B");
      break;
    case nfc.modeTech.POLL | nfc.tech.PASSIVE_NFCF:
      Serial.println("PASSIVE NFC F");
      break;
    case nfc.modeTech.POLL | nfc.tech.PASSIVE_15693:
      Serial.println("PASSIVE 15693");
      break;
  }
}

String getHexRepresentation(const byte *data, const uint32_t numBytes) {
  String hexString;

  if (numBytes == 0) {
    hexString = "null";
  }

  for (uint32_t szPos = 0; szPos < numBytes; szPos++) {
    // hexString += "0x";
    if (data[szPos] <= 0xF)
      hexString += "0";
    // hexString += String(data[szPos] & 0xFF, HEX);
    String hexValue = String(data[szPos] & 0xFF, HEX);
    hexValue.toUpperCase(); // Convierte a mayúsculas
    hexString += hexValue;
    if ((numBytes > 1) && (szPos != numBytes - 1)) {
      hexString += ":";
    }
  }
  return hexString;
}

void customNdefCallback() {
  Serial.println("Processing Callback...");
}

void ndefCallback(unsigned char *pNdefMessage, unsigned short NdefMessageSize) {
  unsigned char *pNdefRecord = pNdefMessage;
  NdefRecord_t NdefRecord;
  unsigned char save;
  String SSID;
  String bluetoothName;
  String bluetoothAddress;

  Serial.println("Processing Callback");

  if (pNdefMessage == NULL) {
    Serial.println("--- Provisioned buffer size too small or NDEF message empty");
    return;
  }

  while (pNdefRecord != NULL) {
    Serial.println("--- NDEF record received:");

    NdefRecord = DetectNdefRecordType(pNdefRecord);

    switch (NdefRecord.recordType) {
      case MEDIA_VCARD: {
        save = NdefRecord.recordPayload[NdefRecord.recordPayloadSize];
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = '\0';
        Serial.print("vCard:");
        Serial.println(reinterpret_cast<const char *>(NdefRecord.recordPayload));
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = save;
      } break;

      case WELL_KNOWN_SIMPLE_TEXT: {
        save = NdefRecord.recordPayload[NdefRecord.recordPayloadSize];
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = '\0';
        Serial.print("Text record: ");
        Serial.println(reinterpret_cast<const char *>(&NdefRecord.recordPayload[NdefRecord.recordPayload[0] + 1]));
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = save;
      } break;

      case WELL_KNOWN_SIMPLE_URI: {
        save = NdefRecord.recordPayload[NdefRecord.recordPayloadSize];
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = '\0';
        Serial.print("URI record: ");
        Serial.println(reinterpret_cast<const char *>(ndef_helper_UriHead(NdefRecord.recordPayload[0]), &NdefRecord.recordPayload[1]));
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = save;
      } break;

      case MEDIA_HANDOVER_WIFI: {
        unsigned char index = 0, i;

        Serial.println("--- Received WIFI credentials:");
        if ((NdefRecord.recordPayload[index] == 0x10) && (NdefRecord.recordPayload[index + 1] == 0x0e))
          index += 4;
        while (index < NdefRecord.recordPayloadSize) {
          if (NdefRecord.recordPayload[index] == 0x10) {
            if (NdefRecord.recordPayload[index + 1] == 0x45) {
              Serial.print("- SSID = ");
              Serial.println(reinterpret_cast<const char *>(&NdefRecord.recordPayload[index + 4 + 0]));
              Serial.println(SSID);
            } else if (NdefRecord.recordPayload[index + 1] == 0x03) {
              Serial.print("- Authenticate Type = ");
              Serial.println(ndef_helper_WifiAuth(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x0f) {
              Serial.print("- Encryption Type = ");
              Serial.println(ndef_helper_WifiEnc(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x27) {
              Serial.print("- Network key = ");
              Serial.println(reinterpret_cast<const char *>(&NdefRecord.recordPayload[index + 4]));
              Serial.print("- Network key = ");
              Serial.println(getHexRepresentation(&NdefRecord.recordPayload[index + 4], NdefRecord.recordPayload[index + 3]));
            }
            index += 4 + NdefRecord.recordPayload[index + 3];
          } else
            continue;
        }
      } break;

      case WELL_KNOWN_HANDOVER_SELECT:
        Serial.print("Handover select version ");
        Serial.print(NdefRecord.recordPayload[0] >> 4);
        Serial.println(NdefRecord.recordPayload[0] & 0xF);
        break;

      case WELL_KNOWN_HANDOVER_REQUEST:
        Serial.print("Handover request version ");
        Serial.print(NdefRecord.recordPayload[0] >> 4);
        Serial.println(NdefRecord.recordPayload[0] & 0xF);
        break;

      case MEDIA_HANDOVER_BT:
        Serial.print("- Payload size: ");
        Serial.println(NdefRecord.recordPayloadSize);
        Serial.print("- Bluetooth Handover payload = ");
        Serial.println(getHexRepresentation(NdefRecord.recordPayload, NdefRecord.recordPayloadSize));
        Serial.print("- Bluetooth name: '");
        bluetoothName = "";
        for (unsigned int i = 10; i < NdefRecord.recordPayloadSize; i++) {
          if (NdefRecord.recordPayload[i] == 0x04) {
            break;
          }
          bluetoothName += (char)NdefRecord.recordPayload[i];
        }
        Serial.println(bluetoothName + "'");

        Serial.print("- Bluetooth address: '");
        bluetoothAddress = "";
        for (unsigned int i = 7; i >= 2; i--) {
          bluetoothAddress += getHexRepresentation(&NdefRecord.recordPayload[i], 1);
          if (i > 2) {
            bluetoothAddress += ":";
          }
        }
        Serial.println(bluetoothAddress + "'");
        break;

      case MEDIA_HANDOVER_BLE:
        Serial.print("- BLE Handover payload = ");
        Serial.println(getHexRepresentation(NdefRecord.recordPayload, NdefRecord.recordPayloadSize));
        break;

      case MEDIA_HANDOVER_BLE_SECURE:
        Serial.print("- BLE secure Handover payload = ");
        Serial.println(getHexRepresentation(NdefRecord.recordPayload, NdefRecord.recordPayloadSize));
        break;

      default:
        Serial.println("Unsupported NDEF record, cannot parse");
        break;
    }
    pNdefRecord = GetNextRecord(pNdefRecord);
  }

  Serial.println("");
}
