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

