# Electroniccats_PN7150 API Reference

The `Electroniccats_PN7150` class enables Arduino library for I2C access to the PN7150 RFID/Near Field Communication chip.

## Class: `Electroniccats_PN7150`

Include and instantiate the Electroniccats_PN7150 class. Creates a global NFC device interface object, attached to pins 7 (IRQ) and 8 (VEN) and using the default I2C address 0x28

```c
#include <Electroniccats_PN7150.h>

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR);

```

- `uint8_t PN7150_IRQ`: IRQ pin for data interrupt.
- `uint8_t PN7150_VEN`: "Reset" or "Enable" pin for device.
- `uint8_t PN7150_ADDR`: Hexadecimal address for device, default `0x28` .

### Example

```c

#include "Electroniccats_PN7150.h"
#define PN7150_IRQ   (8)
#define PN7150_VEN   (7)
#define PN7150_ADDR  (0x28)

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR); // Creates a global NFC device interface object, attached to pins 7 (IRQ) and 8 (VEN) and using the default I2C address 0x28

void setup(){ 
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Detect NFC readers with PN7150");
  uint8_t statusNFC = setupNFC();
  if (!statusNFC) 
    Serial.println("Set up is ok");
  else
    Serial.println("Error while setting up mode, check connections!");
}

int setupNFC(){
  Serial.println("Initializing...");
  int setupOK = nfc.connectNCI();                     // Wake up the board
  if (!setupOK){
    setupOK = nfc.configMode();                       // Set up the configuration mode
    if (!setupOK) setupOK = nfc.startDiscovery();     // NCI Discovery mode
  }
  return setupOK;
}
```

## Electroniccats_PN7150 Methods

### Method: `getFirmwareVersion`

Get the firmware version of the NXP-NCI controller.

```cpp
int getFirmwareVersion();
```

#### Example

```cpp
int version = nfc.getFirmwareVersion();
```

### Method: `connectNCI`

Initialize the connection with the NXP-NCI controller and updates the firmware version that can be obtained with the `getFirmwareVersion` method.

```cppp
uint8_t connectNCI();
```

Returns `0` if the connection is established correctly, otherwise returns `1`.

#### Example

```cpp
uint8_t statusNFC = nfc.connectNCI();

if (!statusNFC) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `configMode`

Configure the device mode. Reader/Writer as default.

```cpp
uint8_t configMode();
```

Returns `0` if the mode is configured correctly, otherwise returns `1`.

#### Example

```cpp
uint8_t status = nfc.configMode();

