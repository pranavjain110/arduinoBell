# arduinoBell
This a a project WEMOS D1R2 to make a bell for a factory shopfloor. The user can set the bell duration and the bell time. 

When it is time for break, first the buzzer relay will turn on for 30 seconds, followed by the relay for Red LED for the duration of the break. And then buzzer will buzz again indicating the end of the break.

Once the break is over, the relay for the Red LED will turn off and the Relay for the Green LED will turn on.


The user can modify the ime of the break and the duration of the break easily from User Input section of the code.

### Wiring Schematic (WEMOS D1R2 board to 8 Relay Module board):
![schematic](https://user-images.githubusercontent.com/59677191/212400557-4811153f-79bf-4474-9ffe-78cf67146363.png)
