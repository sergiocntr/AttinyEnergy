#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H
#include <Arduino.h>
//#define DEBUGU
  #if defined DEBUGU
  #define DEBUG_PRINT(str)    \
     mySerial.print(millis());     \
     mySerial.print(": ");    \
     mySerial.print(':');      \
     mySerial.print(__LINE__);     \
     mySerial.print(' ');      \
     mySerial.println(str);
#else
  #define DEBUG_PRINT(str)
  #endif
#endif
