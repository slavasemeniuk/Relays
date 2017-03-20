#ifndef Command_h
#define Command_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "ArrayList.h"
#include "Argument.h"

class Command {
	public:
		Command(byte key);
		Command(byte key, List<Argument*>* arguments);
		~Command();

		void serialize(byte*& bytes, uint16_t& length);
		static Command* deserialize(byte bytes[], uint16_t size);
		void toSerial();

		byte getKey();
		void setKey(byte key);
		List<Argument*>* getArguments();
		void setArguments(List<Argument*> arguments);
		Argument* getArgument(byte key);

		static const byte START[];
		static const uint8_t START_LENGTH;

		static const byte END[];
		static const uint8_t END_LENGTH;

		static const int EMPTY_LENGTH;
	private:
		byte key;
		List<Argument*>* arguments;
};

#endif
