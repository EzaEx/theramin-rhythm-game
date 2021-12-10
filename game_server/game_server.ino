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

const PROGMEM unsigned short attackNotes[] = { 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 147, 116, 131, 0, 110, 116, 116, 98, 110, 0, 87, 147, 116, 131, 0, 110, 116, 116, 98, 110, 0, 87, 175, 139, 156, 0, 131, 139, 139, 116, 131, 0, 104, 175, 139, 156, 0, 131, 139, 139, 116, 131, 0, 110, 0, 0, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 131, 220, 175, 196, 0, 165, 175, 175, 147, 165, 0, 131, 220, 175, 196, 0, 165, 175, 175, 165, 147, 0, 139, 147, 147, 147, 147, 165, 175, 165, 147, 139, 110, 139, 147, 110, 139, 165, 131, 165, 175, 131, 175, 185, 196, 196, 196, 196, 196, 147, 196, 175, 165, 131, 0, 175, 196, 196, 196, 196, 196, 147, 196, 196, 185, 185, 196, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 196, 196, 175, 147, 0, 175, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 220, 175, 175, 294, 262, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 196, 233, 220, 0, 196, 196, 196, 175, 147, 0, 175, 196, 233, 220, 0, 175, 196, 196, 196, 233, 220, 0, 175, 175, 196, 233, 220, 0, 196, 175, 196, 196, 196, 0 };
const PROGMEM unsigned short attackTimes[] = { 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 1312, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 328, 164, 164, 492, 492, 164, 164, 164, 164, 164, 164, 164, 656, 246, 82, 328, 246, 82, 328, 246, 82, 328, 246, 82, 492, 164, 164, 164, 164, 164, 328, 328, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 328, 328, 492, 164, 656, 164, 164, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 328, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 164, 164, 164, 164, 492, 656, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 328, 164, 164, 164, 492, 328, 164, 164, 164, 328, 164, 164, 164, 164, 164, 164, 328, 164, 328, 164, 164, 164, 164, 164, 164, 1312, 1312, 0 };

const PROGMEM unsigned short marioNotes[] = { 659, 659, 659, 0, 523, 659, 784, 392, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 784, 740, 698, 622, 659, 0, 698, 1047, 0, 698, 1047, 698, 1047, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 622, 0, 587, 0, 523, 0, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 784, 740, 698, 622, 659, 0, 698, 1047, 0, 698, 1047, 698, 1047, 0, 784, 740, 698, 622, 659, 0, 415, 440, 523, 0, 440, 523, 587, 0, 622, 0, 587, 0, 523, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 523, 523, 523, 0, 523, 587, 659, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 659, 659, 659, 0, 523, 659, 784, 392, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 523, 0, 392, 0, 330, 0, 440, 0, 494, 0, 466, 440, 392, 659, 784, 880, 698, 784, 0, 659, 0, 523, 587, 494, 0, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 880, 880, 880, 784, 698, 659, 523, 440, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 698, 698, 698, 659, 587, 523, 392, 392, 262, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 523, 523, 523, 0, 523, 587, 659, 0, 523, 523, 523, 0, 523, 587, 659, 523, 440, 392, 659, 659, 659, 0, 523, 659, 784, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 880, 880, 880, 784, 698, 659, 523, 440, 392, 659, 523, 392, 0, 415, 440, 698, 698, 440, 494, 698, 698, 698, 659, 587, 523, 392, 392, 262, 0 };
const PROGMEM unsigned short marioTimes[] = { 150, 300, 150, 150, 150, 300, 600, 600, 300, 150, 150, 300, 300, 150, 150, 150, 150, 150, 150, 300, 200, 200, 200, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 300, 300, 150, 150, 150, 150, 150, 150, 300, 200, 200, 200, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 600, 600, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 300, 600, 600, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 600, 600, 300, 150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 300, 600, 600, 150, 300, 150, 150, 150, 300, 150, 300, 150, 600, 150, 300, 150, 150, 150, 150, 150, 1200, 150, 300, 150, 150, 150, 300, 150, 300, 150, 600, 150, 300, 150, 150, 150, 300, 600, 600, 300, 150, 150, 300, 300, 150, 150, 150, 150, 150, 150, 300, 200, 200, 200, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 300, 300, 150, 150, 150, 150, 150, 150, 300, 200, 200, 200, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 150, 300, 300, 150, 300, 150, 600, 200, 200, 200, 200, 200, 200, 150, 300, 150, 600, 150, 300, 150, 300, 300, 150, 300, 150, 600, 150, 300, 150, 200, 200, 200, 150, 300, 150, 600, 150, 300, 150, 150, 150, 300, 150, 300, 150, 600, 150, 300, 150, 150, 150, 150, 150, 1200, 150, 300, 150, 150, 150, 300, 150, 300, 150, 600, 150, 300, 150, 150, 150, 300, 600, 600, 150, 300, 150, 300, 300, 150, 300, 150, 600, 200, 200, 200, 200, 200, 200, 150, 300, 150, 600, 150, 300, 150, 300, 300, 150, 300, 150, 600, 150, 300, 150, 200, 200, 200, 150, 300, 150, 600, 0 };

