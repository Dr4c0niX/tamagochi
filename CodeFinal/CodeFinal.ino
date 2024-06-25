#include "images.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const int buttonUp = 14; //N
const int buttonDown = 27; //S
const int buttonLeft = 26; //W
const int buttonRight = 25; //E

int money = 0;
int bestScore = 0;
int score = 0;
int health;
int hunger = 100;
int sleepLevel = 100;
int hygiene = 100;
int happiness = 100;
const long hungerInterval = 15000; // 15 secondes en millisecondes
const long sleepInterval = 30000; // 30 secondes en millisecondes
const long hygieneInterval = 20000; // 20 secondes en millisecondes
const long happinessInterval = 10000; // 10 secondes en millisecondes
unsigned long previousMillisHunger = 0;
unsigned long previousMillisSleep = 0;
unsigned long previousMillisHygiene = 0;
unsigned long previousMillisHappiness = 0;
unsigned long currentMillis;
int displayIndex = 0; //variable pour savoir quel écran afficher (0 = hub, 1 = menu des stats)

int cursorX = 59;
int cursorY = 32;

int rep = 0; // 1 pour oui et 2 pour non
bool selectedChopper = false;
const unsigned char* chopper1; //Chopper choisit avec les yeux ouverts
const unsigned char* chopper2; //Chopper choisit avec les yeux fermés
const unsigned char* chopperWalkingLeft; //Chopper choisit pour l'animation de marche avec la jambe gauche devant
const unsigned char* chopperWalkingRight; //Chopper choisit pour l'animation de marche avec la jambe droite devant
const unsigned char* eatAnimationChopper[8]; //tableau contenant les images de l'animation de manger de Chopper

void displayMessageCenter(const char* message) 
{
  display.clearDisplay();

  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x, &y, &w, &h);
  
  x = (128 - w) / 2;
  y = (64 - h) / 2;

  display.setCursor(x, y);
  display.println(message);
  display.display();
}

void waitForButtonPress()
{
  while(digitalRead(buttonUp) == HIGH) {
    delay(10);
  }

  while(digitalRead(buttonUp) == LOW) {
    delay(10);
  }

  delay(500);
}

