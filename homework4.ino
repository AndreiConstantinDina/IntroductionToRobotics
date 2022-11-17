//DS= [D]ata [S]torage - data
//STCP= [ST]orage [C]lock [P]in latch
//SHCP= [SH]ift register [C]lock [P]in clock
 
#define movingState 0
#define modifyValueState 1

const int UP = 1;
const int DOWN = -1;
const int NEUTRAL = 0;
const int LEFT = 2;
const int RIGHT = -2;

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register
 
const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 8;
  
const int joyLeftThreshold = 300;
const int joyRightThreshold = 700;
const int joyDefaultXValue = 450;
 
const int joyUpThreshold = 700;
const int joyDownThreshold = 300;
const int joyDefaultYValue = 450;
 
const int neutralThreshold = 450;
 
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;
 
const byte regSize = 8; // 1 byte aka 8 bits

const int displayCount = 4;
const int encodingsNumber = 17;
const int DP = 16; // we will use this later to turn on a DP

bool joySWState = HIGH;
bool joyPrevSWState = HIGH;
 

const int segSize = 8;
int nextTransition;
int lastTransition; 
 
bool segmentBlinkingOn = true;
unsigned long lastChangeInBlinking = 0;
long pressTime = 0;
long releaseTime = 0;
 
int blinkingInterval = 500;
 
byte segmentOn = HIGH;
 
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

bool buttonWasReleased = true;
int totalPressedTime = 0;
bool segmentActualized = true;
int shortPressTime = 1000;
 
unsigned long displayedNumber = 0; // the number displayed on the 4 digits 7 segment display

int byteEncodings[encodingsNumber] = { // encoding matrix for our hex digits + DP
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110,  // F
  B00000001 // DP
};
 
int currentPosition = 0; // 0/1/2/3, depending on which digit we are currently on
 
byte currentState = 0; // can be movingState (state 1) or modifyValueState (state 2)

int joyX, joyY; // values read by the joystick

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(joySWPin, INPUT_PULLUP);
 
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }  
  Serial.begin(9600);
}
 
void loop() {
  // writeNumber(1234); // first test with this and increase the delay inside the writeNumber function
 
 
  joySWState = digitalRead(joySWPin);
 
  if(joySWState != joyPrevSWState)
    {
 
    if (joySWState == LOW)
      {
        pressTime = millis();
        buttonWasReleased = false;
      }
 
    else if (!buttonWasReleased)
      {
      releaseTime = millis();
      buttonWasReleased = true;
      totalPressedTime = releaseTime - pressTime;
      segmentActualized = false;
      }
    }
 
 
 
  joyPrevSWState = joySWState;
 
  if (releaseTime - pressTime > 0 && !segmentActualized)
  {
    if (releaseTime - pressTime < shortPressTime ) // short press
        {
          segmentActualized = true;
          if (currentState == movingState)
            currentState = modifyValueState;
          else currentState = movingState;
        }
 
      else // long press
        {
          if (currentState == movingState)        //resetting the board
            {
              displayedNumber = 0;
              currentPosition = 0;
              currentState = movingState;
              pressTime = millis();
              releaseTime = millis();
            }
        }
  }
 
 
 
  joyY = analogRead(joyYPin);
  joyX = analogRead(joyXPin);

  nextTransition = NEUTRAL;
 
  // we have the values for x and y transposed in an interval [-512, 511]
  // checking the direction the joystick points to
 

 
  
  if (currentState == movingState)
  {
    if (joyX > joyRightThreshold)
      nextTransition = RIGHT;
    else if (joyX < joyLeftThreshold)
      nextTransition = LEFT;
    else nextTransition = NEUTRAL; 

    // we will determine whether the joystick is standing still (or almost standing still), is moved to the left or to the right

    if (nextTransition != lastTransition) // we check this so we dont spam move to the left/ right. For every left/ right movement of the joystick we will move exactly once
    {
        lastTransition = nextTransition; 

        if (nextTransition == LEFT)
          {
            currentPosition++;
            if (currentPosition > 3)
              currentPosition = 3;

          }
        else if (nextTransition == RIGHT) {
          {
            currentPosition--;
            if (currentPosition < 0)
              currentPosition = 0;
          }
      }
    }


  if (millis() - lastChangeInBlinking < blinkingInterval)
    writeNumberNoDP(displayedNumber);
  else if (millis() - lastChangeInBlinking < 2 * blinkingInterval)
    writeNumber(displayedNumber);
  else lastChangeInBlinking = millis();


  }
 
  else { // the second state, where we modify the current digit value using up/ down 
    writeNumber(displayedNumber);

     if (joyY > joyUpThreshold)
      nextTransition = UP;
    else if (joyY < joyDownThreshold)
      nextTransition = DOWN;
    else nextTransition = NEUTRAL;
    // we will determine whether the joystick is standing still (or almost standing still), is moved up or down

  if (nextTransition != lastTransition) // we check this so we dont spam move up/ down. For every up/ down movement of the joystick we will move exactly once
      {
        lastTransition = nextTransition;
        if (nextTransition == UP)
          {
            // != 15 means we check if we are going to overflow the value, which we don't want to do
            switch (currentPosition) {
            case 0:
              if (displayedNumber % 16 != 15)
                displayedNumber += 0x1; // add 1 to the _ _ _ x digit
              break;
            case 1:
              if (displayedNumber / 16 % 16 != 15) // add 1 to the _ _ x _ digit
                displayedNumber += 0x10;
              break;
            case 2:
              if (displayedNumber / 256 % 16 != 15) // add 1 to the _ x _ _ digit
               displayedNumber += 0x100;
              break;
            case 3: 
              if (displayedNumber / 4096 % 16 != 15) // add 1 to the x _ _ _ digit
                displayedNumber += 0x1000;
              break;
            }

          }
        else if (nextTransition == DOWN) {
          {
            // != 0 means we check if we are going to overflow the value, which we don't want to do
          switch (currentPosition) {
            case 0:
              if (displayedNumber % 16 != 0) // substract 1 from the _ _ _ x digit
                displayedNumber -= 0x1;
              break;
            case 1:
              if (displayedNumber / 16 % 16 != 0) // substract 1 from the _ _ x _ digit
                displayedNumber -= 0x10;
              break;
            case 2:
              if (displayedNumber / 256 % 16 != 0) // substract 1 from the _ x _ _ digit
               displayedNumber -= 0x100;
              break;
            case 3: 
              if (displayedNumber / 4096 % 16 != 0) // substract 1 from the x _ _ _ digit
                displayedNumber -= 0x1000;
              break;
            }
          }
      }
    }
    
  }
 
} 
 
 
 
