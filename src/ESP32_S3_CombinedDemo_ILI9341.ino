/*
 * ESP32-S3-VROOM Combined Demo for ArduinoDroid
 * All features with ILI9341 TFT display
 * 
 * Features:
 * - GPIO digital input/output
 * - Button handling (A, B, A+B)
 * - RGB LED (WS2812 Neopixel)
 * - TFT display text & colors
 * - Temperature & Pressure (BME280)
 * - Accelerometer (MPU6050)
 * - WiFi + HTTP client
 * 
 * Sensors:
 * - BME280: I2C address 0x76 (SDA=18, SCL=17)
 * - MPU6050: I2C address 0x68 (SDA=18, SCL=17)
 * - ILI9341: SPI (CS=10, DC=9, RST=8, MOSI=11, SCK=12, MISO=13)
 * - RGB LED: GPIO 48
 * - Buttons: GPIO 0 (A), GPIO 14 (B)
 */

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Sensor libraries
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Display library
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// LED library
#include <Adafruit_NeoPixel.h>

// ==================================================
// Pin Definitions
// ==================================================
#define BUTTON_A           0      // Boot button
#define BUTTON_B           14     // GPIO14

#define RGB_PIN            48     // Built-in RGB LED
#define RGB_COUNT          1      // Single LED

// TFT ILI9341 pins
#define TFT_CS             10
#define TFT_DC             9
#define TFT_RST            8
#define TFT_MOSI           11
#define TFT_MISO           13
#define TFT_SCK            12

// I2C for BME280 & MPU6050
#define BME_SDA            18
#define BME_SCL            17

// ==================================================
// WiFi Configuration
// ==================================================
const char* WIFI_SSID = "YOUR_WIFI_SSID";        // Change this
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";    // Change this
const char* HTTP_URL = "https://jsonplaceholder.typicode.com/todos/1";

// ==================================================
// Global Objects
// ==================================================
Adafruit_BME280 bme;                              // Temperature & Pressure sensor
Adafruit_MPU6050 mpu;                             // Accelerometer & Gyroscope
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);  // TFT display
Adafruit_NeoPixel pixels(RGB_COUNT, RGB_PIN, NEO_GRB + NEO_KHZ800); // RGB LED

// ==================================================
// State Variables
// ==================================================
bool lastA = false;
bool lastB = false;
bool wifiConnected = false;

float currentTemp = 0.0;
float currentPressure = 0.0;
float currentAccelX = 0.0;
float currentAccelY = 0.0;
float currentAccelZ = 0.0;

int screenMode = 0;  // 0=Main, 1=Temp, 2=Accel, 3=HTTP

// ==================================================
// Setup
// ==================================================
void setup() {
  // Serial for debugging
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== ESP32-S3-VROOM Combined Demo ===");
  Serial.println("Initializing...");

  // Initialize buttons
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  Serial.println("✓ Buttons initialized");

  // Initialize TFT display
  SPI.begin(TFT_SCK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.setRotation(1);  // Landscape
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("ESP32-S3 VROOM");
  Serial.println("✓ TFT Display initialized");

  // Initialize I2C bus for sensors
  Wire.begin(BME_SDA, BME_SCL);
  Serial.println("✓ I2C bus initialized");

  // Initialize BME280 (Temperature & Pressure)
  tft.setCursor(10, 50);
  if (bme.begin(0x76)) {
    Serial.println("✓ BME280 initialized");
    tft.println("BME280: OK");
  } else {
    Serial.println("✗ BME280 NOT FOUND");
    tft.println("BME280: FAIL");
  }

  // Initialize MPU6050 (Accelerometer & Gyroscope)
  tft.setCursor(10, 80);
  if (mpu.begin(0x68)) {
    Serial.println("✓ MPU6050 initialized");
    tft.println("MPU6050: OK");
  } else {
    Serial.println("✗ MPU6050 NOT FOUND");
    tft.println("MPU6050: FAIL");
  }

  // Initialize NeoPixel RGB LED
  pixels.begin();
  setRGB(0, 0, 0);  // Turn off
  Serial.println("✓ RGB LED initialized");

  // Initialize WiFi
  tft.setCursor(10, 110);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi: ");
  tft.println("WiFi: Connecting...");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    wifiConnected = true;
    tft.setCursor(10, 140);
    tft.print("WiFi: ");
    tft.println(WiFi.localIP().toString());
  } else {
    Serial.println("\n✗ WiFi failed");
    tft.setCursor(10, 140);
    tft.println("WiFi: FAIL");
  }

  delay(2000);
  drawMainScreen();

  Serial.println("\n=== Setup Complete ===");
  Serial.println("Button A: Red screen + show data");
  Serial.println("Button B: Blue screen + toggle mode");
  Serial.println("A + B:    Green screen + HTTP request");
}

