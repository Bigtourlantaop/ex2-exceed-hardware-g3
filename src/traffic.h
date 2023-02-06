#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>

#define CODE "5dukr"

const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app";

const String point = "3";
const int nearby_1 = 2;
const int nearby_2 = 4;

void GET_traffic()
{
  DynamicJsonDocument doc(65536);
  HTTPClient http;
  const String url = baseUrl + "/all_traffic";
  http.begin(url);

  Serial.println("Nearby traffic");
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);

    JsonArray array = doc["all_traffic"].as<JsonArray>();
    for (JsonVariant val : array) {
      // For all objects in array,
      int pt = val["point"].as<int>();
      if (pt == point.toInt() || pt == nearby_1 || pt == nearby_2) {
        String ptStr = String(pt);
        String status = val["traffic"];
        /*
         * String format:
         * point [point]: [status]
         */
        Serial.print("point ");
        Serial.print(ptStr);
        Serial.print(": ");
        Serial.println(status);
      }
    }
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void POST_traffic(String led)
{
  const String url = baseUrl + "/my_traffic?point=" + point;
  String json;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(2048);
  doc["code"] = CODE;
  doc["traffic"] = led;
  serializeJson(doc, json);

  Serial.println("POST " + led);
  int httpResponseCode = http.POST(json);

  // Add verbose response from server
  DynamicJsonDocument responseJson(2048);
  String response = http.getString();
  deserializeJson(responseJson, response);

  Serial.print("status: "); Serial.println((const char*) responseJson["status"]);
  Serial.print("message: "); Serial.println((const char*) responseJson["message"]);

  if (httpResponseCode == 200)
  {
    Serial.print("Done");
    Serial.println();
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}
