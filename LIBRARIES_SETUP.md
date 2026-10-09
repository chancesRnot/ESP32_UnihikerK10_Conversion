# Required Libraries for ArduinoDroid

This guide explains how to install all required libraries for the ESP32-S3-VROOM demo in **ArduinoDroid**.

## ✅ Quick Setup

### Step 1: Update Board Manager URL

1. Open **ArduinoDroid**
2. Tap **⋮ (Menu)** → **Preferences**
3. Find "Additional Boards Manager URLs"
4. Add this URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
5. Click **OK**

### Step 2: Install ESP32 Board Package

1. Tap **⋮ (Menu)** → **Board Manager**
2. Search for `esp32`
3. Install **"esp32"** by Espressif Systems (version 2.0.11 or later)
4. Select **Board**: `ESP32-S3 Dev Module` (or similar)
5. Select **Partition Scheme**: `Huge APP (3MB No OTA)`

### Step 3: Install Required Libraries

Tap **⋮ (Menu)** → **Library Manager** and install each:

#### **Sensor Libraries**
- ✅ `Adafruit BME280` (Temp & Pressure)
- ✅ `Adafruit MPU6050` (Accelerometer)
- ✅ `Adafruit Unified Sensor`

#### **Display Libraries**
- ✅ `Adafruit GFX Library`
- ✅ `Adafruit ILI9341` (TFT Display Driver)

#### **LED Library**
- ✅ `Adafruit NeoPixel` (WS2812 RGB LED)

#### **Built-in (Already Included)**
- WiFi
- HTTPClient
- Wire (I2C)
- SPI
- SD (if using microSD)
- SPIFFS

---

## 📋 Library Installation Details

### Adafruit BME280
**Purpose**: Temperature, Humidity, Pressure sensor
**I2C Address**: 0x76 or 0x77
**Installation**: Library Manager → Search `BME280` → Install `Adafruit BME280`
**Docs**: https://learn.adafruit.com/adafruit-bme280-humidity-barometric-pressure-temperature-sensor-breakout

### Adafruit MPU6050
**Purpose**: Accelerometer, Gyroscope, Temperature
**I2C Address**: 0x68 or 0x69
**Installation**: Library Manager → Search `MPU6050` → Install `Adafruit MPU6050`
**Docs**: https://learn.adafruit.com/mpu6050-6-axis-accelerometer-plus-gyroscope

### Adafruit GFX Library
**Purpose**: Graphics primitives (text, shapes, colors)
**Installation**: Library Manager → Search `Adafruit GFX` → Install
**Note**: Required dependency for display libraries

### Adafruit ILI9341
**Purpose**: 2.4" TFT Display Driver (320x240)
**Installation**: Library Manager → Search `ILI9341` → Install `Adafruit ILI9341`
**Pins**: CS, DC, RST, MOSI, MISO, SCK (configured in code)
**Docs**: https://learn.adafruit.com/adafruit-2-4-tft-lcd-with-touchscreen

### Adafruit NeoPixel
**Purpose**: WS2812 RGB LED control
**Installation**: Library Manager → Search `NeoPixel` → Install `Adafruit NeoPixel`
**Pin**: GPIO 48 (configurable)
**Docs**: https://learn.adafruit.com/adafruit-neopixel-uberguide

---

## 🔧 Manual Library Installation (Alternative)

If Library Manager doesn't work:

1. **Download** `.zip` files from GitHub:
   - https://github.com/adafruit/Adafruit_BME280_Library
   - https://github.com/adafruit/Adafruit_MPU6050
   - https://github.com/adafruit/Adafruit-GFX-Library
   - https://github.com/adafruit/Adafruit_ILI9341
   - https://github.com/adafruit/Adafruit_NeoPixel

2. **In ArduinoDroid**:
   - Tap **⋮ (Menu)** → **Libraries**
   - Tap **+** button
   - Select downloaded `.zip` files
   - ArduinoDroid will extract them automatically

3. **Verify**: Check that libraries appear in **Sketch** → **Include Library**

---

## ✔️ Verify Installation

**Test Sketch** to verify all libraries are installed:

```cpp
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

void setup() {
  Serial.begin(115200);
  Serial.println("All libraries loaded successfully!");
}

void loop() {
  delay(1000);
}
```

If this compiles without errors, all libraries are correctly installed. ✅

---

## 🚨 Troubleshooting

| Issue | Solution |
|-------|----------|
| "Library not found" | Check library name spelling, restart ArduinoDroid |
| "Port not found" | Enable USB debugging on Android, check USB cable |
| Board not detected | Install CH340 or CP2102 USB driver (Android USB OTG) |
| Compilation errors | Verify pin definitions in `pins_config.h` |
| Board manager URL failed | Check internet connection, try alternate URL |
| Out of memory | Reduce PSRAM buffer, disable unused features |

---

## 📦 Complete Library List for ArduinoDroid

```
Required:
✅ ESP32 Board Package (by Espressif)
✅ Adafruit BME280 Library
✅ Adafruit MPU6050 Library
✅ Adafruit Unified Sensor Library
✅ Adafruit GFX Library
✅ Adafruit ILI9341 Library
✅ Adafruit NeoPixel Library

Built-in (No install needed):
✅ WiFi.h
✅ HTTPClient.h
✅ Wire.h (I2C)
✅ SPI.h
✅ SD.h (microSD)
```

---

**After installing all libraries, you're ready to upload any sketch from this project!** 🎉
