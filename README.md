# arduinoBell
This a a project WEMOS D1R2 to make a bell for a factory shopfloor. The user can set the bell duration and the bell time. 

When it is time for break, first the buzzer relay will turn on for 30 seconds, followed by the relay for Red LED for the duration of the break. And then buzzer will buzz again indicating the end of the break.

Once the break is over, the relay for the Red LED will turn off and the Relay for the Green LED will turn on.

In the current setup of the 8 Relay Module board:
Relay 1,2 are used for Green Light 
Relay 3,4 are used for Red Light
Relay 5,6 are used for Buzzer 

The user can modify the time of the break and the duration of the break easily from User Input section of the code.

### Wiring Schematic (WEMOS D1R2 board to 8 Relay Module board):
![schematic](https://user-images.githubusercontent.com/59677191/212400557-4811153f-79bf-4474-9ffe-78cf67146363.png)


### Setup
- Install Arduino IDE https://www.arduino.cc/en/software
- Inside the IDE : Open  File->Preferences, inside 'Addition Boards Manager URLs'  paste http://arduino.esp8266.com/stable/package_esp8266com_index.json
- Open Tools->Boards->Boards Manager, search "esp8266" and install
- Go to Tool->Boards->ESP 8266 Boards, select ```LOLIN(WEMOS) R1 D2 board & mini```
(tools menue will will slightly diferent after this step)
- Go to Tools->Ports, select the right COM port to which the R1D2 board is connected on PC
- Go to Tools->Upload Speed, select 115200
- Open code in src folder in IDE
- Use the tick symbol to verify and compile the code
- Use the Upload button to upload the code on the board
