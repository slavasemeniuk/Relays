#ifndef CircularBuffer_h
#define CircularBuffer_h
#include "Arduino.h"

template< typename T >
class CircularBuffer {
  public:
    CircularBuffer(uint16_t capacity);
    ~CircularBuffer();

    void put(T item);
    T set(uint16_t index, T item);
    T get(uint16_t index);
    T remove(uint16_t index);
    T popStart();
    T popEnd();
    void clear();

    uint16_t size();
    bool isEmpty();

    void print();//@Deprecated
  private:
    void dec(uint16_t* pointer);
    void inc(uint16_t* pointer);
    uint16_t getAbsoluteIndex(uint16_t index);
    uint16_t getRelativeIndex(uint16_t index);
    bool rangeCheck(uint16_t index);

    uint16_t start;
    uint16_t end;
    uint16_t capacity;
    uint16_t valuesSize;
    T* values;
};

template< typename T >
CircularBuffer<T>::CircularBuffer(uint16_t capacity) {
  this->values = new T[capacity];
  this->capacity = capacity;
  this->valuesSize = 0;
  this->start = 0;
  this->end = 0;
}

template< typename T >
CircularBuffer<T>::~CircularBuffer() {
  delete[] values;
}

template< typename T >
void CircularBuffer<T>::dec(uint16_t* pointer) {
  (*pointer)--;
  if (*pointer == -1) *pointer = capacity - 1;
}

template< typename T >
void CircularBuffer<T>::inc(uint16_t* pointer) {
  (*pointer)++;
  if (*pointer == capacity) *pointer = 0;
}

template< typename T >
void CircularBuffer<T>::put(T item) {//TODO: call delete if T is pointer and start == end
  if (valuesSize != 0) {
    inc(&end);
    if (start == end) inc(&start);
    else valuesSize++;
  } else {
    valuesSize++;
  }
  values[end] = item;
}

template< typename T >
T CircularBuffer<T>::set(uint16_t index, T item) {
  if (rangeCheck(index)) {
    T oldValue = values[getAbsoluteIndex(index)];
    values[getAbsoluteIndex(index)] = item;
    return oldValue;
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::get(uint16_t index) {
  if (rangeCheck(index)) {
    return values[getAbsoluteIndex(index)];
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::remove(uint16_t index) {//TODO: check
  if (rangeCheck(index)) {
    uint16_t absoluteIndex = getAbsoluteIndex(index);
    T removed = values[absoluteIndex];
    Serial.print("Removed: "); Serial.print(removed); Serial.print("@"); Serial.println(absoluteIndex);
    if (start != end) {
      if (absoluteIndex == start) {
        inc(&start);
      } else {
        if (absoluteIndex < end) {//if removed element in one side with end pointer (or start < end, etc...)
          memmove(&values[absoluteIndex], &values[absoluteIndex + 1], (end - absoluteIndex) * sizeof(T));//TODO: check
        } else if (absoluteIndex != end) {//if start > end, and removed element is ater start and before array end
          if (absoluteIndex != capacity -1) memmove(&values[absoluteIndex], &values[absoluteIndex + 1], (capacity - absoluteIndex - 1) * sizeof(T));//0...E...S-A-A1-C -> 0...E...S-A1-C-N
          values[capacity - 1] = values[0];//0...E...S-A1-C-N -> 0...E...S-A1-C-0
          if (end > 0) memmove(&values[0], &values[1], end * sizeof(T));//0...E...S-A1-C-0 -> 1...E-N...S-A1-C-0
        }
        dec(&end);
      }
    }
    valuesSize--;
    return removed;
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::popStart() {
  if (rangeCheck(0)) {
    T* result = &values[start];
    if (start != end) inc(&start);
    valuesSize--;
    return *result;
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::popEnd() {
  if (rangeCheck(valuesSize - 1)) {
    T* result = &values[end];
    if (start != end) dec(&end);
    valuesSize--;
    return *result;
  } else return (T) NULL;
}

template< typename T >
void CircularBuffer<T>::clear() {
  valuesSize = 0;
  start = 0;
  end = 0;
}

template< typename T >
uint16_t CircularBuffer<T>::size() {
  return valuesSize;
}

template< typename T >
bool CircularBuffer<T>::isEmpty() {
  return valuesSize == 0;
}

template< typename T >
uint16_t CircularBuffer<T>::getAbsoluteIndex(uint16_t index) {
  uint16_t absoluteIndex = start + index;
  return (absoluteIndex >= capacity) ? absoluteIndex - capacity : absoluteIndex;
}

template< typename T >
bool CircularBuffer<T>::rangeCheck(uint16_t index) {
  if (index < valuesSize) {
    return true;
  } else {
    Serial.print(F("IndexOutOfBoundsException: Index: ")); Serial.print(index); Serial.print(F(", Size: ")); Serial.println(size());
    return false;
  }
}

//@Deprecated
template< typename T >
void CircularBuffer<T>::print(){
  Serial.println(F("Relative data: "));
  for (uint8_t i = 0; i < size(); i++) {
    Serial.print(get(i)); Serial.print(F(", "));
  } Serial.println();

  Serial.println(F("Root data: "));
  for (uint8_t i = 0; i < capacity; i++) {
    Serial.print(values[i]); Serial.print(F(", "));
  }
  Serial.println();
  Serial.print(F("start = ")); Serial.print(start); Serial.print(F(", end = ")); Serial.print(end); Serial.print(F(", size = ")); Serial.println(valuesSize);
  Serial.println();
}

#endif
