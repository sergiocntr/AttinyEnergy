#include <Arduino.h>
#include "TinyWireS.h"
#include <debugutils.h>
#include <I2C_TinyAnything.h>
#include "EnergyMUXconfig.h"
//#define DEBUGMIO
#if defined DEBUGMIO
  #include <SendOnlySoftwareSerial.h>
  SendOnlySoftwareSerial mySerial (1);  // Tx pin
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(3, 1); // RX, TX
#endif
void setup(){
  #if defined DEBUGMIO
    delay(2000);
    mySerial.begin(9600);
  #endif
  DEBUG_PRINT("booting");
  //initADC();
  trovaMedio();

  TinyWireS.begin(SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
  //delay(100);
  haveData=false;
}
void requestEvent(){
  if(!haveData) return
  myener.powerFactor     = phase;     //extract Power Factor into Variable
  myener.supplyVoltage   = volt;           //extract Vrms into Variable
  myener.Irms            = amp;            //extract Irms into Variable
  I2C_writeAnything(myener);
  haveData=false;
}
void loop(){
  if(!haveData)
  {
    calcolaIns();
    //volt+=1;
    haveData=true;
  }
    TinyWireS_stop_check();
}
void initADC(){
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
void trovaMedio(){
  //TROVA IL VALORE MEDIO ADC DELLA LETTURA
  //ELABORA DUE VOLTE
  for (uint8_t i = 0; i < 2; i++) {
    //bitSet(ADMUX, MUX0);

    calcola(2);
    #if defined DEBUGMIO
      DEBUG_PRINT("***ADMUXa: " + String(ADMUX));
      DEBUG_PRINT("***ADCSRA: " + String(ADCSRA));
      stampa();
    #endif
    medioA = (max+min)/2;//volt*0.27;
    //bitClear(ADMUX, MUX0);
    calcola(3);
    #if defined DEBUGMIO
    DEBUG_PRINT("ADMUXv: " + String(ADMUX));
    stampa();
    #endif
    medioV= (max+min)/2;  //amp*0.015;

  }
  DEBUG_PRINT("Mvolt: " + String(medioA));
  DEBUG_PRINT("Mamp; " + String(medioV));
}
void calcola(uint8_t pin){
  numSamples=0;
  min=1024;
  max=0;
  previousMillis = millis();
  uint16_t raw_adc;

  while((millis()-previousMillis) < interval)
  {
    //analogReference(DEFAULT);
    //ADMUX = (0x40) | (pin & 0x07);
    //ADCSRA |= (1 << ADSC);         // start ADC measurement
    //while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    //adc_lobyte = ADCL; // get the sample value from ADCL
    //raw_adc = ADC;   // add lobyte and hibyte
    //ADMUX = (analog_reference << 6) | (pin & 0x07);

    raw_adc = analogRead(pin);
    if(raw_adc >= max)
      max = raw_adc;
    else if(raw_adc <= min)
      min = raw_adc;
    numSamples++;
  }

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
    //bitClear(ADMUX, 0);//PB4 -> VOLT
    //ADCSRA |= (1 << ADSC);         // start ADC measurement
    //while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    //thisvolt = ADC;   // add lobyte and hibyte
    thisvolt =analogRead(3);
    //bitSet(ADMUX, 0);//set bit 1 -> PB3 -> AMPER
    //ADCSRA |= (1 << ADSC);         // start ADC measurement
    //while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    //thisamp = ADC;   // add lobyte and hibyte
    thisamp =analogRead(2);
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
    numSamples++;
  }
  volt=maxV-minV;
  amp=maxA-minA;
  DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("A: " + String(amp));
  DEBUG_PRINT("MIN A: " + String(minA))
  DEBUG_PRINT("MAX A: " + String(maxA));;
  //DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("V: " + String(volt));
  DEBUG_PRINT("MIN V: " + String(minV));
  DEBUG_PRINT("MAX V: " + String(maxV));
  DEBUG_PRINT("numSamples " + String(numSamples));
}
void stampa(){
  //mySerial.begin(9600);
  delay(10);
  DEBUG_PRINT("Samples: " + String(numSamples));
  DEBUG_PRINT("MAX: " + String(max));
  DEBUG_PRINT("MIN " + String(min));
  //delay(5000);

}
