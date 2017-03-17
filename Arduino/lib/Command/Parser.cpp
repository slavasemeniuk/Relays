#include "Parser.h"
#include "MemoryFree.h"//TODO: remove

Parser::Parser() {
  bufferLength = 0;
  buffer = (byte*) malloc(bufferLength);
}

Parser::~Parser() {
  free(buffer);
}

vector<Command*>* Parser::parse(byte* data, uint8_t dataLength) {
  // printf_P(PSTR("Parser::parse: data @%d, len = %d | buffer @%d, len = %d\n"), data, dataLength, buffer, bufferLength);
  // printf_P(PSTR("freemem = %d\n"), freeMemory());
  if (bufferLength > MAX_BUFFER_LENGTH) trim(bufferLength / 2);
  if (bufferLength == 0) {
    int16_t start = searchStart(data, dataLength, 0);
    if (start != NOT_FIND) {
      bufferLength = dataLength - start;
      buffer = (byte*) realloc(buffer, bufferLength);
      memcpy(buffer, data + start, bufferLength);
    }
  } else {
    buffer = (byte*) realloc(buffer, bufferLength + dataLength);
    memcpy(buffer + bufferLength, data, dataLength);
    bufferLength += dataLength;
  }

  vector<Command*>* commands = new vector<Command*>();
  while (true) {
    // printf_P(PSTR(">>>Buffer length = %d\n"), bufferLength);//TODO: remove
    Command* command = searchCommand();
    if (command != NULL) commands->push_back(command);
    else break;
  }
  return commands;
}

/**
 * Used to trim buffer
 * Cut off unused data from start of @buffer,
 * and delete already parsed commands
 *
 * @param from search from this position to find new command start, if not find delete all buffer
 */
void Parser::trim(uint16_t from) {
  // printf_P(PSTR("Parser::trim(%d) freeMem = %d\n"), from, freeMemory());
  int16_t start = searchStart(buffer, bufferLength, from);
  if (start == NOT_FIND) {//TODO:
    bufferLength = 0;
    buffer = (byte*) realloc(buffer, 0);
  } else if (start != 0) {
    bufferLength -= start;//TODO: test this!
    memmove(buffer, buffer + start, bufferLength);
    buffer = (byte*) realloc(buffer, bufferLength);
  }
}

uint16_t Parser::searchStart(byte* buffer, uint16_t length, uint16_t from) {
  for (uint16_t i = from; i < length; i++) {
    if (buffer[i] == Command::START[0]) {
      uint8_t errors = 0;
      int16_t overflow = (i + Command::START_LENGTH) - length;
      for (uint8_t j = 1; j < Command::START_LENGTH - (overflow > 0 ? overflow : 0); j++) {
        if (buffer[i + j] != Command::START[j]) errors++;
      }
      if (errors == 0) return i;
    }
  }
  return NOT_FIND;
}

uint16_t Parser::searchEnd(byte* buffer, uint16_t length, uint16_t from) {
  // from = from - Command::END_LENGTH >= 0 ? from : Command::END_LENGTH;
  for (uint16_t i = from; i < length; i++) {
    if (buffer[i] == Command::END[Command::END_LENGTH - 1]) {
      uint8_t errors = 0;
      for (uint8_t j = 0; j < Command::END_LENGTH; j++) {
        if (buffer[i - Command::END_LENGTH + j + 1] != Command::END[j]) errors++;
      }
      if (errors == 0) return i;
    }
  }
  return NOT_FIND;
}

Command* Parser::searchCommand() {
  for (int16_t start = searchStart(buffer, bufferLength, 0); start != NOT_FIND; start = searchStart(buffer, bufferLength, start + 1)) {
    for (int16_t end = searchEnd(buffer, bufferLength, start + Command::EMPTY_LENGTH - 1); end != NOT_FIND; end = searchEnd(buffer, bufferLength, end + 1)) {
      Command* command = Command::deserialize(buffer + start, end - start + 1);
      if (command != NULL) {
        trim(end + 1);
        return command;
      }
    }
  }
  return NULL;
}
