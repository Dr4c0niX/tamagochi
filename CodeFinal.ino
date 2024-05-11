#include "images.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 0x3C

#define buttonUp 14 //N
#define buttonDown 27 //S
#define buttonLeft 26 //W
#define buttonRight 25 //E

Adafruit_SSD1306 display(128, 64, &Wire, -1);

int hunger = 100;
unsigned long previousMillis = 0; 
const long interval = 60000; // 1 minute en millisecondes

int cursorX = 59;
int cursorY = 32;
int rep = 0; // 1 pour oui et 2 pour non

int selectedChopper = 0; // 0 = Aucun peronnage sélectionné, 1 = Chopper B, 2 = Chopper A

void manageLife(void * parameter) 
{
  for(;;) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;
      if(hunger > 0) 
      {
        hunger--;
      }
    }
    delay(1000);
  }
}

void displayLife(void * parameter) 
{
  for(;;) {
    display.fillRect(21, 4, 30, 8, BLACK);
    display.drawBitmap(0, 0, life, 18, 16, WHITE);
    display.setCursor(21, 4); // Positionne le curseur après l'image
    display.print(hunger);
    display.print("%");
    display.display();
    delay(100);
  }
}

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
  display.drawBitmap(ouiX, y, barre_soulignage, 7, 1, WHITE); //affiche la barre de soulignage
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
      display.fillRect(ouiX, y, 7, 1, BLACK); //efface la barre de soulignage
      display.drawBitmap(nonX, y, barre_soulignage, 7, 1, WHITE); //affiche la barre de soulignage
      display.display();
      tempX = ouiX;
      ouiX = nonX;
      nonX = tempX;

      delay(500); //attente de 500ms pour éviter les rebonds
    }

    if (right)
    {
      display.fillRect(ouiX, y, 7, 1, BLACK); //efface la barre de soulignage
      display.drawBitmap(nonX, y, barre_soulignage, 7, 1, WHITE); //affiche la barre de soulignage
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

void moveCursor() 
{
  delay(50); // petit délai pour laisser le temps au joueur de lâcher le bouton afin que le curseur ne bouge pas instantanement
  while (selectedChopper == 0) 
  {
    bool up = digitalRead(buttonUp) == LOW;
    bool down = digitalRead(buttonDown) == LOW;
    bool left = digitalRead(buttonLeft) == LOW;
    bool right = digitalRead(buttonRight) == LOW; //lecture des boutons : 'True' quand le bouton est appuyé et 'false' quand il est relaché

    if(up && cursorY > 1) cursorY--;
    if(down && cursorY < 63) cursorY++;
    if(left && cursorX > 1) cursorX--;
    if(right && cursorX < 127) cursorX++;

    if (cursorX > 14 && cursorX < 39 && cursorY > 25 && cursorY < 52) 
    {
      display.clearDisplay();
      display.drawBitmap(15, 26, chopperB_brainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(90, 26, chopperA_brainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(12, 23, cadre, 29, 31, WHITE); //affiche le cadre
      display.drawBitmap(cursorX, cursorY, curseur_main, 17, 21, WHITE); // affiche le curseur main
      display.display();

      if ((up + down + right + left) >= 2) 
      { 
        selectedChopper = 1;// Chopper B
      }
    }
    else if (cursorX > 89 && cursorX < 114 && cursorY > 25 && cursorY < 52) 
    {
      display.clearDisplay();
      display.drawBitmap(90, 26, chopperA_brainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(15, 26, chopperB_brainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(87, 23, cadre, 29, 31, WHITE); //affiche le cadre
      display.drawBitmap(cursorX, cursorY, curseur_main, 17, 21, WHITE); // affiche le curseur main
      display.display();

      if ((up + down + right + left) >= 2) 
      { 
        selectedChopper = 2; // Chopper A
      }
    }
    else 
    {
      display.clearDisplay();
      display.drawBitmap(15, 26, chopperB_brainPoint1, 23, 25, WHITE); // affiche l'image de chopperB à gauche
      display.drawBitmap(90, 26, chopperA_brainPoint1, 23 , 25, WHITE); // affiche l'image de chopperA à droite
      display.drawBitmap(cursorX, cursorY, curseur_souris, 11, 18, WHITE); // affiche le curseur flèche
      display.display();
    }
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
    displayMessageCenter("Appuyez sur deux boutons en meme temps pour confirmez votre choix.");
    waitForButtonPress();
    while (rep != 1)
    {
      moveCursor();
      displayMessageConfirmation();
    }
    displayMessageCenter("Bravo !");
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

  firstLaunch(); // RENOMMER TOUS LES BOUTONS EN N W S E

/*   xTaskCreatePinnedToCore(manageLife,"ManageLife",10000, NULL,1,NULL,0);
  xTaskCreatePinnedToCore(displayLife,"DisplayLife",10000,NULL, 1,NULL,0);
 */}

void loop() 
{
}