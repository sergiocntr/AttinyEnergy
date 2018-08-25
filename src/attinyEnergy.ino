/*Example #2 code:
assumes a clock speed of 8MHz on an ATtiny85
uses 10-bit resolution (values from 0-1024)
uses ADC1 on pin PB2
uses internal 1.1V as the reference voltage
*/

#include "TinyWireS.h"
#include <Arduino.h>
#include "EnergyMUXconfig.h"

#include <I2C_TinyAnything.h>
#include <debugutils.h>
//#define DEBUGMIO
#if defined DEBUGMIO
#include <SendOnlySoftwareSerial.h>
//#include <SoftwareSerial.h>
SendOnlySoftwareSerial mySerial (1);  // Tx pin
//SoftwareSerial mySerial(3, 1); // RX, TX
#endif
//#define emonTxV3  //per 3.3volt
// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
//#include "EmonLib.h"             // Include Emon Library
//EnergyMonitor emon1;             // Create an instance
void setup(){
  #if defined DEBUGMIO
    delay(2000);
    mySerial.begin(9600);
  #endif
  DEBUG_PRINT("booting");
  //emon1.voltage(4, 225, 1);  // Voltage: input pin, calibration, phase_shift
  //emon1.current(3, 111);       // Current: input pin, calibration.
  initADC();
  trovaMedio();

  DEBUG_PRINT("into loop!");
  haveData=false;
}
void loop(){
  if(!haveData)
  {
    //emon1.calcVI(100,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
    //emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
    calcolaIns();

    //myener.realPower       = roundTwo(emon1.realPower);        //extract Real Power into variable
    //myener.apparentPower   = roundTwo(emon1.apparentPower);  //extract Apparent Power into variable
    myener.powerFactor     = phase;     //extract Power Factor into Variable
    myener.supplyVoltage   = volt;           //extract Vrms into Variable
    myener.Irms            = amp;            //extract Irms into Variable
    //DEBUG_PRINT("Volt: " + String(myener.supplyVoltage));
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
void initADC()
{
  /*
  MUX3	MUX2	MUX1	MUX0	Single Ended Input
    0	    0	    0	    0	    ADC0
    0	    0	    0	    1	    ADC1
    0     0	    1	    0	    ADC2 ->PB4 VOLT
    0	    0	    1	    1	    ADC3 ->PB3 AMPER
    0	    1     0    	0	    ADC4
    0	    1	    0	    1	    ADC5
    */
  ADMUX =
            (0 << ADLAR) |     // do not left shift result
            (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
            (1 << REFS0) |     // Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
            (0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
            (1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
            (0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // set prescaler to 128, bit 2
            (1 << ADPS1) |     // set prescaler to 128, bit 1
            (1 << ADPS0);      // set prescaler to 128, bit 0
}
void stampa(){
  //mySerial.begin(9600);
  delay(10);
  DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("MAX: " + String(max));
  DEBUG_PRINT("MIN " + String(min));
  //delay(5000);

}
void calcola(){
  numSamples=0;
  min=1024;
  max=0;
  previousMillis = millis();
  uint16_t raw_adc;

  while((millis()-previousMillis) < interval)
  {
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    //adc_lobyte = ADCL; // get the sample value from ADCL
    raw_adc = ADC;   // add lobyte and hibyte

    if(raw_adc >= max)
      max = raw_adc;
    if(raw_adc <= min)
      min = raw_adc;
    numSamples++;
  }

}
void trovaMedio(){
  //TROVA IL VALORE MEDIO ADC DELLA LETTURA
  //ELABORA DUE VOLTE
  for (uint8_t i = 0; i < 2; i++) {
    ADMUX |= (1<<ADMUX); //set bit 1 -> PB3 -> AMPER
    bitSet(ADMUX, 0);
    calcola();
    #if defined DEBUGMIO
      DEBUG_PRINT("ADMUXa: " + String(ADMUX));
      stampa();
    #endif
    medioA = (max+min)/2;//volt*0.27;
    bitClear(ADMUX, 0);
    calcola();
    #if defined DEBUGMIO
    DEBUG_PRINT("ADMUXv: " + String(ADMUX));
    stampa();
    #endif
    medioV= (max+min)/2;  //amp*0.015;

  }
  DEBUG_PRINT("Mvolt: " + String(medioA));
  DEBUG_PRINT("Mamp; " + String(medioV));
}
void calcolaIns(){
  numSamples=0;
  min=1024;
  max=0;
  previousMillis = millis();
  uint16_t thisvolt,thisamp,maxA,minA,maxV,minV;
  maxA=0;minA=1024;maxV=0;minV=1024;
  while((millis()-previousMillis) < interval)
  {
    bitClear(ADMUX, 0);//PB4 -> VOLT
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    thisvolt = ADC;   // add lobyte and hibyte
    bitSet(ADMUX, 0);//set bit 1 -> PB3 -> AMPER
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    thisamp = ADC;   // add lobyte and hibyte

    if(thisvolt >= maxV)
      maxV = thisvolt;
    else if(thisvolt <= minV)
      minV = thisvolt;

    if(thisamp >= maxA)
      maxA = thisamp;
    else if(thisamp <= minA)
      minA = thisamp;
    if (thisamp==medioA) {
      phase=abs(thisvolt-medioV);
      DEBUG_PRINT("zeroamp: " + String(phase));
    }
    //numSamples++;
  }
  volt=maxV-minV;
  amp=maxA-minA;
  //DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("A: " + String(amp));
  //DEBUG_PRINT("MIN A: " + String(minA));
  //DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("V: " + String(volt));
  //DEBUG_PRINT("MIN V" + String(minV));
}
void apettaEsp(){
  TinyWireS.begin(SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
}
