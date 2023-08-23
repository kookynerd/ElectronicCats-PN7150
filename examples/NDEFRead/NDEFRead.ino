#include "Electroniccats_PN7150.h"
#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

// Function prototypes
String getHexRepresentation(const byte *data, const uint32_t dataSize);
void displayDeviceInfo();
void displayRecordInfo(NdefRecord_t record);
void ndefCallback();

// Create a global NFC device interface object, attached to pins 11 (IRQ) and 13 (VEN) and using the default I2C address 0x28
Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);
NdefMessage message;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Detect NFC tags with PN7150");

  // Register a callback function to be called when an NDEF message is received
  nfc.setSendMsgCallback(ndefCallback);

  Serial.println("Initializing...");

  if (nfc.begin()) {
    Serial.println("Error initializing PN7150");
    while (true)
      ;
  }

  message.begin();
  nfc.setReaderWriterMode();
  Serial.print("Waiting for a Card...");
}

void loop() {
  if (nfc.isTagDetected()) {
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
    Serial.println("Restarting...");
    nfc.reset();
    Serial.print("Waiting for a Card");
  }

  Serial.print(".");
  delay(500);
}

String getHexRepresentation(const byte *data, const uint32_t dataSize) {
  String hexString;

  if (dataSize == 0) {
    hexString = "null";
  }

  for (uint32_t index = 0; index < dataSize; index++) {
    if (data[index] <= 0xF)
      hexString += "0";
    String hexValue = String(data[index] & 0xFF, HEX);
    hexValue.toUpperCase();
    hexString += hexValue;
    if ((dataSize > 1) && (index != dataSize - 1)) {
      hexString += ":";
    }
  }
  return hexString;
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

void displayRecordInfo(NdefRecord record) {
  if (record.isEmpty()) {
    return;
  }

  unsigned char save;
  String SSID;
  String bluetoothName;
  String bluetoothAddress;
  Serial.println("--- NDEF record received:");
  Serial.println("\tRecord size: " + String(record.getPayloadSize()) + " bytes");

  switch (record.getType()) {
      // case MEDIA_VCARD: {
      //   save = record.recordPayload[record.recordPayloadSize];
      //   record.recordPayload[record.recordPayloadSize] = '\0';
      //   Serial.print("vCard:");
      //   Serial.println(reinterpret_cast<const char *>(record.recordPayload));
      //   record.recordPayload[record.recordPayloadSize] = save;
      // } break;

    case WELL_KNOWN_SIMPLE_TEXT: {
      // save = record.recordPayload[record.recordPayloadSize];
      // record.recordPayload[record.recordPayloadSize] = '\0';
      Serial.println("\tWell known simple text");
      Serial.println("\tText record: " + record.getText());
      // Serial.println(reinterpret_cast<const char *>(&record.recordPayload[record.recordPayload[0] + 1]));
      // record.recordPayload[record.recordPayloadSize] = save;
    } break;

      // case WELL_KNOWN_SIMPLE_URI: {
      //   save = record.recordPayload[record.recordPayloadSize];
      //   record.recordPayload[record.recordPayloadSize] = '\0';
      //   Serial.print("URI record: ");
      //   Serial.println(reinterpret_cast<const char *>(ndef_helper_UriHead(record.recordPayload[0]), &record.recordPayload[1]));
      //   record.recordPayload[record.recordPayloadSize] = save;
      // } break;

      // case MEDIA_HANDOVER_WIFI: {
      //   unsigned char index = 0, i;

      //   Serial.println("--- Received WIFI credentials:");
      //   if ((record.recordPayload[index] == 0x10) && (record.recordPayload[index + 1] == 0x0e))
      //     index += 4;
      //   while (index < record.recordPayloadSize) {
      //     if (record.recordPayload[index] == 0x10) {
      //       if (record.recordPayload[index + 1] == 0x45) {
      //         Serial.print("- SSID = ");
      //         Serial.println(reinterpret_cast<const char *>(&record.recordPayload[index + 4 + 0]));
      //         Serial.println(SSID);
      //       } else if (record.recordPayload[index + 1] == 0x03) {
      //         Serial.print("- Authenticate Type = ");
      //         Serial.println(ndef_helper_WifiAuth(record.recordPayload[index + 5]));
      //       } else if (record.recordPayload[index + 1] == 0x0f) {
      //         Serial.print("- Encryption Type = ");
      //         Serial.println(ndef_helper_WifiEnc(record.recordPayload[index + 5]));
      //       } else if (record.recordPayload[index + 1] == 0x27) {
      //         Serial.print("- Network key = ");
      //         Serial.println(reinterpret_cast<const char *>(&record.recordPayload[index + 4]));
      //         Serial.print("- Network key = ");
      //         Serial.println(getHexRepresentation(&record.recordPayload[index + 4], record.recordPayload[index + 3]));
      //       }
      //       index += 4 + record.recordPayload[index + 3];
      //     } else
      //       continue;
      //   }
      // } break;

      // case WELL_KNOWN_HANDOVER_SELECT:
      //   Serial.print("Handover select version ");
      //   Serial.print(record.recordPayload[0] >> 4);
      //   Serial.println(record.recordPayload[0] & 0xF);
      //   break;

      // case WELL_KNOWN_HANDOVER_REQUEST:
      //   Serial.print("Handover request version ");
      //   Serial.print(record.recordPayload[0] >> 4);
      //   Serial.println(record.recordPayload[0] & 0xF);
      //   break;

      // case MEDIA_HANDOVER_BT:
      //   Serial.print("- Payload size: ");
      //   Serial.println(record.recordPayloadSize);
      //   Serial.print("- Bluetooth Handover payload = ");
      //   Serial.println(getHexRepresentation(record.recordPayload, record.recordPayloadSize));
      //   Serial.print("- Bluetooth name: '");
      //   bluetoothName = "";
      //   for (unsigned int i = 10; i < record.recordPayloadSize; i++) {
      //     if (record.recordPayload[i] == 0x04) {
      //       break;
      //     }
      //     bluetoothName += (char)record.recordPayload[i];
      //   }
      //   Serial.println(bluetoothName + "'");

      //   Serial.print("- Bluetooth address: '");
      //   bluetoothAddress = "";
      //   for (unsigned int i = 7; i >= 2; i--) {
      //     bluetoothAddress += getHexRepresentation(&record.recordPayload[i], 1);
      //     if (i > 2) {
      //       bluetoothAddress += ":";
      //     }
      //   }
      //   Serial.println(bluetoothAddress + "'");
      //   break;

      // case MEDIA_HANDOVER_BLE:
      //   Serial.print("- BLE Handover payload = ");
      //   Serial.println(getHexRepresentation(record.recordPayload, record.recordPayloadSize));
      //   break;

      // case MEDIA_HANDOVER_BLE_SECURE:
      //   Serial.print("- BLE secure Handover payload = ");
      //   Serial.println(getHexRepresentation(record.recordPayload, record.recordPayloadSize));
      //   break;

    default:
      Serial.println("\tUnsupported NDEF record, cannot parse");
      break;
  }

  Serial.println("");
}

/// @brief Callback function called when an NDEF message is received
void ndefCallback() {
  NdefRecord record;
  Serial.println("Processing Callback...");

  // message is automatically updated when a new NDEF message is received
  // only if we call message.begin() in setup()
  if (message.isEmpty()) {
    Serial.println("--- Provisioned buffer size too small or NDEF message empty");
    return;
  }

  do {
    record.create(message.getRecord());  // Get a new record every time we call getRecord()
    displayRecordInfo(record);
  } while (record.isNotEmpty());
}
