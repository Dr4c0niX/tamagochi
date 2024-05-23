#include "functions.h"
#include "images.h"

void manageStats(void * parameter) 
{
  for(;;) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillisHunger >= hungerInterval) 
    {
      previousMillisHunger = currentMillis;
      if(hunger > 0) 
      {
        hunger--;
      }
    }

    if(currentMillis - previousMillisSleep >= sleepInterval) 
    {
      previousMillisSleep = currentMillis;
      if(sleepLevel > 0) 
      {
        sleepLevel--;
      }
    }

    if(currentMillis - previousMillisHappiness >= happinessInterval) 
    {
      previousMillisHappiness = currentMillis;
      if(happiness > 0) 
      {
        happiness--;
      }
    }

    if(currentMillis - previousMillisHygiene >= hygieneInterval) 
    {
      previousMillisHygiene = currentMillis;
      if(hygiene > 0) 
      {
        hygiene--;
      }
    }

    delay(1000);
  }
}

void displayLifeBar(void * parameter) 
{
  bool lifeBarOn = true;
  display.clearDisplay();
  display.drawBitmap(0, 0, lifeIcon, 18, 16, WHITE);
  display.drawBitmap(4, 20, leftArrow, 3, 5, WHITE);
  display.drawBitmap(121, 20, rightArrow, 3, 5, WHITE);
  display.display();
  
  for(;;) {
    display.fillRect(23, 4, 123, 8, BLACK); //efface le contenu de la barre de vie
    display.fillRect(56, 17, 30, 10, BLACK); //efface le pourcentage affiché

    if (hunger < 10) { //permet de faire clignoter la barre de vie (si hunger < 10)
      lifeBarOn = !lifeBarOn;
    } else {
      lifeBarOn = true;
    }

    if (lifeBarOn) { //rempli la barre de vie si lifeBarOn est vrai
      display.fillRect(23, 4, hunger, 8, WHITE);
    }

    display.drawRect(21, 2, 104, 12, WHITE); //cadre de la barre de vie
    display.setCursor(55, 17);
    display.print(hunger);
    display.println("%");
    display.println(); //saut de ligne
    display.println("Pour remplir cette barre, vous devez nourrir votre Chopper.");

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de vie éteinte/allumée
  }
}