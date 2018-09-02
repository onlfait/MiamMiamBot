/*
* CommandLine.h - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "Arduino.h"

#define COMMAND_LINE_MAX_COMMANDS 10
#define COMMAND_LINE_BUFFER_SIZE 64
#define COMMAND_LINE_MAX_ARGS 10
#define COMMAND_LINE_ARGS_SEPARATOR "|"
#define COMMAND_LINE_NULLCHAR '\0'
#define COMMAND_LINE_EOL '\n'

typedef void (*CommandLineCallback_f) (uint8_t argc, char **argv);

struct CommandLineCallback_t {
  const char *name;
  CommandLineCallback_f callback;
};

class CommandLine {
  public:
    CommandLine();
    void begin(unsigned long baudRate);
    void send(const char *format, ...);
    void send(const __FlashStringHelper *format, ...);
    void defaultCommand(CommandLineCallback_f callback);
    bool addCommand(const char *name, CommandLineCallback_f callback);
    bool addCommand(const __FlashStringHelper *name, CommandLineCallback_f callback);
    bool watch();
  private:
    CommandLineCallback_f _defaultCommand;
    CommandLineCallback_t _commandsList[COMMAND_LINE_MAX_COMMANDS];
};

#endif
