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

int gameState = 0;

int currentNoteIndex = 0;
short nextNoteCountdown = 0;
unsigned short lastNoteCheckTime = 0;

const PROGMEM unsigned short gravityNotes[] = {698, 587, 440, 587, 698, 587, 440, 587, 698, 523, 440, 523, 698, 523, 440, 523, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 587, 659, 698, 880, 784, 784, 880, 523, 587, 659, 698, 659, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 0, 1109, 1109, 1109, 0, 698, 698, 698, 880, 880, 784, 698, 0, 933, 933, 933, 784, 1047, 880, 1109, 587, 698, 880, 1047, 554, 659, 880, 1047, 587, 0, 294, 0, 0};
const PROGMEM unsigned short gravityTimes[] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 1056, 352, 1408, 528, 176, 352, 352, 1408, 1056, 352, 704, 704, 704, 704, 704, 704, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 704, 704, 704, 704, 176, 176, 176, 176, 176, 176, 176, 176, 352, 352, 352, 352, 0};

const PROGMEM unsigned short attackNotes[] = {698, 587, 440, 587, 698, 587, 440, 587, 698, 523, 440, 523, 698, 523, 440, 523, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 587, 659, 698, 880, 784, 784, 880, 523, 587, 659, 698, 659, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 0, 1109, 1109, 1109, 0, 698, 698, 698, 880, 880, 784, 698, 0, 933, 933, 933, 784, 1047, 880, 1109, 587, 698, 880, 1047, 554, 659, 880, 1047, 587, 0, 294, 0, 0};
const PROGMEM unsigned short attackTimes[] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 1056, 352, 1408, 528, 176, 352, 352, 1408, 1056, 352, 704, 704, 704, 704, 704, 704, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 704, 704, 704, 704, 176, 176, 176, 176, 176, 176, 176, 176, 352, 352, 352, 352, 0};

const PROGMEM unsigned short marioNotes[] = {698, 587, 440, 587, 698, 587, 440, 587, 698, 523, 440, 523, 698, 523, 440, 523, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 587, 659, 698, 880, 784, 784, 880, 523, 587, 659, 698, 659, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 0, 1109, 1109, 1109, 0, 698, 698, 698, 880, 880, 784, 698, 0, 933, 933, 933, 784, 1047, 880, 1109, 587, 698, 880, 1047, 554, 659, 880, 1047, 587, 0, 294, 0, 0};
const PROGMEM unsigned short marioTimes[] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 1056, 352, 1408, 528, 176, 352, 352, 1408, 1056, 352, 704, 704, 704, 704, 704, 704, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 704, 704, 704, 704, 176, 176, 176, 176, 176, 176, 176, 176, 352, 352, 352, 352, 0};

const PROGMEM unsigned short kirbyNotes[] = {698, 587, 440, 587, 698, 587, 440, 587, 698, 523, 440, 523, 698, 523, 440, 523, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 587, 659, 698, 880, 784, 784, 880, 523, 587, 659, 698, 659, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 0, 1109, 1109, 1109, 0, 698, 698, 698, 880, 880, 784, 698, 0, 933, 933, 933, 784, 1047, 880, 1109, 587, 698, 880, 1047, 554, 659, 880, 1047, 587, 0, 294, 0, 0};
const PROGMEM unsigned short kirbyTimes[] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 1056, 352, 1408, 528, 176, 352, 352, 1408, 1056, 352, 704, 704, 704, 704, 704, 704, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 704, 704, 704, 704, 176, 176, 176, 176, 176, 176, 176, 176, 352, 352, 352, 352, 0};

const PROGMEM unsigned short strangerNotes[] = {698, 587, 440, 587, 698, 587, 440, 587, 698, 523, 440, 523, 698, 523, 440, 523, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 659, 554, 440, 554, 587, 659, 698, 880, 784, 784, 880, 523, 587, 659, 698, 659, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 784, 880, 784, 698, 0, 698, 698, 698, 880, 880, 784, 698, 0, 880, 880, 880, 0, 1109, 1109, 1109, 0, 698, 698, 698, 880, 880, 784, 698, 0, 933, 933, 933, 784, 1047, 880, 1109, 587, 698, 880, 1047, 554, 659, 880, 1047, 587, 0, 294, 0, 0};
const PROGMEM unsigned short strangerTimes[] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 176, 1056, 352, 1408, 528, 176, 352, 352, 1408, 1056, 352, 704, 704, 704, 704, 704, 704, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 704, 704, 704, 704, 176, 176, 176, 176, 176, 176, 176, 176, 352, 352, 352, 352, 0};


const PROGMEM unsigned short *const songTable[] = {gravityNotes, gravityTimes, attackNotes, attackTimes, marioNotes, marioTimes, kirbyNotes, kirbyTimes, strangerNotes, strangerTimes}; 

byte randNotes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


byte upcomingNotes[16] = {};
const int numberOfSongs = 6;
char songTitle[numberOfSongs][17] = {"Rand Mode", "Mario", "Kirby", "Stranger", "Attack", "Gravity"};
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

  for (byte i = 0; i < 16; i++) { //init array to be all - with one 0
    //rhythmOutput[i] = 0;
  }
}


