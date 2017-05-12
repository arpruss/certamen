#define USE_TFT

#ifdef USE_TFT
// ST7735S TFT 160x128
// Pin connections: 
// SCK (CLK): Digital 52
// SDA (MOSI): Digital 51
// A0 (D/C): Digital 8
// RESET (RST): Digital 10

#include <SPI.h>
#include <PDQ_GFX.h>        // PDQ: Core graphics library
#include "PDQ_ST7735_config.h"
#define ST7735_CS_PIN   7      // <= /CS pin (chip-select, LOW to get attention of ST7735, HIGH and it ignores SPI bus)
#define ST7735_DC_PIN   8      // <= DC pin (1=data or 0=command indicator line) also called RS
#define ST7735_RST_PIN  10
#define ST7735_CHIPSET    ST7735_INITR_BLACKTAB
#include <PDQ_ST7735.h>     // PDQ: Hardware-specific driver library
#endif

/* Code (c) 2017 Alexander Pruss. Licensed under the Gnu Public License 3.0 or higher.
 *  See COPYING
 */

#define TEST_MODE 
#define RANDOM_PRESS_FREQUENCY 0
#define CLEAR_FREQUENCY 0
#define NUMBER_OF_RANDOM_PRESSES 2
#define BUTTON_MODE INPUT_PULLUP // TODO: change for final version
#define OSCILLATION_TEST 
#undef TEST_MODE_PUSHBUTTON
#undef SERIAL_ECHO

#ifndef TEST_MODE
#define RANDOM_PRESS_FREQUENCY 0
#define CLEAR_FREQUENCY 0
#undef OSCILLATION_TEST
#undef SERIAL_ECHO
#endif

typedef unsigned char uint8;
typedef signed char sint8;

#ifdef TEST_MODE
const uint8 testPin = 53;
#ifdef OSCILLATION_TEST
uint8 testPinState = 0;
#endif
#endif

#ifdef USE_TFT
PDQ_ST7735 screen = PDQ_ST7735();

const unsigned int backgroundColor = ST7735_YELLOW;
const unsigned int textColor = ST7735_BLACK;
const uint8 lineHeight = 16;
const uint8 fontSize = 2;
uint8 height;
uint8 width;
uint8 textLines;
const char* certamenModeText[2] = {"Quiz/Test","Certamen"};
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
const uint8 maxPlayers = numTeams * playersPerTeam;
const uint8 playerPins[maxPlayers] = {
30,31,32,33,
34,35,36,37,
38,39,40,41
};
const char playerName[maxPlayers][3] = { "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C1", "C2", "C3", "C4" };
const uint8 clearPin = 42;
volatile uint8* clearPort;
uint8 clearMask;
const uint8 certamenModePin = 3;
volatile uint8* certamenModePort;
uint8 certamenModeMask;

volatile uint8* playerPorts[maxPlayers];
uint8 playerMasks[maxPlayers];

char tonePlaying = 0;
char buttonState = 0;         // variable for reading the pushbutton status
char ledState = 0;
long lastButtonDownTime = -1;
long toneOffTime = 0;
uint8 pressOrder[maxPlayers];
char buttonDown[maxPlayers];
char teamButtonDown[numTeams];
uint8 numPressed = 0;
uint8 certamenMode = 1;
sint8 add[maxPlayers]; // maximum of 128 players
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
    for (int i=0; i<maxPlayers; i++) buttonDown[i] = 0;
    for (int i=0; i<numTeams; i++) teamButtonDown[i] = 0;
#ifdef SERIAL_ECHO
    Serial.println("Clear");
#endif
    updateScreen();
  }
#if defined(TEST_MODE) && !defined(OSCILLATION_TEST)
  digitalWrite(testPin, LOW);
#endif
}


#ifdef USE_TFT
inline void draw() {
  screen.setTextColor(textColor);
}

void setupScreen() {
#if defined(ST7735_RST_PIN)  // reset like Adafruit does
  FastPin<ST7735_RST_PIN>::setOutput();
  FastPin<ST7735_RST_PIN>::hi();
  FastPin<ST7735_RST_PIN>::lo();
  delay(1);
  FastPin<ST7735_RST_PIN>::hi();
#endif
  screen.begin(); // initR(tftVersion);
  screen.setRotation(1);
  screen.fillScreen(backgroundColor);
  draw();
//  screen.setCursor(1,1);
//  screen.print("Pruss's Certamen Machine");
//  screen.setCursor(1,1+10);
//  screen.print("GPL3 licensed firmware");
  draw_nicefont(screen,"Pruss's Certamen Machine",1,1,textColor);
  draw_nicefont(screen,"GPL3 licensed firmware",1,21,textColor);
  delay(2500);
  screen.fillScreen(backgroundColor);
  height = screen.height();
  width = screen.width();
  screen.setTextSize(fontSize);
  textLines = (height-2) / lineHeight;
}

