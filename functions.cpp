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

    health = (hunger + sleepLevel + hygiene + happiness) / 4;
    delay(1000);
  }
}

void displayHealthBar(void * parameter) 
{
  bool infoWindow = false; //permet de savoir si la fenêtre d'information est ouverte ou non
  bool healthBarOn = true;
  display.clearDisplay();
  display.drawBitmap(4, 5, leftArrow, 3, 5, WHITE);
  display.drawBitmap(121, 5, rightArrow, 3, 5, WHITE);
  display.setCursor(50, 5);
  display.print("Sante");
  display.display();
  
  for(;;) {
    if (infoWindow == false) {
      display.fillRect(23, 26, 90, 31, BLACK); //efface la fenêtre d'information
    }

    display.drawBitmap(0, 20, healthIcon, 18, 16, WHITE);
    display.fillRect(23, 24, 123, 8, BLACK); //efface le contenu de la barre de vie
    display.fillRect(0, 37, 128, 27, BLACK); //efface le pourcentage affiché et le texte en bas

    if (health < 10) { //permet de faire clignoter la barre de vie (si health < 10)
      healthBarOn = !healthBarOn;
    } else {
      healthBarOn = true;
    }

    if (healthBarOn) { //rempli la barre de vie si healthBarOn est vrai
      display.fillRect(23, 24, health, 8, WHITE);
    }

    display.drawRect(21, 22, 104, 12, WHITE); //cadre de la barre de vie
    display.setCursor(59, 37);
    display.print(health);
    display.println("%");

    if (digitalRead(buttonDown) == LOW) {
      infoWindow = !infoWindow; //bascule l'état de la fenêtre
      delay(200); //pour éviter le rebond
    }

    if (infoWindow) {
      display.fillRect(110, 55, 17, 7, BLACK); //efface l'icône "appuyez sur S pour plus d'infos"
      display.fillRect(23, 26, 90, 31, BLACK); //efface du contenu pour faire apparaître la fenêtre
      display.drawRect(23, 26, 90, 31, WHITE); //cadre de la fenêtre
      display.setCursor(35, 30);
      display.println("Cumul des 4");
      display.setCursor(33, 45);
      display.println("statistiques");
    } else {
      display.drawBitmap(110, 55, pressButtonSIcon, 17, 7, WHITE); //affiche l'icône "appuyez sur S pour plus d'infos" (obligé de mettre ici car sinon il est effacé par un fillRect()
      display.setCursor(79,55);
      display.print("Infos");
    }

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de bonheur éteinte/allumée

    if (digitalRead(buttonLeft) == LOW)
    {
      //displayMenu(NULL);
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHungerBar(NULL);
    }
  }
}

void displayHungerBar(void * parameter)
{
  bool hungerBarOn = true;
  display.clearDisplay();
  display.drawBitmap(0, 20, hungerIcon, 18, 16, WHITE);
  display.drawBitmap(4, 5, leftArrow, 3, 5, WHITE);
  display.drawBitmap(121, 5, rightArrow, 3, 5, WHITE);
  display.setCursor(54, 5);
  display.print("Faim");
  display.display();
  
  for(;;) {
    display.fillRect(23, 24, 123, 8, BLACK); //efface le contenu de la barre de faim
    display.fillRect(0, 37, 128, 27, BLACK); //efface le pourcentage affiché et le texte en bas

    if (hunger < 10) { //permet de faire clignoter la barre de vie (si hunger < 10)
      hungerBarOn = !hungerBarOn;
    } else {
      hungerBarOn = true;
    }

    if (hungerBarOn) { //rempli la barre de faim si hungerBarOn est vrai
      display.fillRect(23, 24, hunger, 8, WHITE);
    }

    display.drawRect(21, 22, 104, 12, WHITE); //cadre de la barre de faim
    display.setCursor(59, 37);
    display.print(hunger);
    display.println("%");

    if (hunger < 10) {
        display.setCursor(29, 52);
        display.print("Etat : affame");
    } else if (hunger < 40) {
        display.setCursor(36, 52);
        display.print("Etat : faim");
    } else if (hunger < 70) {
        display.setCursor(23, 52);
        display.print("Etat : pas faim");
    } else {
        display.setCursor(23, 52);
        display.print("Etat : rassasie");
    }

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de faim éteinte/allumée

    if (digitalRead(buttonLeft) == LOW)
    {
      displayHealthBar(NULL);
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displaySleepBar(NULL);
    }
  }
}

