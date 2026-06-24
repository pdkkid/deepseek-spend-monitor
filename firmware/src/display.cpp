#include "display.h"
#include "config.h"

static Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void displayInit() {
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        return;
    }
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.display();
}

void displayStatus(const String &msg) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.println(msg);
    oled.display();
}

void displayConnecting() {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 8);
    oled.println(" WiFi Setup ");
    oled.drawLine(0, 20, 128, 20, SSD1306_WHITE);
    oled.setCursor(0, 28);
    oled.setTextSize(1);
    oled.println("Connect to AP:");
    oled.println("");
    oled.setTextSize(1);
    oled.println("DeepSeekMonitor");
    oled.setCursor(0, 52);
    oled.setTextSize(1);
    oled.println("Then: 192.168.4.1");
    oled.display();
}

void displaySpendSummary(const SpendSummary &s) {
    oled.clearDisplay();

    // Title bar
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print("DeepSeek");

    oled.setCursor(100, 0);
    oled.print(s.currency);

    oled.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Remaining balance (big, centered)
    oled.setTextSize(2);
    String bal = "$" + String(s.remainingBalance, 2);
    int16_t x1, y1;
    uint16_t w, h;
    oled.getTextBounds(bal, 0, 0, &x1, &y1, &w, &h);
    oled.setCursor((128 - w) / 2, 16);
    oled.print(bal);

    oled.drawLine(0, 36, 128, 36, SSD1306_WHITE);

    // Spent / Top-up row
    oled.setTextSize(1);
    oled.setCursor(0, 40);
    oled.print("Spent: $");
    oled.print(String(s.totalSpent, 2));

    oled.setCursor(0, 52);
    oled.print("Top-up: $");
    oled.print(String(s.totalTopUp, 2));

    oled.display();
}

void displayError(const String &err) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.println(" ERROR ");
    oled.drawLine(0, 10, 128, 10, SSD1306_WHITE);
    oled.setCursor(0, 16);
    oled.setTextSize(1);
    for (unsigned i = 0; i < err.length() && i < 100; i += 21) {
        oled.println(err.substring(i, i + 21));
    }
    oled.display();
}
