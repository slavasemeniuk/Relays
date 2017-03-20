#ifndef List_h
#define List_h
#include "Arduino.h"

template<typename T>
class List {
  public:
    virtual ~List() {};
    virtual void add(T item) {};
    virtual void add(uint16_t index, T item) {};
    virtual T set(uint16_t index, T item) {return (T) NULL;};
    virtual T get(uint16_t index) {return (T) NULL;};
    virtual T remove(uint16_t index) {return (T) NULL;};
    virtual void clear() {};

    virtual uint16_t size() {return (uint16_t) NULL;};
    virtual bool isEmpty() {return (bool) NULL;};
  private:
};

#endif
