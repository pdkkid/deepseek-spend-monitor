#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>
#include "providers/provider.h"

void displayInit();
void displayStatus(const String &msg);
void displaySpendSummary(const SpendSummary &s);
void displayError(const String &err);
void displayConnecting();

#endif