void selectChopper() 
{
  bool firstChopper = true; //variable pour déterminer sur quel Chopper on se trouve
  bool secondChopper = false; //variable pour déterminer sur quel Chopper on se trouve
  delay(50); // petit délai pour laisser le temps au joueur de lâcher le bouton afin qu'il ne bouge pas instantanément

  //affichage des Choppers
  display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
  display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
  display.drawBitmap(12, 23, frame, 29, 31, WHITE); //affiche le cadre de sélection sur Chopper B

  //boucle pour sélectionner un Chopper
  while (selectedChopper == false)
  {
    display.clearDisplay();

    if (firstChopper == true)
    {
      display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(87, 23, frame, 29, 31, WHITE); //affiche le cadre de sélection sur Chopper A
    }
    else
    {
      display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(12, 23, frame, 29, 31, WHITE); //affiche le cadre de sélection sur Chopper B
    }


    if (digitalRead(buttonLeft) == LOW || digitalRead(buttonRight) == LOW) //permet de changer de Chopper
    {
      if (firstChopper == true)
      {
        firstChopper = false;
        secondChopper = true;
      }
      else
      {
        firstChopper = true;
        secondChopper = false;
      }
      delay(200); //petit délai pour éviter un rebond
    }

    if (digitalRead(buttonUp) == LOW && firstChopper == true && secondChopper == false) //si on appuie sur le bouton 'N' et que le cadre est sur Chopper B on séléctionne Chopper B
    { 
      selectedChopper = true;
      chopper1 = chopperBBrainPoint1; // Chopper B yeux ouverts
      chopper2 = chopperBBrainPoint2; // Chopper B yeux fermés
      chopperWalkingLeft = chopperBWalkingLeft; // Chopper B pour l'animation avec la jambe gauche
      chopperWalkingRight = chopperBWalkingRight; // Chopper B pour l'animation avec la jambe droite
      eatAnimationChopper[0] = eatAnimationChopperB1; //tableau contenant les images de l'animation de manger de Chopper B
      eatAnimationChopper[1] = eatAnimationChopperB2;
      eatAnimationChopper[2] = eatAnimationChopperB3;
      eatAnimationChopper[3] = eatAnimationChopperB4;
      eatAnimationChopper[4] = eatAnimationChopperB5;
      eatAnimationChopper[5] = eatAnimationChopperB6;
      eatAnimationChopper[6] = eatAnimationChopperB7;
      eatAnimationChopper[7] = eatAnimationChopperB8;
    }

    if (digitalRead(buttonUp) == LOW && firstChopper == false && secondChopper == true) //si on appuie sur le bouton 'N' et que le cadre est sur Chopper A on séléctionne Chopper A
    { 
      selectedChopper = true; 
      chopper1 = chopperABrainPoint1; // Chopper A yeux ouverts
      chopper2 = chopperABrainPoint2; // Chopper A yeux fermés
      chopperWalkingLeft = chopperAWalkingLeft; // Chopper A pour l'animation de marche avec la jambe gauche
      chopperWalkingRight = chopperAWalkingRight; // Chopper A pour l'animation de marche avec la jambe droite
      eatAnimationChopper[0] = eatAnimationChopperA1; //tableau contenant les images de l'animation de manger de Chopper A
      eatAnimationChopper[1] = eatAnimationChopperA2;
      eatAnimationChopper[2] = eatAnimationChopperA3;
      eatAnimationChopper[3] = eatAnimationChopperA4;
      eatAnimationChopper[4] = eatAnimationChopperA5;
      eatAnimationChopper[5] = eatAnimationChopperA6;
      eatAnimationChopper[6] = eatAnimationChopperA7;
      eatAnimationChopper[7] = eatAnimationChopperA8;
    }

    display.display();
  }
  
  int ouiX = 14; //position de la barre de soulignage sur 'Oui'
  int nonX = 104; //position de la barre de soulignage sur 'Non'
  int tempX; //variable temporaire pour échanger les positions des curseurs
  int y = 60; //position de la barre de soulignage sur l'axe Y
  rep = 0; // on (re)met rep à 0 car si on appuie sur le bouton 'Non' rep prendra la valeur 2 et lorsque nous serons de retour dans cette fonction la boucle de validation ne tournera plus

  display.clearDisplay();
  display.setCursor(20, 3);
  display.println("Validez avec N.");
  display.setCursor(25, 17);
  display.println("Etes vous sur");
  display.setCursor(17, 27);
  display.println("de votre choix ?");
  display.setCursor(10, 50);
  display.println("Oui");
  display.drawLine(ouiX, y, ouiX+7, y, WHITE); //affiche la barre de soulignage
  display.setCursor(100, 50);
  display.println("Non");
  display.display();
  delay(500); // petit délai pour laisser le temps au joueur de lâcher le bouton afin que le curseur ne bouge pas instantanement

  //boucle pour valider le choix du Chopper
  while (rep == 0)
  {
    //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché
    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW;
    
    if (left)
    {
      display.fillRect(ouiX, y, 8, 1, BLACK); //efface la barre de soulignage
      display.drawLine(nonX, y, nonX+7, y, WHITE); //affiche la barre de soulignage pour savoir où on est
      display.display();
      tempX = ouiX;
      ouiX = nonX;
      nonX = tempX;

      delay(500); //attente de 500ms pour éviter les rebonds
    }

    if (right)
    {
      display.fillRect(ouiX, y, 8, 1, BLACK); //efface la barre de soulignage
      display.drawLine(nonX, y, nonX+7, y, WHITE); //affiche la barre de soulignage pour savoir où on est
      display.display();
      tempX = ouiX;
      ouiX = nonX;
      nonX = tempX;

      delay(500); //attente de 500ms pour éviter les rebonds
    }

    if (up && ouiX == 14) //on valide 'Oui'
    {
      rep = 1;
    }

    if (up && ouiX == 104) // on valide 'Non' et on retourne à la sélection du Chopper
    {
      rep = 2;
      selectedChopper = false;
      cursorX = 59;
      cursorY = 32;
      delay(500); //attente de 500ms pour éviter les rebonds
    }
  }
}

