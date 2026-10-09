/*
 * ESP32-S3-VROOM Pin Configuration
 * 16MB Flash, 8MB PSRAM
 * Compatible with ArduinoDroid
 */

#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// ==================================================
// GPIO - Buttons & Input
// ==================================================
#define BUTTON_A           0      // Boot button (GPIO0)
#define BUTTON_B           14     // GPIO14
#define BUTTON_AB_PIN      21     // GPIO21 (optional third button)

// ==================================================
// I2C - Sensors (Temperature, Pressure, Accelerometer)
// ==================================================
#define BME_SDA            18     // I2C Data
#define BME_SCL            17     // I2C Clock
#define BME280_ADDRESS     0x76   // BME280 I2C address
#define MPU6050_ADDRESS    0x68   // MPU6050 I2C address

// ==================================================
// SPI - TFT Display (ILI9341) & SD Card
// ==================================================
// TFT Display Pins (ILI9341)
#define TFT_CS             10     // Chip Select
#define TFT_DC             9      // Data/Command
#define TFT_RST            8      // Reset
#define TFT_MOSI           11     // Master Out (MOSI)
#define TFT_MISO           13     // Master In (MISO)
#define TFT_SCK            12     // Serial Clock

// microSD Card Pins (SPI)
#define SD_CS              1      // SD Chip Select (alternative: 21)
#define SD_MOSI            11     // Shared with TFT
#define SD_MISO            13     // Shared with TFT
#define SD_SCK             12     // Shared with TFT

// ==================================================
// NeoPixel - RGB LED
// ==================================================
#define RGB_PIN            48     // Built-in RGB LED pin (ESP32-S3 VROOM)
#define RGB_COUNT          1      // Single LED

// ==================================================
// Camera - OV2640 (if connected)
// ==================================================
#define CAM_PWDN          -1      // Power down (optional)
#define CAM_RESET          -1     // Reset (optional)
#define CAM_XCLK           40     // XCLK
#define CAM_SIOD           17     // SIOD (SDA) - CONFLICT with BME_SCL!
#define CAM_SIOC           18     // SIOC (SCL) - CONFLICT with BME_SDA!
#define CAM_Y9             39
#define CAM_Y8             38
#define CAM_Y7             37
#define CAM_Y6             36
#define CAM_Y5             35
#define CAM_Y4             34
#define CAM_Y3             33
#define CAM_Y2             32
#define CAM_VSYNC          6
#define CAM_HREF           7
#define CAM_PCLK           5

// NOTE: Camera I2C (SIOD/SIOC) conflicts with BME280 I2C pins!
// Solutions:
// 1. Use different I2C pins for BME280 (e.g., pins 4 & 5)
// 2. Use software I2C for one device
// 3. Don't use camera and BME280 together

// ==================================================
// Audio - I2S (Microphone & Speaker)
// ==================================================
#define I2S_BCK            4      // Bit Clock
#define I2S_WS             5      // Word Select (LRCK)
#define I2S_DO             2      // Data Out (speaker)
#define I2S_DI             3      // Data In (microphone)

// ==================================================
// UART - Serial Debugging
// ==================================================
#define SERIAL_BAUD        115200 // Serial monitor baud rate

// ==================================================
// Display Settings
// ==================================================
#define TFT_WIDTH          320    // ILI9341 width
#define TFT_HEIGHT         240    // ILI9341 height
#define TFT_ROTATION       1      // 0=Portrait, 1=Landscape, 2=Portrait flip, 3=Landscape flip

// ==================================================
// Memory Settings
// ==================================================
#define USE_PSRAM          true   // Enable 8MB PSRAM
#define PSRAM_BUFFER_SIZE  153600 // Frame buffer (320x240x2 bytes)

// ==================================================
// WiFi Settings (configure in sketch)
// ==================================================
// const char* WIFI_SSID = "YOUR_NETWORK";
// const char* WIFI_PASS = "YOUR_PASSWORD";

#endif // PINS_CONFIG_H
