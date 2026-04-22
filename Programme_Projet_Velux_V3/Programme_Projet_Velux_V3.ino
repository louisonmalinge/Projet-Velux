#include <SPI.h>       // inclure librairie SPI.h
#include <Ethernet.h>  // inclure librairie ethernet.h
#include <Wire.h>      // inclure librairie Wire.h
#include "Ecran.h"     // inclure fichier Ecran .h
#include "Siglent.h"   // inclure fichier Siglent.h


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // définir écran dans le .ino
const int bouton = A1;                                                     // Définir le bouton sur la Pin A1

void setup() {
  Serial.begin(9600);

  pinMode(bouton, INPUT_PULLUP);  // Activer la résistance de Pull Up interne sur la pin A1 (Pin bouton)

  init_eth();     // Fonction pour initialiser le module ethernet
  setup_ecran();  // Fonction pour initialiser l'écran
  delay(2000);  // Délais pour laisser le temps d'initialiser le module ethernet et l'écran

  affichage_test_charge();
  delay(1000);
  affichage_test_multi();
  delay(1000);



  delay(2000);  // Délais pour laisser le temps d'initialiser le module ethernet et l'écran
}

void loop() {
  int etat_bouton = digitalRead(bouton);  // Créer variable et incrémenter la valeur du bouton lu

  if (etat_bouton == LOW)  // Si le bouton et à l'état bas alors
  {
    test_en_cours();
    afficher_Fill_Factor();  // Appeler la fonction qui afficher la valeur du Fill Factor
  } else                     // Sinon0
  {
    affichage_debut();  // Appeler la fonction qui afficher le texte "Appuyer surle bouton"
    delay(10);
  }
}
