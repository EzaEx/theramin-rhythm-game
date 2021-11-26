#include <Wire.h>
#include <LiquidCrystal.h>
#include "Ultrasonic.h"

const byte deviceAddress = 1;

const int pressurePadPin = 3;
const int rs = 12, en = 11, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool prevPressed = false;
int score = 0;
volatile byte currentNote = 0;

Ultrasonic ultrasonic(5);

void setup()
{
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);

  lcd.begin(16, 2);
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(currentNote);
  lcd.setCursor(0, 1);
  lcd.print(score);
  lcd.setCursor(12, 0);
  lcd.print(ultrasonic.MeasureInCentimeters());

  if (digitalRead(pressurePadPin) == false && !prevPressed)
  {
    prevPressed = true;
    if (currentNote == 0)
    {
      score--;
    }
    else
    {
      currentNote = 0;
      score++;
    }
  }
  else if (digitalRead(pressurePadPin) == true && prevPressed)
  {
    prevPressed = false;
  }
}

void receiveEvent(int howMany)
{
  byte b = Wire.read();

  if (b == deviceAddress)
  {
    while(Wire.available())
    {
      byte i = Wire.read();
    }
  }
  else if (b == 0)
  {
    currentNote = Wire.read();
  }
  else
  {
    while(Wire.available())
    {
      byte i = Wire.read();
    }
  }
}
