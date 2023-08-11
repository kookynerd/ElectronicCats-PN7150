#ifndef Technology_H
#define Technology_H

/*
 * Flag definition used as Technologies values
 */
#define TECH_PASSIVE_NFCA 0
#define TECH_PASSIVE_NFCB 1
#define TECH_PASSIVE_NFCF 2
#define TECH_ACTIVE_NFCA 3
#define TECH_ACTIVE_NFCF 5
#define TECH_PASSIVE_15693 6

class Technology {
 public:
  enum Value {
    PASSIVE_NFCA = 0,
    PASSIVE_NFCB = 1,
    PASSIVE_NFCF = 2,
    ACTIVE_NFCA = 3,
    ACTIVE_NFCF = 5,
    PASSIVE_15693 = 6
  };
};

#endif