#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFiManager.h>
#include "config.h"

inline bool wifiSetup(String &outApiKey) {
    WiFiManager wm;
    wm.setDebugOutput(false);
    wm.setConfigPortalTimeout(180);

    WiFiManagerParameter apiKeyParam(
        "apikey", "DeepSeek API Key",
        outApiKey.c_str(), 64, "type='password'");
    wm.addParameter(&apiKeyParam);

    if (wm.autoConnect(WM_PORTAL_NAME, WM_AP_PASSWORD)) {
        outApiKey = apiKeyParam.getValue();
        return true;
    }

    // Try saved credentials
    WiFi.begin();
    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
        delay(500);
    }
    if (WiFi.status() == WL_CONNECTED) {
        outApiKey = apiKeyParam.getValue();
        return true;
    }
    return false;
}

#endif
