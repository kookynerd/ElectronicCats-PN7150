#include "Mode.h"

Mode::Mode() {
  this->_mode = mode.READER_WRITER;
}

bool Mode::setMode(int mode) {
  if (mode < 1 || mode > 3) {
    return false;
  }

  this->_mode = mode;
  return true;
}

int Mode::getMode() {
  return this->_mode;
}
