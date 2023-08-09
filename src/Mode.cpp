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

void Mode::setReaderWriterMode() {
  this->_mode = mode.READER_WRITER;
}

void Mode::setEmulationMode() {
  this->_mode = mode.EMULATION;
}

void Mode::setP2PMode() {
  this->_mode = mode.P2P;
}

int Mode::getMode() {
  return this->_mode;
}
