#ifndef Mode_H
#define Mode_H

#include "Arduino.h"

class Mode {
	private:
		int mode;

	public:
		Mode();
		bool set(unsigned int mode);
		int get();
		enum {
			READER_WRITER = 1,
			EMULATION = 2,
			P2P = 3
		};
};

#endif