#include <Arduino.h> // inclure librairie Arduino.h
#include "Bouton.h"
#include <Arduino.h>


const int boutonPin = A1;  // Pin du bouton

void initialiserBouton() {
    pinMode(boutonPin, INPUT_PULLUP);  // Initialiser une fois
}

// Retourne true si le bouton est appuyé
bool traiterEtatBouton() {
  bool Valeur_Bouton = digitalRead(boutonPin);

      return Valeur_Bouton ;
}
