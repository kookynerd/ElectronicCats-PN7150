#include "Electroniccats_PN7150.h"
#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

#define PRINTF Serial.print

void print_buf(const char *prefix, const uint8_t *data, size_t size) {
  PRINTF(prefix);
  for (size_t loop = 0; loop < size; loop++) {
    PRINTF(data[loop], HEX);
    PRINTF(" ");
  }
  PRINTF("\n");
}

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

void customNdefCallback() {
  Serial.println("Processing Callback...");
}

void ndefCallback(unsigned char *pNdefMessage, unsigned short NdefMessageSize) {
  unsigned char *pNdefRecord = pNdefMessage;
  NdefRecord_t NdefRecord;
  unsigned char save;

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
              // for (i = 0; i < NdefRecord.recordPayload[index + 3]; i++) {
              //   // Serial.print(NdefRecord.recordPayload[index + 4 + i]);
              //   Serial.println(reinterpret_cast<const char *>(&NdefRecord.recordPayload[index + 4 + i]));
              // }
            } else if (NdefRecord.recordPayload[index + 1] == 0x03) {
              Serial.print("- Authenticate Type = ");
              Serial.println(ndef_helper_WifiAuth(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x0f) {
              Serial.print("- Encryption Type = ");
              Serial.println(ndef_helper_WifiEnc(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x27) {
              Serial.print("- Network key = ");
              Serial.println(reinterpret_cast<const char *>(&NdefRecord.recordPayload[index + 4]));
              // for (i = 0; i < NdefRecord.recordPayload[index + 3]; i++) {
              //   Serial.print(reinterpret_cast<const char *>(&NdefRecord.recordPayload[index + 4 + i]));
              // }
              // Serial.println("");
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
        Serial.print("BT Handover payload = ");
        // Serial.print(NdefRecord.recordPayload);
        // Serial.println(NdefRecord.recordPayloadSize);
        print_buf("", NdefRecord.recordPayload, NdefRecord.recordPayloadSize);
        // Serial.print(reinterpret_cast<const char *>(&NdefRecord.recordPayload[0]));
        unsigned char i;
        for (i = 0; i < NdefRecord.recordPayloadSize; i++) {
          Serial.print(reinterpret_cast<const char *>(&NdefRecord.recordPayload[i]));
        }
        Serial.println("");
        break;

      case MEDIA_HANDOVER_BLE:
        Serial.print("BLE Handover payload = ");
        // Serial.print(NdefRecord.recordPayload);
        // Serial.println(NdefRecord.recordPayloadSize);
        break;

      case MEDIA_HANDOVER_BLE_SECURE:
        Serial.print("BLE secure Handover payload = ");
        // Serial.println(NdefRecord.recordPayload, NdefRecord.recordPayloadSize);
        break;

      default:
        Serial.println("Unsupported NDEF record, cannot parse");
        break;
    }
    pNdefRecord = GetNextRecord(pNdefRecord);
  }

  Serial.println("");
}
