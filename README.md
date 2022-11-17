# IntroductionToRobotics - Dina Andrei-Constantin
Introduction to Robotics laboratory homeworks, taken in the third year at the Faculty of Mathematics and Computer Science, University of Bucharest.
Each homework includes requirements, implementation details, code, image files and possibly videos. 
Course professor: Andrei Dumitriu


<details>
<summary>Homework 1</summary>
<br>

Task Requirements: "Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue).  The control must be done with digital electronics(aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected to the led."

Pictures of the setup:

<img src="https://user-images.githubusercontent.com/99658689/197361062-53204f3e-b34f-4619-b5c0-541bb6c48442.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361065-4734f1a1-75c0-46c6-82d4-7b6c7cbc0139.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361066-807e18ae-effc-4eeb-9697-51db8d0ee09e.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361067-ed2d10cd-f074-4884-9254-b36322cb8759.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361068-f04b71ca-1c5e-4565-8f69-07c58bb295ad.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361069-b1e2d4fa-e1c6-48f1-9a74-ac192e90afe3.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361070-ca0e6c88-79ed-49cd-9ffd-dd0ab1e8f10f.jpg" width="360" height="640" />
<img src="https://user-images.githubusercontent.com/99658689/197361071-b073e61d-a1c7-42f9-ba83-6fb0a0ecb830.jpg" width="360" height="640" />
  

A video of the project in which you can see the way everything works:
https://youtu.be/HYCs1vSOPmo

Source code:

```
const int redLedPin = 11;
const int blueLedPin = 10;
const int greenLedPin = 9;

const int pinRedLevel = A3;
const int pinGreenLevel = A5;
const int pinBlueLevel = A4;

const int LOWER_BOUND_ANALOG_READ = 0;
const int HIGHER_BOUND_ANALOG_READ = 1023;
const int LOWER_BOUND_ANALOG_WRITE = 0;
const int HIGHER_BOUND_ANALOG_WRITE = 255;

int rawRedValue, rawGreenValue, rawBlueValue;
int RGBredValue, RGBgreenValue, RGBblueValue;

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(pinRedLevel, INPUT);
  pinMode(pinBlueLevel, INPUT);
  pinMode(pinGreenLevel, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // citim valorile de tip input, valoare intreagi din [0,1023]
  rawGreenValue = analogRead(pinGreenLevel); 
  rawRedValue = analogRead(pinRedLevel);
  rawBlueValue = analogRead(pinBlueLevel);
  
  // transformam valorile citite in valori din intervalul [0,255]
  RGBredValue = map(rawRedValue, LOWER_BOUND_ANALOG_READ, HIGHER_BOUND_ANALOG_READ, LOWER_BOUND_ANALOG_WRITE, HIGHER_BOUND_ANALOG_WRITE);
  RGBgreenValue = map(rawGreenValue, LOWER_BOUND_ANALOG_READ, HIGHER_BOUND_ANALOG_READ, LOWER_BOUND_ANALOG_WRITE, HIGHER_BOUND_ANALOG_WRITE);
  RGBblueValue = map(rawBlueValue, LOWER_BOUND_ANALOG_READ, HIGHER_BOUND_ANALOG_READ, LOWER_BOUND_ANALOG_WRITE, HIGHER_BOUND_ANALOG_WRITE);;
  
  // vom seta un threshold in cazul in care device-ul de input nu este perfect: valoriile < 5 se considera ca fiind 0
  if (RGBredValue < 5)
    RGBredValue = 0;

  if (RGBgreenValue < 5)
    RGBgreenValue = 0;

  if (RGBblueValue < 5)
    RGBblueValue = 0;
  
  // trimitem semnalul catre LED
  assignRGBtoLed(RGBredValue, RGBgreenValue, RGBblueValue);
}

void assignRGBtoLed(int r, int g, int b){
  // functia seteaza intensitatile de rosu, verde si albastru LED-ului nostru
  analogWrite(redLedPin, r);
  analogWrite(blueLedPin, g);
  analogWrite(greenLedPin, b);
}
```
</details>




<details>
<summary>Homework 2</summary>
<br>

