#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <String.h>
#include "neotimer.h"

//************************ User Input *************************//
// Define Break Time
String breakTimeList_24hrClock_Mato[] = {"8:30", "12:30", "13:00","17:00","20:00","6:00"};
int bellDurationMato = 4;
String breakTimeList_24hrClock_Arihant[] = {"9:02","13:00","13:30","17:30","18:30"};
int bellDurationArihant = 4;


// Wi-FI ssid and password
const char *ssid = "MATO-EXT";
const char *password = "Pa$$word";
//*************************************************************//

//D8 on board is actuated by D3 in code
//TX1/D9 on board is actuated by D4 in code
//D10/SS on board is actuated by D8 in code
static const int Relay1 = D4;
static const int Relay2 = D3;
static const int Relay3 = D7;
static const int Relay4 = D6;
static const int Relay5 = D5;
static const int Relay6 = D2;
static const int Relay7 = D1;
static const int Relay8 = D0;

// GPIO 12,13,14 are special and they both actuate 2 pins each on the board
const int RedLight_Mato = Relay3;
const int GreenLight_Mato = Relay5;
const int Buzzer_Mato = Relay1;
// Area 2
const int RedLight_Arihant = Relay4;
const int GreenLight_Arihant = Relay6;
const int Buzzer_Arihant = Relay2;

const long utcOffsetInSeconds = 19800; // UTF time offset for India = 19800

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool bell_1_active = false;
bool bell_2_active = false;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// Area1
int breakHourList_Mato[sizeof(breakTimeList_24hrClock_Mato) / sizeof(breakTimeList_24hrClock_Mato[0])];
int breakMinuteList_Mato[sizeof(breakTimeList_24hrClock_Mato) / sizeof(breakTimeList_24hrClock_Mato[0])];
// Area2
int breakHourList_Arihant[sizeof(breakTimeList_24hrClock_Arihant) / sizeof(breakTimeList_24hrClock_Arihant[0])];
int breakMinuteList_Arihant[sizeof(breakTimeList_24hrClock_Arihant) / sizeof(breakTimeList_24hrClock_Arihant[0])];

void setup()
{
  

  Serial.begin(115200); // Baudrate
  Serial.println("ESP starts");
  // Code to connect to the Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  { // Loop which makes a point every 500ms until the connection process has finished
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP-Address: ");
  Serial.println(WiFi.localIP()); // Displaying the IP Address

  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Mato) / sizeof(breakTimeList_24hrClock_Mato[0]); i++)
  {

    Serial.println(breakTimeList_24hrClock_Mato[i]);
    String breakTime_Mato = breakTimeList_24hrClock_Mato[i];
    breakHourList_Mato[i] = breakTime_Mato.substring(0, breakTime_Mato.indexOf(":")).toInt();
    breakMinuteList_Mato[i] = breakTime_Mato.substring(breakTime_Mato.indexOf(":") + 1).toInt();
  }

  // Area 2
  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Arihant) / sizeof(breakTimeList_24hrClock_Arihant[0]); i++)
  {

    Serial.println(breakTimeList_24hrClock_Arihant[i]);
    String breakTime_Arihant = breakTimeList_24hrClock_Arihant[i];
    breakHourList_Arihant[i] = breakTime_Arihant.substring(0, breakTime_Arihant.indexOf(":")).toInt();
    breakMinuteList_Arihant[i] = breakTime_Arihant.substring(breakTime_Arihant.indexOf(":") + 1).toInt();
  }

  timeClient.begin();


  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);


  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  digitalWrite(Relay5, HIGH);
  digitalWrite(Relay6, HIGH);
  digitalWrite(Relay7, HIGH);
  digitalWrite(Relay8, HIGH);

}

void loop()
{
  // main code here, to run repeatedly:
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  
  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Mato) / sizeof(breakTimeList_24hrClock_Mato[0]); i++)
  {
    if (timeClient.getHours() == breakHourList_Mato[i] && timeClient.getMinutes() == breakMinuteList_Mato[i] && bell_1_active == false)
    {
      bell_1_active = true;
      digitalWrite(Buzzer_Mato, LOW);
      delay(bellDurationMato*1000);
      digitalWrite(Buzzer_Mato, HIGH);
    }  
  
   }
    for (int i = 0; i < sizeof(breakTimeList_24hrClock_Arihant) / sizeof(breakTimeList_24hrClock_Arihant[0]); i++)
  {
        if (timeClient.getHours() == breakHourList_Arihant[i] && timeClient.getMinutes() == breakMinuteList_Arihant[i] && bell_2_active == false)
            {
              bell_2_active = true;
              digitalWrite(Buzzer_Arihant, LOW);
              delay(bellDurationArihant*1000);
              digitalWrite(Buzzer_Arihant, HIGH);
            }

    }
   delay(1000);
   if (bell_1_active and bell_2_active)
    {
      Serial.println("(delay)bell_1_active and bell_2_active");
      bell_1_active = false;
      bell_2_active = false;
      delay((60-bellDurationMato-bellDurationArihant)*1000);
    }
  else if (bell_1_active){
      Serial.println("(delay)bell_1_active");

      bell_1_active = false;
      delay((60-bellDurationMato)*1000);
      }
  else if (bell_2_active){
        Serial.println("(delay)bell_2_active");

        bell_2_active = false;
        delay((60-bellDurationArihant)*1000);
        }
}
