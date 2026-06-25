#include "deepseek.h"
#include "../config.h"

bool LLMProvider::httpsGet(const char *host, const char *path,
                           const char *apiKey, String &payload) {
    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(HTTP_TIMEOUT_MS / 1000);

    HTTPClient http;
    if (!http.begin(client, host, 443, path, true)) return false;

    http.addHeader("Authorization", String("Bearer ") + apiKey);
    http.addHeader("Accept", "application/json");

    int code = http.GET();
    if (code != 200) { http.end(); return false; }

    payload = http.getString();
    http.end();
    return true;
}

bool DeepSeekProvider::fetch(SpendSummary &out) {
    String payload;
    if (!httpsGet(DEEPSEEK_API_HOST, DEEPSEEK_BALANCE_PATH,
                  m_apiKey.c_str(), payload)) {
        out.valid = false;
        return false;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) { out.valid = false; return false; }

    if (!doc["is_available"].as<bool>() ||
        doc["balance_infos"].size() == 0) {
        out.valid = false; return false;
    }

    JsonObject info = doc["balance_infos"][0];

    // Helper: parse a JSON field that may be a string or a number
    auto parseBalance = [](JsonVariant v) -> float {
        if (v.is<float>())        return v.as<float>();
        if (v.is<const char*>())  return String(v.as<const char*>()).toFloat();
        return 0.0f;
    };

    float total    = parseBalance(info["total_balance"]);
    float toppedUp = parseBalance(info["topped_up_balance"]);
    float granted  = parseBalance(info["granted_balance"]);

    out.currency         = info["currency"].as<String>();
    out.totalTopUp       = toppedUp;
    out.remainingBalance = total;
    out.totalSpent       = max(0.0f, (toppedUp + granted) - total);
    out.valid            = true;

    // Serial debug so you can verify values on the monitor
    Serial.printf("DeepSeek API: total=%.2f toppedUp=%.2f granted=%.2f spent=%.2f\n",
                  total, toppedUp, granted, out.totalSpent);

    return true;
}
