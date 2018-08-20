/*Example #2 code:
assumes a clock speed of 8MHz on an ATtiny85
uses 10-bit resolution (values from 0-1024)
uses ADC1 on pin PB2
uses internal 1.1V as the reference voltage
*/

#include "TinyWireS.h"
#include <Arduino.h>
#include "Energyconfig.h"
#include <I2C_TinyAnything.h>
#include <debugutils.h>
//#define DEBUGMIO
#if defined DEBUGMIO
#include <SendOnlySoftwareSerial.h>
#include <SoftwareSerial.h>
SendOnlySoftwareSerial mySerial (1);  // Tx pin
//SoftwareSerial mySerial(3, 1); // RX, TX
#endif
#define emonTxV3  //per 3.3volt
// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance
void setup(){
  #if defined DEBUGMIO
    delay(2000);
    mySerial.begin(9600);
  #endif
  DEBUG_PRINT("booting");
  emon1.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 111.1);       // Current: input pin, calibration.
  TinyWireS.begin(SLAVE_ADDRESS);
  //TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
  DEBUG_PRINT("into loop!");
  haveData=false;
}
void loop(){
  if(!haveData)
  {
    //emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  //emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

    //myener.realPower       = emon1.realPower;        //extract Real Power into variable
    //myener.apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
    //myener.powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
    //myener.supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
    //myener.Irms            = emon1.Irms;             //extract Irms into Variable
    //DEBUG_PRINT("Volt: " + String(myener.supplyVoltage));
    haveData=true;
  }
  TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent(){
  myener.supplyVoltage=55.55;
  //DEBUG_PRINT("Got request!");
  //TinyWireS.send(0X10);
   I2C_writeAnything(myener);
   haveData=false;
   //DEBUG_PRINT("end request!");
}