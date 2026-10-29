// Project: Simon
// Author : NPone29
// Date   : 29.10.2026
// Version: 1.3
// Description: This code allows you to play the game Simon.

// Importing the necessary modules
#include <Bounce2.h>
#include "variables.h"

Bounce valButton[4];

// Definition of ode to joy melody
const int ode_to_joy_melody[] PROGMEM = {
  NOTE_E4, 8, 4, NOTE_E4, 8, 4,  NOTE_F4, 8, 2,  NOTE_G4, 8, 5,
  NOTE_G4, 8, 5,  NOTE_F4, 8, 2, NOTE_E4, 8, 4, NOTE_D4, 8, 3,
  NOTE_C4, 8, 2, NOTE_C4, 8, 2,  NOTE_D4, 8, 3,  NOTE_E4, 8, 4,
  NOTE_E4, 4, 4, NOTE_D4, 8, 3,  NOTE_D4, 4, 3
};

// Definition of the start melody
const int start_melody[] PROGMEM = {
  NOTE_C4, 8, 2, NOTE_D4, 8, 3, NOTE_E4, 8, 4, NOTE_D4, 8, 3, NOTE_C4, 8, 2,
  NOTE_G4, 8, 5, NOTE_C4, 8, 2
};

// Definition of the musical notes for the LEDs
int game_notes[4] = { NOTE_E4, NOTE_C4, NOTE_A3, NOTE_E3 };

void setup() {

  // Set the LEDs to OUTPUT pinmode
  for(int i=FIRST_LED_PIN;i<=LAST_LED_PIN;i++) pinMode(i,OUTPUT);

  // Set the buttons as a bounce one in the button array.
  for(int button = 0; button <= 3;button++) {
    valButton[button] = Bounce();
    valButton[button].attach(A2 + button, INPUT_PULLUP);
    valButton[button].interval(25);
  }

  // Initializes the random number generator using the analog reading from pin A0.
  randomSeed(analogRead(A0));
}

void loop() {

  // Update the first button status
  valButton[0].update();

  // Wait for the user to press button 1
  if(valButton[0].fell()) {

    // loop that start the simon game initiation
    while(true) {

      // Important variable definition
      int currentCheckIndex = 0;
      int randomNum = -1;
      int combinations[MAX_COMBINATIONS];
      int nbElements = -1;
      bool wrong_combination = false;
      bool game_won = false;
      bool isShowingSequence = true;

      // Play the start melody and wait 0.1 seconds before starting the game.
      play_melody("start");
      delay(100);

      // Loop that start the game
      while(true) {

        // Update the status of all buttons
        for(int button=0; button < 4; button++) {
          valButton[button].update();
        }

        // If the variable is true, then
        if(isShowingSequence) {

          // Repeat all previous sequences
          for(int i = 0; i<=nbElements;i++) {
            int currentNote = combinations[i];
            show_note(currentNote);
            delay(100);
          }

          // Add a new sequence in the list
          randomNum = random(0, 4);
          combinations[nbElements + 1] = randomNum;
          nbElements++;
          show_note(randomNum);

          // Set the variable to false to allow the user to play.
          isShowingSequence = false;
        } else {

          // the correct combination
          int correct_combination = combinations[currentCheckIndex];

          // If the wrong combination was pressed
          if(wrong_combination) {
            // Play the lose sound with all leds
            defeat();
            // The break allows you to stop the current game and start over.
            break;
          }

          if(game_won) {
            // Play the win sound with all leds
            victory();
            // The break allows you to stop the current game and start over.
            break;
          }

          // Loop for the 4 buttons
          for (int button = 0; button < 4; button++) {

            // If a button is pressed
            if (valButton[button].fell()) {
              show_note(button);

              // If it is not the correct combination
              if (button != correct_combination) {
                // Set wrong_combination to true
                wrong_combination = true;
                // Break the loop for the buttons
                break;
              }

              // Add 1 to the currentCheckIndex
              currentCheckIndex++;

              // If all combinations have been made, then
              if(currentCheckIndex >= nbElements + 1) {
                delay(100);

                // If the number of combinations reaches the maximum, the user has won.
                if(nbElements + 1 == MAX_COMBINATIONS) {
                  game_won = true;
                  break;
                }

                // If the number of combinations reaches 31, the user has won and the melody of "Ode to Joy" plays.
                if(nbElements + 1 == 31) {
                  play_melody("ode_to_joy");
                  delay(500);
                }

                // Reset the index so the player restarts from the first code of the sequence
                currentCheckIndex = 0;
                // return to the showing sequence
                isShowingSequence = true;

                // break the buttons variable
                break;
              }
            }
          }
        }
      }
    }
  }
}

