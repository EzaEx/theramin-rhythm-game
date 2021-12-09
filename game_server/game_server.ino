#include <Wire.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //define lcd pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //instantiate lcd object w/ pins

//const int CS = 2,SCK = 8, MDSI = 9, MISD = 10; //define microSD reader pins
//define component pins
const int potPin = 3;
const int buzzerPin = 13;
const int buzzerPin2 = 10;
const int buttonPin = 3;
const int noteTime = 400;

int rhythmOutput[32] = {};

unsigned short melody[] = { 
784, 698, 622, 587, 466, 392, 523, 587, 622, 698, 587, 0, 1047, 784, 622, 587, 523, 523, 587, 622, 523, 466, 523, 392, 1047, 784, 622, 587, 523, 523, 587, 622, 698, 587, 466, 523, 392, 523, 587, 622, 698, 784, 831, 1047, 1047, 784, 622, 587, 523, 523, 587, 622, 523, 466, 523, 392, 1047, 784, 622, 698, 784, 523, 587, 698, 587, 466, 523, 349, 349, 415, 523, 622, 587, 523, 392, 349, 349, 415, 523, 622, 698, 784, 831, 784, 698, 622, 698, 784, 523, 784, 698, 698, 698, 622, 587, 622, 698, 622, 698, 784, 622, 349, 349, 415, 523, 622, 587, 523, 392, 349, 349, 415, 523, 622, 698, 784, 831, 784, 698, 831, 933, 1047, 784, 622, 523, 587, 587, 587, 698, 587, 466, 392, 466, 523, 466, 622, 622, 622, 622, 622, 698, 784, 784, 784, 698, 622, 587, 587, 587, 587, 587, 622, 587, 622, 587, 523, 523, 523, 523, 523, 587, 622, 622, 622, 587, 523, 466, 466, 466, 466, 523, 587, 523, 466, 415, 415, 415, 415, 466, 523, 523, 523, 466, 415, 392, 392, 392, 622, 698, 622, 466, 622, 622, 622, 622, 698, 740, 740, 740, 831, 740, 698, 698, 698, 698, 740, 698, 784, 698, 622, 622, 622, 622, 622, 698, 784, 784, 784, 698, 622, 587, 587, 587, 587, 587, 622, 587, 622, 587, 523, 523, 523, 523, 523, 587, 622, 622, 622, 587, 523, 466, 466, 466, 466, 523, 587, 523, 466, 415, 415, 415, 415, 466, 523, 523, 523, 587, 698, 784, 784, 784, 784, 933, 784, 784, 698, 622, 587, 698, 784, 587, 698, 784, 587, 698, 587, 698, 784, 1047, 988, 523, 587, 622, 698, 0 
};
unsigned short noteDurations[] = { 
308, 154, 154, 154, 154, 308, 154, 154, 154, 154, 308, 308, 616, 616, 308, 308, 616, 308, 308, 308, 308, 308, 308, 616, 616, 616, 308, 308, 308, 154, 154, 308, 308, 308, 308, 308, 308, 154, 77, 77, 77, 77, 77, 77, 616, 616, 308, 308, 616, 308, 308, 308, 308, 308, 308, 616, 616, 616, 308, 308, 308, 308, 308, 308, 308, 308, 1232, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 616, 462, 154, 308, 154, 154, 308, 308, 616, 231, 77, 154, 154, 308, 154, 154, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 616, 462, 154, 308, 154, 154, 308, 308, 308, 308, 231, 77, 154, 154, 154, 154, 154, 154, 924, 308, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 77, 77, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 308, 308, 154, 77, 77, 231, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 77, 77, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 308, 77, 77, 77, 77, 0 
};
int currentNoteIndex = 0;
short nextNoteCountdown = 0;
unsigned short lastNoteCheckTime = 0;

const int numberOfSongs = 5;
char songTitle[numberOfSongs + 1][17] ={"1,2, Oatmeal    ","Stranger Things ","Mario ft C.Pratt","Tubular Bells I ","Endless Mode    "};
char fileNames[numberOfSongs][4] = {"oat","str","pra","tub"};
int selectedSong;
int previousSong;
File songBank;

void setup()
{
  Wire.begin(4); // join i2c bus (address optional for master)
  Wire.onReceive(receiveEvent); // assign "recieve event" method to run when data is recieved
  Serial.begin(9600); // start serial for output
  pinMode(buttonPin, INPUT);
  
  lcd.begin(16, 2); //set up lcd
  
  for (byte i = 0; i < 32; i++) { //init array to be all - with one 0
    rhythmOutput[i] = 0;  
  }
}


void loop()
{
  lcd.clear();
  lcd.print("Select a Song:  ");
  previousSong = 0;
  while (true){
    
    lcd.setCursor(0,1);
    selectedSong = map(analogRead(potPin),1023,0,0,numberOfSongs);
    if (previousSong != selectedSong){
      lcd.print(songTitle[selectedSong]);
      previousSong = selectedSong;
    }
    
    if (digitalRead(buttonPin) == HIGH){
      break;
    }
  }
  if (selectedSong == numberOfSongs + 1){
    while (true){ //we break when we recieve confirmation that either all or all but one players have been eliminated
      //endless mode
    
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

    Wire.beginTransmission(4); //transmit down wire 4
    Wire.write(0);        // send ID byte
    Wire.write(rhythmOutput[0]);              // sends one byte  
    Wire.endTransmission();    // stop transmitting
  }

  rhythmOutput[31] = newNote;
  
  delay(noteTime);

  Wire.beginTransmission(4); //transmit down wire 4
  Wire.write(0);        // send ID byte
  Wire.write(rhythmOutput[0]);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
    }
  }else{
    songBank = SD.open(fileNames[selectedSong]);
    // = songBank.read(); not sure what kind of data structure we need
    songBank.close();
    //Then we insert the code that Sam made
  }
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
