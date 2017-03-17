#ifndef Parser_h
#define Parser_h
#include <vector>
#include "Arduino.h"
#include "Command.h"

class Parser {
  public:
    Parser();
    ~Parser();
    vector<Command*>* parse(byte* data, uint8_t size);
  private:
    void trim(uint16_t from);
    uint16_t searchStart(byte* buffer, uint16_t length, uint16_t from);
    uint16_t searchEnd(byte* buffer, uint16_t length, uint16_t from);
    Command* searchCommand();

    byte* buffer;
    uint16_t bufferLength;
    static const int16_t MAX_BUFFER_LENGTH = 128;
    static const int8_t NOT_FIND = - 1;
};

#endif
