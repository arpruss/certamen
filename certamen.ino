/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

const int debounceTime = 50;
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int buzzerPin = 9;
const int numTeams = 3;
const int playersPerTeam = 4;
const int playerPins[numTeams][playersPerTeam] = {
{30,31,32,33},
{34,35,36,37},
{38,39,40,41}
};
const int clearPin = 42;

const int certamenModePin = 3;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int ledState = 0;
long lastButtonDownTime = -1;
long toneOffTime = -1;
int pressOrder[numTeams*playersPerTeam];
char buttonDown[numTeams*playersPerTeam];
char teamButtonDown[numTeams];
int numPressed = 0;
int certamenMode = 1;
char add[numTeams*playersPerTeam];

inline int getPlayerID(int team, int player) {
  return team*playersPerTeam + player;
}

inline int getTeamFromID(int id) {
  return id/playersPerTeam;
}

void clearState(char force) {
  if (numPressed > 0 || force) {
    numPressed = 0;
    for (int i=0; i<numTeams*playersPerTeam; i++) buttonDown[i] = 0;
    for (int i=0; i<numTeams; i++) teamButtonDown[i] = 0;
    /* TODO: screen */
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(clearPin, INPUT);
  pinMode(certamenModePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  for (int i=0; i<numTeams; i++) for (int j=0; j<playersPerTeam; j++) pinMode(playerPins[i][j], INPUT);
  clearState(1);
}

void scan() {
  int toAdd = 0;
  int previousNumPressed = numPressed;
  
  for (int i=0; i<numTeams; i++) {
    if (!certamenMode || !teamButtonDown[i])
      for (int j=0; j<playersPerTeam; j++) {
        int id = getPlayerID(i,j);
        if (!buttonDown[id]) {
          char down = HIGH == digitalRead(buttonPin);
          if (down) {
            buttonDown[id] = 1;
            add[toAdd] = id;
            toAdd++;
          }
        }
      }
  }

  if (toAdd) {
    if (toAdd == 1) { /* the usual, optimized case */
      pressOrder[numPressed++] = add[0];
      teamButtonDown[getTeamFromID(add[0])] = 1;
    }
    else {
      /* randomize simultaneous presses */
      for (int i=0; i<toAdd; i++) {
        int n = random() % (toAdd-i);
        for (int j=0; j<toAdd; j++) {
          if (add[j] >= -1) {
            if (n==0) {
              int id = add[j];
              teamButtonDown[getTeamFromID(id)] = 1;
              pressOrder[numPressed++] = id;
              add[j] = -1;
              break;
            }
            n--;
          }
        }
      }
    }

    if (previousNumPressed == 0) {
        tone(buzzerPin, 800);
        toneOffTime = millis() + 500;
    }
    /* TODO: display */
  }
}

void loop() {
  // read the state of the pushbutton value:
  int currentButton = digitalRead(buttonPin);

  if (toneOffTime != -1 && millis() >= toneOffTime)
    noTone(buzzerPin);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (currentButton == HIGH) {
      if (!buttonState) {
        buttonState = 1;
        ledState = ! ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW); // LED state
        tone(buzzerPin, 800);
        toneOffTime = millis() + 500;
      }
      lastButtonDownTime = millis();
  } else {
    if(buttonState) {
      if (millis() >= lastButtonDownTime + debounceTime) {
        buttonState = 0;
      }
    }
  }

  char certamenModeSwitch = HIGH == digitalRead(certamenModePin);

  if (certamenModeSwitch != certamenMode) {
    clearState(0);
    certamenMode = certamenModeSwitch;
  }

  if (HIGH == digitalRead(clearPin)) {
    clearState(0);
  }
  else {
    scan();
  }
}

