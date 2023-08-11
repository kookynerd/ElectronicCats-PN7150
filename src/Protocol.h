#ifndef Protocol_H
#define Protocol_H

class Protocol {
 public:
  enum Value {
    UNDETERMINED = 0x0,
    T1T = 0x1,       // NFC FORUM Type 1 Tag (based on Topaz/Jewel) ISO 14443A
    T2T = 0x2,       // NFC Type 2 Tag ISO 14443B
    T3T = 0x3,       // NFC Type 3 Tag Sony FeliCa standard
    ISODEP = 0x4,    // NFC Type 4 Tag ISO14443-4
    NFCDEP = 0x5,    // NFC Data Exchange Protocol
    ISO15693 = 0x6,  // NFC-V
    MIFARE = 0x80    // NFC Type 5 MIFARE Classic
  };
};

#endif