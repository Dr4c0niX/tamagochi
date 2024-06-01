#include "images.h"
#include "functions.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0x3C

const int buttonUp = 14; //N
const int buttonDown = 27; //S
const int buttonLeft = 26; //W
const int buttonRight = 25; //E

Adafruit_SSD1306 display(128, 64, &Wire, -1);

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
int displayIndex = 0; //variable pour savoir quel écran afficher (0 = menu principal, 1 = menu des stats)

int cursorX = 59;
int cursorY = 32;

int rep = 0; // 1 pour oui et 2 pour non
bool selectedChopper = false;
const unsigned char* chopper1; //Chopper choisit avec les yeux ouverts
const unsigned char* chopper2; //Chopper choisit avec les yeux fermés
const unsigned char* chopperWalkingLeft; //Chopper choisit pour l'animation de marche avec la jambe gauche devant
const unsigned char* chopperWalkingRight; //Chopper choisit pour l'animation de marche avec la jambe droite devant

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

void displayMessageConfirmation()
{
  int ouiX = 14;
  int nonX = 104;
  int tempX;
  int y = 60;
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

  while (rep == 0)
  {
    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW; //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché
    
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

    if (up && ouiX == 104) // on valide 'Non'
    {
      rep = 2;
      selectedChopper = 0;
      cursorX = 59;
      cursorY = 32;
    }
  }
}

void waitForButtonPress() {
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
  delay(50); // petit délai pour laisser le temps au joueur de lâcher le bouton afin que le curseur ne bouge pas instantanement
  while (selectedChopper == false) 
  {
    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW; //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché

    if(up && cursorY > 1) {cursorY--;}
    if(down && cursorY < 63) {cursorY++;}
    if(left && cursorX > 1) {cursorX--;}
    if(right && cursorX < 127) {cursorX++;}

    if (cursorX > 14 && cursorX < 39 && cursorY > 25 && cursorY < 52) 
    {
      display.clearDisplay();
      display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(12, 23, frame, 29, 31, WHITE); //affiche le cadre
      display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); // affiche le curseur main
      display.display();

      if ((up + down + right + left) >= 2) 
      { 
        selectedChopper = true;
        chopper1 = chopperBBrainPoint1; // Chopper B yeux ouverts
        chopper2 = chopperBBrainPoint2; // Chopper B yeux fermés
        chopperWalkingLeft = chopperBWalkingLeft; // Chopper B pour l'animation avec la jambe gauche
        chopperWalkingRight = chopperBWalkingRight; // Chopper B pour l'animation avec la jambe droite
      }
    }
    else if (cursorX > 89 && cursorX < 114 && cursorY > 25 && cursorY < 52) 
    {
      display.clearDisplay();
      display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(87, 23, frame, 29, 31, WHITE); //affiche le cadre
      display.drawBitmap(cursorX, cursorY, handCursor, 17, 21, WHITE); // affiche le curseur main
      display.display();

      if ((up + down + right + left) >= 2) 
      { 
        selectedChopper = true; 
        chopper1 = chopperABrainPoint1; // Chopper A yeux ouverts
        chopper2 = chopperABrainPoint2; // Chopper A yeux fermés
        chopperWalkingLeft = chopperAWalkingLeft; // Chopper A pour l'animation de marche avec la jambe gauche
        chopperWalkingRight = chopperAWalkingRight; // Chopper A pour l'animation de marche avec la jambe droite
      }
    }
    else 
    {
      display.clearDisplay();
      display.drawBitmap(15, 26, chopperBBrainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(90, 26, chopperABrainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(cursorX, cursorY, mouseCursor, 11, 18, WHITE); // affiche le curseur flèche
      display.display();
    }
  }
}

void displayHub()
{
  cursorX = 52; //position de départ de Chopper
  cursorY = 38; //position de départ de Chopper
  bool eyesOpen = true; //variable pour le clignotement des yeux
  bool animation = true; //variable pour l'animation de marche
  while (displayIndex == 0)
  {
    display.clearDisplay();

    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW; //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché

    //limitation de la zone de déplacement du personnage
    if(up && cursorY > 9 && cursorX > 30) {cursorY--;} else if (up && cursorY > 22 && cursorX > 23) {cursorY--;} else if (up && cursorY > 38 && cursorX > 16) {cursorY--;} //limite de la zone de déplacement avec bouton haut en tenant compte de la barrière
    if(down && cursorY < 40) {cursorY++;}
    if(left && cursorX > 20 && cursorY > 27) {cursorX--;} else if (left && cursorX > 16 && cursorY > 35) {cursorX--;} else if (left && cursorX > 28){cursorX--;} //limite de la zone de déplacement avec bouton gauche en tenant compte de la barrière
    if(right && cursorX < 104) {cursorX++;}

    if (cursorX > 45 && cursorX < 68 && cursorY < 20) //si Chopper est dans la zone de la porte
    {
      display.drawBitmap(0, 0, hubDoorOpen, 128, 64, WHITE); // affiche l'image de la porte ouverte
      if ((up + down + right + left) == 0)
      {
        display.fillRect(cursorX, cursorY, 23, 25, BLACK); //efface la zone de l'image pour afficher l'image de Chopper
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
}

void firstLaunch() {
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
      displayMessageConfirmation();
    }
    displayMessageCenter("Bravo !");
    delay(3000);

    displayHub();

    //xTaskCreatePinnedToCore(manageStats,"ManageAllStats",10000, NULL,1,NULL,0); //appelle la fonction manageStats sur le coeur 0
    //xTaskCreatePinnedToCore(displayHealthBar,"DisplayStatsMenu",10000,NULL, 1,NULL,1);
}

void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, OLED_RESET); 
  Serial.begin(9600);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);

  firstLaunch(); // LA PROCHAINE FOIS RENOMMER TOUS LES BOUTONS EN N W S E
}

void loop() 
{
}