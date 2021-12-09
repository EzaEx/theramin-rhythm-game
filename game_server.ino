// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup()
{
  Wire.begin(4); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}


void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(1);        // sends five bytes
  Wire.write(92);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  delay(500);

  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(2);        // sends five bytes
  Wire.write(202);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  delay(500);

  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(0);        // sends five bytes
  Wire.write(42);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  delay(500);
}

void receiveEvent(int howMany)
{
  int c = Wire.read();
  String from = "From unidentified: ";
  
  if (c == 1){
    from = "From client 1: ";
  }
  if (c == 2){
    from = "From client 2: ";
  }
  
  while(Wire.available()) // loop through all but the last
  {
    int c = Wire.read(); // receive byte as a character
    Serial.print(from);
    Serial.print(c);         // print the character
    Serial.println();
  }
}