if (!status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `setReaderWriterMode`

Configure the device mode to Reader/Writer.

```cpp
bool setReaderWriterMode();
```

Returns `true` if the mode is configured correctly, otherwise returns `false`.

#### Example

```cpp
bool status = nfc.setReaderWriterMode();

if (status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `setEmulationMode`

Configure the device mode to Card Emulation.

```cpp
bool setEmulationMode();
```

Returns `true` if the mode is configured correctly, otherwise returns `false`.

#### Example

```cpp
bool status = nfc.setEmulationMode();

if (status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `setP2PMode`

Configure the device mode to Peer to Peer.

```cpp
bool setP2PMode();
```

Returns `true` if the mode is configured correctly, otherwise returns `false`.

#### Example

```cpp
bool status = nfc.setP2PMode();

if (status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `getMode`

Get the current device mode.

```cpp
int getMode();
```

#### Example

```cpp
switch (nfc.getMode()) {
  case nfc.mode.READER_WRITER:
    Serial.println("Reader/Writer mode");
    break;
  case nfc.mode.EMULATION:
    Serial.println("Card Emulation mode");
    break;
  case nfc.mode.P2P:
    Serial.println("Peer to Peer mode");
    break;
  default:
    Serial.println("Unknown mode");
    break;
}
```

### Method: `configureSettings`

Configure some aspects of the NFC controller, such as the hardware configuration, RF (radio frequency) configuration, and other settings.

```cpp
bool configureSettings();
```

A custom NFC UID can be configured by passing the UID and its length as parameters.

```cpp
bool configureSettings(uint8_t *nfcuid, uint8_t uidlen);
```

Returns `0` if the parameters are configured correctly, otherwise returns `1`.

#### Example 1

```cpp
bool status = nfc.configureSettings();

if (!status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

#### Example 2

```cpp
uint8_t nfcuid[] = {0x08, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t uidlen = 6;

bool status = nfc.configureSettings(nfcuid, uidlen);

if (!status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Method: `startDiscovery`

Start the discovery mode for the device.

```cpp
uint8_t startDiscovery();
```

Returns `0` if the mode is configured correctly, otherwise returns `1`.

#### Example

```cpp
uint8_t status = nfc.startDiscovery();

if (!status) {
  Serial.println("Set up is ok");
} else {
  Serial.println("Error while setting up mode, check connections!");
}
```

### Methods: `stopDiscovery`

Stop the discovery process of the device.

```cpp
bool stopDiscovery();
```

Returns `0`. Never returns `1`.

#### Example

```cpp
nfc.stopDiscovery();
```

### Method: `isTagDetected`

Returns `true` if a tag is detected, otherwise returns `false`. The timeout is set to 500ms by default and can be changed by passing a parameter.

```cpp
bool isTagDetected(uint16_t tout = 500);
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.println("Tag detected!");
  // Do something
}
```

### Method: `cardModeSend`

Send a data packet in card mode.

```cpp
bool cardModeSend(unsigned char *pData, unsigned char DataSize);
```

### Method: `cardModeReceive`

Receive a data packet from a card.

```cpp
bool cardModeReceive(unsigned char *pData, unsigned char *pDataSize);
```

### Method: `handleCardEmulation`

Resets the card emulation state and processes the card mode for card emulation.

```cpp
void handleCardEmulation();
```

### Method: `waitForTagRemoval`

Waits for the tag to be removed.

```cpp
void waitForTagRemoval();
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.println("Remove the Card");
  nfc.waitForTagRemoval();
  Serial.println("Card removed!");
}
```

### Method: `readerTagCmd`

Sends a command to the reader.

```cpp
bool readerTagCmd(unsigned char *pCommand, unsigned char CommandSize, unsigned char *pAnswer, unsigned char *pAnswerSize);
```

### Method: `readerReActivate`

Reactivates a target after it has been deactivated.

```cpp
bool readerReActivate();
```

### Method: `activateNextTagDiscovery`

Activates the next tag discovered.

```cpp
bool activateNextTagDiscovery();
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  // It can detect multiple cards at the same time if they use the same protocol
  if (nfc.remoteDevice.hasMoreTags()) {
    Serial.println("Multiple cards are detected!");
    nfc.activateNextTagDiscovery();
  }
}
```

### Method: `readNdefMessage`

Reads the NDEF message from the tag.

```cpp
void readNdefMessage();
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  nfc.readNdefMessage();
}
```

### Method: `writeNdefMessage`

Writes the NDEF message to the tag.

```cpp
void writeNdefMessage();
```

### Method: `nciFactoryTestPrbs`

Performs a factory test for the NCI controller.

```cpp
bool nciFactoryTestPrbs(NxpNci_TechType_t type, NxpNci_Bitrate_t bitrate);
```

### Method: `nciFactoryTestRfOn`

Performs a factory test for the NCI controller.

```cpp
bool nciFactoryTestRfOn();
```

### Method: `reset`

Stops the discovery process, configures the device mode if it is necessary and starts the discovery process again.

```cpp
bool reset();
```

#### Example

```cpp
nfc.reset();
```

### Method: `setReadMsgCallback`

Registers a callback function to be called when an NDEF message is received.

```cpp
void setReadMsgCallback(CustomCallback_t function);
```

#### Example

```cpp
void messageReceived() {
  Serial.println("Message received!");
}

