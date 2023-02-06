// If you want to run in WOKWi
// change pin and wifi
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "traffic.h"

#define red 26
#define yellow 25
#define green 33
#define ldr 34
#define button 27

int state = 1;
int count = 0;
Bounce debouncer = Bounce();

void Connect_Wifi();

void setup()
{
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(ldr, INPUT);
  debouncer.attach(button, INPUT_PULLUP);
  debouncer.interval(25);
  Connect_Wifi();

  delay(200);
  // start LED with GREEN and POST to database
  digitalWrite(green, HIGH);
  POST_traffic("green");
  GET_traffic();
}

void loop()
{
  if (state == 1)
  {
    debouncer.update();
    if(debouncer.fell())
    {
      digitalWrite(green,LOW);
      state = 2;
      digitalWrite(yellow,HIGH);
      POST_traffic("yellow");
    }
  }
  else if (state == 2)
  {
    delay(8000);
    digitalWrite(yellow,LOW);
    state = 3;
    digitalWrite(red,HIGH);
    POST_traffic("red");
    GET_traffic();
  }
  else if (state == 3)
  {
    delay(5000);
    while(1) {
      int light = map(analogRead(ldr),1000,4095,0,255);
      if(light <= 30){
        digitalWrite(red,LOW);
        state = 1;
        digitalWrite(green,HIGH);
        POST_traffic("green");
        GET_traffic();
        break;
      }
    }
  }
}

void Connect_Wifi()
{
  const char *ssid = "GTZ";
  const char *password = "pass1234";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}