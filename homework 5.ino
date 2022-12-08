#include <LiquidCrystal.h>
#include "LedControl.h"
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 2;
const byte d7 = 4;
const byte LCDContrastPin = 3;
const byte LCDBrightnessPin = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte joyXPin = A1;
const byte joyYPin = A0;
const byte joySWPin = A2;

const byte MAX_DIFFICULTY = 3;
const byte MIN_DIFFICULTY = 1;

const byte MAX_LCD_BRIGHTNESS_LVL = 5;
const byte MIN_LCD_BRIGHTNESS_LVL = 1;

const byte MAX_MATRIX_BRIGHTNESS_LVL = 5;
const byte MIN_MATRIX_BRIGHTNESS_LVL = 1;

const byte ON = 1;
const byte OFF = 0;

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER


byte matrixBrightness = 2;

bool soundOn = true;
byte soundState = ON;
byte lastSoundState = OFF;

int LCDBrightnessLvl = 2;
int lastLCDBrightnessLvl = 0;

int MatrixBrightnessLvl = 3;
int lastMatrixBrightnessLvl = 0;

int difficultyLvl = 2;
int lastDifficultyLvl = 0;

int LCDContrastValue = 120;
int LCDBrightnessValue = 100;

int MatrixBrightnessValue;

const byte START = 0;
const byte DIFFICULTY = 1;
const byte LCD_BRIGHTNESS = 2;
const byte MATRIX_BRIGHTNESS = 3;
const byte SOUNDS = 4;
const byte ABOUT = 5;

const int LEFT = -1;
const int RIGHT = 1;
const int UP = 1;
const int DOWN = -1;
const int NEUTRAL = 0;
int valueOX, valueOY, valueSW;
int lastValueSW;
int lastValue;

const byte MENU = 0;
const byte GAME = 1;

const byte SETTINGS = 0;
const byte MODIFY_SETTINGS = 1;

byte currentMenuState = SETTINGS;

bool SWjustPressed = false;

bool programState = MENU;

int settingsState = START;
// 0 -> START
// 1 -> DIFFICULTY
// 2 -> LCD_BRIGHTNESS
// 3 -> MATRIX_BRIGHTNESS
// 4 -> SOUNDS
// 5 -> ABOUT







void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  pinMode(joySWPin, INPUT_PULLUP);
  
  pinMode(LCDContrastPin, OUTPUT);
  pinMode(LCDBrightnessPin, OUTPUT);

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  analogWrite(LCDBrightnessPin, LCDBrightnessValue);

  analogWrite(LCDContrastPin, LCDContrastValue);
  lcd.begin(16, 2);
  // initialize the serial communications:
  lcd.write("Welcome, player");
  delay(500);
  lcd.clear();
  
  writeMenuOnLCD();

}


void loop() {
// when characters arrive over the serial port...

  if (programState == MENU){ // menu state
    menuState();
  }
  else { // in game state

  }
  
}

void menuState(){
  
  valueSW = digitalRead(joySWPin);
  if (valueSW == LOW && lastValueSW == HIGH)
    {
      if (currentMenuState == SETTINGS)
            {
              SWjustPressed = true;
              currentMenuState = MODIFY_SETTINGS;
            }
      else {
        writeMenuOnLCD();
        currentMenuState = SETTINGS;
        
        }
    }
  lastValueSW = valueSW;

  if (currentMenuState == SETTINGS){
    valueOY = analogRead(joyYPin);
     

    if (valueOY < 200)
      valueOY = DOWN;
    else if (valueOY > 1000)
      valueOY = UP;
    else valueOY = NEUTRAL;

    if (lastValue != valueOY){
      lastValue = valueOY;
      settingsState += valueOY;
      if (settingsState > 5)
        settingsState = 0;
      if (settingsState < 0)
        settingsState = 5;
      writeMenuOnLCD();
      lastDifficultyLvl = -1;

    }
    
  }

  else if (currentMenuState == MODIFY_SETTINGS){
      modifySettingsMenu();
  }
 
  

}


