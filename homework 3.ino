// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

// joystick pins
const int joyXPin = A1;
const int joyYPin = A0;
const int joySWPin = 12;

int blinkingSegment = pinDP; // the blinking pin, when the program starts it should be pinDP

const int joyLeftThreshold = -100;
const int joyRightThreshold = 100;
const int joyDefaultXValue = 0;

const int joyUpThreshold = 100;
const int joyDownThreshold = -100;
const int joyDefaultYValue = 0;

const int neutralThreshold = 200;
bool joyIsNeutral = true;

bool joySWState = HIGH;
bool joyPrevSWState = HIGH;
bool dpState = false;

char nextTransition[] = "NEUTRAL";
char lastTransition[] = "NEUTRAL";

char movementStateOY[] = "NEUTRAL";
int absValueX, absValueY;

bool segmentBlinkingOn = true;
unsigned long lastChangeInBlinking = 0;
unsigned long lastReadSWState = 0;
long pressTime = 0;
long releaseTime = 0;

int blinkingInterval = 500;

const int segSize = 8;
// modify if you have common anode
bool commonAnode = false; 

byte segmentOn = HIGH;
int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int currentDigit = 0;

const int noOfDigits = 10;
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

int statePinA = LOW;
int statePinB = LOW;
int statePinC = LOW;
int statePinD = LOW;
int statePinE = LOW;
int statePinF = LOW;
int statePinG = LOW;
int statePinDP = LOW;
int stateCurrentPin = LOW;
char movementStateOX[] = "NEUTRAL";

bool buttonWasReleased = true;
int totalPressedTime = 0;
bool segmentActualized = true;
int currentBlink = LOW;
bool blinkingMode = true;
int shortPressTime = 1000;

int analogReadMinValue = 0;
int analogReadMaxValue = 1023;
int mappedMinAnalogValue = -512;
int mappedMaxAnalogValue = 511;

void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(joySWPin, INPUT_PULLUP);
  if (commonAnode == true) {
    segmentOn = !segmentOn;
  }
  
  Serial.begin(9600);
}

void loop() {

 
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
        blinkingMode = !blinkingMode;
        segmentActualized = true;
      } 

      else // long press
        {
        statePinA = LOW;
        statePinB = LOW;
        statePinC = LOW;
        statePinD = LOW;
        statePinE = LOW;
        statePinF = LOW;
        statePinG = LOW;
        statePinDP = LOW;
        digitalWrite(pinA, statePinA);
        digitalWrite(pinB, statePinB);  
        digitalWrite(pinC, statePinC);
        digitalWrite(pinD, statePinD);
        digitalWrite(pinE, statePinE);
        digitalWrite(pinF, statePinF);
        digitalWrite(pinG, statePinG);
        digitalWrite(pinDP, statePinDP);
        segmentActualized = true;
        blinkingSegment = pinDP;
        //resetting the board
        }
  }



  int joyY = analogRead(joyYPin);
  int joyX = analogRead(joyXPin);
  joyY = map(joyY, analogReadMinValue, analogReadMaxValue, mappedMinAnalogValue, mappedMaxAnalogValue);
  joyX = map(joyX, analogReadMinValue, analogReadMaxValue, mappedMinAnalogValue, mappedMaxAnalogValue);
  strcpy(nextTransition, "NEUTRAL");

  // we have the values for x and y transposed in an interval [-512, 511]
  // checking the direction the joystick points to

  if (joyY > joyUpThreshold)
    {
      strcpy(movementStateOY, "UP");
      absValueY = joyY - joyUpThreshold;
    }

  else if (joyY < joyDownThreshold)
    {
      strcpy(movementStateOY, "DOWN");
      absValueY = joyDownThreshold - joyY;
    }
    
  if (joyX > joyLeftThreshold)
    {
      strcpy(movementStateOX, "RIGHT");
      absValueX = joyX - joyLeftThreshold;
    }
  else if (joyX < joyRightThreshold)
    {
      strcpy(movementStateOX, "LEFT");
      absValueX = joyRightThreshold -  joyX;
    }

  // if the joystick is not moved enough, we will consider its position is neutral
  if (abs (joyX - joyDefaultXValue) < neutralThreshold)
    strcpy(movementStateOX, "NEUTRAL");
  if (abs (joyY - joyDefaultYValue) < neutralThreshold)
    strcpy(movementStateOY, "NEUTRAL");
  

  strcpy(nextTransition, absValueX < absValueY ? movementStateOY : movementStateOX); // save the transition for the next step: for example, if the joystick is both pointed down and a little to the right, it will be considered as pointing down

  if (blinkingMode){ // if the segment is blinking, just blink and move between segments using the joystick

    if (millis() - lastChangeInBlinking > blinkingInterval) // blinking 
      {
        digitalWrite(blinkingSegment, currentBlink);
        
        if (currentBlink == LOW)
          currentBlink = HIGH;
        else currentBlink = LOW;

        lastChangeInBlinking = millis();
      }


    if (strcmp(nextTransition, lastTransition)) // moving between segments. We will check if the joystick changed positions between moves. We do this because we dont want to move up 100 positions in one little movement of the joystick, just once
      {
        stateCurrentPin = getSegmentState(blinkingSegment); // find the next segment
        digitalWrite(blinkingSegment, stateCurrentPin); // if we move away from the segment, the state should be the same as before we "arrived" there
        blinkingSegment = changeSegment(blinkingSegment, nextTransition); // move to the next segment
        strcpy(lastTransition, nextTransition); // save the last transition
      }

  }

  else { // in this mode we change the selected segment state using up and down movements of the joystick
      if ((!strcmp(nextTransition, "UP") || !strcmp(nextTransition, "DOWN")) && strcmp(nextTransition, lastTransition))
      {
            if (blinkingSegment == pinA)
              statePinA = !statePinA;
            if (blinkingSegment == pinB)
              statePinB = !statePinB;
            if (blinkingSegment == pinC)
              statePinC = !statePinC;
            if (blinkingSegment == pinD)
              statePinD = !statePinD;
            if (blinkingSegment == pinE)
              statePinE = !statePinE;
            if (blinkingSegment == pinF)
              statePinF = !statePinF;
            if (blinkingSegment == pinG)
              statePinG = !statePinG;
            if (blinkingSegment == pinDP)
              statePinDP = !statePinDP;

            stateCurrentPin = getSegmentState(blinkingSegment);
            digitalWrite(blinkingSegment, stateCurrentPin);
            
            strcpy(lastTransition, nextTransition);
      }
  } 
}

