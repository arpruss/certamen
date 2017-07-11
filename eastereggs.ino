#include <avr/wdt.h>

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

char lastClearState = 0;
void checkClear() {
  if (! (*clearPort & clearMask) ) {
    if (lastClearState) {
      wdt_enable(WDTO_15MS);
      //((void(*)(void))0)();
    }
  }
  else {
    lastClearState = 1;
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
    checkClear();
    char c = pgm_read_byte_near(ae+i);
    if (c==0)
      return;
    scrollingWrite(c);
    delay(150);
    i++;
  }
}

void easterEgg1() {
  
}

void easterEggs() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (1 == digitalRead(certamenModePin)) {
    lcd.print("Easter Egg #2");
    delay(20);
    easterEgg2();
  }
  else {
    lcd.print("Easter Egg #1");
    song();
  }
}

