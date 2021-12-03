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

unsigned short melody[] = { 
196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 147, 116, 131, 0, 110, 116, 116, 98, 110, 0, 87, 147, 116, 131, 0, 110, 116, 116, 98, 110, 0, 87, 175, 139, 156, 0, 131, 139, 139, 116, 131, 0, 104, 175, 139, 156, 0, 131, 139, 139, 116, 131, 0, 110, 0, 0, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 139, 147, 147, 147, 147, 165, 175, 165, 147, 139, 110, 139, 147, 110, 139, 165, 131, 165, 175, 131, 175, 185, 196, 196, 196, 196, 196, 147, 196, 175, 165, 131, 0, 175, 196, 196, 196, 196, 196, 147, 196, 196, 185, 185, 196, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 196, 196, 175, 147, 0, 175, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 220, 175, 175, 294, 262, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 196, 196, 175, 147, 0, 175, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 175, 196, 233, 220, 0, 196, 175, 196, 196, 196, 0 
};
unsigned short noteDurations[] = { 
328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 1312, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 492, 164, 164, 164, 164, 164, 164, 164, 656, 246, 82, 328, 246, 82, 328, 246, 82, 328, 246, 82, 492, 164, 164, 164, 164, 164, 328, 328, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 328, 328, 492, 164, 656, 164, 164, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 328, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 164, 164, 164, 164, 492, 656, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 328, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 328, 164, 328, 164, 164, 164, 164, 164, 164, 1312, 1312, 0 
};
int currentNoteIndex = 0;
short nextNoteCountdown = 0;
unsigned short lastNoteCheckTime = 0;

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

  if (nextNoteCountdown > 0)
  {
    nextNoteCountdown -= millis() - lastNoteCheckTime;
    lastNoteCheckTime = millis();
    Serial.println(nextNoteCountdown);
  }
  else
  {
    lcd.clear();
    currentNoteIndex++;
    if (currentNoteIndex >= sizeof(melody) / 2)
    {
      currentNoteIndex = 0;
    }
    tone(buzzerPin, melody[currentNoteIndex], noteDurations[currentNoteIndex]);
    nextNoteCountdown = noteDurations[currentNoteIndex];
    Serial.println(nextNoteCountdown);

    queuePop(rhythmOutput, 32);

    int newNote = 0;
    if(random(1, 4) <= 2){
      newNote = random(1,7);
    }
    rhythmOutput[31] = newNote;
  }

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