void manageStats(void* parameter) 
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

void displayHealthBar() 
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
      displayIndex = 0;
      displayHub(NULL);
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHungerBar();
    }
  }
}

void displayHungerBar()
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
      displayHealthBar();
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displaySleepBar();
    }
  }
}

void displaySleepBar() 
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
      displayHungerBar();
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHygieneBar();
    }
  }
}

void displayHygieneBar() 
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
      displaySleepBar();
    }

    if (digitalRead(buttonRight) == LOW)
    {
      displayHappinessBar();
    }
  }  
}

void displayHappinessBar() 
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
      displayHygieneBar();
    }
  }
}

void chopperFeed()
{
  int x = 5;
  int y = 30;
  bool eyesOpen = true;
  bool animation = true;
  bool objectiveReached = false;
  //début de l'animation
  display.clearDisplay();
  display.drawBitmap(0, 0, kitchenDoorClose, 128, 64, WHITE); //porte fermée
  display.display();
  delay(1000); //affiche l'image pendant 1 seconde
  display.clearDisplay();
  display.drawBitmap(0, 0, kitchenDoorOpen, 128, 64, WHITE); //porte ouverte
  delay(1000); //affiche l'image pendant 1 seconde
  display.fillRect(x, y, 23, 25, BLACK);
  display.drawBitmap(x, y, chopper1, 23, 25, WHITE); //Chopper entre par la porte
  display.display();
  delay(2000); //affiche l'image pendant 2 secondes

  for (int i = 0; i < 6; i++) //la porte se ferme et Chopper affiche une phrase en clignant des yeux
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, kitchenDoorClose, 128, 64, WHITE); //porte fermée
    display.fillRect(x, y, 23, 25, BLACK);
    if (eyesOpen)
    {
      display.drawBitmap(x, y, chopper1, 23, 25, WHITE); //Chopper yeux ouverts
    }
    else
    {
      display.drawBitmap(x, y, chopper2, 23, 25, WHITE); //Chopper yeux fermés
    }
    display.fillRect(30, 2, 46, 29, BLACK);
    display.drawBitmap(30, 2, chopperPhrase, 46, 29, WHITE); //affiche la phrase de Chopper
    display.display();
    delay(500); //petit délai pour faire clignoter les yeux
    eyesOpen = !eyesOpen; //inverse l'état des yeux
  }

  while (objectiveReached == false) //tant que l'utilisateur n'a pas fait bougé Chopper jusqu'à la table, il peut le déplacer à gauche ou à droite
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, kitchenDoorClose, 128, 64, WHITE);
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW;

    if(right && x < 13) {x++; y++;}
    if(left && x > 5 && x < 13) {x--; y--;}
    if (right && x > 12 && x < 29) {x++;}
    if (left && x > 12 && x < 29) {x--;}
    if (right && x > 28 && x < 34) {x++; y--;}
    if (left && x > 28 && x < 34) {x--; y++;}

    if ((right + left) == 0) //si aucun bouton n'est appuyé, Chopper cligne des yeux
    {
      display.fillRect(x, y, 23, 25, BLACK); //efface la zone de l'image pour afficher l'image de Chopper
      if (eyesOpen)
      {
        display.drawBitmap(x, y, chopper1, 23, 25, WHITE); //affiche l'image de Chopper avec les yeux ouverts
      }
      else
      {
        display.drawBitmap(x, y, chopper2, 23, 25, WHITE); //affiche l'image de Chopper avec les yeux fermés
      }
      delay(500); //petit délai pour faire clignoter les yeux
      eyesOpen = !eyesOpen;
    } 
    else //si un bouton est appuyé alors Chopper marche
    {
      display.fillRect(x, y, 23, 26, BLACK); //efface la zone de l'image pour afficher l'image de Chopper qui marche, je dois le faire ici car l'image de Chopper qui marche fait y=26 et non y=25
      if (animation)
      {
        display.drawBitmap(x, y, chopperWalkingLeft, 23, 26, WHITE); // affiche l'image de Chopper qui marche avec la jambe gauche devant
      }
      else
      {
        display.drawBitmap(x, y, chopperWalkingRight, 23, 26, WHITE); // affiche l'image de Chopper qui marche avec la jambe droite devant
      }
      animation = !animation;
      delay(50); //petit délai pour l'animation de marche
    }

    if (x == 34) //quand Chopper atteint la table on enlève son contrôle au joueur
    {
      objectiveReached = true;
    }

    display.display();
  }

  for (int i=0; i<8; i++) //animation de Chopper qui mange
  {
    display.clearDisplay();
    if (i< 4)
    {
      display.drawBitmap(0, 0, eatAnimationChopper[i], 128, 64, WHITE);
      delay(200);
    }
    else
    {
      display.drawBitmap(0, 0, eatAnimationChopper[i], 128, 64, WHITE);
      delay(500);
    }
    display.display();
  }
  delay(1000);
  int addHunger = random(20, 61); 
  hunger += addHunger; //ajoute un nombre aléatoire entre 20 et 60 à la faim
  if (hunger > 100) //si la faim dépasse 100, on la remet à 100
  {
    hunger = 100;
  }
  display.clearDisplay();
  display.drawRect(17, 17, 94, 43, WHITE);
  display.drawBitmap(25, 32, hungerIcon, 18, 16, WHITE);
  display.setCursor(52, 35);
  display.print("Faim +" + String(addHunger) + "%");  
  display.display();
  delay(4000);

  for (int x2 = 128; x2 >= -47; x2-=2)
  {
    display.clearDisplay();
    display.fillRect(0, 1, x2, 64, WHITE );
    display.drawBitmap(x2, 1, transition1, 8, 64, WHITE);
    display.drawBitmap(x2+8, 1, transition2, 8, 64, WHITE);
    display.drawBitmap(x2+16, 1, transition3, 7, 64, WHITE);
    display.drawBitmap(x2+23, 1, transition4, 8, 64, WHITE);
    display.drawBitmap(x2+31, 1, transition5, 8, 64, WHITE);
    display.drawBitmap(x2+39, 1, transition5, 8, 64, WHITE);
    display.display();
  }
  delay(1000);
  displayIndex = 0;
  displayHub(NULL);
}

