#include "Mode.h"

Mode::Mode() {
  this->_mode = 1;
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
