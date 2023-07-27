#include <Electroniccats_PN7150.h>
#include "ndef_helper.h"

#define PN7150_IRQ (11)
#define PN7150_VEN (13)
#define PN7150_ADDR (0x28)

Electroniccats_PN7150 nfc(PN7150_IRQ, PN7150_VEN, PN7150_ADDR); // Creates a global NFC device interface object, attached to pins 7 (IRQ) and 8 (VEN) and using the default I2C address 0x28
RfIntf_t RfInterface;                                           // Interface to save data for multiple tags

uint8_t mode = 1; // modes: 1 = Reader/ Writer, 2 = Emulation, 3 = Peer to peer P2P

unsigned char STATUSOK[] = {0x90, 0x00}, Cmd[256], CmdSize;

const char NDEF_MESSAGE[] = {0xD1,      // MB/ME/CF/1/IL/TNF
                             0x01,      // TYPE LENGTH
                             0x07,      // PAYLOAD LENTGH
                             'T',       // TYPE
                             0x02,      // Status
                             'e', 'n',  // Language
                             'T', 'e', 's', 't'};

void setup() {
  Serial.begin(9600);
	while (!Serial)
		;
	Serial.println("NDEF Message with PN7150");

	if(T4T_NDEF_EMU_SetMessage((unsigned char *) NDEF_MESSAGE, sizeof(NDEF_MESSAGE), (void*)*ndefPush_Cb)) {
    Serial.println("Set message ok\r\n");  
  } else {
    Serial.println("Set message error\r\n");
	}

	Serial.println("Initializing...");
	if (nfc.connectNCI()) { // Wake up the board
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

	mode = 3;
	resetMode();
	Serial.println("Waiting for an NDEF device...");
}

void loop() {
	if(nfc.CardModeReceive(Cmd, &CmdSize) == 0) { //Data in buffer?
		if ((CmdSize >= 2) && (Cmd[0] == 0x00)) { //Expect at least two bytes
			switch (Cmd[1]) {
				case 0xA4: //Something tries to select a file, meaning that it is a reader
						Serial.println("Reader detected!");
					break;

				case 0xB0: //SFI
				Serial.println("0xB0 detected!");
					break;

				case 0xD0: //...
				Serial.println("0xD0 detected!");
					break;

				default:
					break;
			}
			nfc.CardModeSend(STATUSOK, sizeof(STATUSOK));
		}
  }
}

void resetMode() {  // Reset the configuration mode after each reading
  Serial.println("\nRe-initializing...");
  nfc.ConfigMode(mode);
  nfc.StartDiscovery(mode);
}

void ndefPush_Cb(unsigned char *pNdefRecord, unsigned short NdefRecordSize) {
  Serial.println("--- NDEF Record sent");
}