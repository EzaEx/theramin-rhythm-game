#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //define lcd pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //instantiate lcd object w/ pins

//define component pins
const int potPin = 2;
const int buzzerPin = 13;
const int buzzerPin2 = 10;
const int buttonPin = 3;
const int noteTime = 400;

int rhythmOutput[32] = {};


void setup()
{
  Wire.begin(4); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent); // assign "recieve event" method to run when data is recieved
  Serial.begin(9600); // start serial for output
  
  lcd.begin(16, 2); //set up lcd

  for (byte i = 0; i < 32; i++) { //init array to be all - with one 0
    rhythmOutput[i] = 0;  
  }
}


void loop()
{
  lcd.clear();
  for (byte i = 0; i < 32; i++) {
    int col = i % 16;
    int row = i / 16;
    lcd.setCursor(col, row);
    if (rhythmOutput[i] != 0){
      lcd.print(rhythmOutput[i]);
    }
    else{
      lcd.print('-');
    }
  }

  if (rhythmOutput[0] != 0){
    int note = rhythmOutput[0] * 250 / 1.5;
    if(rhythmOutput[0] == rhythmOutput[1]){
      tone(buzzerPin, note, noteTime * 2);
      tone(buzzerPin2, note / 2, noteTime * 2);
    }
    else{
      tone(buzzerPin, note, noteTime);
      tone(buzzerPin2, note / 2, noteTime);
    }
    
  }
  
  queuePop(rhythmOutput, 32);

  int newNote = 0;
  if(random(1, 4) <= 2){
    newNote = random(1,7);
  }
  rhythmOutput[31] = newNote;
  
  delay(noteTime);

  Wire.beginTransmission(4); //transmit down wire 4
  Wire.write(0);        // send ID byte
  Wire.write(rhythmOutput[0]);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
}


void queuePop(int arr[], int arrSize){
  for (byte i = 0; i < arrSize; i++) {
    if (i < arrSize - 1){
      arr[i] = arr[i + 1];
    }
    else{
      arr[i] = 0;
    }
  }
}


void receiveEvent(int howMany)
{
  int c = Wire.read();
  String from = "From unknown: ";
  
  if (c == 1){
    from = "From client 1: ";
  }
  else if (c == 2){
    from = "From client 2: ";
  }
  
  while(Wire.available()) // loop through all remaining bytes
  {
    int c = Wire.read(); // receive byte as a character
    Serial.print(from);
    Serial.println(c);
  }
}
