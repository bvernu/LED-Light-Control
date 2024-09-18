# LED-Light-Control
A system that controls an LED using a pushbutton and keys on your keyboard. Programmed using the Tiva  C Series Launchpad Board.
- the system has two states, POSITIVE and NEGATIVE, triggered by pressing 'p' or 'n' respectively, on the users keyboard (connected to board via UART)
- in the positive state, the LED will be on when the butten is pressed, and off when the button is not being pressed
- in the negative state, the LED will be off when the button is pressed, and on when the button is not being pressed
- the system defaults to positive state when the state is not specified.

The hardware aspects of the board are detailed below.
