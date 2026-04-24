#include <SPI.h>       // inclure la librairie SPI.h
#include <Ethernet.h>  // inclure la librairie Ethernet.h
#include <Wire.h>      // inclure la librairie Wire.h
#include "Ecran.h"     // inclure le fichier Ecran.h
#include "Siglent.h"   // inclure le fichier Siglent.h
#include "Bouton.h"    // inclure le fichier Bouton.h


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // définir l'écran dans le .ino


/*=========================SETUP pour initialiser tous les composants=========================*/                                                  
void setup() 
{
  init_com();     // Fonction pour initialiser les communications
  setup_ecran();  // Fonction pour initialiser l'écran
  delay(1000);    // Délai pour laisser le temps d'initialiser le module Ethernet et l'écran

  affichage_test_charge(); // Fonction pour tester la connexion avec la charge
  delay(500);
  affichage_test_multi();  // Fonction pour tester la connexion avec le multimètre
  delay(500);
}


/*=========================LOOP pour appeler toutes les fonctions en boucle=========================*/     
void loop() 
{
  bool Valeur_Bouton = traiterEtatBouton(); // Incrémenter la variable avec le résultat de la fonction pour connaître l'état du bouton

  if (Valeur_Bouton == 0 ) //Si la valeur du bouton est égal à 0 alors lancer le test
  {
    test_en_cours();        // Fonction pour afficher "Le test est en cours" et tester la connexion du panneau solaire
    afficher_Fill_Factor(); // Appeler la fonction qui affiche la valeur du Fill Factor
  } 
  else                     // Sinon
  {
    affichage_debut();      // Appeler la fonction qui affiche le texte "Appuyer sur le bouton"
    delay(10);
  }
}
