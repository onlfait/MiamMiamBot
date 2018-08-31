/*
* CommandLine.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "Arduino.h"
//#include <stdarg.h>

#define COMMAND_LINE_MAX_COMMANDS 10
#define COMMAND_LINE_BUFFER_SIZE 64
#define COMMAND_LINE_MAX_ARGS 10
#define COMMAND_LINE_ARGS_SEPARATOR "|"
#define COMMAND_LINE_NULLCHAR '\0'
#define COMMAND_LINE_EOL '\n'

class CommandLine {
  public:
    CommandLine();
    void begin(unsigned long baudRate);
    void send(const char* format, ...);
    void send(const __FlashStringHelper *format, ...);
    bool addCommand(const char *name, void (*command)(uint8_t argc, char **argv));
    bool addCommand(const __FlashStringHelper *name, void (*command)(uint8_t argc, char **argv));
    void defaultCommand(void (*command)(uint8_t argc, char **argv));
    bool read();
    bool parse();
    bool watch();
  private:
    char _line[COMMAND_LINE_BUFFER_SIZE + 1];
    void (*_defaultCommand)(uint8_t argc, char **argv);
    typedef void (*_Command)(uint8_t argc, char **argv);
    typedef struct {
      const char* name;
      _Command func;
    } _CommandStruct;
    _CommandStruct _commandsList[COMMAND_LINE_MAX_COMMANDS];
};

#endif