inline void erase() {
  screen.setTextColor(backgroundColor);
}

void updateScreen() {
  static uint8 currentCertamenMode = 255;
  static uint8 currentPlayersShown[maxPlayers];
  static uint8 numCurrentPlayersShown = 0;
  
  if (certamenMode != currentCertamenMode) {

    if (currentCertamenMode != 255) {
      erase();
      screen.setCursor(1,1);
      screen.print(certamenModeText[currentCertamenMode]);
    }
    draw();
    screen.setCursor(1,1);
    screen.print(certamenModeText[certamenMode]);
    currentCertamenMode = certamenMode;
  }
  
  if (numCurrentPlayersShown != numPressed || memcmp(currentPlayersShown, pressOrder, numPressed) != 0) {
    int numToUpdate = numPressed > numCurrentPlayersShown ? numPressed : numCurrentPlayersShown;
    uint8 numColumns = (maxPlayers+textLines-2) / (textLines-1);
    uint8 columnWidth = (width-2) / numColumns;
    for (int i=0; i<numToUpdate; i++) {
      if ((i>=numPressed != i>=numCurrentPlayersShown) || (i<numPressed && currentPlayersShown[i] != pressOrder[i])) {
        uint8 y = 1 + (1 + ( i % (textLines-1) )) * lineHeight;
        uint8 x = 1 + ( i / (textLines-1) ) * columnWidth;
        if (i < numCurrentPlayersShown) {
          erase();
          screen.setCursor(x, y);
          screen.print(playerName[currentPlayersShown[i]]);
        }
        if ( i < numPressed ) {
          uint8 id = pressOrder[i];
          draw();
          screen.setCursor(x, y);
          screen.print( playerName[id] );
          currentPlayersShown[i] = id;
        }
      }
    }
    numCurrentPlayersShown = numPressed;
  }
}

#else
void setupScreen() {
}
void updateScreen() {
}
#endif

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
    playerPorts[id] = portInputRegister(digitalPinToPort(playerPins[id]));
    playerMasks[id] = digitalPinToBitMask(playerPins[id]);
    //TODO: disable failed?
  }
  clearPort = portInputRegister(digitalPinToPort(clearPin));
  clearMask = digitalPinToBitMask(clearPin);
  certamenModePort = portInputRegister(digitalPinToPort(certamenModePin));
  certamenModeMask = digitalPinToBitMask(certamenModePin);
#ifdef SERIAL_ECHO
  Serial.begin(9600);
  Serial.println("Certamen test mode\r\n");
#endif
  setupScreen();
  clearState(1);
}

void scan() {
#if defined(TEST_MODE) && defined(OSCILLATION_TEST)
  testPinState = ! testPinState;
  digitalWrite(testPin, testPinState);
#endif

  uint8 toAdd = 0;
  uint8 previousNumPressed = numPressed;

  for (int id=numTeams*playersPerTeam-1; id>=0; id--) {
      if (!(*(playerPorts[id]) & playerMasks[id]) && !buttonDown[id] && (!certamenMode || !teamButtonDown[getTeamFromID(id)])) {
          buttonDown[id] = 1;
          add[toAdd++] = id;
      }
  }

#if RANDOM_PRESS_FREQUENCY
  if (toAdd == 0 && random(RANDOM_PRESS_FREQUENCY) == 0) {
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
#ifdef SERIAL_ECHO
        Serial.println("buzz");
#endif
    }
    updateScreen();
#ifdef SERIAL_ECHO
    Serial.println(certamenMode ? "pressed in certamen mode:" : "pressed in non-certamen mode:");
    for (uint8 i=0; i<numPressed;i++) {
      Serial.println(playerName[pressOrder[i]]);
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

  char certamenModeSwitch = 0 != (*certamenModePort & certamenModeMask);

  if (certamenModeSwitch != certamenMode) {
    // TODO: might want to debounce this a little to avoid too many calls to clearState(),
    // but is it worth the bother? Well, it might depend on how the display works.
    clearState(0);
    certamenMode = certamenModeSwitch;
  } 

#if CLEAR_FREQUENCY
  if (random(CLEAR_FREQUENCY)) clearState(0);
#endif

  if (! (*clearPort & clearMask) ) {
    clearState(0);
  }
  else {
    scan();
  }
}

