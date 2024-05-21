#include "functions.h"
#include "images.h"

void manageStats(void * parameter) 
{
  for(;;) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= hungerInterval) 
    {
      previousMillis = currentMillis;
      if(hunger > 0) 
      {
        hunger--;
      }
    }
    //AJOUTER POUR LES AUTRES STATS
    delay(1000);
  }
}

void displayLifeBar(void * parameter) 
{
  bool lifeBarOn = true;
  display.drawBitmap(0, 0, life, 18, 16, WHITE);
  
  for(;;) {
    display.fillRect(22, 4, 122, 8, BLACK);

    if (hunger < 10) { //permet de faire clignoter la barre de vie (si hunger < 10)
      lifeBarOn = !lifeBarOn;
    } else {
      lifeBarOn = true;
    }

    if (lifeBarOn) { //rempli la barre de vie si lifeBarOn est vrai
      display.fillRect(22, 4, hunger, 8, WHITE);
    }

    display.drawRect(20, 2, 104, 12, WHITE); //cadre de la barre de vie
    display.display();

    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de vie éteinte/allumée
  }
}