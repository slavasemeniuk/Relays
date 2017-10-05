#include "Commands.h"
#include "Parser.h"
#include "Command.h"

const uint16_t ZDRelays[8] = {26, 28, 30, 32, 40, 38, 36, 34};
const uint16_t KPins[8] = {46, 48, 50, 52, 53, 51, 49, 47};

const uint16_t helpRelays[2] = {27, 29};

Parser parser = Parser();

void serialEvent() {
  while (Serial.available()) {
    byte *data = new byte[64];
    int dataLength = Serial.available();
    Serial.readBytes(data, dataLength);
    List<Command*>* commands = parser.parse(data, dataLength);
    for (uint16_t i = 0; i < commands->size(); i++) {
      runCommand(commands->get(i));
      delete commands->get(i);
    }
    delete commands;
    delete data;
  }
}

void runCommand(Command *command) {
  switch (command->getKey()) {
  case TURN: {
    uint16_t group = *(int16_t*) command->getArgument('g')->getValue();
    boolean state = *(boolean*) command->getArgument('o')->getValue();
    uint16_t number = *(int16_t*) command->getArgument('n')->getValue();
    if (group == 0) {
      turnZDRelays(number, state);
    } else {
      turnHelpRelays(number, state);
    }
    break;
  }
  case GET_STATES: {
    sendAllStates();
    break;
  }
  }
}

void setup() {
  for (uint16_t i = 0; i < 8; i++) {
    pinMode(ZDRelays[i], OUTPUT);
    pinMode(KPins[i], INPUT);
    digitalWrite(ZDRelays[i], HIGH);
  }
  for (uint16_t i = 0; i < 2; i++) {
    pinMode(helpRelays[i], OUTPUT);
    digitalWrite(helpRelays[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {}

void sendAllStates() {
  for (uint16_t i = 0; i < 2; i++) {
    boolean on = digitalRead(helpRelays[i]) == LOW;
    sendToSerial(1, on, i);
  }
  for (uint16_t i = 0; i < 8; i++) {
    boolean zdOn = digitalRead(ZDRelays[i]) == LOW;
    sendToSerial(0, zdOn, i);
  }
}

void turnZDRelays(uint16_t relayID, boolean state) {
  if (digitalRead(KPins[relayID]) == HIGH) { return; }

  boolean on = state;
  Command* command = new Command(TURN);
  digitalWrite(ZDRelays[relayID], !on);
  sendToSerial(0, on, relayID);
  delete command;
}

void turnHelpRelays(uint16_t relayID, boolean state) {
  digitalWrite(helpRelays[relayID], !state);
  sendToSerial(1, state, relayID);
}

void sendToSerial(uint16_t group, boolean on, uint16_t relayID) {
  Command* command = new Command(TURN);
  command->getArguments()->add(new Argument(STATE, 1, &on));
  command->getArguments()->add(new Argument(GROUP, 2, &group));
  command->getArguments()->add(new Argument(NUMBER, 2, &relayID));
  command->toSerial();
  delete command;
}