// Function that turns on the LED and plays its sound.
void show_note(int combination) {

  // Definition of the ledPin
  int ledPin = combination + 2;

  // Start turning on the LEDs and trigger the sound.
  digitalWrite(ledPin, HIGH);
  tone(BUZZER_PIN, game_notes[combination]);

  // After 0.4 seconds, turn off the LED and stop the sound
  delay(400);
  digitalWrite(ledPin, LOW);
  noTone(BUZZER_PIN);
}

// Function that allows for playing the opening melody and "Ode to Joy"
void play_melody(String music) {

  // Definition of all necessary variables
  const int* selectedMelody = NULL;
  int totalNotes = 0;
  int tempo = 80;

  // Change the melody, note, and tempo variables according to the music
  if (music == "start") {
    selectedMelody = start_melody;
    totalNotes = sizeof(start_melody) / sizeof(start_melody[0]) / 3;
    tempo = START_MELODY_TEMPO;
  }
  else if (music == "ode_to_joy") {
    selectedMelody = ode_to_joy_melody;
    totalNotes = sizeof(ode_to_joy_melody) / sizeof(ode_to_joy_melody[0]) / 3;
  }

  // If the melody is invalid, then return
  if (selectedMelody == NULL) return;

  // Calculate the duration of a whole note in ms based on tempo
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  // Loop through the melody: each note uses 3 consecutive values (Frequency, Duration/Divider, LED Pin)
  for (int thisNote = 0; thisNote < totalNotes * 3; thisNote = thisNote + 3) {

      // Read the duration divider from Flash memory (PROGMEM)
      divider = pgm_read_word_near(selectedMelody + thisNote + 1);

      // Depending on the value of divider, assign a different duration to the note.
      if (divider > 0) {
        noteDuration = wholenote / divider;
      } else if (divider < 0) {
        noteDuration = wholenote / abs(divider);
        noteDuration *= 1.5;
      }

      // Retrieve the LED pin associated with this note from Flash memory
      int ledPin = pgm_read_word_near(selectedMelody + thisNote + 2);
      digitalWrite(ledPin, HIGH);

      // Play the frequency from Flash memory for 90% of its duration to articulate the notes
      tone(BUZZER_PIN, pgm_read_word_near(selectedMelody + thisNote), noteDuration * 0.9);

      // Wait for the note duration to complete
      delay(noteDuration);
      // Turn off the LED and stop the sound
      digitalWrite(ledPin, LOW);
      noTone(BUZZER_PIN);
  }
}

// Function that turns on all the LEDs and trigger the end-of-game sound.
void defeat() {
  // Loop that executes 4 times
  for(int i = 0; i <=3;i++) {

    //Loop that turn on the 4 LEDs
    for(int ledPin = FIRST_LED_PIN; ledPin <= LAST_LED_PIN; ledPin++) digitalWrite(ledPin, HIGH);

    // trigger the end-of-game sound
    tone(BUZZER_PIN, NOTE_C2);

    //After 0.25 seconds, turn off all LEDs and stop the end-of-game sound.
    delay(250);

    // Loop that turn off the 4 LEDs
    for(int ledPin = FIRST_LED_PIN; ledPin <= LAST_LED_PIN; ledPin++) {
      digitalWrite(ledPin, LOW);
    }

    // Stop the end-of-game sound
    noTone(BUZZER_PIN);

    // wait the next sequence or the end of the loop
    delay(250);
  }
}

// Function that play a LEDs spectacle and play the "Ode to Joy" melody
void victory() {
  // Loop that executes 3 times
  for (int tour = 0; tour < 3; tour++) {
    // Loop that turn on the 4 LEDs one by one
    for (int led = FIRST_LED_PIN; led <= LAST_LED_PIN; led++) {
      digitalWrite(led, HIGH); 
      delay(80); 
      digitalWrite(led, LOW);
    }
  }

  // Play the "Ode to Joy" melody
  play_melody("ode_to_joy");

  // Loop that executes 3 times
  for (int i = 0; i < 3; i++) {
    // Loop that turn on the 4 LEDs
    for (int led = FIRST_LED_PIN; led <= LAST_LED_PIN; led++) digitalWrite(led, HIGH);
    delay(300);
    // Loop that turn off the 4 LEDs
    for (int led = FIRST_LED_PIN; led <= LAST_LED_PIN; led++) digitalWrite(led, LOW);
    delay(300);
  }
}
