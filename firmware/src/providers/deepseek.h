#ifndef DEEPSEEK_PROVIDER_H
#define DEEPSEEK_PROVIDER_H

#include "provider.h"

class DeepSeekProvider : public LLMProvider {
public:
    explicit DeepSeekProvider(const String &apiKey)
        : m_apiKey(apiKey) {}

    String name() const override { return "DeepSeek"; }
    bool fetch(SpendSummary &out) override;

private:
    String m_apiKey;
};

#endif
