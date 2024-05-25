#ifndef FUNCTIONS_H //vérifie si FUNCTIONS.H n'est pas déjà défini (évite les problèmes d'inclusions multiples)
#define FUNCTIONS_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

extern const int buttonUp;
extern const int buttonDown;
extern const int buttonLeft;
extern const int buttonRight;
extern unsigned long currentMillis;
extern unsigned long previousMillisHunger;
extern unsigned long previousMillisSleep;
extern unsigned long previousMillisHappiness;
extern unsigned long previousMillisHygiene;
extern const long hungerInterval;
extern const long sleepInterval;
extern const long happinessInterval;
extern const long hygieneInterval;
extern int health;
extern int hunger;
extern int sleepLevel;
extern int happiness;
extern int hygiene;
extern int displayIndex;
extern int displayIndexStats;
extern const uint8_t hungerIcon[];
extern const uint8_t sleepIcon[];
extern const uint8_t healthIcon[];
extern const uint8_t hygieneIcon[];
extern const uint8_t happyIcon[];
extern const uint8_t angryIcon[];
extern const uint8_t neutralIcon[];
extern const uint8_t rightArrow[];
extern const uint8_t leftArrow[];
extern const uint8_t pressButtonSIcon[];

void manageStats(void * parameter);
void displayHealthBar(void * parameter);
void displayHungerBar(void * parameter);
void displaySleepBar(void * parameter);
void displayHygieneBar(void * parameter);
void displayHappinessBar(void * parameter);
void statsMenu(void* parameter);

#endif // FUNCTIONS_H