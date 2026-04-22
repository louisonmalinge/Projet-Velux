#include <SPI.h> // inclure librairie SPI.h
#include <Ethernet.h> // inclure librairie ethernet.h
#include  <Wire.h>  // inclure librairie Wire.h
#include  "Ecran.h" // inclure fichier Ecran .h
#include "Siglent.h" // inclure fichier Siglent.h


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // définir écran dans le .ino
const int bouton = A1;  // Définir le bouton sur la Pin A1

void setup() {
  Serial.begin(9600);

  pinMode(bouton, INPUT_PULLUP); // Activer la résistance de Pull Up interne sur la pin A1 (Pin bouton)
  
  init_eth(); // Fonction pour initialiser le module ethernet
  setup_ecran(); // Fonction pour initialiser l'écran 

  delay(2000);
}

void loop() 
{
  int etat = 0 ; // Déclarer variable pour le switch case 
  int etat_bouton = digitalRead(bouton);
  Serial.print(etat_bouton);
  if (etat_bouton == LOW)
  {
    etat = 1;
  }


  switch(etat)
  {

  case 1:
  afficher_Fill_Factor();
  delay(2000);
  etat = 0;
  delay(10000);
  break ;

 /* case 2:
  afficher_resultat_Isc();
  delay(2000);
  etat = 3;
  delay(2000);
  break ;

  case 3:
  afficher_resultat_Pmpp();
  delay(5000);
  etat=0;
  break ;*/

  default : 
  affichage_debut();
  break;

  }

}

  

  