const PROGMEM unsigned short kirbyNotes[] = { 784, 698, 622, 587, 466, 392, 523, 587, 622, 698, 587, 0, 1047, 784, 622, 587, 523, 523, 587, 622, 523, 466, 523, 392, 1047, 784, 622, 587, 523, 523, 587, 622, 698, 587, 466, 523, 392, 523, 587, 622, 698, 784, 831, 1047, 1047, 784, 622, 587, 523, 523, 587, 622, 523, 466, 523, 392, 1047, 784, 622, 698, 784, 523, 587, 698, 587, 466, 523, 349, 349, 415, 523, 622, 587, 523, 392, 349, 349, 415, 523, 622, 698, 784, 831, 784, 698, 622, 698, 784, 523, 784, 698, 698, 698, 622, 587, 622, 698, 622, 698, 784, 622, 349, 349, 415, 523, 622, 587, 523, 392, 349, 349, 415, 523, 622, 698, 784, 831, 784, 698, 831, 933, 1047, 784, 622, 523, 587, 587, 587, 698, 587, 466, 392, 466, 523, 466, 622, 622, 622, 622, 622, 698, 784, 784, 784, 698, 622, 587, 587, 587, 587, 587, 622, 587, 622, 587, 523, 523, 523, 523, 523, 587, 622, 622, 622, 587, 523, 466, 466, 466, 466, 523, 587, 523, 466, 415, 415, 415, 415, 466, 523, 523, 523, 466, 415, 392, 392, 392, 622, 698, 622, 466, 622, 622, 622, 622, 698, 740, 740, 740, 831, 740, 698, 698, 698, 698, 740, 698, 784, 698, 622, 622, 622, 622, 622, 698, 784, 784, 784, 698, 622, 587, 587, 587, 587, 587, 622, 587, 622, 587, 523, 523, 523, 523, 523, 587, 622, 622, 622, 587, 523, 466, 466, 466, 466, 523, 587, 523, 466, 415, 415, 415, 415, 466, 523, 523, 523, 587, 698, 784, 784, 784, 784, 933, 784, 784, 698, 622, 587, 698, 784, 587, 698, 784, 587, 698, 587, 698, 784, 1047, 988, 523, 587, 622, 698, 0 };
const PROGMEM unsigned short kirbyTimes[] = { 308, 154, 154, 154, 154, 308, 154, 154, 154, 154, 308, 308, 616, 616, 308, 308, 616, 308, 308, 308, 308, 308, 308, 616, 616, 616, 308, 308, 308, 154, 154, 308, 308, 308, 308, 308, 308, 154, 77, 77, 77, 77, 77, 77, 616, 616, 308, 308, 616, 308, 308, 308, 308, 308, 308, 616, 616, 616, 308, 308, 308, 308, 308, 308, 308, 308, 1232, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 616, 462, 154, 308, 154, 154, 308, 308, 616, 231, 77, 154, 154, 308, 154, 154, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 616, 462, 154, 308, 154, 154, 308, 308, 308, 308, 231, 77, 154, 154, 154, 154, 154, 154, 924, 308, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 77, 77, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 308, 308, 154, 77, 77, 231, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 77, 77, 462, 77, 77, 154, 77, 77, 154, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 462, 77, 77, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 77, 77, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 308, 77, 77, 77, 77, 0 };

