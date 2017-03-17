#ifndef Argument_h
#define Argument_h
#include "Arduino.h"

class Argument {
  public:
    Argument(byte key);
    Argument(byte key, byte size, void* value);
    Argument(byte* bytes, int8_t length);
    ~Argument();

    byte getKey();
    byte getSize();
    void* getValue();

    byte* getBytes();
    uint8_t getLength();

    // void print();

    static const uint8_t KEY = 0;
    static const uint8_t SIZE = 1;
    static const uint8_t OFFSET = 2;
  private:
	  byte* bytes;
};

#endif
