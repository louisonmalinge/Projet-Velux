#include <Arduino.h> // inclure la librairie Arduino.h
#include "Bouton.h"  // inclure le fichier Bouton.h

#define boutonPin A1  // Définir la pin du bouton sur A1

/*=========================Fonction pour lire l'état du bouton=========================*/ 
bool traiterEtatBouton() {
    pinMode(boutonPin, INPUT_PULLUP);                    // Configurer la pin en entrée avec pull-up interne
    bool Valeur_Bouton = digitalRead(boutonPin);         // Lire l'état du bouton
    return Valeur_Bouton;                                // Retourner la valeur lue
}
