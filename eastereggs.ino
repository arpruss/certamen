#include <avr/wdt.h>
#include "synth.h"
#include "notes.h"
#include "song.h"

const char ae[] PROGMEM = "arma uirumque cano Troiae qui primus ab oris / "
"Italiam fato profugus Lauiniaque uenit / "
"litora multum ille et terris iactatus et alto / "
"ui superum saeuae memorem Iunonis ob iram / "
"multa quoque et bello passus dum conderet urbem / "
"inferretque deos Latio genus unde Latinum / "
"Albanique patres atque altae moenia Romae / "
"Musa mihi causas memora quo numine laeso / "
"quidue dolens regina deum tot uoluere casus / "
"insignem pietate uirum tot adire labores / "
"impulerit tantaene animis caelestibus irae / "
"urbs antiqua fuit Tyrii tenuere coloni / "
"Karthago Italiam contra Tiberinaque longe / "
"ostia diues opum studiisque asperrima belli / "
"quam Iuno fertur terris magis omnibus unam / "
"posthabita coluisse Samo hic illius arma / "
"hic currus fuit hoc regnum dea gentibus esse / "
"si qua fata sinant iam tum tenditque fouetque / "
"progeniem sed enim Troiano a sanguine duci / "
"audierat Tyrias olim quae uerteret arces / "
"hinc populum late regem belloque superbum / "
"uenturum excidio Libyae sic uoluere Parcas / "
"id metuens ueterisque memor Saturnia belli / "
"prima quod ad Troiam pro caris gesserat Argis / "
"necdum etiam causae irarum saeuique dolores                           ";

char eggModeSwitch;

void checkExit() {
  if ( (0 != (*certamenModePort & certamenModeMask)) != eggModeSwitch ) {
     wdt_enable(WDTO_15MS);
  }
}

char text[33] = "";
void scrollingWrite(char c) {
  int l = strlen(text);
  if (l<32) {
    text[l] = c;
    text[l+1] = 0;
  }
  else {
    memmove(text,text+1,31);
    text[31] = c;
  }
  lcd.setCursor(0,0);
  int i;
  for (i=0;i<16 && text[i];i++) lcd.write(text[i]);
  if (i<16)
    return;
  lcd.setCursor(0,1);
  for (i=16;i<32 && text[i];i++) lcd.write(text[i]); 
}

void easterEgg2() {
  int i = 0;
  lcd.setCursor(0,0);
  while(1) {
    checkExit();
    char c = pgm_read_byte_near(ae+i);
    if (c==0)
      return;
    scrollingWrite(c);
    delay(150);
    i++;
  }
}

synth edgar;

#if 0
void easterEgg1() {
  edgar.begin(CHB);

  edgar.setupVoice(0,SINE,60,ENVELOPE0,80,64);
  edgar.setupVoice(1,SINE,62,ENVELOPE0,100,64);
  edgar.setupVoice(2,SINE,64,ENVELOPE2,110,64);
  edgar.setupVoice(3,SINE,67,ENVELOPE0,110,64);

  edgar.mTrigger(0,53+32);
  delay(1000);
#if 0

  for (int thisNote = 0; thisNote < 1000; thisNote++) {

    /*
    to calculate the note duration, take one second divided by the note type.
     e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     */
    int noteDuration = 800/noteDurations[thisNote];
    //    tone(8, melody[thisNote],noteDuration);

    if(melody[thisNote]<=NOTE_E4)
      edgar.mTrigger(1,melody[thisNote]+32);
    else
      edgar.mTrigger(0,melody[thisNote]+32);
    delay(noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }

#endif
}
#endif

#if 1
void easterEgg1() {
  edgar.begin(CHB);
  
  edgar.setupVoice(0,SINE,60,ENVELOPE0,80,64);
  edgar.setupVoice(1,SINE,62,ENVELOPE0,100,64);
  edgar.setupVoice(2,SINE,64,ENVELOPE2,110,64);
  edgar.setupVoice(3,SINE,67,ENVELOPE0,110,64);

  uint8_t buttons[sizeof(playerPins)+1];
  uint8_t curNotes[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
  uint8_t notes[4];

  volatile uint8_t* ports[maxPlayers+1];
  uint8_t masks[maxPlayers+1];

  memcpy(ports, playerPorts, sizeof(uint8_t*)*maxPlayers);
  memcpy(masks, playerMasks, sizeof(uint8_t)*maxPlayers);
  ports[maxPlayers] = clearPort;
  masks[maxPlayers] = clearMask;

  while(1) {
    checkExit();
    uint8_t count = 0;
    for (uint8_t i=0; i<=maxPlayers; i++) {
      if (0 == (masks[i] & *(ports[i]))) {
        notes[count++] = 37+i; // C4
        if (count>=4)
          break;
      }
    }
    for (; count<4; count++) 
      notes[count] = 0xFF;

    for(uint8_t i=0; i<4; i++) {
        if (notes[i] != 0xFF) {
          edgar.setLength(i, 128); 
          if (notes[i] != curNotes[i]) {
            edgar.mTrigger(i, 32+notes[i]);
            curNotes[i] = notes[i];
          }
        }
        else {
          if (notes[i] != curNotes[i]) {
            edgar.setLength(i, 0); 
            curNotes[i] = 0xFF;
          }
        }          
    }
  }
}
#endif


void easterEggs() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Exit: Toggle");
  lcd.setCursor(0, 0);
  eggModeSwitch = 0 != (*certamenModePort & certamenModeMask);
  
  if (1 == eggModeSwitch) {
    lcd.print("Easter Egg #2");
    delay(200);
    easterEgg2();
  }
  else {
    lcd.print("Easter Egg #1");
    easterEgg1();
  }
}

