#include <Arduino.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>
// log async to avoid blocking
std::string logString = "";
void IRAM_ATTR loga(char *fmt, ...) {
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, 128, fmt, args);
  va_end(args);
  logString += buf;
  logString += "\n";
}

void logWrite() {
  if (logString.length() > 0) {
    Serial.print(logString.c_str());
    Serial.print('\n');
    logString = "";
  }
}
void log(const char *fmt, ...) {
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, 128, fmt, args);
  va_end(args);
  Serial.println(buf);
  // WebSerial.println(buf);
}
