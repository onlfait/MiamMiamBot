/*
* CommandLine.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

CommandLine::CommandLine () {}

void CommandLine::begin (unsigned long baudRate) {
  while (!Serial);
  Serial.begin(baudRate);
}

void CommandLine::send (const char *format, ...) {
  char buf[COMMAND_LINE_BUFFER_SIZE + 1];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, COMMAND_LINE_BUFFER_SIZE, format, args);
  va_end(args);
  Serial.print(buf);
}

void CommandLine::send (const __FlashStringHelper *format, ...) {
  char buf[COMMAND_LINE_BUFFER_SIZE + 1];
  va_list args;
  va_start(args, format);
  #ifdef __AVR__
    vsnprintf_P(buf, COMMAND_LINE_BUFFER_SIZE, (const char *) format, args);
  #else
    vsnprintf(buf, COMMAND_LINE_BUFFER_SIZE, (const char *) format, args);
  #endif
  va_end(args);
  Serial.print(buf);
}

void CommandLine::defaultCommand (void (*command)(uint8_t argc, char **argv)) {
  _defaultCommand = command;
}

bool CommandLine::addCommand (const char* name, void (*command)(uint8_t argc, char **argv)) {
  static uint8_t commandsCount = 0;
  if (commandsCount < COMMAND_LINE_MAX_COMMANDS) {
    _CommandStruct commandStruct = { name, command };
    _commandsList[commandsCount++] = commandStruct;
    return true;
  }
  return false;
}

bool CommandLine::addCommand (const __FlashStringHelper *name, void (*command)(uint8_t argc, char **argv)) {
  return addCommand((const char*) name, command);
}

bool CommandLine::read () {
  static uint8_t offset = 0;
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case COMMAND_LINE_EOL:
        _line[offset] = COMMAND_LINE_NULLCHAR;
        if (offset > 0)  {
          offset = 0;
          return true;
        }
        break;
      default:
        if (offset < COMMAND_LINE_BUFFER_SIZE) {
          _line[offset++] = c;
        }
        _line[offset] = COMMAND_LINE_NULLCHAR;
        break;
    }
  }

  return false;
}

bool CommandLine::parse () {
  uint8_t argc = 0;
  char *argv[COMMAND_LINE_MAX_ARGS];
  char buf[COMMAND_LINE_BUFFER_SIZE];
  argv[argc] = strtok(_line, COMMAND_LINE_ARGS_SEPARATOR);
  do {
    argv[++argc] = strtok(NULL, COMMAND_LINE_ARGS_SEPARATOR);
  } while ((argc < COMMAND_LINE_MAX_ARGS) && (argv[argc] != NULL));
  uint8_t count = sizeof(_commandsList)/sizeof(_commandsList[0]);
  for (uint8_t i = 0; i < count; i++) {
    _CommandStruct command = _commandsList[i];
    if (strcmp(argv[0], command.name) == 0
    || strcmp_P(argv[0], command.name) == 0) {
      command.func(argc, argv);
      return true;
    }
  }
  if (_defaultCommand != NULL) {
    _defaultCommand(argc, argv);
  }
  return false;
}

bool CommandLine::watch () {
  return read() && parse();
}
