#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#define CR '\r'
#define LF '\n'
#define SPACE ' '
#define NULLCHAR '\0'

#define LINE_BUF_SIZE 128
#define ARG_BUF_SIZE   64
#define MAX_NUM_ARGS    8

char commandLine[LINE_BUF_SIZE + 1];
char commandArgs[MAX_NUM_ARGS][ARG_BUF_SIZE];

void parseCommandLine () {
  char *argument;
  int counter = 0;
  char line[LINE_BUF_SIZE + 1];

  strcpy(line, commandLine);
  argument = strtok(line, " ");
  memset(commandArgs, 0, sizeof(commandArgs));

  while (argument != NULL) {
    if (counter < MAX_NUM_ARGS) {
      if (strlen(argument) < ARG_BUF_SIZE) {
        strcpy(commandArgs[counter], argument);
        argument = strtok(NULL, " ");
        counter++;
      } else {
        break;
      }
    } else {
      break;
    }
  }
}

bool readCommandLine () {
  static uint8_t charsRead = 0;

  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:
      case LF:
        commandLine[charsRead] = NULLCHAR;
        if (charsRead > 0)  {
          parseCommandLine();
          charsRead = 0;
          return true;
        }
        break;
      default:
        if (charsRead < LINE_BUF_SIZE) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;
        break;
    }
  }

  return false;
}

bool commandLineMatch (const char* input) {
  return strcmp(commandLine, input) == 0;
}

bool commandArgMatch (int index, const char* input) {
  return strcmp(commandArgs[index], input) == 0;
}

char* getCommandArg (int index) {
  return commandArgs[index];
}

int getCommandArgInt (int index) {
  return atoi(commandArgs[index]);
}

#endif
