/*
* CommandLine.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "Arduino.h"
#include <stdarg.h>

#define COMMAND_LINE_MAX_COMMANDS 10
#define COMMAND_LINE_BUFFER_SIZE 32
#define COMMAND_LINE_NULLCHAR '\0'
#define COMMAND_LINE_EOL '\n'
#define COMMAND_LINE_SEPARATOR "|"
#define COMMAND_LINE_ARG_BUFFER_SIZE 32
#define COMMAND_LINE_MAX_ARGUMENTS 8

class CommandLine {
  public:
    CommandLine();
    void begin(unsigned long baudRate);
    void addCommand(const char* name, void (*command)());
    void addCommand(const __FlashStringHelper *name, void (*command)());
    void send(const char* format, ...);
    void send(const __FlashStringHelper *format, ...);
    bool read();
    bool parse();
    bool execute();
    void watch();
  private:
    typedef void (*_Command)();
    typedef struct {
      const char* name;
      _Command func;
    } _CommandStruct;
    uint8_t _commandsCount;
    _CommandStruct _commandsList[COMMAND_LINE_MAX_COMMANDS];
    char _line[COMMAND_LINE_BUFFER_SIZE + 1];
    uint8_t _argsCount;
    char _args[COMMAND_LINE_MAX_ARGUMENTS][COMMAND_LINE_ARG_BUFFER_SIZE];
};

#endif
