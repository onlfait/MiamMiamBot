/*
* CommandLine.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

CommandLine::CommandLine () {
  _commandsCount = 0;
}

void CommandLine::begin (unsigned long baudRate) {
  while (!Serial);
  Serial.begin(baudRate);
}

void CommandLine::addCommand (const char* name, void (*command)()) {
  if (_commandsCount < COMMAND_LINE_MAX_COMMANDS) {
    _CommandStruct commandStruct = { name, command };
    _commandsList[_commandsCount++] = commandStruct;
  }
}

void CommandLine::addCommand (const __FlashStringHelper *name, void (*command)()) {
  addCommand((const char*) name, command);
}

void CommandLine::send (const char* format, ...) {
  char buf[COMMAND_LINE_BUFFER_SIZE];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, COMMAND_LINE_BUFFER_SIZE, format, args);
  va_end(args);
  Serial.print(buf);
}

void CommandLine::send (const __FlashStringHelper *format, ...) {
  char buf[COMMAND_LINE_BUFFER_SIZE];
  va_list args;
  va_start(args, format);
  #ifdef __AVR__
    vsnprintf_P(buf, COMMAND_LINE_BUFFER_SIZE, (const char*) format, args);
  #else
    vsnprintf(buf, COMMAND_LINE_BUFFER_SIZE, (const char*) format, args);
  #endif
  va_end(args);
  Serial.print(buf);
}

bool CommandLine::read () {
  static uint8_t charsRead = 0;

  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case COMMAND_LINE_EOL:
        _line[charsRead] = COMMAND_LINE_NULLCHAR;
        if (charsRead > 0)  {
          charsRead = 0;
          return true;
        }
        break;
      default:
        if (charsRead < COMMAND_LINE_BUFFER_SIZE) {
          _line[charsRead++] = c;
        }
        _line[charsRead] = COMMAND_LINE_NULLCHAR;
        break;
    }
  }

  return false;
}

bool CommandLine::parse () {
  char *argument;
  _argsCount = 0;
  char line[COMMAND_LINE_BUFFER_SIZE + 1];

  strcpy(line, _line);
  argument = strtok(line, COMMAND_LINE_SEPARATOR);
  memset(_args, 0, sizeof(_args));

  while (argument != NULL) {
    if (_argsCount < COMMAND_LINE_MAX_ARGUMENTS) {
      if (strlen(argument) < COMMAND_LINE_ARG_BUFFER_SIZE) {
        strcpy(_args[_argsCount], argument);
        argument = strtok(NULL, COMMAND_LINE_SEPARATOR);
        _argsCount++;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

bool CommandLine::execute () {
  return true;
}

void CommandLine::watch () {
  if (read() && parse()) {
    execute();
  }
}
