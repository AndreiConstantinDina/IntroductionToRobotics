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