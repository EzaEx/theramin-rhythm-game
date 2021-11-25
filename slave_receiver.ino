#include <Wire.h>
#include <LiquidCrystal.h>

const byte deviceAddress = 1;

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  lcd.begin(16, 2);
  lcd.print("hello, world!");
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

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  byte b = Wire.read();

  if (b == deviceAddress or b == 0)
  {
    while(Wire.available()) // loop through all but the last
    {
      int i = Wire.read(); // receive byte as a character
      lcd.clear();
      lcd.print(i);         // print the character
    }
  }
  else
  {
    while(Wire.available()) // loop through all but the last
    {
      int i = Wire.read(); // receive byte as a character
    }
  }
  
}
