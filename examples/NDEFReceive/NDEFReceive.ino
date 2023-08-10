#include "Electroniccats_PN7150.h"
#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);  // creates a global NFC device interface object, attached to pins 7 (IRQ) and 8 (VEN) and using the default I2C address 0x28
RfIntf_t RfIntf;                                            // Intarface to save data for multiple tags

const char ndefMessage[] = {0xD1,                      // MB/ME/CF/1/IL/TNF
                            0x01,                      // Type length (1 byte)
                            0x08,                      // Payload length
                            'T',                       // Type -> 'T' for text, 'U' for URI
                            0x02,                      // Status
                            'e', 'n',                  // Language
                            'H', 'e', 'l', 'l', 'o'};  // Message Payload

void PrintBuf(const byte *data, const uint32_t numBytes) {  // Print hex data buffer in format
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

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Detect NFC tags with PN7150");

  RW_NDEF_RegisterPullCallback((void *)*ndefPull_Cb);

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

  nfc.setReaderWriterMode();

  if (nfc.configMode()) {  // Set up the configuration mode
    Serial.println("The Configure Mode is failed!!");
    while (1)
      ;
  }
  nfc.startDiscovery();  // NCI Discovery mode
  Serial.println("Waiting for a Card...");
}

void loop() {
  if (!nfc.waitForDiscoveryNotification(&RfIntf)) {  // Waiting to detect cards
    displayDeviceInfo();
    switch (nfc.remoteDevice.protocol) {
      case PROT_T1T:
      case PROT_T2T:
      case PROT_T3T:
      case PROT_ISODEP:
        // nfc.processReaderMode(RfIntf, READ_NDEF);  // TODO: update it to use the new API
        nfc.readNdefMessage();
        break;

      case PROT_ISO15693:
        break;

      case PROT_MIFARE:
        // nfc.processReaderMode(RfIntf, READ_NDEF);
        nfc.readNdefMessage();
        break;

      default:
        break;
    }

    // It can detect multiple cards at the same time if they use the same protocol
    if (nfc.remoteDevice.moreTags) {
      nfc.readerActivateNext(&RfIntf);
    }
    // Wait for card removal
    Serial.println("Remove the Card");
    nfc.processReaderMode(RfIntf, PRESENCE_CHECK);
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
  switch (nfc.remoteDevice.interface) {
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

  Serial.print("Protocol: ");
  switch (nfc.remoteDevice.protocol) {
    case PROT_UNDETERMINED:
      Serial.println("UNDETERMINED");
      break;
    case PROT_T1T:
      Serial.println("T1T");
      break;
    case PROT_T2T:
      Serial.println("T2T");
      break;
    case PROT_T3T:
      Serial.println("T3T");
      break;
    case PROT_ISODEP:
      Serial.println("ISO-DEP");
      break;
    case PROT_NFCDEP:
      Serial.println("NFC-DEP");
      break;
    case PROT_ISO15693:
      Serial.println("ISO15693");
      break;
    case PROT_MIFARE:
      Serial.println("MIFARE");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }

  Serial.print("Mode: ");
  switch (nfc.remoteDevice.modeTech) {
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
}

void ndefPull_Cb(unsigned char *pNdefMessage, unsigned short NdefMessageSize) {
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
        // Serial.println(NdefRecord.recordPayload);
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = save;
      } break;

      case WELL_KNOWN_SIMPLE_TEXT: {
        save = NdefRecord.recordPayload[NdefRecord.recordPayloadSize];
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = '\0';
        Serial.print("Text record: ");
        // Serial.println(&NdefRecord.recordPayload[NdefRecord.recordPayload[0]+1]);
        Serial.println(reinterpret_cast<const char*>(&NdefRecord.recordPayload[NdefRecord.recordPayload[0] + 1]));
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = save;
      } break;

      case WELL_KNOWN_SIMPLE_URI: {
        save = NdefRecord.recordPayload[NdefRecord.recordPayloadSize];
        NdefRecord.recordPayload[NdefRecord.recordPayloadSize] = '\0';
        Serial.print("URI record: ");
        // Serial.println(ndef_helper_UriHead(NdefRecord.recordPayload[0]), &NdefRecord.recordPayload[1]);
        Serial.println(reinterpret_cast<const char*>(ndef_helper_UriHead(NdefRecord.recordPayload[0]), &NdefRecord.recordPayload[1]));
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
              for (i = 0; i < NdefRecord.recordPayload[index + 3]; i++)
                Serial.print(NdefRecord.recordPayload[index + 4 + i]);
              Serial.println("");
            } else if (NdefRecord.recordPayload[index + 1] == 0x03) {
              Serial.print("- Authenticate Type = ");
              Serial.println(ndef_helper_WifiAuth(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x0f) {
              Serial.print("- Encryption Type = ");
              Serial.println(ndef_helper_WifiEnc(NdefRecord.recordPayload[index + 5]));
            } else if (NdefRecord.recordPayload[index + 1] == 0x27) {
              Serial.print("- Network key = ");
              for (i = 0; i < NdefRecord.recordPayload[index + 3]; i++)
                Serial.print("#");
              Serial.println("");
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
