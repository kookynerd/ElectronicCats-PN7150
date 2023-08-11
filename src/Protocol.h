#ifndef Protocol_H
#define Protocol_H

/*
 * Flag definition used as Protocol values
 */
#define PROT_UNDETERMINED 0x0
#define PROT_T1T 0x1       // NFC FORUM Type 1 Tag (based on Topaz/Jewel) ISO 14443A
#define PROT_T2T 0x2       // NFC Type 2 Tag ISO 14443B
#define PROT_T3T 0x3       // NFC Type 3 Tag Sony FeliCa standard
#define PROT_ISODEP 0x4    // NFC Type 4 Tag ISO14443-4
#define PROT_NFCDEP 0x5    // NFC Data Exchange Protocol
#define PROT_ISO15693 0x6  // NFC-V
#define PROT_MIFARE 0x80   // NFC Type 5 MIFARE Classic

class Protocol {
 public:
  enum Value {
    UNDETERMINED = 0x0,
    T1T = 0x1,
    T2T = 0x2,
    T3T = 0x3,
    ISODEP = 0x4,
    NFCDEP = 0x5,
    ISO15693 = 0x6,
    MIFARE = 0x80
  };
};

#endif