#ifndef FUNCTIONS_H //vérifie si FUNCTIONS.H n'est pas déjà défini (évite les problèmes d'inclusions multiples)
#define FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern int hunger;
extern const long hungerInterval;
extern unsigned long currentMillis;
extern unsigned long previousMillis;
extern const uint8_t life[];

void manageStats(void * parameter);
void displayLifeBar(void * parameter);

#endif // FUNCTIONS_H