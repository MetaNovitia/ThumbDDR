# ThumbDDR

## Software
To operate the game, we used 2 ATmegas, transferring data using 6 pins and synchronizing using two clocks (2 pins).

The first ATmega was tasked to control the speaker, LCD module, joystick inputs, and game logic. Two timers were used; one for continuously reading analog input from the joysticks, the other for doing post-width modulation to produce sound for the speaker. Each note from the song will produce an instruction for the right hand, and a pseudo-random instruction will be given for the left hand for every quarter note. Everytime an instruction needs to be sent, it will turn the bits corresponding to the instruction on, and toggle the clock for the other ATmega to read. The user will have to move the joystick in the correct direction and points will be counted for each correct movement, which is shown on the LCD.

The second ATmega is tasked to control the LEDs and their light levels. Two timers were also used; one for turning the LEDs on and off, the other for increasing the light level (to make a glowing/pulsing effect). The light level is adjusted using post-width modulation, manipulating its duty cycle. To set which LED to start glowing next, it waits for a clock pulse from the other ATmega in the main loop, and change the level corresponding to the instruction/direction to 1, which will be incremented to 15 and then off in the second timer interrupt routine.

## Hardware
Main:
- ATMEGA32 (2)
- Joysticks (4)
- Speaker Module (1)
- LCD Module (1)
- LEDs (16)

Others:
- Resistors
- Battery
- Wires
- Regulators
- Crystals
- Capacitors

## Extra
We also coded a python script to convert MIDI files into the format we need for our c code. It uses the python Mido library.
