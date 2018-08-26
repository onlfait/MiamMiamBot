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

#define BAUD_RATE 115200

#define CR        '\r'
#define LF        '\n'
#define NULLCHAR  '\0'

#define LINE_BUF_SIZE 128
#define ARG_BUF_SIZE   64
#define MAX_NUM_ARGS    8

#define MAX_COMMANDS 32

typedef void (*CommandCallback) ();

typedef struct {
  const char* name;
  CommandCallback func;
} CommandStruct;

char* fts(float val); // float to string

class CommandLine {
  public:
    CommandLine();
    void begin();
    void begin(int baudRate);
    void addCommand(const char* name, CommandCallback func);
    bool read();
    void executeCommand();
    void watch();
    void send(const char *fmt, ...);
    void send(const __FlashStringHelper *fmt, ...);
    char* getLine();
    bool lineMatch(const char* input);
    bool argMatch(int index, const char* input);
    int getArgsCount();
    char* getArg(int index);
    int getArgAsInt(int index);
    float getArgAsFloat(int index);
  private:
    char _line[LINE_BUF_SIZE + 1];
    char _args[MAX_NUM_ARGS][ARG_BUF_SIZE];
    CommandStruct _commands[MAX_COMMANDS];
    int _argsCount;
    int _commandsCount;
    const char* _separator;
    void _parse();
};

#endif
