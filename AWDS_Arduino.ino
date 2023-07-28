#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h> // Add this line to include ArduinoJson library

const char* ssid     = "ITC_AP";
const char* password = "";
const char* serverIP = "172.16.193.216"; // Replace with your Spring Boot application's IP address

WiFiClient client;
WiFiServer server(9090);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address and number of columns/rows

void setup() {

  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check WiFi status
  if (WiFi.status() == WL_CONNECTED) {
    
  } 

  //http://172.16.193.216:9090/api/volumes
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to the internet");
    // You can perform internet-related tasks here
    // Make an HTTP GET request to your Spring Boot application's endpoint
    HTTPClient http;
    String url = "http://" + String(serverIP) + ":9090/api/volumes"; // Replace with your endpoint URL
    
    http.begin(client, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Response: " + response);

      // Parse the JSON response
      StaticJsonDocument<5000> doc; // Adjust the JSON document size as needed
      DeserializationError error = deserializeJson(doc, response);

      if (error) {
        Serial.println("Error parsing JSON response");
      } else {
        // Extract the id and volume data
        const char* id = doc["id"];
        const char* volume = doc["volume"];

        // Display the id and volume on the LCD
        lcd.setCursor(0, 0);
        lcd.print("ID: ");
        lcd.print(id);

        lcd.setCursor(0, 1);
        lcd.print("Volume: ");
        lcd.print(volume);
      }
    } else {
      Serial.println("Error: " + String(httpResponseCode));
    }

    http.end();
  }

  else {
    Serial.println("Not connected to the internet");
    // Handle the case when the NodeMCU is not connected to the internet
  }

  delay(5000);
}
