#include "TinyWireS.h"
#include <Arduino.h>
#include "Energyconfig.h"
#include <I2C_TinyAnything.h>
#include <debugutils.h>
//#define DEBUGMIO
#if defined DEBUGMIO
#include <SendOnlySoftwareSerial.h>
SendOnlySoftwareSerial mySerial (1);  // Tx pin
#endif
// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance
void setup(){
  #if defined DEBUGMIO
    delay(2000);
    mySerial.begin(9600);
  #endif
  DEBUG_PRINT("booting");
  emon1.voltage(2, 225, 1);  // Voltage: input pin, calibration, phase_shift
  emon1.current(3, 111);       // Current: input pin, calibration.
  TinyWireS.begin(SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
  DEBUG_PRINT("into loop!");
  haveData=false;
}
void loop(){
  if(!haveData)
  {
    emon1.calcVI(100,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    myener.realPower       = roundTwo(emon1.realPower);        //extract Real Power into variable
    myener.apparentPower   = roundTwo(emon1.apparentPower);  //extract Apparent Power into variable
    myener.powerFactor     = roundTwo(emon1.powerFactor);     //extract Power Factor into Variable
    myener.supplyVoltage   = roundTwo(emon1.Vrms);           //extract Vrms into Variable
    myener.Irms            = roundTwo(emon1.Irms);            //extract Irms into Variable
    DEBUG_PRINT("Volt: " + String(myener.supplyVoltage));
    haveData=true;
  }
  TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent(){
  if(!haveData) return;
  I2C_writeAnything(myener);
  haveData=false;

}
uint16_t roundTwo(float pippo){
  uint16_t buf = pippo * 100;
  return (uint16_t)buf;

}
