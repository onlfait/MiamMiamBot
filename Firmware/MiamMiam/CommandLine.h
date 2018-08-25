/*
* CommandLine.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "Arduino.h"

#define BAUD_RATE 115200

#define CR       '\r'
#define LF       '\n'
#define SPACE    ' '
#define NULLCHAR '\0'

#define LINE_BUF_SIZE 128
#define ARG_BUF_SIZE   64
#define MAX_NUM_ARGS    8

class CommandLine {
  public:
    CommandLine();
    void begin();
    void begin(int baudRate);
    bool read();
    char* getLine();
    bool lineMatch(const char* input);
    bool argMatch(int index, const char* input);
    char* getArg(int index);
    int getArgAsInt(int index);
    float getArgAsFloat(int index);
  private:
    char _line[LINE_BUF_SIZE + 1];
    char _args[MAX_NUM_ARGS][ARG_BUF_SIZE];
    void _parse();
};

#endif