void displayHub(void* parameter) //augmenter la hitbox des cadres
{
  cursorX = 52; //position de départ de Chopper
  cursorY = 38; //position de départ de Chopper
  bool eyesOpen = true; //variable pour le clignotement des yeux
  bool animation = true; //variable pour l'animation de marche
  bool optionSelected = false; //variable pour savoir si une option a été sélectionnée quand on est dans le menu
  bool gameSelected = false; //variable pour savoir si un jeu a été sélectionné
  bool page1 = true; //variable pour savoir si on est sur la page 1 du menu
  while (displayIndex == 0)
  {
    display.clearDisplay();

    //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché
    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW;

    //limitation de la zone de déplacement du personnage
    if(up && cursorY > 7 && cursorX > 30) {cursorY--;} else if (up && cursorY > 21 && cursorX > 23) {cursorY--;} else if (up && cursorY > 34 && cursorX > 16) {cursorY--;} //limite de la zone de déplacement avec bouton haut en tenant compte de la barrière
    if(down && cursorY < 40) {cursorY++;}
    if(left && cursorX > 20 && cursorY > 27) {cursorX--;} else if (left && cursorX > 16 && cursorY > 35) {cursorX--;} else if (left && cursorX > 28){cursorX--;} //limite de la zone de déplacement avec bouton gauche en tenant compte de la barrière
    if(right && cursorX < 104) {cursorX++;}

    if (cursorX > 45 && cursorX < 68 && cursorY < 20) //si Chopper est dans la zone de la porte
    {
      display.drawBitmap(0, 0, hubDoorOpen, 128, 64, WHITE); // affiche l'image de la porte ouverte
      if (cursorX > 54 && cursorX < 56 && cursorY < 9) //si Chopper est au centre de la porte, ouvre le menu
      {
        delay(500); //petit délai pour éviter que le curseur bouge dès le début
        cursorX = 59; //position de départ du curseur dans le menu
        cursorY = 32; //position de départ du curseur dans le menu
        while (optionSelected == false) //tant qu'aucune option n'a été choisie, on reste dans ce menu
        {
          //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché
          bool up = digitalRead(buttonUp) == LOW;
          bool down = digitalRead(buttonDown) == LOW;
          bool left = digitalRead(buttonLeft) == LOW;
          bool right = digitalRead(buttonRight) == LOW;
          //limitation de la zone de déplacement du curseur
          if(up && cursorY > 1) {cursorY--;}
          if(down && cursorY < 63) {cursorY++;}
          if(left && cursorX > 1) {cursorX--;}
          if(right && cursorX < 127) {cursorX++;}
          display.clearDisplay();
          
          if (page1 == true)
          {
            display.drawBitmap(0, 0, hubMenuPage1, 128, 64, WHITE); //affiche l'image de la page 1 du menu
            display.drawBitmap(121, 38, rightArrow, 3, 5, WHITE); //affiche la flèche droite pour indiquer la page suivante

            if (cursorX > 13 && cursorX < 51 && cursorY > 23 && cursorY < 33) //curseur sur l'option nourrir
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(12, 22, 39, 11, WHITE); //affiche le cadre autour de l'option nourrir
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                optionSelected = true;
                displayIndex = 2; //on passe à l'écran de nourrissage
              }
            }
            else if (cursorX > 80 && cursorX < 109 && cursorY > 23 && cursorY < 33) //curseur sur l'option jouer
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(79, 22, 30, 11, WHITE); //affiche le cadre autour de l'option jouer
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                while (gameSelected == false)
                {
                  //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché
                  bool up = digitalRead(buttonUp) == LOW;
                  bool down = digitalRead(buttonDown) == LOW;
                  bool left = digitalRead(buttonLeft) == LOW;
                  bool right = digitalRead(buttonRight) == LOW;
                  //limitation de la zone de déplacement du curseur
                  if(up && cursorY > 1) {cursorY--;}
                  if(down && cursorY < 63) {cursorY++;}
                  if(left && cursorX > 1) {cursorX--;}
                  if(right && cursorX < 127) {cursorX++;}
                  display.clearDisplay();
                  display.drawBitmap(0, 0, hubGamesMenu, 128, 64, WHITE); //affiche l'image du menu de choix de jeu

                  if (cursorX > 11 && cursorX < 62 && cursorY > 27 && cursorY < 37)
                  {
                    display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
                    display.drawRect(12, 27, 50, 10, WHITE); //affiche le cadre autour de l'option jeu 1 (CanonShot)
                    if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
                    {
                      gameSelected = true;
                      optionSelected = true;
                      displayIndex = 3; //on passe à l'écran du jeu 1 (CanonShot)
                    }
                  }
                  else if (cursorX > 70 && cursorX < 126 && cursorY > 27 && cursorY < 37)
                  {
                    display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
                    display.drawRect(71, 27, 50, 10, WHITE); //affiche le cadre autour de l'option jeu 2 (ChopperRun)
                    if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
                    {
                      gameSelected = true;
                      optionSelected = true;
                      displayIndex = 4; //on passe à l'écran du jeu 2 (ChopperRun)
                    }
                  }
                  else if (cursorX > 45 && cursorX < 87 && cursorY > 48 && cursorY < 58)
                  {
                    display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
                    display.drawRect(46, 48, 40, 10, WHITE); //affiche le cadre autour de l'option retour
                    if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
                    {
                      gameSelected = true;
                    }
                  }
                }
                gameSelected = false; //on remet la variable à false pour pouvoir retourner dans le menu de choix de jeu
              }
            }
            else if (cursorX > 15 && cursorX < 49 && cursorY > 48 && cursorY < 58) //curseur sur l'option dormir
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(14, 47, 35, 11, WHITE); //affiche le cadre autour de l'option dormir
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                optionSelected = true;
                displayIndex = 5; //on passe à l'écran de sommeil
              }
            }
            else if (cursorX > 80 && cursorX < 110 && cursorY > 48 && cursorY < 58) //curseur sur l'option hygiène
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(79, 47, 31, 11, WHITE); //affiche le cadre autour de l'option hygiène
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                optionSelected = true;
                displayIndex = 6; //on passe à l'écran d'hygiène
              }
            }
            else if (cursorX > 117 && cursorX < 126 && cursorY > 35 && cursorY < 45) //curseur sur la flèche page suivante
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(117, 35, 10, 11, WHITE); //affiche le cadre autour de la flèche page suivante
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on passe à la page suivante
              {
                page1 = false;
              }
            }
            else 
            {
              display.drawBitmap(cursorX, cursorY, mouseCursor, 11, 18, WHITE); //affiche le curseur flèche
            }
          }
          else //page 1 == false , donc page 2
          {
            display.drawBitmap(0, 0, hubMenuPage2, 128, 64, WHITE); //affiche l'image de la page 2 du menu
            display.drawBitmap(7, 38, leftArrow, 3, 5, WHITE); //affiche la flèche gauche pour indiquer la page précédente

            if (cursorX > 32 && cursorX < 97 && cursorY > 23 && cursorY < 33) //curseur sur l'option statistiques
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(31, 22, 66, 11, WHITE); //affiche le cadre autour de l'option statistiques
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                optionSelected = true;
                displayIndex = 1; //on passe à l'écran des statistiques
              }
            }
            else if (cursorX > 44 && cursorX < 84 && cursorY > 47 && cursorY < 57) //curseur sur l'option quitter
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(43, 46, 41, 11, WHITE); //affiche le cadre autour de l'option quitter
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on sélectionne l'option
              {
                optionSelected = true;
                displayIndex = 0; //on passe à l'écran de quitter
              }
            }
            else if (cursorX > 4 && cursorX < 12 && cursorY > 35 && cursorY < 45) //curseur sur la flèche page précédente
            {
              display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); //affiche le curseur main
              display.drawRect(4, 35, 10, 11, WHITE); //affiche le cadre autour de la flèche page précédente
              if ((up + down + right + left) >= 2) //si deux boutons sont appuyés en même temps, on passe à la page précédente
              {
                page1 = true;
              }
            }
            else 
            {
              display.drawBitmap(cursorX, cursorY, mouseCursor, 11, 18, WHITE); //affiche le curseur flèche
            }
          }
          display.display();
        }
        optionSelected = false; //on remet la variable à false pour la prochaine fois qu'on ouvrira le menu
        page1 = true; //on remet la page à 1 pour la prochaine fois qu'on ouvrira le menu
        cursorX = 52; //remet Chopper à sa position de départ
        cursorY = 38; //remet Chopper à sa position de départ
      }
      else if ((up + down + right + left) == 0) //si aucun bouton n'est appuyé alors Chopper clignote des yeux
      {
        display.fillRect(cursorX, cursorY, 23, 25, BLACK); //efface la zone de l'image pour afficher l'image de Chopper
        if (eyesOpen)
        {
          display.drawBitmap(cursorX, cursorY, chopper1, 23, 25, WHITE); //affiche l'image de Chopper avec les yeux ouverts
        }
        else
        {
          display.drawBitmap(cursorX, cursorY, chopper2, 23, 25, WHITE); //affiche l'image de Chopper avec les yeux fermés
        }
        delay(500); //petit délai pour faire clignoter les yeux
        eyesOpen = !eyesOpen;
      } 
      else //si un bouton est appuyé alors Chopper marche
      {
        display.fillRect(cursorX, cursorY, 23, 26, BLACK); //efface la zone de l'image pour afficher l'image de Chopper qui marche, je dois le faire ici car l'image de Chopper qui marche fait y=26 et non y=25
        if (animation)
        {
          display.drawBitmap(cursorX, cursorY, chopperWalkingLeft, 23, 25, WHITE); // affiche l'image de Chopper qui marche avec la jambe gauche devant
        }
        else
        {
          display.drawBitmap(cursorX, cursorY, chopperWalkingRight, 23, 25, WHITE); // affiche l'image de Chopper qui marche avec la jambe droite devant
        }
        animation = !animation;
        delay(50); //petit délai pour l'animation de marche
      }
    } 
    else
    {
      display.drawBitmap(0, 0, hubDoorClose, 128, 64, WHITE); // affiche l'image de la porte fermée
      display.fillRect(cursorX, cursorY, 23, 25, BLACK); //efface la zone de l'image pour afficher l'image de Chopper
      if ((up + down + right + left) == 0)
      {
        if (eyesOpen)
        {
          display.drawBitmap(cursorX, cursorY, chopper1, 23, 25, WHITE); // affiche l'image de Chopper
        }
        else
        {
          display.drawBitmap(cursorX, cursorY, chopper2, 23, 25, WHITE); // affiche l'image de Chopper
        }
        delay(500); //petit délai pour faire clignoter les yeux
        eyesOpen = !eyesOpen;
      } 
      else 
      {
        display.drawRect(cursorX, cursorY, 23, 26, BLACK); //efface la zone de l'image pour afficher l'image de Chopper qui marche, je dois le faire ici car l'image de Chopper qui marche fait y=26 et non y=25
        if (animation)
        {
          display.drawBitmap(cursorX, cursorY, chopperWalkingLeft, 23, 25, WHITE); // affiche l'image de Chopper qui marche avec la jambe gauche devant
        }
        else
        {
          display.drawBitmap(cursorX, cursorY, chopperWalkingRight, 23, 25, WHITE); // affiche l'image de Chopper qui marche avec la jambe droite devant
        }
        animation = !animation;
        delay(50); //petit délai pour l'animation de marche
      }
    }
    display.display();
  }

  if (displayIndex == 1) //si on a choisi statistiques
  {
    displayHealthBar();
  }
   else if (displayIndex == 2) //si on a choisi nourrir
  {
    chopperFeed();
  }
  /*
  else if (displayIndex == 3) //si on a choisi de jouer à CanonShot
  {
    displayCanonShot();
  }
  else if (displayIndex == 4) //si on a choisi de jouer à ChoppeRun
  {
    displayChopperRun();
  }
  else if (displayIndex == 5) //si on a choisi dormir
  {
    displaySleep();
  }
  else if (displayIndex == 6) //si on a choisi hygiène
  {
    displayHygiene();
  } */
}

