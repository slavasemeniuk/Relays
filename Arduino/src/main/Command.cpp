#include "Command.h"

const byte Command::START[] = {':', ':'};
const uint8_t Command::START_LENGTH = 2;

const byte Command::END[] = {'\r', '\n'};
const uint8_t Command::END_LENGTH = 2;

const int Command::EMPTY_LENGTH = START_LENGTH + 2 + END_LENGTH;

Command::Command(byte key) {
	Command::key = key;
	arguments = new ArrayList<Argument*>();
}

Command::Command(byte key, List<Argument*>* arguments) {
	Command::key = key;
	Command::arguments = arguments;
}

Command::~Command() {
	// Serial.println(F("Command.destructor()"));
	for (uint16_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
	delete arguments;
}

void Command::serialize(byte*& bytes, uint16_t& length) {//TODO: check on multy argument commands
	length = EMPTY_LENGTH;
	for (uint8_t i = 0; i < arguments->size(); i++) length += Argument::OFFSET + arguments->get(i)->getSize();
	bytes = new byte[length];
	memcpy(bytes, START, START_LENGTH);//TODO: test
	uint8_t pos = START_LENGTH;
	bytes[pos++] = key;
	bytes[pos++] = arguments->size();
	for (uint8_t i = 0; i < arguments->size(); i++) {
		memcpy(&bytes[pos], arguments->get(i)->getBytes(), arguments->get(i)->getLength());
		pos += arguments->get(i)->getLength();
	}
	memcpy(&bytes[pos], END, END_LENGTH);//TODO: check
}

Command* Command::deserialize(byte bytes[], uint16_t bytesLength) {
	if (bytesLength < EMPTY_LENGTH) return NULL;
	uint16_t pos = START_LENGTH;
	uint8_t key = bytes[pos++];
	uint8_t argsCount = bytes[pos++];
	// printf_P(PSTR("Command::deserialize: lenght = %d, key = %d, argsCount = %d\n"), bytesLength, key, argsCount);//TODO: debug

	List<Argument*>* arguments = new ArrayList<Argument*>(argsCount);
	for (uint8_t i = 0; i < argsCount; i++) {
		uint8_t argSize = bytes[pos + Argument::SIZE];
		if (pos + Argument::OFFSET + argSize + END_LENGTH > bytesLength) break;//TODO: "|| argSize < 0" check argsize, it can't be negative

		arguments->add(new Argument(&bytes[pos], argSize + Argument::OFFSET));
		pos += arguments->get(i)->getLength();

		// arguments->get(i)->print();//TODO: debug
	}
	if ((arguments->size() == argsCount) && (pos + END_LENGTH == bytesLength)) {
		return new Command(key, arguments);
	} else {
		for (uint8_t i = 0; i < arguments->size(); i++) delete arguments->get(i);
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
		Serial.write(arguments->get(i)->getBytes(), arguments->get(i)->getLength());
	}
	Serial.write(END, END_LENGTH);
}

byte Command::getKey() {
	return key;
}

void Command::setKey(byte key) {
	Command::key = key;
}

List<Argument*>* Command::getArguments() {
	return arguments;
}

void Command::setArguments(List<Argument*> arguments) {
	delete Command::arguments;//TODO: check it, what if old arguments list was not empty?
	*Command::arguments = arguments;
}

Argument* Command::getArgument(byte key) {
	for (uint8_t i = 0; i < arguments->size(); i++) {
		if (arguments->get(i)->getKey() == key) return arguments->get(i);
	}
	return NULL;
}