Task Requirements: 
  Building  the  traffic  lights  for  a  crosswalk.   Using 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). See the states it needs to go through:
    <br> 1. State 1 (default, reinstated after state 4 ends):  green light for cars,red  light  for  people,  no  sounds.   Duration:  indefinite,  changed  bypressing the button.
    <br> 2. State 2 (initiated by counting down 8 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration:  3 seconds.
    <br> 3. State 3 (initiated after state 2 ends):  red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration:8 seconds.
    <br> 4. State 4 (initiated after state 3 ends):  red for cars,blinking greenfor people and a beeping sound from the buzzer,  at a constant in-terval,  faster than the beeping in state 3.  This state should last 4seconds.
  
  Note: pressing the button in any state other than state 1 will not yield any actions.

Pictures of the setup:

<img src="https://user-images.githubusercontent.com/99658689/199517632-51095031-59dc-4b2b-be39-d3efe7a342c0.jpg" width="360" height="640" />
  
A video of the project in which you can see the way everything works: https://youtu.be/fbVpC9-l-Uo

Source code:

```
// used pins

const int buttonPin = 5;
const int buzzerPin = 6;

const int pedestrianGreenPin = 7;
const int pedestrianRedPin = 8;

const int carGreenPin = 9;
const int carYellowPin = 10;
const int carRedPin = 11;

// used values
const int pedestrianWaitTime = 8000;
const int yellowLightTimer = 3000;
const int pedestrianGreenLightTimer = 8000;
const int pedestrianGreenLightTimer2 = 4000;
const int soundFrequency = 10000;
const int beepingInterval1 = 500;
const int beepingInterval2 = 250;

int sequenceTime = 0;

byte buttonState = 0;
byte currentButtonState = 0;
byte previousButtonState = 0;

bool buttonIsPressable = true;
bool buzzerOn = false;

int buttonPressCount = 0;
byte minimumPressTime = 50;
bool pedestrianGreenLightOn = false;

unsigned long lastReadingTime = 0;
unsigned long waitingSequence, yellowLightSequence, pedestrianGreenLightSequence1, pedestrianGreenLightSequence2;
unsigned long lastBuzzTimer = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pedestrianGreenPin, OUTPUT);
  pinMode(pedestrianRedPin, OUTPUT);
  pinMode(carGreenPin, OUTPUT);
  pinMode(carRedPin, OUTPUT);
  pinMode(carYellowPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  constrain(buttonPressCount, 0, 2);


  if (buttonPressCount == 0){
    noTone(buzzerPin);
    digitalWrite(pedestrianGreenPin, LOW);
    digitalWrite(pedestrianRedPin, HIGH);
    digitalWrite(carRedPin, LOW);
    digitalWrite(carGreenPin, HIGH);
    digitalWrite(carYellowPin, LOW);
  }

  currentButtonState = !digitalRead(buttonPin);


  if (buttonIsPressable)
    if (currentButtonState != previousButtonState)
      if (millis() - lastReadingTime > minimumPressTime)
          {
            buttonPressCount++;
            previousButtonState = currentButtonState;
            lastReadingTime = millis();
            if (buttonPressCount % 2 == 0){
              waitingSequence = millis() + pedestrianWaitTime;
              yellowLightSequence = yellowLightTimer + waitingSequence;
              pedestrianGreenLightSequence1 = pedestrianGreenLightTimer + yellowLightSequence;
              pedestrianGreenLightSequence2 = pedestrianGreenLightTimer2 + pedestrianGreenLightSequence1;
            }
          }

  if (buttonPressCount % 2 == 0 && buttonPressCount){
    buttonIsPressable = false; // in case a pedestrian spams the button, it will only initiate a sequence the first time it's pressed

    if (millis() < waitingSequence); // pedestrians waiting for green light timer

    else if (millis() < yellowLightSequence){
        digitalWrite(carGreenPin, LOW);
        digitalWrite(carYellowPin, HIGH);
        delay(1);
    }  

    else if (millis() < pedestrianGreenLightSequence1){
      digitalWrite(carYellowPin, LOW);
      digitalWrite(carRedPin, HIGH);
      if (millis() - lastBuzzTimer > beepingInterval1)
        {
        if (buzzerOn)
          noTone(buzzerPin);
        else tone(buzzerPin, soundFrequency, pedestrianGreenLightTimer);
        lastBuzzTimer = millis();
        buzzerOn = !buzzerOn;
        pedestrianGreenLightOn = true;
        }
      
      digitalWrite(pedestrianGreenPin, HIGH);
      digitalWrite(pedestrianRedPin, LOW);
      delay(1);

    }  
    else if (millis() < pedestrianGreenLightSequence2){
      if (millis() - lastBuzzTimer > beepingInterval2)
        {
        if (buzzerOn){
          digitalWrite(pedestrianGreenPin, LOW);
          noTone(buzzerPin);

        }
        else {         
          digitalWrite(pedestrianGreenPin, HIGH);
          tone(buzzerPin, soundFrequency, pedestrianGreenLightTimer);
        }

        lastBuzzTimer = millis();
        buzzerOn = !buzzerOn;
        pedestrianGreenLightOn = !pedestrianGreenLightOn;
        }
      
      delay(1);

    }  
    else{
      noTone(buzzerPin);
      digitalWrite(pedestrianGreenPin, LOW);
      digitalWrite(pedestrianRedPin, HIGH);
      digitalWrite(carRedPin, LOW);
      digitalWrite(carGreenPin, HIGH);
      digitalWrite(carYellowPin, LOW);
      delay(1);
      buttonIsPressable=true;
    }  
    
  }  
  delay(1);    
}
```
</details>

  
<details>
<summary>Homework 3</summary>
<br>

Task Requirements: 

  You will use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segment sshould be natural (meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”. The system has the following states:
  <br> 1.State  1 (default,  but  also  initiated  after  a  button  press  in  State2):  Current  position  blinking.   Can  use  the  joystick  to  move  fromone  position  to  neighbors.   Short  pressing  the  button  toggles  state2.  Long pressing the button in state 1 resets the entire display byturning all the segments OFF and moving the current position to thedecimal point.
  <br> 2.State  2(initiated  after  a  button  press  in  State  1):   The  currentsegment  stops  blinking,  adopting  the  state  of  the  segment  beforeselection (ON or OFF). Toggling the X (or Y, you chose) axis shouldchange  the  segment  state  from  ON  to  OFF  or  from  OFF  to  ON.Clicking the joystick should save the segment state and exit back tostate 1.
  
Pictures of the setup:

<img src="https://user-images.githubusercontent.com/99658689/200854800-f2460ecf-d976-42fd-bf2f-4a86a4a3bf6c.jpg" width="360" height="640" />
  
  
A video of the project in which you can see the way everything works: https://youtu.be/9i24RCIUL2s

Source code:

```
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
```
</details>
  
  
  
 
  
<details>
<summary>Homework 3</summary>
<br>

Task Requirements: 

TBA
  
Pictures of the setup:

<img src="https://user-images.githubusercontent.com/99658689/202426602-97054055-9548-4e72-9ea7-5385e2a38fe9.jpg" width="360" height="640" />
  
A video of the project in which you can see the way everything works: https://youtu.be/soZ97woMAm0

Source code:

```
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
```
</details>




