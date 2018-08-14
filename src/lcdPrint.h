#ifndef lcdPrint_h
#define lcdPrint_h
#include <TinyWireM.h>
#include <LiquidCrystal_I2C.h>
#include <cxonfig.h>
LiquidCrystal_I2C lcd(0x27,16,2);
char _buffer[16];
void lcdsetup()
{
  delay(50);
  lcd.init();                      // initialize the lcd
  lcd.backlight();
}
void lcdPrint(EnergyData ener)
{
  lcd.clear();
  sprintf(_buffer, "RPow %.2f W",ener.realPower);
  lcd.setCursor(0,0);
  lcd.print(_buffer);
  sprintf(_buffer, "APow %.2f W",ener.apparentPower);
  lcd.setCursor(0,1);
  lcd.print(_buffer);
  delay(2000);
  //
  lcd.clear();
  sprintf(_buffer, "Tens %.2f V",ener.supplyVoltage);
  lcd.setCursor(0,0);
  lcd.print(_buffer);
  sprintf(_buffer, "Corr %.2f A",ener.Irms);
  lcd.setCursor(0,1);
  lcd.print(_buffer);
  delay(2000);
}
#endif