const PROGMEM unsigned short strangerNotes[] = { 0, 165, 247, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 41, 82, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 392, 82, 123, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 0, 73, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 41, 82, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 0, 73, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 41, 49, 82, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 73, 62, 0, 62, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 62, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 0, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 330, 392, 659, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 330, 392, 392, 494, 659, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 392, 494, 659, 587, 494, 41, 82, 494, 82, 98, 123, 131, 123, 98, 82, 82, 0, 0 };
const PROGMEM unsigned short strangerTimes[] = { 3008, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 752, 2256, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 1504, 752, 752, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 1504, 752, 752, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 1504, 564, 376, 188, 376, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 3008, 3008, 3008, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 752, 752, 2256, 2256, 2256, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 1504, 1504, 1504, 752, 752, 3008, 3008, 3008, 188, 188, 188, 188, 188, 188, 376, 1504, 3008, 0 };




const unsigned short* const songTable[] = {gravityNotes, gravityTimes, attackNotes, attackTimes, marioNotes, marioTimes, kirbyNotes, kirbyTimes, strangerNotes, strangerTimes}; 

byte randNotes[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int playerScores[3] = {0, 0, 0};

byte upcomingNotes[16] = {};
const int numberOfSongs = 6;
char songTitle[numberOfSongs][17] = {"Rand Mode", "Gravity", "Attack", "Mario", "Kirby", "Stranger"};
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
        Serial.println(selectedSong);
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

        int highest_score = -10000;
        int highest_player = -1;
        for (int i = 0; i < 3; i++)
        {
          if (playerScores[i] >highest_score){
            highest_score = playerScores[i];
            highest_player = i;
          }
          else if (playerScores[i] == highest_score){
            highest_player = -1;
          }
        }
        lcd.setCursor(0, 1);
        if (highest_player != -1){
          lcd.print("P");
          lcd.print(highest_player);
          lcd.print(" in lead");
        }
        else{
          lcd.print("tie!");
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

      tone(buzzerPin, pgm_read_word((songTable[(selectedSong - 1) * 2]) + currentNoteIndex), pgm_read_word((songTable[(selectedSong - 1) * 2 + 1]) + currentNoteIndex));
      nextNoteCountdown = pgm_read_word((songTable[(selectedSong - 1) * 2 + 1]) + currentNoteIndex);
      byte newNote = frequencyToNote(pgm_read_word((songTable[(selectedSong - 1) * 2]) + currentNoteIndex));

        queuePop(upcomingNotes, 16);
        upcomingNotes[15] = frequencyToNote(pgm_read_word((songTable[(selectedSong - 1) * 2]) + currentNoteIndex + 15 * 8));
        lcd.setCursor(0, 0);
        for (int i = 0; i < 16; i++)
        {
          lcd.print(upcomingNotes[i]);
        }
       int highest_score = -10000;
        int highest_player = -1;
        for (int i = 0; i < 3; i++)
        {
          if (playerScores[i] >highest_score){
            highest_score = playerScores[i];
            highest_player = i;
          }
          else if (playerScores[i] == highest_score){
            highest_player = -1;
          }
        }
        lcd.setCursor(0, 1);
        if (highest_player != -1){
          lcd.print("P");
          lcd.print(highest_player + 1);
          lcd.print(" in lead");
        }
        else{
          lcd.print("tie!");
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
    byte note = frequencyToNote(pgm_read_word((songTable[(selectedSong - 1) * 2]) + i));
    upcomingNotes[i] = note;
  }
}

byte frequencyToNote(int frequency)
{
  int newNote = min(pgm_read_word((songTable[(selectedSong - 1) * 2]) + currentNoteIndex) / 100, 8) - 2;
  return max(newNote, 0);
}

void receiveEvent(int howMany)
{
  int index = Wire.read();

  int score = Wire.read() << 8;
  score |= Wire.read();

  playerScores[index - 1] = score;

  while(Wire.available()){
    Wire.read();
  }
  
}