void loop()
{
  if (gameState ==  0)
  { //song select
    lcd.clear();

    int pot_val = analogRead(potPin);
    selectedSong = map(pot_val, 1023, 15, numberOfSongs - 1, 0); //min pot_val 15 as 0 not attainable

    lcd.setCursor(0, 0);
    lcd.print("Select a Song:  ");
    lcd.setCursor(0, 1);
    lcd.print(songTitle[selectedSong]);

    if (digitalRead(buttonPin) == HIGH)
    {
      if (selectedSong == 0)
      {
        gameState = 1;
      }
      else
      {
        setupGame();
        gameState = 2;
      }
    }
    delay(100);
  }
  else if (gameState == 1)
  { //random mode
    if (nextNoteCountdown > 0)
    {
      nextNoteCountdown -= millis() - lastNoteCheckTime;
      lastNoteCheckTime = millis();
    }
    else
    {
      lcd.clear();

      
      tone(buzzerPin, (int)randNotes[0] * 100, noteTime);
      nextNoteCountdown = noteTime;
      byte newNote = random(0, 7);

        queuePop(randNotes, 16);
        randNotes[15] = newNote;
        lcd.setCursor(0, 0);
        for (int i = 0; i < 16; i++)
        {
          lcd.print(randNotes[i]);
        }

        Wire.beginTransmission(4); //transmit down wire 4
        Wire.write(0);        // send ID byte
        Wire.write(newNote);              // sends one byte
        Wire.endTransmission();    // stop transmitting
    }
  }

  else if (gameState == 2)
  { //gameloop
    if (nextNoteCountdown > 0)
    {
      nextNoteCountdown -= millis() - lastNoteCheckTime;
      lastNoteCheckTime = millis();
    }
    else
    {
      lcd.clear();

      tone(buzzerPin, pgm_read_word(&(songTable[(selectedSong - 1) * 2]) + currentNoteIndex), pgm_read_word(&(songTable[(selectedSong - 1) * 2 + 1]) + currentNoteIndex));
      nextNoteCountdown = pgm_read_word(&(songTable[(selectedSong - 1) * 2 + 1]) + currentNoteIndex);
      byte newNote = frequencyToNote(pgm_read_word(&(songTable[(selectedSong - 1) * 2]) + currentNoteIndex));

        queuePop(upcomingNotes, 16);
        upcomingNotes[15] = frequencyToNote(pgm_read_word(&(songTable[(selectedSong - 1) * 2]) + currentNoteIndex + 15));
        lcd.setCursor(0, 0);
        for (int i = 0; i < 16; i++)
        {
          lcd.print(upcomingNotes[i]);
        }

        Wire.beginTransmission(4); //transmit down wire 4
        Wire.write(0);        // send ID byte
        Wire.write(newNote);              // sends one byte
        Wire.endTransmission();    // stop transmitting

      currentNoteIndex++;
      if (currentNoteIndex >= (sizeof(gravityNotes) / 2))
      {
        Serial.println(currentNoteIndex);
        gameState = 0;
      }
    }
  }
  else
  {
    Serial.println("Unrecognised Gamestate");
  }
  
  if (1 == 2)
  {
    while (true)
    {
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Select a Song:  ");

      int pot_val = analogRead(potPin);
      selectedSong = map(pot_val, 1023, 15, numberOfSongs - 1, 0); //min pot_val 15 as 0 not attainable

      lcd.setCursor(0, 1);
      lcd.print(songTitle[selectedSong]);

      if (digitalRead(buttonPin) == HIGH)
      {
        break;
      }

      delay(100);
    }
    if (selectedSong == numberOfSongs + 1)
    {
      while (true)
      { //we break when we recieve confirmation that either all or all but one players have been eliminated
        //endless mode

        lcd.clear();
        for (byte i = 0; i < 32; i++)
        {
          int col = i % 16;
          int row = i / 16;
          lcd.setCursor(col, row);
          //if (rhythmOutput[i] != 0)
          //{
            //lcd.print(rhythmOutput[i]);
          //}
          //else
          //{
            lcd.print('-');
          //}
        }

        //if (rhythmOutput[0] != 0)
        //{
          //int note = rhythmOutput[0] * 250 / 1.5;
          //if (rhythmOutput[0] == rhythmOutput[1])
          //{
            //tone(buzzerPin, note, noteTime * 2);
            //tone(buzzerPin2, note / 2, noteTime * 2);
          //}
          //else
          //{
            //tone(buzzerPin, note, noteTime);
            //tone(buzzerPin2, note / 2, noteTime);
          //}

        //}

        //queuePop(rhythmOutput, 32);

        int newNote = 0;
        if (random(1, 4) <= 2)
        {
          newNote = random(1, 7);
        }
      //rhythmOutput[31] = newNote;

        delay(noteTime);

        Wire.beginTransmission(4); //transmit down wire 4
        Wire.write(0);        // send ID byte
        //Wire.write(rhythmOutput[0]);              // sends one byte
        Wire.endTransmission();    // stop transmitting
      }
    }
  }
}


void queuePop(byte arr[], int arrSize)
{
  for (byte i = 0; i < arrSize; i++)
  {
    if (i < arrSize - 1)
    {
      arr[i] = arr[i + 1];
    }
    else
    {
      arr[i] = 0;
    }
  }
}

void setupGame()
{
  for (byte i = 0; i < 16; i++)
  {
    byte note = frequencyToNote(pgm_read_word(&(songTable[(selectedSong - 1) * 2]) + i));
    upcomingNotes[i] = note;
  }
}

byte frequencyToNote(int frequency)
{
  int newNote = min(pgm_read_word(&(songTable[(selectedSong - 1) * 2]) + currentNoteIndex) / 100, 8) - 2;
  return max(newNote, 0);
}

void receiveEvent(int howMany)
{
  int c = Wire.read();
  String from = "From unknown: ";

  if (c == 1)
  {
    from = "From client 1: ";
  }
  else if (c == 2)
  {
    from = "From client 2: ";
  }

  while (Wire.available()) // loop through all remaining bytes
  {
    int c = Wire.read(); // receive byte as a character
    Serial.print(from);
    Serial.println(c);
  }
}