void firstLaunch()
{
    display.setCursor(12, 20);
    display.println("Appuyez sur N pour");
    display.setCursor(12, 35);
    display.println("passer a la suite.");
    display.display();
    waitForButtonPress();
    displayMessageCenter("Bienvenue !");
    waitForButtonPress();
    displayMessageCenter("Pour commencer, vous allez devoir choisir votre personnage.");
    waitForButtonPress();
    displayMessageCenter("Appuyez sur deux boutons en meme temps pour confirmer votre choix.");
    waitForButtonPress();
    while (rep != 1)
    {
      selectChopper();
    }
    displayMessageCenter("Bravo !");
    delay(3000);

    xTaskCreatePinnedToCore(manageStats,"ManageAllStats",10000, NULL,1,NULL,0); //appelle la fonction manageStats sur le coeur 0
    xTaskCreatePinnedToCore(displayHub,"DisplayHub",10000,NULL, 1,NULL,1); //appelle la fonction displayHub sur le coeur 1
}

void setup()  
{
  display.begin(SSD1306_SWITCHCAPVCC, OLED_RESET); 
  Serial.begin(115200);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP); //enlever ca 

  firstLaunch(); // LA PROCHAINE FOIS RENOMMER TOUS LES BOUTONS EN N W S E et faire en sorte que tous les boutons puissent etre préssés pour aller à la suite
}

void loop() 
{
}