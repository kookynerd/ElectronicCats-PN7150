#ifndef ModeTech_H
#define ModeTech_H

/*
 * Flag definition used as Mode values
 */
#define MODE_POLL 0x00
#define MODE_LISTEN 0x80
#define MODE_MASK 0xF0

class ModeTech {
 public:
  enum Value {
    POLL = 0x00,
    LISTEN = 0x80,
    MASK = 0xF0
  };
};

#endif