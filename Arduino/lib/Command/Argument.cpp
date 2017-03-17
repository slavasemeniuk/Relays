#include "Argument.h"

Argument::Argument(byte key) {//TODO: test it!
  bytes = (byte*) malloc(2);
  bytes[0] = key;
  bytes[1] = 0;
}

Argument::Argument(byte key, byte size, void* value) {//Replace void* with &void
  bytes = (byte*) malloc(OFFSET + size);
  bytes[0] = key;
  bytes[1] = size;
  memcpy(&bytes[OFFSET], value, size);
}

Argument::Argument(byte* bytes, int8_t length) {
  this->bytes = (byte*) memcpy(malloc(length), bytes, length);
}

Argument::~Argument() {
  free(bytes);
}

byte Argument::getKey() {
  return bytes[KEY];
}

byte Argument::getSize() {
  return bytes[SIZE];
}

void* Argument::getValue() {
  return &bytes[OFFSET];
}

byte* Argument::getBytes() {
  return bytes;
}

uint8_t Argument::getLength() {
  return OFFSET + bytes[SIZE];
}

// void Argument::print() {
//   printf_P(PSTR("arg.key = %d | arg.size = %d\n"), getKey(), getSize());
//   for(int i = 0; i < getSize(); i++) {
//     printf_P(PSTR("[%d]@%d = %d, "), i, bytes + OFFSET + i, *(byte*) (bytes + OFFSET + i));
//   } Serial.println();
// }
