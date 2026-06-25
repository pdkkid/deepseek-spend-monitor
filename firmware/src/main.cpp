#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include "config.h"
#include "display.h"
#include "wifi_setup.h"
#include "providers/deepseek.h"

static DeepSeekProvider *g_provider = nullptr;
static SpendSummary      g_summary;
static Preferences        g_prefs;
static unsigned long      g_lastFetch = 0;

void saveApiKey(const String &key) {
    g_prefs.putString("ds_key", key);
}

String loadApiKey() {
    return g_prefs.getString("ds_key", "");
}

void setup() {
    Serial.begin(115200);
    delay(500);

    displayInit();
    displayStatus("Booting...");

    g_prefs.begin("deepseek-mon", false);

    // WiFi setup with captive portal
    displayConnecting();
    String apiKey = loadApiKey();
    if (!wifiSetup(apiKey)) {
        displayError("No WiFi.\nReset to retry.");
        while (1) delay(1000);
    }

    if (apiKey.length() > 0) {
        saveApiKey(apiKey);
    } else {
        apiKey = loadApiKey();
    }

    if (apiKey.length() == 0) {
        displayError("No API key.\nRun WiFi setup\nagain.");
        while (1) delay(1000);
    }

    g_provider = new DeepSeekProvider(apiKey);

    // Initial fetch
    displayStatus("Fetching...");
    if (g_provider->fetch(g_summary)) {
        displaySpendSummary(g_summary);
    } else {
        displayError("API error.\nCheck key &\nconnection.");
    }
    g_lastFetch = millis();
}

void loop() {
    unsigned long now = millis();

    if (now - g_lastFetch >= REFRESH_INTERVAL_MS) {
        if (WiFi.status() == WL_CONNECTED) {
            SpendSummary fresh;
            if (g_provider->fetch(fresh)) {
                g_summary = fresh;
                displaySpendSummary(g_summary);
            }
        }
        g_lastFetch = now;
    }

    delay(100);  // short yield — loop checks every ~100ms
}
