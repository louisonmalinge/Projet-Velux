#include <SPI.h>       // inclure librairie SPI.h
#include <Ethernet.h>  // inclure librairie ethernet.h
#include <Wire.h>      // inclure librairie Wire.h
#include "Ecran.h"     // inclure fichier Ecran .h
#include "Siglent.h"   // inclure fichier Siglent.h
#include "Bouton.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // définir écran dans le .ino
                                                   
void setup() 
{
  Serial.begin(9600);

  init_eth();     // Fonction pour initialiser le module ethernet
  setup_ecran();  // Fonction pour initialiser l'écran
  initialiserBouton();
  delay(2000);  // Délais pour laisser le temps d'initialiser le module ethernet et l'écran

  affichage_test_charge();
  delay(1000);
  affichage_test_multi();
  delay(1000);
}

void loop() 
{
  bool Valeur_Bouton = traiterEtatBouton();

  if (Valeur_Bouton == 0 ) 
  {
    panneau_solaire_branche();
    test_en_cours();
    afficher_Fill_Factor();  // Appeler la fonction qui afficher la valeur du Fill Factor
  } 
  else                     // Sinon
  {
    affichage_debut();  // Appeler la fonction qui afficher le texte "Appuyer surle bouton"
    delay(10);
  }
}
