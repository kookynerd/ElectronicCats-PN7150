#ifndef Mode_H
#define Mode_H

#include "Arduino.h"

struct Mode_t {
  enum {
    READER_WRITER = 1,
    EMULATION = 2,
    P2P = 3
  };
};

class Mode {
 private:
  int _mode;

 protected:
  bool setMode(int mode);  // Only for internal use

 public:
  Mode();
  Mode_t mode;
  int getMode();
};

#endif