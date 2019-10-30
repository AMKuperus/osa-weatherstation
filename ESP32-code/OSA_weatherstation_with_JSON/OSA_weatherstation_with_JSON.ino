#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <ArduinoJson.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BME280 bme;

const char* ssid = "";
const char* password = "";

float last_temp = 0.0;
float temperature = 0.0;

WiFiServer server(80);

String header;

// unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME 280 test"));

  //bool status;

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check sensor and wiring");
    while (1);
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();


  //}

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}

void loop() {
  WiFiClient client = server.available();

  // The JSON that can be retrieved by the worker.
  if(!client) return;

  Serial.println(F("New Client"));

  while(client.available()) client.read();

  // Allocate temp JsonDoc with capacity
  StaticJsonDocument<500> msg;

  JsonObject root = msg.to<JsonObject>();
  root["temperature"] = bme.readTemperature();
  root["pressure"] = bme.readPressure() / 100.0F;
  root["humidity"] = bme.readHumidity();
  
  Serial.print(F("Sending..."));
  serializeJsonPretty(msg, Serial);
  Serial.println();

  // Write responseheader
  client.println(F("HTTP/1.0 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection close"));
  client.print("Content-Length: ");
  client.println(measureJsonPretty(msg));
  
  client.println();
  
  // Write JSON
  serializeJsonPretty(msg, client);
  // Disconnect client
  client.stop();

  // Display static text on the display
  temperature = bme.readTemperature();
  if (last_temp != temperature) {
    last_temp = temperature;
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 0);
    display.println("Temperature");
  // Print the temperature variable
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 12);
    display.print(temperature);
    display.drawCircle(90, 14, 2, WHITE);
    display.setCursor(95, 12);
    display.println("C");
    display.display();
  }

}
