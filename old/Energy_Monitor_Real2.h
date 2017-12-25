#ifndef energy_m_h
#define energy_m_h
#include <Filters.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

float measure_current(unsigned long printPeriod)
  {// in milliseconds
  float testFrequency = 60;                     // test signal frequency (Hz)
  float windowLength = 20 / testFrequency;   // how long to average the signal, for statistist
  int sensorValue = 0;
  float intercept = 0;//-0.1129; // to be adjusted based on calibration testing
  float slope = 0;//0.0405; // to be adjusted based on calibration testing
  float current_amps; // estimated actual current in amps

  int voltage =225; //change this to 110 if you have 110Vac
  // Track time in milliseconds since last reading
  unsigned long previousMillis = 0;
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
  previousMillis = millis();   // update time

  while ((unsigned long)(millis() - previousMillis) < printPeriod) {
    sensorValue = ads.readADC_SingleEnded(0);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
  }
  // display current values to the screen
  Serial.print( "\n" );
  // output sigma or variation values associated with the inputValue itsel
  Serial.print( "\tSigma: " ); Serial.print( inputStats.sigma() );
  // convert signal sigma value to current in amps
  current_amps = intercept + slope * inputStats.sigma();
  Serial.print( "\tCurrent: " ); Serial.print( current_amps ,4);Serial.print( " A rms");
  Serial.print( "\tPower Comsuption: " ); Serial.print( current_amps * voltage );Serial.print( " W");
}


#endif
