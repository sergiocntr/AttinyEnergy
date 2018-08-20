// Written by Nick Gammon
// May 2012
//modified by Sergio Contro
#include <Arduino.h>
#include <TinyWireS.h>
template <typename T> unsigned int I2C_writeAnything (const T& value)
  {
    const byte * p = (const byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)

          TinyWireS.send(*p++);
    return i;
  }  // end of I2C_writeAnything

template <typename T> unsigned int I2C_readAnything(T& value)
  {
    byte * p = (byte*) &value;
    unsigned int i;
    for (i = 0; i < sizeof value; i++)
          *p++ = TinyWireS.receive();
    return i;
  }  // end of I2C_readAnything
