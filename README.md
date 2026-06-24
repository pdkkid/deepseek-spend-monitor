# DeepSeek Spend Monitor

A tiny ESP32-C3 + SSD1306 OLED device that shows your DeepSeek API spending
at a glance. Flash it right from Chrome — no toolchain required.

## Hardware

| Part | Notes |
|------|-------|
| ESP32-C3 Supermini | ~$3 on AliExpress |
| SSD1306 128×64 I2C OLED | ~$3 on AliExpress |
| 4 jumper wires | — |

## Wiring

| OLED | ESP32-C3 Supermini |
|------|-------------------|
| VCC  | 3.3V              |
| GND  | GND               |
| SDA  | GPIO 8            |
| SCL  | GPIO 9            |

> Some Supermini boards use GPIO 10 (SDA) and GPIO 8 (SCL). Adjust in `config.h`.

## Flash Firmware (no install required)

1. Go to **your-username.github.io/deepseek-spend-monitor**
2. Plug in your ESP32-C3 via USB
3. Hold BOOT, tap RST, release BOOT
4. Click **Connect Serial Port** then **Flash Firmware**

## First Boot Setup

1. After flashing, the device creates a WiFi access point: **DeepSeekMonitor**
2. Connect to it from your phone/laptop
3. Visit `192.168.4.1` in a browser
4. Enter your home WiFi credentials and **DeepSeek API key**
5. The device reboots, connects to WiFi, and displays your balance

## Adding More Providers

The `providers/` directory uses a simple abstract interface:

1. Create `firmware/src/providers/openai.h` / `.cpp`
2. Subclass `LLMProvider` — implement `name()` and `fetch()`
3. Instantiate in `main.cpp` and add a second screen

```cpp
class OpenAIProvider : public LLMProvider { ... };
```

## Local Build

```bash
cd firmware
pip install platformio
pio run -e esp32-c3-supermini
# Binary: .pio/build/esp32-c3-supermini/firmware.bin
```

## License

MIT
