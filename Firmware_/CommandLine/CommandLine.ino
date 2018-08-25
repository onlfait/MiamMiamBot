#include "CommandLine.h"

void setup () {
  Serial.begin(115200);
}

void loop () {
  if (readCommandLine()) {
    Serial.println(commandLine);
    if (commandArgMatch(0, "print")) {
      Serial.println("PRINT");
      if (commandArgMatch(1, "hour")) {
        Serial.println("HOUR");
        Serial.println(getCommandArg(2));
        Serial.println(getCommandArgInt(2));
      }
    }
  }
}
