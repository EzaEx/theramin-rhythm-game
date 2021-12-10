#include <Wire.h>
#include <LiquidCrystal.h>
#include "Ultrasonic.h"

// need to change this variable for each client (1,2,3)
const byte deviceAddress = 2;

const int pressurePadPin = 3;
const int rs = 12, en = 11, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool prevPressed = false;
int score = 0;
volatile byte currentNote = 0;

unsigned long timeReceived = 0;

//bool playingSong = false; variable we might need later?

Ultrasonic ultrasonic(5);

void setup()
{
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("PLAYER ");
  lcd.setCursor(10,0);
  lcd.print(deviceAddress);
  
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print(currentNote);
  // print score
  lcd.setCursor(0, 10);
  if (score < 100000) lcd.print(" ");
  if (score < 10000) lcd.print(" ");
  if (score < 1000) lcd.print(" ");
  if (score < 100) lcd.print(" ");
  if (score < 10) lcd.print(" ");
  lcd.print(score);
  // print ultrasensor reading
  lcd.setCursor(12, 0);
  int height = ultrasonic.MeasureInCentimeters();
  if (height/5 < 1000) lcd.print(" ");
  if (height/5 < 100) lcd.print(" ");
  if (height/5 < 10) lcd.print(" ");
  lcd.print(height/5);

  if (digitalRead(pressurePadPin) == false && !prevPressed)
  {
    prevPressed = true;
    unsigned long timePressed = millis();
    if (currentNote == 0)
    {
      score -= 300;
      score = max(score, 0);
    }
    else
    {
      // calculate points in regards to the ultrasensor and how fast the player was
      unsigned long playerSpeed = timePressed - timeReceived;
      int points = 1000 - min(abs(currentNote - height/5) * 200 - playerSpeed, 1000);
      currentNote = 0;
      score += points;
      Wire.beginTransmission(4); //transmit down wire 4
      Wire.write(deviceAddress);        // send ID byte
      Wire.write(score >> 8);
      Wire.write(score & 255);
      Wire.endTransmission();    // stop transmitting
    }
  }
  else if (digitalRead(pressurePadPin) == true && prevPressed)
  {
    prevPressed = false;
  }
}

void receiveEvent(int howMany)
{
  timeReceived = millis();
  byte b = Wire.read();
  Serial.println(b);

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
