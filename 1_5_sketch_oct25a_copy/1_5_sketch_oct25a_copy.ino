
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Adafruit_NeoPixel.h>

// Hardware Configurations
#define LED_PIN     2  
// Pin 2 for waveshare eps32s3 Zero connected to the WS2812B Data line
// Pin 18 for eps32s3 N16R8 connected to the WS2812B Data line
#define NUM_LEDS    50   // Total number of drawers/LEDs

// --- LED Timeout Configuration ---
unsigned long ledTurnOnTime = 0;          // Stores the exact millisecond timestamp when the LED turned on
const unsigned long LED_TIMEOUT = 10000;  // Auto-timeout duration in milliseconds (10000 ms = 10 seconds)
bool ledsAreActive = false;               // Flag tracking whether the strip is currently illuminated


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Wi-Fi Parameters
const char* ssid = "AndroidAPB82E";
const char* password = "aaaaDddd";

WebServer server(80);



void handleLight() {
  if (server.hasArg("led")) {
    int targetLed = server.arg("led").toInt();
    
    // 1. Clear any previous colors on the strip
    strip.clear();
    
    // 2. Set the target drawer LED (e.g., Bright Green: R=0, G=255, B=0)
    strip.setPixelColor(targetLed, strip.Color(0, 255, 0));
    strip.show();
    
    // 3. Set timer flags for non-blocking auto-off
    ledTurnOnTime = millis();
    ledsAreActive = true;
    
    server.send(200, "text/plain", "OK");
    Serial.print("⚡ LED "); 
    Serial.print(targetLed); 
    Serial.println(" activated. Timeout timer started.");
  } else {
    server.send(400, "text/plain", "Missing 'led' argument");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500); // Give the serial connection time to stabilize
  Serial.println("\nInitializing System...");

  // Initialize LED Strip safely at low brightness
  strip.begin();
  strip.setBrightness(30); // KEEP LOW (30 out of 255) to reduce boot current draw
  strip.clear();
  strip.show();

  // Connect to Local Wi-Fi Network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize mDNS Service
  if (!MDNS.begin("electronics-leds")) { 
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started: electronics-leds.local");
    MDNS.addService("http", "tcp", 80);
  }

  // Setup Web Server Endpoints
  server.on("/light", HTTP_GET, handleLight);
  
  server.on("/ping", HTTP_GET, []() {
    server.send(200, "text/plain", "WLED_S3_READY");
  });

  server.begin();
  Serial.println("HTTP Server started");
}

void loop() {
  // Keeps your HTTP web server alive and listening for Django requests
  server.handleClient();
  
  // Non-blocking timeout checker
  if (ledsAreActive && (millis() - ledTurnOnTime >= LED_TIMEOUT)) {
    strip.clear();   // Clear pixel colors arrays
    strip.show();    // Write the blank buffer physically to the WS2812B strip
    ledsAreActive = false; // Turn off the flag tracking engine
    
    Serial.println("⏳ LED Auto-timeout reached. Strip turned off automatically.");
  }
}



