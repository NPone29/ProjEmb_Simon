
// Project: Simon
// Author : NPone29
// Date   : 08.10.2026
// Version: 1.0
// Description: This code allows you to play the game Simon.


#include <Bounce2.h>
#include "variables.h"

Bounce valButton[4];

const int ode_to_joy_melody[] PROGMEM = {
  NOTE_E4, 8, 4, NOTE_E4, 8, 4,  NOTE_F4, 8, 2,  NOTE_G4, 8, 5,
  NOTE_G4, 8, 5,  NOTE_F4, 8, 2, NOTE_E4, 8, 4, NOTE_D4, 8, 3,
  NOTE_C4, 8, 2, NOTE_C4, 8, 2,  NOTE_D4, 8, 3,  NOTE_E4, 8, 4,
  NOTE_E4, 4, 4, NOTE_D4, 8, 3,  NOTE_D4, 4, 3
};

const int start_melody[] PROGMEM = {
  NOTE_C4, 8, 2, NOTE_D4, 8, 3, NOTE_E4, 8, 4, NOTE_D4, 8, 3, NOTE_C4, 8, 2,
  NOTE_G4, 8, 5, NOTE_C4, 8, 2 
};

void setup() {
  for(int i=2;i<=5;i++) pinMode(i,OUTPUT);

  for(int i = 0; i <= 3;i++) {
    valButton[i] = Bounce();
    valButton[i].attach(A2 + i, INPUT_PULLUP);
    valButton[i].interval(25);
  }
}

void loop() {

  valButton[0].update();
  

  if(valButton[0].fell()) {

    while(true) {

      int currentCheckIndex = 0;
      int randomNum = -1;
      int combinations[100];
      int nbElements = -1;
      bool wrong_combination = false;

      bool playing = true;

      play_melody("start");
      delay(100);

      while(true) {

        for(int i=0;i< 4;i++) {
          valButton[i].update();
        }

        if(playing) {

          for(int i = 0; i<=nbElements;i++) {
            int currentNote = combinations[i];
            show_note(currentNote);
            delay(100);
          }

          randomNum = random(0, 4);
          combinations[nbElements + 1] = randomNum;
          nbElements++;
          show_note(randomNum);

          playing = false;
        } else {

          int correct_response = combinations[currentCheckIndex];

          if(wrong_combination) {
            play_lose_sound();
            wrong_combination = false;
            break;
          }

          for (int i = 0; i < 4; i++) {
            if (valButton[i].fell()) { 
              show_note(i);

              if (i != correct_response) {
                wrong_combination = true;
                break;
              }

              currentCheckIndex++;

              if(currentCheckIndex >= nbElements + 1) {

                if(nbElements == 2) {
                  delay(100);
                  play_melody("ode_to_joy");
                  delay(500);
                }

                currentCheckIndex = 0;
                playing = true; 
                delay(100);
                break;
              }
            }
          }
        }
      }
    }
  }
}

void show_note(int combination) {

  int game_notes[5] = { NOTE_MI4, NOTE_DO4, NOTE_LA3, NOTE_MI3 };
  int ledPin = combination + 2;

  digitalWrite(ledPin, HIGH);
  tone(BUZZER_PIN, game_notes[combination]);
  delay(400);
  digitalWrite(ledPin, LOW);
  noTone(BUZZER_PIN);
}

void play_melody(String music) {

  const int* selectedMelody = NULL;
  int totalNotes = 0;
  int tempo = 80;

  if (music == "start") {
    selectedMelody = start_melody;
    totalNotes = sizeof(start_melody) / sizeof(start_melody[0]) / 3;
    tempo = START_MELODY_TEMPO;
  } 
  else if (music == "ode_to_joy") {
    selectedMelody = ode_to_joy_melody;
    totalNotes = sizeof(ode_to_joy_melody) / sizeof(ode_to_joy_melody[0]) / 3;
  }

  if (selectedMelody == NULL) return;

  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;
  
  for (int thisNote = 0; thisNote < totalNotes * 3; thisNote = thisNote + 3) {

      divider = pgm_read_word_near(selectedMelody + thisNote + 1);
      if (divider > 0) {
       
        noteDuration = wholenote / divider;
      } else if (divider < 0) {
       
        noteDuration = wholenote / abs(divider);
        noteDuration *= 1.5;
      }

      int ledPin = pgm_read_word_near(selectedMelody + thisNote + 2);
      digitalWrite(ledPin, HIGH); 

      tone(BUZZER_PIN, pgm_read_word_near(selectedMelody + thisNote), noteDuration * 0.9);
      delay(noteDuration);
      digitalWrite(ledPin, LOW);
      noTone(BUZZER_PIN);
  }
}

void play_lose_sound() {
  for(int i = 0; i <=3;i++) {
    tone(BUZZER_PIN, NOTE_DO2);
    delay(500);
    noTone(BUZZER_PIN);
  }
}

