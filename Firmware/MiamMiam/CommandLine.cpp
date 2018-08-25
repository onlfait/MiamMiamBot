/*
* CommandLine.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

CommandLine::CommandLine() { }

void CommandLine::begin() {
  while (!Serial);
  Serial.begin(BAUD_RATE);
}

void CommandLine::begin(int baudRate) {
  while (!Serial);
  Serial.begin(baudRate);
}

void CommandLine::_parse () {
  char *argument;
  int counter = 0;
  char line[LINE_BUF_SIZE + 1];

  strcpy(line, _line);
  argument = strtok(line, " ");
  memset(_args, 0, sizeof(_args));

  while (argument != NULL) {
    if (counter < MAX_NUM_ARGS) {
      if (strlen(argument) < ARG_BUF_SIZE) {
        strcpy(_args[counter], argument);
        argument = strtok(NULL, " ");
        counter++;
      } else {
        break;
      }
    } else {
      break;
    }
  }
}

bool CommandLine::read () {
  static uint8_t charsRead = 0;

  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:
      case LF:
        _line[charsRead] = NULLCHAR;
        if (charsRead > 0)  {
          charsRead = 0;
          _parse();
          return true;
        }
        break;
      default:
        if (charsRead < LINE_BUF_SIZE) {
          _line[charsRead++] = c;
        }
        _line[charsRead] = NULLCHAR;
        break;
    }
  }

  return false;
}

char* CommandLine::getLine () {
  return _line;
}

bool CommandLine::lineMatch (const char* input) {
  return strcmp(_line, input) == 0;
}

bool CommandLine::argMatch (int index, const char* input) {
  return strcmp(_args[index], input) == 0;
}

char* CommandLine::getArg (int index) {
  return _args[index];
}

int CommandLine::getArgAsInt (int index) {
  return atoi(_args[index]);
}

float CommandLine::getArgAsFloat (int index) {
  return atof(_args[index]);
}
