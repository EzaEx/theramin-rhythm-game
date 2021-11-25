// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

const byte deviceAddress = 2;

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(500);
  Wire.beginTransmission(4);
  Wire.write(deviceAddress);
  Wire.write(24);
  Wire.endTransmission();
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  byte b = Wire.read();

  if (b == 0 || b == deviceAddress)
  {
      while(Wire.available())
      {
        byte b = Wire.read();
        Serial.print(b);
      }
      Serial.println();
  }
  else
  {
    while(Wire.available())
    {
      byte b = Wire.read();
    }
  }
}
