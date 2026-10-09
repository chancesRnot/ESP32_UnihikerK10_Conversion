# ESP32-S3-VROOM Hardware Setup & Wiring Guide

## 📋 ESP32-S3-VROOM Board Specifications

- **Processor**: Dual-core ESP32-S3 (240MHz)
- **Flash Memory**: 16MB
- **PSRAM**: 8MB
- **GPIO Pins**: 45 (3.3V logic)
- **Analog Input**: 12-bit ADC (8 channels)
- **Analog Output**: 8-bit PWM DAC (2 channels)
- **Peripherals**: SPI, I2C, UART, I2S, USB
- **Power**: 5V USB or 3.3V pin

---

## 🔌 Pin Configuration

### Button Inputs
```
Button A (Menu)  ──→ GPIO 0 (BOOT)
Button B (Select) ──→ GPIO 14
Button AB (Alt)   ──→ GPIO 21 (optional)
```

### I2C Sensors (BME280 & MPU6050)
```
BME280/MPU6050 GND  ──→ GND
BME280/MPU6050 3.3V ──→ 3.3V
BME280/MPU6050 SDA  ──→ GPIO 18
BME280/MPU6050 SCL  ──→ GPIO 17
```

**I2C Address**:
- BME280: `0x76` (configurable via jumper to `0x77`)
- MPU6050: `0x68` (configurable via jumper to `0x69`)

### SPI Display (ILI9341 TFT)
```
ILI9341 VCC  ──→ 5V (or 3.3V if capable)
ILI9341 GND  ──→ GND
ILI9341 CS   ──→ GPIO 10
ILI9341 DC   ──→ GPIO 9
ILI9341 RST  ──→ GPIO 8
ILI9341 MOSI ──→ GPIO 11
ILI9341 MISO ──→ GPIO 13
ILI9341 SCK  ──→ GPIO 12
```

### RGB LED (WS2812 Neopixel)
```
Neopixel GND   ──→ GND
Neopixel 5V    ──→ 5V (or USB power)
Neopixel Data  ──→ GPIO 48
```

### microSD Card (SPI)
```
SD Card GND   ──→ GND
SD Card 3.3V  ──→ 3.3V
SD Card CS    ──→ GPIO 1 (or alternative GPIO 21)
SD Card MOSI  ──→ GPIO 11 (shared with TFT)
SD Card MISO  ──→ GPIO 13 (shared with TFT)
SD Card SCK   ──→ GPIO 12 (shared with TFT)
```

### Camera (OV2640) - Advanced
```
⚠️ Camera shares I2C pins with BME280!

Camera XCLK   ──→ GPIO 40
Camera PCLK   ──→ GPIO 5
Camera VSYNC  ──→ GPIO 6
Camera HREF   ──→ GPIO 7
Camera SDA    ──→ GPIO 17 (CONFLICT with BME_SCL)
Camera SCL    ──→ GPIO 18 (CONFLICT with BME_SDA)
Camera Y2-Y9  ──→ GPIO 32-39 (8 parallel data lines)

Solution: Use camera with software I2C or skip BME280
```

### Audio I2S (Optional)
```
Microphone/Speaker GND  ──→ GND
Microphone/Speaker 3.3V ──→ 3.3V
I2S BCK (Bit Clock)     ──→ GPIO 4
I2S WS (Word Select)    ──→ GPIO 5
I2S Data Out (Speaker)  ──→ GPIO 2
I2S Data In (Mic)       ──→ GPIO 3
```

---

## 🛠️ Complete Wiring Diagram

```
┌─────────────────────┐
│   ESP32-S3-VROOM    │
│                     │
│ GND ───��────┬───────┼──→ GND (Common)
│ 5V  ────────┼───────┼──→ 5V Power
│ 3.3V ───────┼───────┼──→ 3.3V Power
│             │       │
│ GPIO 17 ────┼───────┼──→ I2C SCL (BME280, MPU6050)
│ GPIO 18 ────┼───────┼──→ I2C SDA (BME280, MPU6050)
│             │       │
│ GPIO 0  ────┼───────┼──→ Button A
│ GPIO 14 ────┼───────┼──→ Button B
│             │       │
│ GPIO 8  ────┼───────┼──→ TFT RST
│ GPIO 9  ────┼───────┼──→ TFT DC
│ GPIO 10 ────┼───────┼──→ TFT CS
│ GPIO 11 ────┼───────┼──→ TFT MOSI (shared SD)
│ GPIO 12 ────┼───────┼──→ TFT SCK (shared SD)
│ GPIO 13 ────┼───────┼──→ TFT MISO (shared SD)
│             │       │
│ GPIO 48 ────┼───────┼──→ RGB LED (Neopixel)
│             │       │
│ GPIO 1  ────┼───────┼──→ SD Card CS
│             │       │
└─────────────────────┘
      ↕ USB ↕
```

---

## 📦 Recommended Modules to Buy

