/*
* CommandLine.h - Part of MiamMiam Firmware
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

typedef void (*CommandLineCallback) (uint8_t argc, char **argv);

typedef struct {
  const char *name;
  CommandLineCallback callback;
} CommandLineCallbackStruct;


class CommandLine {
  public:
    CommandLine();
    void begin(unsigned long baudRate);
    void send(const char *format, ...);
    void send(const __FlashStringHelper *format, ...);
    void defaultCommand(CommandLineCallback callback);
    bool addCommand(const char *name, CommandLineCallback callback);
    bool addCommand(const __FlashStringHelper *name, CommandLineCallback callback);
    bool watch();
  private:
    CommandLineCallback _defaultCommand;
    CommandLineCallbackStruct _commandsList[COMMAND_LINE_MAX_COMMANDS];
};

#endif
