#include <Wire.h>
#include <LiquidCrystal.h>
#include "Ultrasonic.h"

const byte deviceAddress = 1; //identifies this client device (value 1 through 3)

const int rs = 12, en = 11, d4 = 6, d5 = 7, d6 = 8, d7 = 9; //define lcd pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //instantiate lcd object w/ pins

//define component pins
const int pressurePadPin = 3;
Ultrasonic ultrasonic(5);

//define game variables
bool prevPressed = false;
int score = 0;
volatile byte currentNote = 0;
unsigned long timeReceived = 0;


void setup()
{
  Wire.begin(4); // join communication bus
  Wire.onReceive(receiveEvent); // assign "recieve event" method to run when data is recieved
  Serial.begin(9600); // start serial for output
  pinMode(3, INPUT_PULLUP);
  
  lcd.begin(16, 2); //setup lcd

  //display this player's ID
  lcd.setCursor(3, 0);
  lcd.print("PLAYER ");
  lcd.setCursor(10,0);
  lcd.print(deviceAddress);
}

void loop()
{
  //print the current playing note (from server)
  lcd.setCursor(0, 0);
  lcd.print(currentNote);
  
  // print ultrasensor reading as a 'note'
  lcd.setCursor(12, 0);
  int height = ultrasonic.MeasureInCentimeters();
  if (height/5 < 1000) lcd.print(" ");
  if (height/5 < 100) lcd.print(" ");
  if (height/5 < 10) lcd.print(" ");
  lcd.print(height / 5);

  if (digitalRead(pressurePadPin) == false && !prevPressed) //pressure pad was pushed since last cycle
  {
    prevPressed = true; 
    unsigned long timePressed = millis();
    if (currentNote == 0)
    {
      score -= 15;
      score = max(score, 0); //prevent negative score
      displayScore();
    }
    else //there is currently a playing note
    {
      // calculate points in regards to the ultrasensor and how fast the player was
      unsigned long playerSpeed = timePressed - timeReceived;
      int points = 50 - min(abs(currentNote - height/5) * 10 - (playerSpeed / 20), 49);
      currentNote = 0;
      score += points;
      displayScore();
      
      Wire.beginTransmission(4); //transmit down wire pin 4
      Wire.write(deviceAddress);  // use ID byte of deviceAddress (meaning value comes from this client)
      
      //send integer score as two seperate bytes
      Wire.write(score >> 8);
      Wire.write(score & 255);
      Wire.endTransmission();
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
  
  byte ID = Wire.read(); //read ID byte

  if (ID == deviceAddress) // direct singnal to this client
  {
    //read all remaining values
    while(Wire.available())
    {
      byte i = Wire.read();
    }
  }
  
  else if (ID == 0) //a broadcast to all clients
  {
    //must be current note data, so read as current note
    currentNote = Wire.read();
  }
  
  else if (ID == 255) //a broacast to reset scores
  {
    //reset and display new score
    score = 0;
    displayScore();
  }
  
  else //unknown ID
  {
    //read all remaining values
    while(Wire.available())
    {
      byte i = Wire.read();
    }
  }
}

void displayScore(){
  //displays current score on lcd
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(4, 1);
  lcd.print(score);
}