### Essentials (Start Here)
1. **BME280 Breakout** (~$3-5)
   - Amazon: Search "BME280 module"
   - GND, 3.3V, SDA, SCL (I2C)

2. **MPU6050 Breakout** (~$2-4)
   - Amazon: Search "MPU6050 accelerometer"
   - GND, 3.3V, SDA, SCL (I2C)

3. **ILI9341 2.4" TFT Display** (~$8-12)
   - Amazon: Search "ILI9341 2.4 inch TFT"
   - SPI interface (CS, DC, RST, MOSI, SCK, MISO)

4. **WS2812 RGB LED Module** (~$1-3)
   - Amazon: Search "WS2812 Neopixel"
   - Single LED or strip, 5V, GND, Data

5. **3 Momentary Pushbuttons** (~$1-2)
   - Amazon: Search "tactile pushbutton"
   - GPIO 0, GPIO 14, optional GPIO 21

### Optional Upgrades
- **microSD Card Module** (~$2-3) for audio/photos
- **OV2640 Camera Module** (~$5-10) for photo capture
- **I2S Microphone** (~$5-8) for audio recording
- **I2S Speaker/DAC** (~$3-8) for audio playback

---

## ⚡ Power Supply Recommendations

**Option 1: USB Power (Recommended for Development)**
- USB-C cable from computer or 5V USB adapter
- Power: 500mA typical (1A recommended)
- Advantage: Simple, built-in programming

**Option 2: Battery Power**
- Li-Po 3.7V single cell (USB-C charging module required)
- Capacity: 1000-5000mAh recommended
- Pro Tip: Use a voltage regulator for 5V sensors

**Option 3: External 5V Supply**
- 5V power adapter (2A or higher)
- Connect to USB power input or VIN pin
- Add capacitors: 10µF + 100µF near power pins

---

## 🔗 Soldering & Connection Tips

✅ **DO:**
- Use 0.1μF + 10μF capacitors near power pins for stability
- Add 4.7kΩ pull-up resistors on I2C lines (optional, usually built-in)
- Use short wires (< 30cm) for SPI/I2C
- Test each sensor individually before combining
- Keep GND reference on all circuits

❌ **DON'T:**
- Connect 5V logic directly to ESP32 (3.3V max)
- Use long wires for high-speed signals (SPI, I2C)
- Mix power sources without common GND
- Forget to connect GND lines
- Exceed GPIO current limits (40mA per pin, 1A total)

---

## 🧪 Quick Test Circuit

Start with this minimal setup:

```
ESP32-S3 ──→ Button (GPIO0) ──→ GND
ESP32-S3 ──→ LED (GPIO48) ──→ 150Ω resistor ──→ GND
ESP32-S3 ──→ I2C SCL (GPIO17) ──→ BME280 SCL
ESP32-S3 ──→ I2C SDA (GPIO18) ──→ BME280 SDA
ESP32-S3 ──→ 3.3V ──→ Pull-up resistors (4.7kΩ on SCL/SDA)
```

Verify this works before adding more sensors!

---

## 📝 Pinout Reference Card

| GPIO | Function | Used By | Voltage |
|------|----------|---------|----------|
| 0 | Button A (BOOT) | Button | 3.3V |
| 1 | SD CS | SD Card | 3.3V |
| 2 | I2S Data (Speaker) | Audio | 3.3V |
| 3 | I2S Data (Mic) | Audio | 3.3V |
| 4 | I2S BCK | Audio | 3.3V |
| 5 | I2S WS | Audio | 3.3V |
| 8 | TFT RST | Display | 3.3V |
| 9 | TFT DC | Display | 3.3V |
| 10 | TFT CS | Display | 3.3V |
| 11 | TFT MOSI | Display + SD | 3.3V |
| 12 | TFT SCK | Display + SD | 3.3V |
| 13 | TFT MISO | Display + SD | 3.3V |
| 14 | Button B | Button | 3.3V |
| 17 | I2C SCL | BME280, MPU | 3.3V |
| 18 | I2C SDA | BME280, MPU | 3.3V |
| 21 | Button AB | Button | 3.3V |
| 32-39 | Camera Data | Camera | 3.3V |
| 40 | Camera XCLK | Camera | 3.3V |
| 48 | RGB LED | Neopixel | 3.3V |

---

## ✅ Pre-Upload Checklist

Before uploading your sketch:

- [ ] Verify all pin numbers in `pins_config.h`
- [ ] Check WiFi credentials in sketch
- [ ] Test I2C address with I2C scanner
- [ ] Confirm SPI bus connections (CS, DC, MOSI, SCK)
- [ ] Test button functionality with Serial monitor
- [ ] Verify power supply is adequate (1A+)
- [ ] Check all GND connections
- [ ] Disable unnecessary features if memory is tight

---

**Ready to wire up? Start with the BME280 + MPU6050 + ILI9341 combo. Once that works, add buttons and RGB LED!** 🚀
