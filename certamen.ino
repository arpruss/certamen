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
#undef USE_TFT
#define USE_LIQUID_CRYSTAL

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

#ifdef USE_LIQUID_CRYSTAL
#include <LiquidCrystal.h>
#define BACKLIGHT_PIN 7
#define CONTRAST_PIN 8
#define RS 43
#define ENABLE 44
#define D0 45
#define D1 46
#define D2 47
#define D3 48
#define D4 49
#define D5 50
#define D6 51
#define D7 52

#define CONTRAST 125

#define SCREEN_LINES 2
#define SCREEN_COLUMNS 16

LiquidCrystal lcd(RS,ENABLE,D0,D1,D2,D3,D4,D5,D6,D7);
#endif

const uint8_t buttonPin = 2;     // the number of the pushbutton pin
const uint8_t ledPin =  13;      // the number of the LED pin
const uint8_t buzzerPin = 9;
const uint8_t clearPin = 42;
const uint8_t certamenModePin = 3;
const uint8_t unconnectedAnalogPin = 5;
#ifdef TEST_MODE
const uint8_t testPin = 53;
#endif
const uint8_t playerPins[] = {
30,31,32,33,
34,35,36,37,
38,39,40,41
};


#ifndef TEST_MODE
#define RANDOM_PRESS_FREQUENCY 0
#define CLEAR_FREQUENCY 0
#undef OSCILLATION_TEST
#undef SERIAL_ECHO
#endif

#ifdef TEST_MODE
#ifdef OSCILLATION_TEST
uint8_t testPinState = 0;
#endif
#endif

#ifdef USE_TFT
PDQ_ST7735 screen = PDQ_ST7735();

const unsigned int backgroundColor = ST7735_YELLOW;
const unsigned int textColor = ST7735_BLACK;
const uint8_t lineHeight = 16;
const uint8_t fontSize = 2;
uint8_t height;
uint8_t width;
uint8_t textLines;
const char* certamenModeText[2] = {"Quiz/Test","Certamen"};
#endif

#ifdef USE_LIQUID_CRYSTAL
const char* certamenModeText[2] = {"Quiz ","Cert."};
#endif


const uint8_t debounceTime = 50;


const uint8_t numTeams = 3;
const uint8_t playersPerTeam = 4;
const uint8_t maxPlayers = numTeams * playersPerTeam;
const char playerName[maxPlayers][3] = { "A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4", "C1", "C2", "C3", "C4" };
volatile uint8_t* clearPort;
uint8_t clearMask;
volatile uint8_t* certamenModePort;
uint8_t certamenModeMask;

volatile uint8_t* playerPorts[maxPlayers];
uint8_t playerMasks[maxPlayers];

char tonePlaying = 0;
char buttonState = 0;         // variable for reading the pushbutton status
char ledState = 0;
long lastButtonDownTime = -1;
long toneOffTime = 0;
uint8_t pressOrder[maxPlayers];
char buttonDown[maxPlayers];
char teamButtonDown[numTeams];
uint8_t numPressed = 0;
uint8_t certamenMode = 1;
int8_t add[maxPlayers]; // maximum of 128 players
char stringBuffer[128];

inline uint8_t getPlayerID(uint8_t team, uint8_t player) {
  return team*playersPerTeam + player;
}

inline int getTeamFromID(uint8_t id) {
  return id/playersPerTeam;
}

inline int getPlayerFromID(uint8_t id) {
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
  static uint8_t currentCertamenMode = 255;
  static uint8_t currentPlayersShown[maxPlayers];
  static uint8_t numCurrentPlayersShown = 0;
  
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
    uint8_t numColumns = (maxPlayers+textLines-2) / (textLines-1);
    uint8_t columnWidth = (width-2) / numColumns;
    for (int i=0; i<numToUpdate; i++) {
      if ((i>=numPressed != i>=numCurrentPlayersShown) || (i<numPressed && currentPlayersShown[i] != pressOrder[i])) {
        uint8_t y = 1 + (1 + ( i % (textLines-1) )) * lineHeight;
        uint8_t x = 1 + ( i / (textLines-1) ) * columnWidth;
        if (i < numCurrentPlayersShown) {
          erase();
          screen.setCursor(x, y);
          screen.print(playerName[currentPlayersShown[i]]);
        }
        if ( i < numPressed ) {
          uint8_t id = pressOrder[i];
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

#elif defined(USE_LIQUID_CRYSTAL)
void setupScreen() {
  pinMode(BACKLIGHT_PIN, HIGH);
  digitalWrite(BACKLIGHT_PIN, HIGH);
  analogWrite(CONTRAST_PIN, 125);
  lcd.begin(SCREEN_COLUMNS, SCREEN_LINES);
  lcd.setCursor(0,0);
  lcd.print("Pruss's Certamen");
  lcd.setCursor(0,1);
  lcd.print("GPLv3 license");
  delay(2500);
  lcd.clear();
}

inline uint8_t lineNumber(uint8_t pos) {
  return pos<5 ? 0 : 1;
}

inline uint8_t columnNumber(uint8_t pos) {
  return pos<5 ? 6+pos*2 : (pos-5)*2;
}

void updateScreen() {
  static uint8_t currentCertamenMode = 255;
  static uint8_t currentPlayersShown[maxPlayers];
  static uint8_t numCurrentPlayersShown = 0;
  
  if (certamenMode != currentCertamenMode) {
    lcd.setCursor(0,0);
    lcd.print(certamenModeText[certamenMode]);
    currentCertamenMode = certamenMode;
  }

  if (numCurrentPlayersShown != numPressed || memcmp(currentPlayersShown, pressOrder, numPressed) != 0) {
    int numToUpdate = numPressed > numCurrentPlayersShown ? numPressed : numCurrentPlayersShown;
    for (int i=0; i<numToUpdate; i++) {
      if ((i>=numPressed != i>=numCurrentPlayersShown) || (i<numPressed && currentPlayersShown[i] != pressOrder[i])) {
        lcd.setCursor(columnNumber(i), lineNumber(i));
        if ( i < numPressed ) {
          uint8_t id = pressOrder[i];
          lcd.print( playerName[id] );
          currentPlayersShown[i] = id;
        }
        else {
          lcd.print( "  " );
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

  uint8_t toAdd = 0;
  uint8_t previousNumPressed = numPressed;

  for (int id=numTeams*playersPerTeam-1; id>=0; id--) {
      if (!(*(playerPorts[id]) & playerMasks[id]) && !buttonDown[id] && (!certamenMode || !teamButtonDown[getTeamFromID(id)])) {
          buttonDown[id] = 1;
          add[toAdd++] = id;
      }
  }

#if RANDOM_PRESS_FREQUENCY
  if (toAdd == 0 && random(RANDOM_PRESS_FREQUENCY) == 0) {
    for (uint8_t i = 0; i < NUMBER_OF_RANDOM_PRESSES ; i++) {
      uint8_t id = random(numTeams*playersPerTeam);
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
      int8_t leftToAdd = toAdd;
      uint8_t j;
      do {
        uint8_t n = random() % leftToAdd;
        for (j=0; j<toAdd; j++) {
          if (add[j] > -1) {
            if (n==0) {
              uint8_t id = add[j];
              uint8_t team = getTeamFromID(id);
              teamButtonDown[team] = 1;
              pressOrder[numPressed++] = id;
              if (certamenMode) {
                for (uint8_t k=0; k<toAdd; k++) {
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
    for (uint8_t i=0; i<numPressed;i++) {
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
  uint8_t currentButton = digitalRead(buttonPin);

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

