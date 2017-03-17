#ifndef Command_h
#define Command_h
#include <vector>
#include "Arduino.h"
#include "MemoryFree.h"
#include "Argument.h"

using namespace std;

class Command {
	public:
		Command(byte key);
		Command(byte key, vector<Argument*>* arguments);
		~Command();

		void serialize(byte*& bytes, uint16_t& length);
		static Command* deserialize(byte bytes[], uint16_t size);
		void toSerial();//@Deprecated @InTesting

		byte getKey();
		void setKey(byte key);
		vector<Argument*>* getArguments();
		void setArguments(vector<Argument*> arguments);
		Argument* getArgument(byte key);

		static const byte START[];
		static const uint8_t START_LENGTH;

		static const byte END[];
		static const uint8_t END_LENGTH;

		static const int EMPTY_LENGTH;
	private:
		byte key;
		vector<Argument*>* arguments;
};

#endif
