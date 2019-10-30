#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <WiFi.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// SSD1306 display values
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//
// WiFi network and server values
const char* ssid = "";       // Enter networkname here
const char* password = "";   // Enter password here
WiFiServer server(80);

String header;

//
// BME280 sensor values
#define SEALEVELPRESSURE_HPA (1010)

Adafruit_BME280 bme;

float last_temp = 0.0;
float temperature = 0.0;
//

void setup() {
  Serial.begin(115200);
  Serial.println(F("Open Source Academy BME 280 test"));

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
  //

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}

void loop() {
  WiFiClient client = server.available();


  // Display
  temperature = bme.readTemperature();
  // If temperature changes update the display
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
    // Draws degrees celcius on the display
    display.drawCircle(90, 14, 2, WHITE);
    display.setCursor(95, 12);
    display.println("C");
    display.display();
  }



  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // This code is what shows in the webbrowser when going to device ip-Address
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Setup header and style for website
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"> ");
            client.println("<ling rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { background-color: #878e94; text-align: center; font-family: Arial;}");
            client.println("table { border-collapse: collapse; width: 35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 1em; background-color: #f3a70f; color: #111; }");
            client.println("tr { background-color: #c6c2b9; border: 0.2em solid #c6c2b9; padding: 1em; }");
            client.println("tr:hover { background-color: #c6c2b9; }");
            client.println("td { border: none; padding: 0.5em; }");
            client.println(".sensor { color: #42413E; font-family: monospace; font-weight: bold; }");

            client.println("</style><h2>ESP32 with BME280</h2>");
            // Create a table for the values
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");

            client.println("<tr><td>Temperature</td><td><span class=\"sensor\"> ");
            // Print the value from the sensor in hte website
            client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");

            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            // TODO fix altitude values
            //client.println("<tr><td>Approx Altitude</td><td><span class=\"sensor\">");
            //client.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
            //client.println(" m</span></td></tr>");

            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>");
            // Close the hmtl
            client.println("</body></html>");
            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
