#include <Arduino.h> // inclure librairie arduino
#include "Ecran.h"   // inclure fichier Ecran .h
#include "Siglent.h" // inclure fichier Siglent .h


void setup_ecran() 
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("l'ecran n'est pas connecte \n"));        // Savoir si l'écran est bien trouvé par l'arduino
    for(;;); 
  }
  display.display();
  display.clearDisplay(); 
  display.setTextSize(1);      // Définir la taille du texte a 1
  display.setTextColor(SSD1306_WHITE); // Définir la couleur du texte en blanc
  display.clearDisplay(); 
  delay(500);
}

void affichage_debut()
{
  display.print("Appuyer sur le bouton");
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display(); 
}

 void afficher_resultat_Voc() 
{
  extern float Voc = mesure_voc(); // Incrémenter le variable avec le résulat de la fonction mesure_voc
  display.print("Le VOC est : ");
  display.print(Voc);             // Afficher sur l'écran la valeur de la tesnion à vide
  display.print(" V \n");
  display.display();
  delay(500);
}

 void afficher_resultat_Isc() 
{ 
  extern float Isc = mesure_isc(); // Incrémenter le variable avec le résulat de la fonction mesure_isc
  display.print("L'ISC est : ");
  display.print(Isc* 1000.0, 1);  // Afficher sur l'écran la valeur de l'intensité de court-circuit en mA'
  display.print(" A \n");
  display.display();
  delay(500);
}


void afficher_resultat_Pmpp()
{
  extern float Pmpp = mesure_Pmpp();
  display.print("La puissance Max est de ");
  display.print(Pmpp* 1000.0, 1);             // Afficher sur l'écran la valeur du Pmpp
  display.print(" mW \n");
  display.display();
  delay(500);
}

void afficher_Fill_Factor()
{
  float FF= Pmpp /(Voc*Isc)
}