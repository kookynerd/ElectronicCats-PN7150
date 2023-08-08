#include "Mode.h"

Mode::Mode() {
    mode = 1;
}

bool Mode::set(int mode) {
    if (mode < 1 || mode > 3) {
        return false;
    }

    this->mode = mode;
    return true;
}

int Mode::get() {
    return mode;
}
