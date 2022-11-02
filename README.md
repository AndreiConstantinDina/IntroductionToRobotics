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
    2. State 2 (initiated by counting down 8 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration:  3 seconds.
    3. State 3 (initiated after state 2 ends):  red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration:8 seconds.
    4. State 4 (initiated after state 3 ends):  red for cars,blinking greenfor people and a beeping sound from the buzzer,  at a constant in-terval,  faster than the beeping in state 3.  This state should last 4seconds.
  
  Be  careful:  pressing  the  button  in  any  state  other  than  state  1  shouldNOT yield any actions.

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


