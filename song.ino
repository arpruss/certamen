/*
Certamen Machine
Copyright (C) 2017 Alexander R. Pruss
The Synth
Copyright (C) 2014-2017 Dzl/Illutron 2014 dzlsevilgeniuslair.blogspot.dk illutron.dk

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Example based on this demo:
//https://github.com/electricmango/Arduino-Music-Project.git
//

//Hardware connections:

//                    +10µF 
//PIN 11 ---[ 1k ]--+---||--->> Audio out
//                  |
//                 === 10nF
//                  |
//                 GND

// DZL 2014
// HTTP://dzlsevilgeniuslair.blogspot.dk
// HTTP://illutron.dk

#include "synth.h"
#include "notes.h"
#include "song.h"

synth edgar;

void song() 
{
  edgar.begin(CHB);

  edgar.setupVoice(0,SINE,60,ENVELOPE0,80,64);
  edgar.setupVoice(1,SINE,62,ENVELOPE0,100,64);
  edgar.setupVoice(2,SINE,64,ENVELOPE2,110,64);
  edgar.setupVoice(3,SINE,67,ENVELOPE0,110,64);


  for (int thisNote = 0; thisNote < 1000; thisNote++) {
    checkClear();
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
    // stop the tone playing:
    //noTone(8);
  }

}

