#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <String.h>
#include "neotimer.h"

//************************ User Input *************************//
// Define Break Time
String breakTimeList_24hrClock_Area1[] = {"9:00", "5:30", "20:00"};
int breakDurationList_Minute_Area1[] = {15, 30, 30};

String breakTimeList_24hrClock_Area2[] = {"9:00","5:45","19:30"};
int breakDurationList_Minute_Area2[] = {15, 30, 30};

// Wi-FI ssid and password
const char *ssid = "AIRTELBB";
const char *password = "Pa$$word";
//*************************************************************//

Neotimer timer_Area1;
Neotimer timer_Area2;

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
const int RedLight_Area1 = Relay1;
const int GreenLight_Area1 = Relay2;
const int Buzzer_Area1 = Relay3;
// Area 2
const int RedLight_Area2 = Relay4;
const int GreenLight_Area2 = Relay5;
const int Buzzer_Area2 = Relay6;

const long utcOffsetInSeconds = 19800; // UTF time offset for India = 19800

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// Area1
int breakHourList_Area1[sizeof(breakTimeList_24hrClock_Area1) / sizeof(breakTimeList_24hrClock_Area1[0])];
int breakMinuteList_Area1[sizeof(breakTimeList_24hrClock_Area1) / sizeof(breakTimeList_24hrClock_Area1[0])];
// Area2
int breakHourList_Area2[sizeof(breakTimeList_24hrClock_Area2) / sizeof(breakTimeList_24hrClock_Area2[0])];
int breakMinuteList_Area2[sizeof(breakTimeList_24hrClock_Area2) / sizeof(breakTimeList_24hrClock_Area2[0])];

void setup()
{
  Serial.begin(115200); // Baudrate
  Serial.println("ESP starts");
  // Code to connect to the Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  //  while (WiFi.status() != WL_CONNECTED)
  //  { // Loop which makes a point every 500ms until the connection process has finished
  //    delay(500);
  //    Serial.print(".");
  //  }
  Serial.println();
  Serial.print("Connected! IP-Address: ");
  Serial.println(WiFi.localIP()); // Displaying the IP Address

  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Area1) / sizeof(breakTimeList_24hrClock_Area1[0]); i++)
  {

    Serial.println(breakTimeList_24hrClock_Area1[i]);
    String breakTime_Area1 = breakTimeList_24hrClock_Area1[i];
    breakHourList_Area1[i] = breakTime_Area1.substring(0, breakTime_Area1.indexOf(":")).toInt();
    breakMinuteList_Area1[i] = breakTime_Area1.substring(breakTime_Area1.indexOf(":") + 1).toInt();
  }

  // Area 2
  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Area2) / sizeof(breakTimeList_24hrClock_Area2[0]); i++)
  {

    Serial.println(breakTimeList_24hrClock_Area2[i]);
    String breakTime_Area2 = breakTimeList_24hrClock_Area2[i];
    breakHourList_Area2[i] = breakTime_Area2.substring(0, breakTime_Area2.indexOf(":")).toInt();
    breakMinuteList_Area2[i] = breakTime_Area2.substring(breakTime_Area2.indexOf(":") + 1).toInt();
  }

  timeClient.begin();

  // Code to set the GPIO as output type
//  pinMode(RedLight_Area1, OUTPUT);
//  pinMode(GreenLight_Area1, OUTPUT);
//  pinMode(Buzzer_Area1, OUTPUT);
//  pinMode(RedLight_Area2, OUTPUT);
//  pinMode(GreenLight_Area2, OUTPUT);
//  pinMode(Buzzer_Area2, OUTPUT);
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
  digitalWrite(GreenLight_Area1, LOW);
  digitalWrite(GreenLight_Area2, LOW);
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

  // Serial.println(timeClient.getFormattedTime());

  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Area1) / sizeof(breakTimeList_24hrClock_Area1[0]); i++)
  {
    if (not(timer_Area1.waiting()))
    {
      if (timeClient.getHours() == breakHourList_Area1[i] && timeClient.getMinutes() == breakMinuteList_Area1[i])
      {
        Serial.println("Break Time");
        timer_Area1 = Neotimer((breakDurationList_Minute_Area1[i] * 60) * 1000);
        timer_Area1.start();
        digitalWrite(RedLight_Area1, LOW);
        digitalWrite(GreenLight_Area1, HIGH);
        digitalWrite(Buzzer_Area1, LOW);
        delay(10 * 1000);
        digitalWrite(Buzzer_Area1, HIGH);
      }
    }
    else
    {
      Serial.println("Timer 1 is running");
    }
    if (timer_Area1.done())
    {
      Serial.println("Break Time Over");
      timer_Area1.stop();
      timer_Area1.reset();
      digitalWrite(Buzzer_Area1, LOW);
      delay(10 * 1000);
      digitalWrite(Buzzer_Area1, HIGH);
      digitalWrite(RedLight_Area1, HIGH);
      digitalWrite(GreenLight_Area1, LOW);
    }
  }

  // Area 2
  for (int i = 0; i < sizeof(breakTimeList_24hrClock_Area2) / sizeof(breakTimeList_24hrClock_Area2[0]); i++)
  {
    if (not(timer_Area2.waiting()))
    {
      if (timeClient.getHours() == breakHourList_Area2[i] && timeClient.getMinutes() == breakMinuteList_Area2[i])
      {
        Serial.println("Break Time");
        timer_Area2 = Neotimer((breakDurationList_Minute_Area2[i] * 60) * 1000);
        timer_Area2.start();
        digitalWrite(RedLight_Area2, LOW);
        digitalWrite(GreenLight_Area2, HIGH);
        digitalWrite(Buzzer_Area2, LOW);
        delay(10 * 1000);
        digitalWrite(Buzzer_Area2, HIGH);
      }
    }
    else
    {
      Serial.println("Timer 2 is running");
    }
    if (timer_Area2.done())
    {

      Serial.println("Break Time Over");
      timer_Area2.stop();
      timer_Area2.reset();
      digitalWrite(Buzzer_Area2, LOW);
      delay(10 * 1000);
      digitalWrite(Buzzer_Area2, HIGH);

      digitalWrite(RedLight_Area2, HIGH);
      digitalWrite(GreenLight_Area2, LOW);
    }

  }

  

  delay(1000); // wait for a second
}