void setup() {
  nfc.setReadMsgCallback(messageReceived);
}
```

### Method: `isReaderDetected`

Returns `true` if a reader is detected, otherwise returns `false`.

```cpp
bool isReaderDetected();
```

#### Example

```cpp
if (nfc.isReaderDetected()) {
  Serial.println("Reader detected!");
  // Do something
}
```

### Method: `closeCommunication`

Send a command to the reader to close the communication.

```cpp
void closeCommunication();
```

#### Example

```cpp
if (nfc.isReaderDetected()) {
  Serial.println("Reader detected!");
  nfc.handleCardEmulation();
  nfc.closeCommunication();
}
```

### Method: `sendMessage`

Send an NDEF message to the reader.

```cpp
void sendMessage();
```

#### Example

```cpp
if (nfc.isReaderDetected()) {
  Serial.println("Reader detected!");
  Serial.println("Sending NDEF message...");
  nfc.sendMessage();
}
```

## Class Interface

### Constant `UNDETERMINED`

Constant for the undetermined interface.

```cpp
UNDETERMINED = 0x0
```

#### Example

```cpp
nfc.interface.UNDETERMINED;
```

### Constant `FRAME`

Constant for the frame interface.

```cpp
FRAME = 0x1
```

#### Example

```cpp
nfc.interface.FRAME;
```

### Constant `ISODEP`

Constant for the ISO-DEP interface.

```cpp
ISODEP = 0x2
```

#### Example

```cpp
nfc.interface.ISODEP;
```

### Constant `NFCDEP`

Constant for the NFC-DEP interface.

```cpp
NFCDEP = 0x3
```

#### Example

```cpp
nfc.interface.NFCDEP;
```

### Constant `TAGCMD`

Constant for the tag interface.

```cpp
TAGCMD = 0x80
```

#### Example

```cpp
nfc.interface.TAGCMD;
```

## Class Mode

### Constant `READER_WRITER`

Constant for the Reader/Writer mode.

```cpp
READER_WRITER = 1
```

#### Example

```cpp
nfc.mode.READER_WRITER;
```

### Constant `EMULATION`

Constant for the Card Emulation mode.

```cpp
EMULATION = 2
```

#### Example

```cpp
nfc.mode.EMULATION;
```

### Constant `P2P`

Constant for the Peer to Peer mode.

```cpp
P2P = 3
```

#### Example

```cpp
nfc.mode.P2P;
```

## Class ModeTech

### Constant `POLL`

Constant for the Poll mode.

```cpp
POLL = 0x00
```

#### Example

```cpp
nfc.modeTech.POLL;
```

### Constant `LISTEN`

Constant for the Listen mode.

```cpp
LISTEN = 0x80
```

#### Example

```cpp
nfc.modeTech.LISTEN;
```

### Constant `MASK`

Constant for the Mask mode.

```cpp
MASK = 0xF0
```

#### Example

```cpp
nfc.modeTech.MASK;
```

## Class Protocol

### Constant `UNDETERMINED`

Constant for the undetermined protocol.

```cpp
UNDETERMINED = 0x0
```

#### Example

```cpp
nfc.protocol.UNDETERMINED;
```

### Constant `T1T`

Constant for the T1T protocol.

```cpp
T1T = 0x1
```

#### Example

```cpp
nfc.protocol.T1T;
```

### Constant `T2T`

Constant for the T2T protocol.

```cpp
T2T = 0x2
```

#### Example

```cpp
nfc.protocol.T2T;
```

### Constant `T3T`

Constant for the T3T protocol.

```cpp
T3T = 0x3
```

#### Example

```cpp
nfc.protocol.T3T;
```

### Constant `ISODEP`

Constant for the ISO-DEP protocol.

```cpp
ISODEP = 0x4
```

#### Example

```cpp
nfc.protocol.ISODEP;
```

### Constant `NFCDEP`

Constant for the NFC-DEP protocol.

```cpp
NFCDEP = 0x5
```

#### Example

```cpp
nfc.protocol.NFCDEP;
```

### Constant `ISO15693`

Constant for the ISO15693 protocol.

```cpp
ISO15693 = 0x6
```

#### Example

```cpp
nfc.protocol.ISO15693;
```

### Constant `MIFARE`

Constant for the MIFARE protocol.

```cpp
MIFARE = 0x80
```

#### Example

```cpp
nfc.protocol.MIFARE;
```

## Class Tech

### Constant `PASSIVE_NFCA`

Constant for the Passive NFC-A technology.

```cpp
PASSIVE_NFCA = 0
```

#### Example

```cpp
nfc.tech.PASSIVE_NFCA;
```

### Constant `PASSIVE_NFCB`

Constant for the Passive NFC-B technology.

```cpp
PASSIVE_NFCB = 1
```

#### Example

```cpp
nfc.tech.PASSIVE_NFCB;
```

### Constant `PASSIVE_NFCF`

Constant for the Passive NFC-F technology.

```cpp
PASSIVE_NFCF = 2
```

#### Example

```cpp
nfc.tech.PASSIVE_NFCF;
```

### Constant `ACTIVE_NFCA`

Constant for the Active NFC-A technology.

```cpp
ACTIVE_NFCA = 3
```

#### Example

```cpp
nfc.tech.ACTIVE_NFCA;
```

### Constant `ACTIVE_NFCF`

Constant for the Active NFC-F technology.

```cpp
ACTIVE_NFCF = 5
```

#### Example

```cpp
nfc.tech.ACTIVE_NFCF;
```

### Constant `PASSIVE_15693`

Constant for the Passive 15693 technology.

```cpp
PASSIVE_15693 = 6
```

#### Example

```cpp
nfc.tech.PASSIVE_15693;
```

### Constant `PASSIVE_NFCV`

Constant for the Passive NFC-V technology (ISO 15693).

```cpp
PASSIVE_NFCV = 6
```

#### Example

```cpp
nfc.tech.PASSIVE_NFCV;
```

## Class RemoteDevice

A `RemoteDevice` object represents a remote NFC device such as a tag or a reader.

### Getters for device properties

### Method: `getInterface`

Get the interface of the device.

```cpp
unsigned char getInterface() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
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
}
```

### Method: `getProtocol`

Get the protocol of the device.

```cpp
unsigned char getProtocol() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("Protocol: ");
  switch (nfc.remoteDevice.getProtocol()) {
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
    case nfc.protocol.UNDETERMINED:
      Serial.println("UNDETERMINED");
      break;
    default:
      Serial.println("UNKNOWN");
      break;
  }
}
```

### Method: `getModeTech`

Get the mode tech of the device.

```cpp
unsigned char getModeTech() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("Technology: ");
  switch (nfc.remoteDevice.getModeTech()) {
    case nfc.tech.PASSIVE_NFCA:
      Serial.println("PASSIVE NFC A");
      break;
    case nfc.tech.PASSIVE_NFCB:
      Serial.println("PASSIVE NFC B");
      break;
    case nfc.tech.PASSIVE_NFCF:
      Serial.println("PASSIVE NFC F");
      break;
    case nfc.tech.PASSIVE_15693:
      Serial.println("PASSIVE 15693");
      break;
  }
}
```

### Method: `hasMoreTags`

Returns `true` if there are more tags to be discovered, otherwise returns `false`.

```cpp
bool hasMoreTags() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  if (nfc.remoteDevice.hasMoreTags()) {
    Serial.println("Multiple cards are detected!");
    nfc.activateNextTagDiscovery();
  }
}
```

### Getters for device information properties

### Method: `getSensResLen`

Get the SENS RES length of the device.

```cpp
unsigned char getSensResLen() const;
```

### Method: `getSensRes`

Get the SENS RES (ATQA) of the device. Only available for tags that use passive communication and the NFC-A, NFC-B or NFC-F technologies, otherwise returns `NULL`.

```cpp
const unsigned char* getSensRes() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("ATQA: ");
  for (int i = 0; i < nfc.remoteDevice.getSensResLen(); i++) {
    Serial.print(nfc.remoteDevice.getSensRes()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

### Method: `getSelResLen`

Get the SEL RES length of the device.

```cpp
unsigned char getSelResLen() const;
```

### Method: `getSelRes`

Get the SEL RES (SAK) of the device. Only available for tags that use passive communication and the NFC-A technology, otherwise returns `NULL`.

```cpp
const unsigned char* getSelRes() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("SAK: ");
  for (int i = 0; i < nfc.remoteDevice.getSelResLen(); i++) {
    Serial.print(nfc.remoteDevice.getSelRes()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

### Method: `getNFCIDLen`

Get the NFCID length of the device.

```cpp
unsigned char getNFCIDLen() const;
```

### Method: `getNFCID`

Get the NFCID of the device. Only available for tags that use passive communication and the NFC-A technology, otherwise returns `NULL`.

```cpp
const unsigned char* getNFCID() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("NFCID: ");
  for (int i = 0; i < nfc.remoteDevice.getNFCIDLen(); i++) {
    Serial.print(nfc.remoteDevice.getNFCID()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

### Method: `getRatsLen`

Get the RATS length of the device.

```cpp
unsigned char getRatsLen() const;
```

### Method: `getRats`

Get the RATS of the device. Only available for tags that use passive communication and the NFC-A technology, otherwise returns `NULL`.

```cpp
const unsigned char* getRats() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("RATS: ");
  for (int i = 0; i < nfc.remoteDevice.getRatsLen(); i++) {
    Serial.print(nfc.remoteDevice.getRats()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

### Method: `getAttribResLen`

Get the ATTRIB RES length of the device.

```cpp
unsigned char getAttribResLen() const;
```

### Method: `getAttribRes`

Get the ATTRIB RES of the device. Only available for tags that use passive communication and the NFC-B technology, otherwise returns `NULL`.

```cpp
const unsigned char* getAttribRes() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("ATTRIB RES: ");
  for (int i = 0; i < nfc.remoteDevice.getAttribResLen(); i++) {
    Serial.print(nfc.remoteDevice.getAttribRes()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

### Method: `getBitRate`

Get the bit rate of the device. Only available for tags that use passive communication and the NFC-F technology, otherwise returns `NULL`.

```cpp
unsigned char getBitRate() const;
```

#### Example

```cpp
Serial.print("Bitrate = ");
Serial.println((nfc.remoteDevice.getBitRate() == 1) ? "212" : "424");
```

### Method: `getAFI`

Get the AFI of the device. Only available for tags that use passive communication and the NFC-V technology, otherwise returns `NULL`.

```cpp
unsigned char getAFI() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("AFI = ");
  Serial.println(nfc.remoteDevice.getAFI());
}
```

### Method: `getDSFID`

Get the DSF ID of the device. Only available for tags that use passive communication and the NFC-V technology, otherwise returns `NULL`.

```cpp
unsigned char getDSFID() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("DSF ID = ");
  Serial.println(nfc.remoteDevice.getDSFID(), HEX);
}
```

### Method: `getID`

Get the ID of the device. Only available for tags that use passive communication and the NFC-V technology, otherwise returns `NULL`.

```cpp
const unsigned char* getID() const;
```

#### Example

```cpp
if (nfc.isTagDetected()) {
  Serial.print("ID: ");
  for (int i = 0; i < sizeof(nfc.remoteDevice.getID()); i++) {
    Serial.print(nfc.remoteDevice.getID()[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
```

## Class NdefMessage

A `NdefMessage` object represents an NDEF message. An NDEF message is a container for one or more NDEF records.

### Method: `begin`

Registers a callback function to be called when an NDEF message is received wich updates the message content.

```cpp
void begin();
```

#### Example

```cpp
NdefMessage message;

void setup() {
  message.begin();
}
```

### Method: `getContentSize`

Get the content size of the message.

```cpp
static unsigned short getContentSize();
```

### Method: `getContent`

Get the content of the message.

```cpp
static unsigned char *getContent();
```

### Method: `setContent`

Set the content of the message.

```cpp
static void setContent(unsigned char *content, unsigned short contentSize);
```

### Method: `getRecord`

Get the record of the message.

```cpp
NdefRecord_t getRecord();
```

The record is a structure that contains the following properties:

```cpp
typedef struct {
  NdefRecordType_e recordType;
  unsigned char *recordPayload;
  unsigned int recordPayloadSize;
} NdefRecord_t;
```

#### Example

```cpp
message.getRecord();
```

### Method: `isEmpty`

Returns `true` if the message is empty, otherwise returns `false`.

```cpp
bool isEmpty();
```

#### Example

```cpp
if (message.isEmpty()) {
  Serial.println("The message is empty!");
}
```

### Method: `isNotEmpty`

Returns `true` if the message is not empty, otherwise returns `false`.

```cpp
bool isNotEmpty();
```

#### Example

```cpp
if (message.isNotEmpty()) {
  Serial.println("The message is not empty!");
}
```

### Method: `hasRecord`

Returns `true` if the message has a record, otherwise returns `false`.

```cpp
bool hasRecord();
```

#### Example

```cpp
if (message.hasRecord()) {
  Serial.println("The message has a record!");
}
```

## Class NdefRecord

A `NdefRecord` object represents an NDEF record. An NDEF record is a data structure that contains data that is stored or transported in an NDEF message.

#### Example

```cpp
NdefRecord record;
```

### Method: `create`

Creates a new NDEF record.

```cpp
void create(NdefRecord_t record);
```

#### Example

```cpp
record.create(message.getRecord());
```

### Method: `isEmpty`

Returns `true` if the record is empty, otherwise returns `false`.

```cpp
bool isEmpty();
```

#### Example

```cpp
if (record.isEmpty()) {
  Serial.println("The record is empty!");
}
```

### Method: `isNotEmpty`

Returns `true` if the record is not empty, otherwise returns `false`.

```cpp
bool isNotEmpty();
```

#### Example

```cpp
if (record.isNotEmpty()) {
  Serial.println("The record is not empty!");
}
```

### Method: `getType`

Get the type of the record.

```cpp
NdefRecordType_e getType();
```

The type is a structure that contains the following properties:

```cpp
typedef enum {
  WELL_KNOWN_SIMPLE_TEXT,
  WELL_KNOWN_SIMPLE_URI,
  WELL_KNOWN_SMART_POSTER,
  WELL_KNOWN_HANDOVER_SELECT,
  WELL_KNOWN_HANDOVER_REQUEST,
  WELL_KNOWN_ALTERNATIVE_CARRIER,
  WELL_KNOWN_COLLISION_RESOLUTION,
  MEDIA_VCARD,
  MEDIA_HANDOVER_WIFI,
  MEDIA_HANDOVER_BT,
  MEDIA_HANDOVER_BLE,
  MEDIA_HANDOVER_BLE_SECURE,
  ABSOLUTE_URI,
  UNSUPPORTED_NDEF_RECORD = 0xFF
} NdefRecordType_e;
```

### Method: `getPayloadSize`

Get the payload size of the record.

```cpp
unsigned short getPayloadSize();
```

### Method: `getPayload`

Get the payload of the record.

```cpp
unsigned char *getPayload();
```

#### Example

```cpp
Serial.print("Payload: ");
for (int i = 0; i < record.getPayloadSize(); i++) {
  Serial.print(record.getPayload()[i], HEX);
  Serial.print(" ");
}
Serial.println();
```

### Method: `getText`

Get the text of the record if the type is `WELL_KNOWN_SIMPLE_TEXT`, otherwise returns `"null"`.

```cpp
String getText();
```

#### Example

```cpp
Serial.print("Text: ");
Serial.println(record.getText());
```

### Method: `getBluetoothName`

Get the Bluetooth name of the record if the type is `MEDIA_HANDOVER_BT`, otherwise returns `"null"`.

```cpp
String getBluetoothName();
```

#### Example

```cpp
Serial.print("Bluetooth name: ");
Serial.println(record.getBluetoothName());
```

### Method: `getBluetoothAddress`

Get the Bluetooth address of the record if the type is `MEDIA_HANDOVER_BT`, otherwise returns `"null"`.

```cpp
String getBluetoothAddress();
```

#### Example

```cpp
Serial.print("Bluetooth address: ");
Serial.println(record.getBluetoothAddress());
```

### Method: `getWifiSSID`

Get the WiFi SSID of the record if the type is `MEDIA_HANDOVER_WIFI`, otherwise returns `"null"`.

```cpp
String getWifiSSID();
```

#### Example

```cpp
Serial.print("WiFi SSID: ");
Serial.println(record.getWifiSSID());
```

### Method: `getWifiPassword`

Get the WiFi password of the record if the type is `MEDIA_HANDOVER_WIFI`, otherwise returns `"null"`.

```cpp
String getWiFiPassword();
```

#### Example

```cpp
Serial.print("WiFi password: ");
Serial.println(record.getWiFiPassword());
```

### Method: `getWiFiAuthenticationType`

Get the WiFi authentication type of the record if the type is `MEDIA_HANDOVER_WIFI`, otherwise returns `"null"`.

```cpp
String getWiFiAuthenticationType();
```

#### Example

```cpp
Serial.print("WiFi authentication type: ");
Serial.println(record.getWiFiAuthenticationType());
```

### Method: `getWiFiEncryptionType`

Get the WiFi encryption type of the record if the type is `MEDIA_HANDOVER_WIFI`, otherwise returns `"null"`.

```cpp
String getWiFiEncryptionType();
```

#### Example

```cpp
Serial.print("WiFi encryption type: ");
Serial.println(record.getWiFiEncryptionType());
```

### Method: `getVCardContent`

Get the vCard content of the record if the type is `MEDIA_VCARD`, otherwise returns `"null"`.

```cpp
String getVCardContent();
```

#### Example

```cpp
Serial.print("vCard content: ");
Serial.println(record.getVCardContent());
```

### Method: `getUri`

Get the URI of the record if the type is `WELL_KNOWN_SIMPLE_URI`, otherwise returns `"null"`.

```cpp
String getUri();
```

#### Example

```cpp
Serial.print("URI: ");
Serial.println(record.getUri());
```