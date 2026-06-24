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

    String totalStr    = info["total_balance"].as<String>();
    String toppedUpStr = info["topped_up_balance"].as<String>();
    String grantedStr  = info["granted_balance"].as<String>();

    float total    = totalStr.toFloat();
    float toppedUp = toppedUpStr.toFloat();
    float granted  = grantedStr.toFloat();

    out.currency         = info["currency"].as<String>();
    out.totalTopUp       = toppedUp;
    out.remainingBalance = total;
    out.totalSpent       = (toppedUp + granted) - total;
    out.valid            = true;

    return true;
}