void writeReg(int digit) {
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);
    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    /* ST_CP on Rising to move the data from shift register
     * to storage register, making the bits available for output.
     */
    digitalWrite(latchPin, HIGH);
}


void activateDisplay(int displayNumber) { 
  // first, disable all the display digits
  for (int i = 0; i < displayCount; i++) 
    digitalWrite(displayDigits[i], HIGH);
  // then enable only the digit you want to use now
  digitalWrite(displayDigits[displayNumber], LOW);
  delay(2);
}
 
void writeNumber(unsigned long number) {
  unsigned long currentNumber = number; 
  int displayDigit = 0;
  int lastDigit = 0;
 
  while (currentNumber != 0)
  {
    // get the last digit of the number
    lastDigit = currentNumber % 16; // 3
    // send the number to the display
    writeReg(byteEncodings[lastDigit]);
    // enable only the display digit for that 
    activateDisplay(displayDigit);
    delay(2);
    // increase the delay to see multiplexing in action
    // increment the display digit
    displayDigit++;
    // eliminate the last digit of the number
    currentNumber /= 16;
  }
  while (displayDigit <= 3)
  {
    // if our number doesn't have enough digits, we will complete by adding 0's to the left
    writeReg(byteEncodings[0]);
    activateDisplay(displayDigit);
    delay(2);
    displayDigit++;
  }

  writeReg(byteEncodings[DP]);
  activateDisplay(currentPosition);


}


void writeNumberNoDP(unsigned long number) {
  unsigned long currentNumber = number; 
  int displayDigit = 0;
  int lastDigit = 0;
 
  while (currentNumber != 0) 
  {
    // get the last digit of the number
    lastDigit = currentNumber % 16; // 3
    // send the number to the display
    writeReg(byteEncodings[lastDigit]);
    // enable only the display digit for that 
    activateDisplay(displayDigit);
    delay(2);
    // increase the delay to see multiplexing in action
    // increment the display digit
    displayDigit++;
    // eliminate the last digit of the number
    currentNumber /= 16;
  }

  while (displayDigit <= 3)
  {
    // if our number doesn't have enough digits, we will complete by adding 0's to the left
    writeReg(byteEncodings[0]);
    activateDisplay(displayDigit);
    delay(2);
    displayDigit++;
  }

}