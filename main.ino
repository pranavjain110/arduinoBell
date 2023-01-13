#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <String.h>

//************************ User Input *************************//
// Define Break Time
String breakTimeList_24hrClock[] = {"1:00", "23:55", "22:22"};
int breakDurationList_Minute[] = {1, 2, 3};

// Wi-FI ssid and password
//  const char *ssid     = "AIRTELBB";
//  const char *password = "893C9563DA";
const char *ssid = "Dimple";
const char *password = "zaqq3566";
//*************************************************************//


// GPIO 12,13,14 are special and they both actuate 2 pins each on the board
//C:\.....\ArduinoData\packages\esp8266\hardware\esp8266\3.1.0\variants\d1_mini/pins_arduino.h has declation for D5,D6,D7
const int RedLight = D6;   // D6,D12
const int GreenLight = D7; // D7,D11
const int Buzzer = D5;     // D5,D13

const long utcOffsetInSeconds = 19800; // UTF time offset for India = 19800

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int breakHourList[sizeof(breakTimeList_24hrClock) / sizeof(breakTimeList_24hrClock[0])];
int breakMinuteList[sizeof(breakTimeList_24hrClock) / sizeof(breakTimeList_24hrClock[0])];

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

  for (int i = 0; i < sizeof(breakTimeList_24hrClock) / sizeof(breakTimeList_24hrClock[0]); i++)
  {

    Serial.println(breakTimeList_24hrClock[i]);
    String breakTime = breakTimeList_24hrClock[i];
    breakHourList[i] = breakTime.substring(0, breakTime.indexOf(":")).toInt();
    breakMinuteList[i] = breakTime.substring(breakTime.indexOf(":") + 1).toInt();
  }

  timeClient.begin();

  // Code to set the GPIO as output type
  pinMode(RedLight, OUTPUT);
  pinMode(GreenLight, OUTPUT);
  pinMode(Buzzer, OUTPUT);
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

  for (int i = 0; i < sizeof(breakTimeList_24hrClock) / sizeof(breakTimeList_24hrClock[0]); i++)
  {
    if (timeClient.getHours() == breakHourList[i] && timeClient.getMinutes() == breakMinuteList[i])
    {
      Serial.println("Break Time");
      digitalWrite(Buzzer, LOW);
      digitalWrite(RedLight, LOW);
      digitalWrite(GreenLight, HIGH);

      // Turn off the buzzer for 30 seconds
      delay(30 * 1000);
      digitalWrite(Buzzer, HIGH);

      if (breakDurationList_Minute[i] > 1)
      {
        delay((breakDurationList_Minute[i] - 1) * 60 * 1000); //Convert minute to millisecond
      }

      // Turn on the buzzer for 30 seconds
      digitalWrite(Buzzer, LOW);
      delay(30 * 1000);
      digitalWrite(Buzzer, HIGH);
    }
  }

  digitalWrite(RedLight, HIGH);
  digitalWrite(GreenLight, LOW);
  digitalWrite(Buzzer, HIGH);

  delay(1000); // wait for a second
}
