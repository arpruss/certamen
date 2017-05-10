/* Code (c) 2017 Alexander Pruss.
 *  
 *  Uses some public domain stuff from http://www.arduino.cc/en/Tutorial/Button
 */

#define TEST_MODE 

#define BUTTON_MODE INPUT_PULLUP // TODO: change for final version

typedef unsigned char uint8;
typedef signed char sint8;

const uint8 testPin = 53;

const uint8 debounceTime = 50;
// constants won't change. They're used here to
// set pin numbers:
const uint8 buttonPin = 2;     // the number of the pushbutton pin
const uint8 ledPin =  13;      // the number of the LED pin
const uint8 buzzerPin = 9;
const uint8 numTeams = 3;
const uint8 playersPerTeam = 4;
const uint8 playerPins[numTeams][playersPerTeam] = {
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
uint8 numPressed = 0;
char certamenMode = 1;
sint8 add[numTeams*playersPerTeam]; // maximum of 128 players

inline uint8 getPlayerID(uint8 team, uint8 player) {
  return team*playersPerTeam + player;
}

inline int getTeamFromID(uint8 id) {
  return id/playersPerTeam;
}

void clearState(char force) {
  if (numPressed > 0 || force) {
    numPressed = 0;
    for (int i=0; i<numTeams*playersPerTeam; i++) buttonDown[i] = 0;
    for (int i=0; i<numTeams; i++) teamButtonDown[i] = 0;
#ifdef TEST_MODE
    Serial.println("Clear");
#endif
  }
#ifdef TEST_MODE
  digitalWrite(testPin, LOW);
#endif
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(clearPin, INPUT_PULLUP);
  pinMode(certamenModePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(testPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  for (int i=0; i<numTeams; i++) for (int j=0; j<playersPerTeam; j++) {
    pinMode(playerPins[i][j], BUTTON_MODE); // for testing
    //TODO: disable failed?
  }
  clearState(1);
#ifdef TEST_MODE
  Serial.begin(9600);
  Serial.println("Certamen test mode\r\n");
#endif
}

void scan() {
  uint8 toAdd = 0;
  uint8 previousNumPressed = numPressed;
  
  for (uint8 i=0; i<numTeams; i++) {
    if (!certamenMode || !teamButtonDown[i])
      for (uint8 j=0; j<playersPerTeam; j++) {
        uint8 id = getPlayerID(i,j);
        if (!buttonDown[id] && LOW == digitalRead(playerPins[i][j])) {
          buttonDown[id] = 1;
          add[toAdd] = id;
          toAdd++;
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
      /* TODO: test this in software, as it so rarely triggers */
      for (uint8 i=0; i<toAdd; i++) {
        uint8 n = random() % (toAdd-i);
        for (uint8 j=0; j<toAdd; j++) {
          if (add[j] > -1) {
            if (n==0) {
              uint8 id = add[j];
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

#ifdef TEST_MODE
    digitalWrite(testPin, HIGH);
#endif

    if (previousNumPressed == 0) {
        tone(buzzerPin, 800);
        toneOffTime = millis() + 500;
#ifdef TEST_MODE
        Serial.println("buzz");
#endif
    }
#ifdef TEST_MODE
    Serial.println("pressed: ");
    for (uint8 i=0; i<numPressed;i++) {
      Serial.println(""+String(pressOrder[i]));
    }
#endif
    /* TODO: display */
  }
}

void loop() {
  if (toneOffTime != -1 && millis() >= toneOffTime) {
    noTone(buzzerPin);
    toneOffTime = -1;
  }
  
#ifdef TEST_MODE
  // read the state of the pushbutton value
  uint8 currentButton = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (currentButton == LOW) {
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
#endif

  char certamenModeSwitch = HIGH == digitalRead(certamenModePin);

  if (certamenModeSwitch != certamenMode) {
    clearState(0);
    certamenMode = certamenModeSwitch;
  } 

  if (LOW == digitalRead(clearPin)) {
    clearState(0);
  }
  else {
    scan();
  }
}

