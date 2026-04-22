#ifndef ECRAN_H   // Si écran pas défini
#define ECRAN_H   // Définir l'écran maintenant 

#include <Wire.h>
#include <Adafruit_GFX.h>       // Inclure les bibliothèque 
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Définir nombre de pixel en largeur 
#define SCREEN_HEIGHT 64 // Définir nombre de pixel en Hauteur 
#define OLED_RESET     -1 //Définir la pin reset à -1
#define SCREEN_ADDRESS 0x3C // définir l'adresse I2C de l'écran

extern Adafruit_SSD1306 display;

void setup_ecran(); // Fonction pour initialiser l'ecran

void affichage_test_charge(); // Fonction pour afficher le test de la charge

void affichage_test_multi();  // Fonction pour afficher le test du multimètre

void affichage_debut(); // Fonction pour afficher "Brancher l'écran"zz

void test_en_cours(); // Fonction pour afficher "Le test est en cours"

void afficher_Fill_Factor(); // Fonction pour afficher le résulat du Fill Factor

#endif
