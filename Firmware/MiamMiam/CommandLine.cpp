/*
* CommandLine.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

CommandLine::CommandLine() {
  _commandsCount = 0;
  _argsCount = 0;
  _separator = "|";
}

void CommandLine::begin() {
  while (!Serial);
  Serial.begin(BAUD_RATE);
}

void CommandLine::begin(int baudRate) {
  while (!Serial);
  Serial.begin(baudRate);
}

void CommandLine::send (const char *fmt, ...) {
  char buf[LINE_BUF_SIZE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, LINE_BUF_SIZE, fmt, args);
  va_end(args);
  Serial.print(buf);
}

void CommandLine::send (const __FlashStringHelper *fmt, ...) {
  char buf[LINE_BUF_SIZE];
  va_list args;
  va_start(args, fmt);
  #ifdef __AVR__
    vsnprintf_P(buf, LINE_BUF_SIZE, (const char *)fmt, args);
  #else
    vsnprintf(buf, LINE_BUF_SIZE, (const char *)fmt, args);
  #endif
  va_end(args);
  Serial.print(buf);
}

void CommandLine::addCommand(const char* name, CommandCallback func) {
  if (_commandsCount < MAX_COMMANDS) {
    CommandStruct command = { name, func };
    _commands[_commandsCount++] = command;
  }
}

void CommandLine::_parse () {
  char *argument;
  _argsCount = 0;
  char line[LINE_BUF_SIZE + 1];

  strcpy(line, _line);
  argument = strtok(line, _separator);
  memset(_args, 0, sizeof(_args));

  while (argument != NULL) {
    if (_argsCount < MAX_NUM_ARGS) {
      if (strlen(argument) < ARG_BUF_SIZE) {
        strcpy(_args[_argsCount], argument);
        argument = strtok(NULL, _separator);
        _argsCount++;
      } else {
        break;
      }
    } else {
      break;
    }
  }
}

void CommandLine::executeCommand () {
  for (int i = 0; i < _commandsCount; i++) {
    CommandStruct command = _commands[i];
    if (strcmp(_args[0], command.name) == 0) {
        command.func();
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

void CommandLine::watch () {
  if (read()) {
    executeCommand();
  }
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

int CommandLine::getArgsCount () {
  return _argsCount;
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
