#include "Commands.h"
#include "Parser.h"
#include "Command.h"

const int ZDRelays[8] = {26, 28, 30, 32, 40, 38, 36, 34};
const int BIRTRelays[8] = {27, 29, 31, 33, 41, 39, 37, 35};
int id = -1;
Parser parser = Parser();
byte *data = new byte[64];

void serialEvent() {
    Serial.print("Command");

  while (Serial.available()) {
    int dataLength = Serial.available();
    Serial.readBytes(data, dataLength);
    List<Command*>* commands = parser.parse(data, dataLength);
    for (int i = 0; i < commands->size(); i++) {
      runCommand(commands->get(i));
      delete commands->get(i);
    }
    delete commands;
  }
}

void runCommand(Command *command) {
  Serial.print("Command");
  switch (command->getKey()) {
    case TURN:
      Argument *group = command->getArgument('g');
      Argument *state = command->getArgument('o');
      Argument *number = command->getArgument('n');
      
      if (group == 0) {
        turnZDRelays(*(int*) number->getValue(), *(int*)state->getValue());
      } else {
        turnBIRTRelays(*(int*) number->getValue(), *(int*)state->getValue());
      }
  }
}

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(ZDRelays[i], OUTPUT);
    pinMode(BIRTRelays[i], OUTPUT);
    digitalWrite(ZDRelays[i], HIGH);
    digitalWrite(BIRTRelays[i], LOW);
  }
}

void loop() {
}

void turnZDRelays(int relayID, int state) {
  if (state == 1 && digitalRead(ZDRelays[relayID]) == 0) {
    return;
  }
  digitalWrite(ZDRelays[relayID], state);
  return;
}

void turnBIRTRelays(uint8_t relayID, uint8_t state) {
  if (id == -1) {
    if (state == 0 && digitalRead(ZDRelays[relayID]) == 0) {
      id = relayID;
      digitalWrite(BIRTRelays[relayID], 0);
      return;
    }
    return;
  }
  if (state == 0) {
    return;
  }
  if (relayID == id) {
    digitalWrite(BIRTRelays[relayID], 1);
    id = -1;
    return;
  }
}