// ==================================================
// Main Loop
// ==================================================
void loop() {
  // Read button states
  bool aPressed = (digitalRead(BUTTON_A) == LOW);
  bool bPressed = (digitalRead(BUTTON_B) == LOW);

  // ===== Button A: Show sensor data =====
  if (aPressed && !lastA) {
    Serial.println("[Button A pressed]");
    setRGB(255, 0, 0);  // Red LED
    showSensorScreen();
  }

  // ===== Button B: Toggle display mode =====
  if (bPressed && !lastB) {
    Serial.println("[Button B pressed]");
    setRGB(0, 0, 255);  // Blue LED
    screenMode = (screenMode + 1) % 3;  // Cycle: 0->1->2->0
  }

  // ===== A + B: HTTP request =====
  if (aPressed && bPressed) {
    Serial.println("[A + B pressed - HTTP Request]");
    setRGB(0, 255, 0);  // Green LED
    if (wifiConnected) {
      sendHttpRequest();
    }
  }

  // Read sensor values
  readSensors();

  // Update display
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 500) {  // Update every 500ms
    updateDisplay();
    lastUpdate = millis();
  }

  // Periodic HTTP requests (every 30 seconds)
  static unsigned long lastHttp = 0;
  if (millis() - lastHttp > 30000 && wifiConnected) {
    // Commented out to avoid too many requests
    // sendHttpRequest();
    lastHttp = millis();
  }

  // Save button state
  lastA = aPressed;
  lastB = bPressed;

  delay(50);
}

// ==================================================
// Sensor Reading Functions
// ==================================================
void readSensors() {
  // Read BME280
  if (bme.begin(0x76)) {
    currentTemp = bme.readTemperature();
    currentPressure = bme.readPressure() / 100.0F;
  }

  // Read MPU6050
  if (mpu.begin(0x68)) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    currentAccelX = a.acceleration.x;
    currentAccelY = a.acceleration.y;
    currentAccelZ = a.acceleration.z;
  }
}

// ==================================================
// Display Functions
// ==================================================
void drawMainScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(3);
  tft.setCursor(20, 20);
  tft.println("ESP32-S3");
  tft.setCursor(20, 60);
  tft.println("Monitor");

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 120);
  tft.println("Press buttons:");
  tft.setCursor(20, 150);
  tft.println("A = Show data");
  tft.setCursor(20, 180);
  tft.println("B = Toggle mode");
  tft.setCursor(20, 210);
  tft.println("A+B = HTTP req");
}

void showSensorScreen() {
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("SENSOR DATA");

  tft.setCursor(10, 50);
  tft.print("Temp: ");
  tft.print(currentTemp, 1);
  tft.println(" C");

  tft.setCursor(10, 80);
  tft.print("Press: ");
  tft.print(currentPressure, 1);
  tft.println(" hPa");

  tft.setCursor(10, 110);
  tft.print("AccelX: ");
  tft.println(currentAccelX, 2);

  tft.setCursor(10, 140);
  tft.print("AccelY: ");
  tft.println(currentAccelY, 2);

  tft.setCursor(10, 170);
  tft.print("AccelZ: ");
  tft.println(currentAccelZ, 2);

  delay(2000);
  drawMainScreen();
}

void updateDisplay() {
  // This updates the main screen with live sensor values
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(1);

  // Temperature at bottom
  tft.fillRect(10, 220, 300, 20, ILI9341_BLACK);
  tft.setCursor(10, 220);
  tft.print("T: ");
  tft.print(currentTemp, 1);
  tft.print("C | P: ");
  tft.print(currentPressure, 0);
  tft.print(" hPa | X: ");
  tft.print(currentAccelX, 2);
}

void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}

// ==================================================
// WiFi & HTTP Functions
// ==================================================
void sendHttpRequest() {
  if (!wifiConnected) {
    Serial.println("WiFi not connected");
    return;
  }

  Serial.println("\n[HTTP Request]");
  HTTPClient http;
  http.begin(HTTP_URL);
  http.setConnectTimeout(5000);
  http.setTimeout(5000);

  int httpCode = http.GET();
  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Response:");
    Serial.println(payload);

    // Show on display
    tft.fillScreen(ILI9341_BLUE);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("HTTP Success");
    tft.setCursor(10, 50);
    tft.print("Code: ");
    tft.println(httpCode);
    tft.setCursor(10, 90);
    tft.println(payload.substring(0, 100));
    delay(3000);
    drawMainScreen();
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(http.errorToString(httpCode));
    tft.fillScreen(ILI9341_RED);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("HTTP Error");
    delay(2000);
    drawMainScreen();
  }

  http.end();
}
