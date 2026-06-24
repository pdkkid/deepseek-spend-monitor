#ifndef CONFIG_H
#define CONFIG_H

#define I2C_SDA         8
#define I2C_SCL         9
#define OLED_ADDR       0x3C
#define OLED_WIDTH      128
#define OLED_HEIGHT     64

#define REFRESH_INTERVAL_MS   60000
#define SCREEN_ROTATION_MS    5000

#define DEEPSEEK_API_HOST      "api.deepseek.com"
#define DEEPSEEK_BALANCE_PATH  "/user/balance"
#define HTTP_TIMEOUT_MS        10000

#define WM_PORTAL_NAME         "DeepSeekMonitor"
#define WM_AP_PASSWORD         ""

#endif
