#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H
#include <Arduino.h>
//#define DEBUGMIO
  #if defined DEBUGMIO
  #define DEBUG_PRINT(str)    \
     mySerial.print(millis());     \
     mySerial.print(": ");    \
     mySerial.print(':');      \
     mySerial.print(__LINE__);     \
     mySerial.print(' ');      \
     mySerial.println(str);


  #else
  #define DEBUG_PRINT(str)
    //mySerial.println(str);
  #endif
  #if defined DEBUGMIO
  #define DEBUG_LCD(_buffer)    \
     lcd.clear();               \
     lcd.setCursor(0,0);        \
     lcd.print(_buffer);        \
     delay(2000);
  #else
  #define DEBUG_LCD(_buffer)
  #endif
#endif
/*
#define DEBUG_PRINT(str)    \
   mySerial.print(millis());     \
   mySerial.print(": ");    \
   mySerial.print(__PRETTY_FUNCTION__); \
   mySerial.print(' ');      \
   mySerial.print(__FILE__);     \
   mySerial.print(':');      \
   mySerial.print(__LINE__);     \
   mySerial.print(' ');      \
   mySerial.println(str);
   */