void modifySettingsMenu(){
  switch(settingsState){
    case(0):
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
        lc.setLed(0, row, col, false); // turns off LED at col, row
        delay(25);
         }
      }
      break;
    
    case(1):
      if (SWjustPressed){
        lcd.clear();
          lcd.setCursor(3, 0);
          lcd.write("DIFFICULTY");
          lcd.setCursor(7, 1);
          lcd.print(difficultyLvl);
        SWjustPressed = !SWjustPressed;
      }
      if (lastDifficultyLvl != difficultyLvl){
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.write("DIFFICULTY");
          lcd.setCursor(7, 1);
          lcd.print(difficultyLvl);
          lastDifficultyLvl = difficultyLvl;
      }
      valueOY = analogRead(joyYPin);

      if (valueOY < 200)
        valueOY = DOWN;
      else if (valueOY > 1000)
        valueOY = UP;
      else valueOY = NEUTRAL;

      if (lastValue != valueOY){
          
        lastValue = valueOY;
        difficultyLvl -= valueOY;
        if (difficultyLvl > MAX_DIFFICULTY)
          difficultyLvl = MAX_DIFFICULTY;
        if (difficultyLvl < MIN_DIFFICULTY)
          difficultyLvl = MIN_DIFFICULTY;
      }

      break;
    
    case(2):
      if (SWjustPressed){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("BRIGHTNESS LVL");
        lcd.setCursor(7, 1);
        lcd.print(LCDBrightnessLvl);
        SWjustPressed = !SWjustPressed;
      }
      if (lastLCDBrightnessLvl != LCDBrightnessLvl){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("BRIGHTNESS LVL");
        lcd.setCursor(7, 1);
        lcd.print(LCDBrightnessLvl);
        lastLCDBrightnessLvl = LCDBrightnessLvl;
        LCDBrightnessValue = map (LCDBrightnessLvl, 1, 5, 100, 255);
        analogWrite(LCDBrightnessPin, LCDBrightnessValue);

      }
      valueOY = analogRead(joyYPin);

      if (valueOY < 200)
        valueOY = DOWN;
      else if (valueOY > 1000)
        valueOY = UP;
      else valueOY = NEUTRAL;

      if (lastValue != valueOY){
          
        lastValue = valueOY;
        LCDBrightnessLvl -= valueOY;
        if (LCDBrightnessLvl > MAX_LCD_BRIGHTNESS_LVL)
          LCDBrightnessLvl = MAX_LCD_BRIGHTNESS_LVL;
        if (LCDBrightnessLvl < MIN_LCD_BRIGHTNESS_LVL)
          LCDBrightnessLvl = MIN_LCD_BRIGHTNESS_LVL;
      }
      break;
    
    case(3):
      if (SWjustPressed){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("BRIGHTNESS LVL");
        lcd.setCursor(7, 1);
        lcd.print(MatrixBrightnessLvl);
        SWjustPressed = !SWjustPressed;
      }
      if (lastMatrixBrightnessLvl != MatrixBrightnessLvl){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("BRIGHTNESS LVL");
        lcd.setCursor(7, 1);
        lcd.print(MatrixBrightnessLvl);
        lastMatrixBrightnessLvl = MatrixBrightnessLvl;
        MatrixBrightnessValue = map (MatrixBrightnessLvl, 1, 5, 1, 15);
        lc.setIntensity(0, MatrixBrightnessValue);

      }
      valueOY = analogRead(joyYPin);

      if (valueOY < 200)
        valueOY = DOWN;
      else if (valueOY > 1000)
        valueOY = UP;
      else valueOY = NEUTRAL;

      if (lastValue != valueOY){
          
        lastValue = valueOY;
        MatrixBrightnessLvl -= valueOY;
        if (MatrixBrightnessLvl > MAX_MATRIX_BRIGHTNESS_LVL)
          MatrixBrightnessLvl = MAX_MATRIX_BRIGHTNESS_LVL;
        if (MatrixBrightnessLvl < MIN_MATRIX_BRIGHTNESS_LVL)
          MatrixBrightnessLvl = MIN_MATRIX_BRIGHTNESS_LVL;
      }
      break;
    
    case(4):
      if (SWjustPressed){
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.write("SOUND");
        lcd.setCursor(7, 1);
        if (soundOn)
          lcd.write("ON");
        else lcd.write("OFF");
        SWjustPressed = !SWjustPressed;
      }
      if (lastSoundState != soundState){
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.write("SOUND");
        lcd.setCursor(7, 1);
        if (soundOn)
          lcd.write("ON");
        else lcd.write("OFF");
        lastSoundState = soundState;
        //MatrixBrightnessValue = map (MatrixBrightnessLvl, 1, 5, 100, 255);
        //analogWrite(MatrixBrightnessPin, MatrixBrightnessValue);

      }
      valueOY = analogRead(joyYPin);

      if (valueOY < 200)
        valueOY = DOWN;
      else if (valueOY > 1000)
        valueOY = UP;
      else valueOY = NEUTRAL;

      if (lastValue != valueOY)
        {
          if (valueOY != NEUTRAL){
          lastValue = valueOY;
          soundOn = !soundOn;
          if (soundOn)
            soundState = ON;
          else soundState = OFF;
          }

        }
      break;
    
    case(5):

      break;
    default:
      lcd.clear();
  }
}

void writeMenuOnLCD(){
  lcd.clear();
  switch(settingsState){
    case(0):
      lcd.setCursor(0, 0);
      lcd.write("START");
      lcd.setCursor(0, 1);
      lcd.write("DIFFICULTY");
      break;
    
    case(1):
      lcd.setCursor(0, 0);
      lcd.write("DIFFICULTY");
      lcd.setCursor(0, 1);
      lcd.write("LCD BRIGHTNESS");
      break;
    
    case(2):
      lcd.setCursor(0, 0);
      lcd.write("LCD BRIGHTNESS");
      lcd.setCursor(0, 1);
      lcd.write("MATR BRIGHTNESS");
      break;
    
    case(3):
      lcd.setCursor(0, 0);
      lcd.write("MATR BRIGHTNESS");
      lcd.setCursor(0, 1);
      lcd.write("SOUND");
      break;
    
    case(4):
      lcd.setCursor(0, 0);
      lcd.write("SOUND");
      lcd.setCursor(0, 1);
      lcd.write("ABOUT");
      break;
    
    case(5):
      lcd.setCursor(0, 0);
      lcd.write("ABOUT");
      lcd.setCursor(0, 1);
      lcd.write("START");
      break;
    default:
      lcd.clear();
  }


}