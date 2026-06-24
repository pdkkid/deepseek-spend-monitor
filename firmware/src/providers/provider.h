#ifndef PROVIDER_H
#define PROVIDER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <vector>

struct SpendSummary {
    float totalSpent;
    float remainingBalance;
    float totalTopUp;
    String currency;
    bool valid;

    struct ModelSpend {
        String modelName;
        float spent;
    };
    std::vector<ModelSpend> modelBreakdown;
};

class LLMProvider {
public:
    virtual ~LLMProvider() = default;
    virtual String name() const = 0;
    virtual bool fetch(SpendSummary &out) = 0;

protected:
    static bool httpsGet(const char *host, const char *path,
                         const char *apiKey, String &payload);
};

#endif
