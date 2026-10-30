# ProjEmb_Simon
Welcome to my Simon game project! This project contains the C++ code to play the Simon memory game on an Arduino (preferably an Arduino Uno).

## Requirements
- [The Arduino IDE](https://www.arduino.cc/en/software), to compile the code and upload it to your Arduino.
- The Veroboard shield with the LEDs, buttons and buzzer. See the [documentation](Docs/documentation_french.md) for the schematics and how to build it.

## Installation
Clone the project:
```bash
git clone https://github.com/NPone29/ProjEmb_Simon
cd ProjEmb_Simon
```

Open `ProjEmb_Simon.ino` with the Arduino IDE.

Before compiling, install the **Bounce2** library. It filters the bounce of the buttons, so that a single press isn't read as several presses.
```text
Tools > Manage Libraries... > search for "Bounce2" > Install
```
![Bounce2 library](/Docs/Images/library_bounce.png)

Then select your board and port (`Tools > Board > Arduino Uno` and `Tools > Port`), and click **Upload**.

## How to play
1. Press the blue button (A2) to start a game.
2. Watch the sequence of lights and sounds, then repeat it with the buttons.
3. Each time you succeed, a new color is added to the sequence. One mistake and the game starts over!

**You're all set! Now all that's left is to have some fun.**

![Simon game](/Docs/Images/arduino_photo.jpg)

## Author
- [NPone29](https://github.com/NPone29)