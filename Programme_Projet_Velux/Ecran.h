#ifndef ECRAN_H   // Si l'écran n'est pas défini
#define ECRAN_H   // Définir l'écran maintenant

#include <Wire.h>
#include <Adafruit_GFX.h>       // Inclure les bibliothèques
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // Définir le nombre de pixels en largeur
#define SCREEN_HEIGHT 64  // Définir le nombre de pixels en hauteur
#define OLED_RESET     -1 // Définir la pin reset à -1
#define SCREEN_ADDRESS 0x3C // Définir l'adresse I2C de l'écran

extern Adafruit_SSD1306 display; 

void setup_ecran();              // Fonction pour initialiser l'écran
void affichage_test_charge();    // Fonction pour afficher le test de la charge
void affichage_test_multi();     // Fonction pour afficher le test du multimètre
void affichage_debut();          // Fonction pour afficher "Brancher l'écran"
void test_en_cours();            // Fonction pour afficher "Le test est en cours"
void afficher_Fill_Factor();     // Fonction pour afficher le résultat du Fill Factor

#endif
