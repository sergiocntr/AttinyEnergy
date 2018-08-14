/*Example #2 code:
assumes a clock speed of 8MHz on an ATtiny85
uses 10-bit resolution (values from 0-1024)
uses ADC1 on pin PB2
uses internal 1.1V as the reference voltage
*/
#include <Arduino.h>
#include "TinyWireS.h"
#include <I2C_TinyAnything.h>
#include "lcdPrint.h"
//TinyDebugSerial mySerial= TinyDebugSerial();
#define I2C_SLAVE_ADDRESS 0x02 // the 7-bit address (remember to change this when adapting this example)
// I2C
//attiny pin 0 = not(OC1A) = PORTB <- _BV(0) = SOIC pin 5 (I2C SDA, PWM)
//attiny pin 2 =           = PORTB <- _BV(2) = SOIC pin 7 (I2C SCL, Analog 1)
// The default buffer size, Can't recall the scope of defines right now
#ifndef TWI_TX_BUFFER_SIZE
#define TWI_TX_BUFFER_SIZE ( 32 )
#endif
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 8 )
#endif
#include <Arduino.h>
#define emonTxV3  //per 3.3volt
// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
volatile byte reg_position = 0;
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

void setup(){
  //emon1.voltage(2, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  //emon1.current(1, 111.1);       // Current: input pin, calibration.
  lcdsetup();
  //TinyWireS.begin(I2C_SLAVE_ADDRESS);
  //TinyWireS.onReceive(receiveEvent);
  //TinyWireS.onRequest(requestEvent);
}
void loop(){
  /*emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

  ener.realPower       = emon1.realPower;        //extract Real Power into variable
  ener.apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  ener.powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  ener.supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  ener.Irms            = emon1.Irms;             //extract Irms into Variable
  lcdPrint(ener);
  lcd.clear();
  sprintf(_buffer, "RPow %.2f W",ener.realPower);
  */
  void lcdsetup();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("_buffer");
  //unsigned long timeout = millis();
  //while (millis() - timeout > 3000) {
  //  TinyWireS_stop_check();
  //}
  delay(5000);
}
void requestEvent()
{
  //TinyWireS.send(cv.dati[reg_position]);
  // Increment the reg position on each read, and loop back to zero
  //reg_position++;
  //TinyWireS.send(cv.dati[reg_position]);
  //if (reg_position >= reg_size)
  //{
  //  reg_position = 0;
  //}
}
void receiveEvent(uint8_t howMany)
{
    if (howMany < 1)
    {
        // Sanity-check
        return;
    }
    if (howMany > TWI_RX_BUFFER_SIZE)
    {
        // Also insane number
        return;
    }
    reg_position = TinyWireS.receive();
    if(reg_position == 20) I2C_writeAnything(ener);
}
