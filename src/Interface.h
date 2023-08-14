#ifndef Interface_H
#define Interface_H

/*
 * Flag definition used as Interface values
 */
#define INTF_UNDETERMINED 0x0
#define INTF_FRAME 0x1
#define INTF_ISODEP 0x2
#define INTF_NFCDEP 0x3
#define INTF_TAGCMD 0x80

class Interface {
 public:
  enum Value {
    UNDETERMINED = 0x0,
    FRAME = 0x1,
    ISODEP = 0x2,
    NFCDEP = 0x3,
    TAGCMD = 0x80
  };
};

#endif