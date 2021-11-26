#include <Wire.h>
#include <LiquidCrystal.h>
#include "./Libraries/Ultrasonic.h"

const byte deviceAddress = 1;

const int pressurePadPin = 3;
const int rs = 12, en = 11, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile int score = 0;

Ultrasonic ultrasonic(5);

void setup()
{
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pressurePadPin), pressurePadHit, FALLING);

  lcd.begin(16, 2);
}

void loop()
{
  lcd.setCursor(0, 1);
  
  delay(500);
  Wire.beginTransmission(4);
  Wire.write(deviceAddress);
  Wire.write(13);
  Wire.endTransmission();
}

void pressurePadHit()
{
  score += ultrasonic.MeasureInCentimeters();
  lcd.clear();
  lcd.print(score);
}

void receiveEvent(int howMany)
{
  byte b = Wire.read();

  if (b == deviceAddress or b == 0)
  {
    while(Wire.available())
    {
      int i = Wire.read();
      lcd.clear();
      lcd.print(i);
    }
  }
  else
  {
    while(Wire.available())
    {
      int i = Wire.read();
    }
  }
}