void displaySleepBar(void * parameter) 
{
  bool sleepBarOn = true;
  display.clearDisplay();
  display.drawBitmap(0, 20, sleepIcon, 18, 16, WHITE);
  display.drawBitmap(4, 5, leftArrow, 3, 5, WHITE);
  display.drawBitmap(121, 5, rightArrow, 3, 5, WHITE);
  display.setCursor(45, 5);
  display.print("Sommeil");
  display.display();
  
  for(;;) {
    display.fillRect(23, 24, 123, 8, BLACK); //efface le contenu de la barre de sommeil
    display.fillRect(0, 37, 128, 27, BLACK); //efface le pourcentage affiché et le texte en bas

    if (sleepLevel < 10) { //permet de faire clignoter la barre de sommeil (si sleepLevel < 10)
      sleepBarOn = !sleepBarOn;
    } else {
      sleepBarOn = true;
    }

    if (sleepBarOn) { //rempli la barre de sommeil si sleepBarOn est vrai
      display.fillRect(23, 24, sleepLevel, 8, WHITE);
    }

    display.drawRect(21, 22, 104, 12, WHITE); //cadre de la barre de vie
    display.setCursor(59, 37);
    display.print(sleepLevel);
    display.println("%");

    if (sleepLevel < 10) {
        display.setCursor(27, 52);
        display.print("Etat : epuise");
    } else if (sleepLevel < 40) {
        display.setCursor(25, 52);
        display.print("Etat : fatigue");
    } else if (sleepLevel < 70) {
        display.setCursor(27, 52);
        display.print("Etat : repose");
    } else {
        display.setCursor(12, 52);
        display.print("Etat : bien repose");
    }

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de sommeil éteinte/allumée

    if (digitalRead(buttonLeft) == LOW)
    {
      displayHungerBar(NULL);
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHygieneBar(NULL);
    }
  }
}

void displayHygieneBar(void * parameter) 
{
  bool hygieneBarOn = true;
  display.clearDisplay();
  display.drawBitmap(0, 20, hygieneIcon, 18, 16, WHITE);
  display.drawBitmap(4, 5, leftArrow, 3, 5, WHITE);
  display.drawBitmap(121, 5, rightArrow, 3, 5, WHITE);
  display.setCursor(45, 5);
  display.print("Hygiene");
  display.display();
  
  for(;;) {
    display.fillRect(23, 24, 123, 8, BLACK); //efface le contenu de la barre d'hygiène
    display.fillRect(0, 37, 128, 27, BLACK); //efface le pourcentage affiché et le texte en bas

    if (hygiene < 10) { //permet de faire clignoter la barre d'hygiène (si hygiene < 10)
      hygieneBarOn = !hygieneBarOn;
    } else {
      hygieneBarOn = true;
    }

    if (hygieneBarOn) { //rempli la barre d'hygiene si hygieneBarOn est vrai
      display.fillRect(23, 24, hygiene, 8, WHITE);
    }

    display.drawRect(21, 22, 104, 12, WHITE); //cadre de la barre d'hygiene
    display.setCursor(59, 37);
    display.print(hygiene);
    display.println("%");

    if (hygiene < 10) {
        display.setCursor(30, 52);
        display.print("Etat : sale");
    } else if (hygiene < 40) {
        display.setCursor(8, 52);
        display.print("Etat : sens mauvais");
    } else if (hygiene < 70) {
        display.setCursor(27, 52);
        display.print("Etat : propre");
    } else {
        display.setCursor(13, 52);
        display.print("Etat : tout propre");
    }

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre d'hygiene éteinte/allumée

    if (digitalRead(buttonLeft) == LOW)
    {
      displaySleepBar(NULL);
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHappinessBar(NULL);
    }
  }  
}

void displayHappinessBar(void * parameter) 
{
  bool happinessBarOn = true;
  display.clearDisplay();
  display.drawBitmap(4, 5, leftArrow, 3, 5, WHITE);
  display.setCursor(46, 5);
  display.print("Bonheur");
  display.display();
  
  for(;;) {
    display.fillRect(0, 20, 18, 16, BLACK); //efface l'icône pour la mettre à jour
    display.fillRect(23, 24, 123, 8, BLACK); //efface le contenu de la barre de bonheur
    display.fillRect(0, 37, 128, 27, BLACK); //efface le pourcentage affiché et le texte en bas

    if (happiness < 10) { //permet de faire clignoter la barre de bonheur (si happiness < 10)
      happinessBarOn = !happinessBarOn;
    } else {
      happinessBarOn = true;
    }

    if (happinessBarOn) { //rempli la barre de bonheur si happinessBarOn est vrai
      display.fillRect(23, 24, happiness, 8, WHITE);
    }

    display.drawRect(21, 22, 104, 12, WHITE); //cadre de la barre de bonheur
    display.setCursor(59, 37);
    display.print(happiness);
    display.println("%");

    if (happiness < 30) {
        display.drawBitmap(0, 20, angryIcon, 18, 16, WHITE);
        display.setCursor(30, 52);
        display.print("Etat : fache");
    } else if (happiness < 60) {
        display.drawBitmap(0, 20, neutralIcon, 18, 16, WHITE);
        display.setCursor(33, 52);
        display.print("Etat : ennui");
    } else {
        display.drawBitmap(0, 20, happyIcon, 18, 16, WHITE);
        display.setCursor(27, 52);
        display.print("Etat : heureux");
    }

    display.display();
    delay(200); //petit délai pour que quand il y a le clignotement, on puisse voir la barre de bonheur éteinte/allumée

    if (digitalRead(buttonLeft) == LOW)
    {
      displayHygieneBar(NULL);
    }
  }
}
