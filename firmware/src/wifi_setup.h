#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFiManager.h>
#include "config.h"

inline bool wifiSetup(String &outApiKey) {
    WiFiManager wm;
    wm.setDebugOutput(true);   // enable serial logging for AP/connection diag
    wm.setConfigPortalTimeout(180);

    // Explicit mode required on ESP32-C3 before starting WiFiManager
    WiFi.mode(WIFI_STA);

    WiFiManagerParameter apiKeyParam(
        "apikey", "DeepSeek API Key",
        outApiKey.c_str(), 64, "type='password'");
    wm.addParameter(&apiKeyParam);

    // autoConnect: tries saved credentials first, then captive portal AP.
    // Returns true only when a WiFi connection is established.
    bool ok = wm.autoConnect(WM_PORTAL_NAME, WM_AP_PASSWORD);
    outApiKey = apiKeyParam.getValue();

    // Double-check — sometimes autoConnect reports false when actually connected
    if (ok || WiFi.status() == WL_CONNECTED) {
        return true;
    }

    return false;
}

#endif
