#include "Command.h"

const byte Command::START[] = {':', ':'};
const uint8_t Command::START_LENGTH = 2;

const byte Command::END[] = {'\r', '\n'};
const uint8_t Command::END_LENGTH = 2;

const int Command::EMPTY_LENGTH = START_LENGTH + 2 + END_LENGTH;

Command::Command(byte key) {
	Command::key = key;
	arguments = new vector<Argument*>();//TODO: check this!
}

Command::Command(byte key, vector<Argument*>* arguments) {
	Command::key = key;
	Command::arguments = arguments;
}

Command::~Command() {
	// Serial.println(F("Command.destructor()"));
	for (uint16_t i = 0; i < arguments->size(); i++) delete arguments->at(i);
	delete arguments;
}

void Command::serialize(byte*& bytes, uint16_t& length) {//TODO: check on multy argument commands
	length = EMPTY_LENGTH;
	for (uint8_t i = 0; i < arguments->size(); i++) length += Argument::OFFSET + arguments->at(i)->getSize();
	bytes = new byte[length];
	memcpy(bytes, START, START_LENGTH);//TODO: test
	uint8_t pos = START_LENGTH;
	bytes[pos++] = key;
	bytes[pos++] = arguments->size();
	for (uint8_t i = 0; i < arguments->size(); i++) {
		memcpy(&bytes[pos], arguments->at(i)->getBytes(), arguments->at(i)->getLength());
		pos += arguments->at(i)->getLength();
	}
	memcpy(&bytes[pos], END, END_LENGTH);//TODO: check
}

Command* Command::deserialize(byte bytes[], uint16_t bytesLength) {
	if (bytesLength < EMPTY_LENGTH) return NULL;
	uint16_t pos = START_LENGTH;
	uint8_t key = bytes[pos++];
	uint8_t argsCount = bytes[pos++];
	// printf_P(PSTR("Command::deserialize: lenght = %d, key = %d, argsCount = %d\n"), bytesLength, key, argsCount);//TODO: debug

	vector<Argument*>* arguments = new vector<Argument*>(argsCount);
	for (uint8_t i = 0; i < argsCount; i++) {
		uint8_t argSize = bytes[pos + Argument::SIZE];
		if (pos + Argument::OFFSET + argSize + END_LENGTH > bytesLength) break;//TODO: "|| argSize < 0" check argsize, it can't be negative

		arguments->push_back(new Argument(&bytes[pos], argSize + Argument::OFFSET));
		pos += arguments->at(i)->getLength();

		// arguments->get(i)->print();//TODO: debug
	}
	if ((arguments->size() == argsCount) && (pos + END_LENGTH == bytesLength)) {
		return new Command(key, arguments);
	} else {
		for (uint8_t i = 0; i < arguments->size(); i++) delete arguments->at(i);
		delete arguments;
		return NULL;
	}
}

/**
* Only for microcontrollers
* TODO: test this. If improvements in speed are not big, remove this
*/
void Command::toSerial() {
	Serial.write(START, START_LENGTH);
	Serial.write(key);
	Serial.write(arguments->size());
	for (uint8_t i = 0; i < arguments->size(); i++) {
		Serial.write(arguments->at(i)->getBytes(), arguments->at(i)->getLength());
	}
	Serial.write(END, END_LENGTH);
}

byte Command::getKey() {
	return key;
}

void Command::setKey(byte key) {
	Command::key = key;
}

vector<Argument*>* Command::getArguments() {
	return arguments;
}

void Command::setArguments(vector<Argument*> arguments) {
	delete Command::arguments;//TODO: check it, what if old arguments vector was not empty?
	*Command::arguments = arguments;
}

Argument* Command::getArgument(byte key) {
	for (uint8_t i = 0; i < arguments->size(); i++) {
		if (arguments->at(i)->getKey() == key) return arguments->at(i);
	}
	return NULL;
}
