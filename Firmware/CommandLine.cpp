/*
 *CommandLine.cpp - Part of MiamMiamBot Firmware
 *Created by Sébastien Mischler & Raymond Humbert
 *Released into the public domain
 *http://www.onlfait.ch/MiamMiam
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
    vsnprintf_P(buf, COMMAND_LINE_BUFFER_SIZE, (const char*) format, args);
  #else
    vsnprintf(buf, COMMAND_LINE_BUFFER_SIZE, (const char*) format, args);
  #endif
  va_end(args);
  Serial.print(buf);
}

void CommandLine::defaultCommand (CommandLineCallback_f callback) {
  _defaultCommand = callback;
}

bool CommandLine::addCommand (const char *name, CommandLineCallback_f callback) {
  static uint8_t commandsCount = 0;
  if (commandsCount < COMMAND_LINE_MAX_COMMANDS) {
    CommandLineCallback_t commandStruct = { name, callback };
    _commandsList[commandsCount++] = commandStruct;
    return true;
  }
  return false;
}

bool CommandLine::addCommand (const __FlashStringHelper *name, CommandLineCallback_f callback) {
  return addCommand((const char*) name, callback);
}

bool CommandLine::watch () {
  // read
  char c;
  static uint8_t offset = 0;
  static char buffer[COMMAND_LINE_BUFFER_SIZE + 1];
  while (Serial.available()) {
    c = Serial.read();
    if (offset < COMMAND_LINE_BUFFER_SIZE) {
      if (c == COMMAND_LINE_EOL) {
        buffer[offset] = COMMAND_LINE_NULLCHAR;
        break;
      }
      buffer[offset] = c;
      buffer[offset + 1] = COMMAND_LINE_NULLCHAR;
    }
    offset++;
  }
  if (offset >= COMMAND_LINE_BUFFER_SIZE) {
    if (c == COMMAND_LINE_EOL) {
      offset = 0;
    }
    return false;
  }
  if (c != COMMAND_LINE_EOL) {
    return false;
  }
  offset = 0;
  // parse
  uint8_t argc = 0;
  char *argv[COMMAND_LINE_MAX_ARGS];
  argv[argc] = strtok(buffer, COMMAND_LINE_ARGS_SEPARATOR);
  do {
    argv[++argc] = strtok(NULL, COMMAND_LINE_ARGS_SEPARATOR);
  } while ((argc < COMMAND_LINE_MAX_ARGS) && (argv[argc] != NULL));
  uint8_t count = sizeof(_commandsList)/sizeof(_commandsList[0]);
  for (uint8_t i = 0; i < count; i++) {
    CommandLineCallback_t command = _commandsList[i];
    if (strcmp(argv[0], command.name) == 0
    || strcmp_P(argv[0], command.name) == 0) {
      command.callback(argc, argv);
      return true;
    }
  }
  if (_defaultCommand != NULL) {
    _defaultCommand(argc, argv);
  }
  return false;
}
