/* Code (c) 2017 Alexander Pruss. Licensed under the Gnu Public License 3.0 or higher.
 *  See COPYING
 */

#define TEST_MODE 
#define RANDOM_PRESS_FREQUENCY 10000
#define CLEAR_FREQUENCY 10000
#define NUMBER_OF_RANDOM_PRESSES 2
#define BUTTON_MODE INPUT_PULLUP // TODO: change for final version
#define OSCILLATION_TEST 1
#undef TEST_MODE_PUSHBUTTON

typedef unsigned char uint8;
typedef signed char sint8;

#ifdef TEST_MODE
const uint8 testPin = 53;
#ifdef OSCILLATION_TEST
uint8 testPinState = 0;
#endif
#endif

const uint8 unconnectedAnalogPin = 5;
const uint8 debounceTime = 50;
// constants won't change. They're used here to
// set pin numbers:
const uint8 buttonPin = 2;     // the number of the pushbutton pin
const uint8 ledPin =  13;      // the number of the LED pin
const uint8 buzzerPin = 9;
const uint8 numTeams = 3;
const uint8 playersPerTeam = 4;
const uint8 playerPins[numTeams*playersPerTeam] = {
30,31,32,33,
34,35,36,37,
38,39,40,41
};
const uint8 clearPin = 42;
const uint8 certamenModePin = 3;

char tonePlaying = 0;
char buttonState = 0;         // variable for reading the pushbutton status
char ledState = 0;
long lastButtonDownTime = -1;
long toneOffTime = 0;
uint8 pressOrder[numTeams*playersPerTeam];
char buttonDown[numTeams*playersPerTeam];
char teamButtonDown[numTeams];
uint8 numPressed = 0;
char certamenMode = 1;
sint8 add[numTeams*playersPerTeam]; // maximum of 128 players
char stringBuffer[128];

inline uint8 getPlayerID(uint8 team, uint8 player) {
  return team*playersPerTeam + player;
}

inline int getTeamFromID(uint8 id) {
  return id/playersPerTeam;
}

inline int getPlayerFromID(uint8 id) {
  return id % playersPerTeam;
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
#if defined(TEST_MODE) && !defined(OSCILLATION_TEST)
  digitalWrite(testPin, LOW);
#endif
}

char* describePlayerID(uint8 id) {
  uint8 team = getTeamFromID(id);
  uint8 player = getPlayerFromID(id);
  sprintf(stringBuffer, "%c%d", 'A'+team,player+1);
  return stringBuffer;
}

void setup() {
  randomSeed(analogRead(unconnectedAnalogPin));
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(clearPin, INPUT_PULLUP);
  pinMode(certamenModePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(testPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  for (int id=0; id<numTeams*playersPerTeam; id++) {
    pinMode(playerPins[id], BUTTON_MODE); 
    //TODO: disable failed?
  }
  clearState(1);
#ifdef TEST_MODE
  Serial.begin(9600);
  Serial.println("Certamen test mode\r\n");
#endif
}

void scan() {
#if defined(TEST_MODE) && defined(OSCILLATION_TEST)
  testPinState = ! testPinState;
  digitalWrite(testPin, testPinState);
#endif

  uint8 toAdd = 0;
  uint8 previousNumPressed = numPressed;

  for (int id=numTeams*playersPerTeam-1; id>=0; id--) {
      if (LOW == digitalRead(playerPins[id]) && !buttonDown[id] && (!certamenMode || !teamButtonDown[getTeamFromID(id)])) {
          buttonDown[id] = 1;
          add[toAdd++] = id;
      }
  }

#if defined(TEST_MODE) && RANDOM_PRESS_FREQUENCY>0
  if (toAdd == 0 && random(RANDOM_PRESS_FREQUENCY) == 0) {
//    Serial.println("Random presses");
//    Serial.println(String("Teams ") + (teamButtonDown[0]?"yes ":"no ") +(teamButtonDown[1]?"yes ":"no ")+(teamButtonDown[2]?"yes":"no"));
    for (uint8 i = 0; i < NUMBER_OF_RANDOM_PRESSES ; i++) {
      uint8 id = random(numTeams*playersPerTeam);
      if (!buttonDown[id] && (!certamenMode || !teamButtonDown[getTeamFromID(id)] ) ) {
        buttonDown[id] = 1;
        add[toAdd++] = id;
      }
    }
  }
#endif  

  if (toAdd) {
    if (toAdd == 1) { /* the usual, optimized case */
      pressOrder[numPressed++] = add[0];
      teamButtonDown[getTeamFromID(add[0])] = 1;
    }
    else {
      /* randomize simultaneous presses */
      /* TODO: test this in software, as it so rarely triggers */
      sint8 leftToAdd = toAdd;
      uint8 j;
      do {
        uint8 n = random() % leftToAdd;
        for (j=0; j<toAdd; j++) {
          if (add[j] > -1) {
            if (n==0) {
              uint8 id = add[j];
              uint8 team = getTeamFromID(id);
              teamButtonDown[team] = 1;
              pressOrder[numPressed++] = id;
              if (certamenMode) {
                for (uint8 k=0; k<toAdd; k++) {
                  id = add[k];
                  if (id > -1 && getTeamFromID(id) == team) {
                    add[k] = -1;
                    leftToAdd--;
                  }
                }
              }
              else {
                add[j] = -1;
                leftToAdd--;
              }
              break;
            }
            n--;
          }
        }
      } while(leftToAdd > 0 && j<toAdd); // the second condition should never be satisfied
    }

#if defined(TEST_MODE) && !defined(OSCILLATION_TEST)
    digitalWrite(testPin, HIGH);
#endif

    if (previousNumPressed == 0) {
        tone(buzzerPin, 800);
        toneOffTime = millis() + 500;
        tonePlaying = 1;
#ifdef TEST_MODE
        Serial.println("buzz");
#endif
    }
#ifdef TEST_MODE
    Serial.println(certamenMode ? "pressed in certamen mode:" : "pressed in non-certamen mode:");
    for (uint8 i=0; i<numPressed;i++) {
      Serial.println(describePlayerID(pressOrder[i]));
    }
#endif
    /* TODO: display */
  }
}

void loop() {
  if (tonePlaying && millis() >= toneOffTime) {
    noTone(buzzerPin);
    tonePlaying = 0;
  }
  
#ifdef TEST_MODE_PUSHBUTTON
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
        tonePlaying = 1;
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
    // TODO: might want to debounce this a little to avoid too many calls to clearState(),
    // but is it worth the bother? Well, it might depend on how the display works.
    clearState(0);
    certamenMode = certamenModeSwitch;
  } 

#if defined(TEST_MODE) && CLEAR_FREQUENCY
  if (random(CLEAR_FREQUENCY)) clearState(0);
#endif
  if (LOW == digitalRead(clearPin)) {
    clearState(0);
  }
  else {
    scan();
  }
}