int getSegmentState (int pin){
  if (pin == pinA)
    return statePinA;
  if (pin == pinB)
    return statePinB;
  if (pin == pinC)
    return statePinC;
  if (pin == pinD)
    return statePinD;
  if (pin == pinE)
    return statePinE;
  if (pin == pinF)
    return statePinF;
  if (pin == pinG)
    return statePinG;
  if (pin == pinDP)
    return statePinDP;
  
}
int changeSegment (int currentSegment, char transition[]){
  // the function returns the next segment we move to (if we can move to another segment through the movement stored in transition parameter)
  if (!strcmp(transition, "NEUTRAL"))
    return currentSegment;
  switch(currentSegment){
    case (pinA):
      if (!strcmp(transition, "LEFT"))
        return pinF;
      if (!strcmp(transition, "RIGHT"))
        return pinB;
      if (!strcmp(transition, "UP"))
        return currentSegment;
      if (!strcmp(transition, "DOWN"))
        return pinG;
      break;

    case (pinB):
      if (!strcmp(transition, "LEFT"))
        return pinF;
      if (!strcmp(transition, "RIGHT"))
        return currentSegment;
      if (!strcmp(transition, "UP"))
        return pinA;
      if (!strcmp(transition, "DOWN"))
        return pinG;
      break;

    case (pinC):
      if (!strcmp(transition, "LEFT"))
        return pinE;
      if (!strcmp(transition, "RIGHT"))
        return pinDP;
      if (!strcmp(transition, "UP"))
        return pinG;
      if (!strcmp(transition, "DOWN"))
        return pinD;
      break;

    case (pinD):
      if (!strcmp(transition, "LEFT"))
        return pinE;
      if (!strcmp(transition, "RIGHT"))
        return pinC;
      if (!strcmp(transition, "UP"))
        return pinG;
      if (!strcmp(transition, "DOWN"))
        return currentSegment;     
      break;

    case (pinE):
      if (!strcmp(transition, "LEFT"))
        return currentSegment;
      if (!strcmp(transition, "RIGHT"))
        return pinC;
      if (!strcmp(transition, "UP"))
        return pinG;
      if (!strcmp(transition, "DOWN"))
        return pinD;
      break;

    case (pinF):
      if (!strcmp(transition, "LEFT"))
        return currentSegment;
      if (!strcmp(transition, "RIGHT"))
        return pinB;
      if (!strcmp(transition, "UP"))
        return pinA;
      if (!strcmp(transition, "DOWN"))
        return pinG;
      break;

    case (pinG):
      if (!strcmp(transition, "LEFT"))
        return currentSegment;
      if (!strcmp(transition, "RIGHT"))
        return currentSegment;
      if (!strcmp(transition, "UP"))
        return pinA;
      if (!strcmp(transition, "DOWN"))
        return pinD;
      break;

    case (pinDP):
      if (!strcmp(transition, "LEFT"))
        return pinC;
      if (!strcmp(transition, "RIGHT"))
        return currentSegment;
      if (!strcmp(transition, "UP"))
        return currentSegment;
      if (!strcmp(transition, "DOWN"))
        return currentSegment;
      break;

  }

}









