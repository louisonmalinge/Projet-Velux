#include <Arduino.h> // inclure la librairie Arduino
#include "Ecran.h"   // inclure le fichier Ecran.h
#include "Siglent.h" // inclure le fichier Siglent.h
#include "Bouton.h"  // inclure le fichier Bouton.h


/*=========================SETUP pour initialiser l'écran=========================*/     
void setup_ecran() 
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // Si l'écran n'est pas connecté
  {
    for(;;); // Arrêter le programme
  }
  display.display();
  display.clearDisplay(); 
  display.setTextSize(1);                // Définir la taille du texte à 1
  display.setTextColor(SSD1306_WHITE);  // Définir la couleur du texte en blanc
  display.clearDisplay(); 
  delay(500);
}


/*=========================Fonction pour afficher un texte=========================*/     
void affichage_debut()
{
  display.clearDisplay();             // Nettoyer l'écran
  display.setCursor(0, 32);           // Placer le curseur 
  display.setTextSize(1);             // Définir la taille du texte à 1
  display.print("Appuyer sur le bouton");  // Envoyer le texte sur l'écran  
  display.display();                  // Afficher le texte
}


/*=========================Fonction pour tester la connexion du panneau solaire=========================*/
void test_en_cours()
{
float Voc = mesure_voc();
while (Voc < 0.5) // Tant que res est égal à 0
  {
    display.clearDisplay(); // Nettoyer l'écran
    display.setCursor(0, 32);  // Placer le curseur
    display.print("Veuillez brancher le \npanneau solaire"); // Envoyer le texte sur l'écran  la cha
    display.display(); // Afficher le text
    Voc = mesure_voc(); // Incrémenter variable par le résultat de la fonction
  }
    display.clearDisplay(); // Nettoyer l'écran
    display.setCursor(0, 32);  // Placer le curseur
    display.print("Panneau Solaire \nBranche"); // Envoyer le texte sur l'écran  
    display.display(); // Afficher le texte
    delay(2000); // Durant 2 secondes
    display.clearDisplay(); // Nettoyer l'écran
    display.setCursor(0, 32);  // Placer le curseur
    display.print("Le test est en cours");   // Envoyer le texte sur l'écran
    display.display();                       // Afficher le texte
}

int nb_valide=0;
int nb_pas_valide=0;
/*=========================Fonction pour afficher le résultat du Fill Factor=========================*/
void afficher_Fill_Factor()
{
  bool incrementer = false; 
  Variable_Fill_Factor result = calculer_Fill_Factor(); // Récupérer le résultat de la fonction
  float Fill_Factor = result.FF;                         // Récupérer la valeur du Fill Factor
  float Pmpp = result.Pmpp;                              // Récupérer la valeur du Pmpp
  display.clearDisplay();                                // Effacer l'écran

 while (traiterEtatBouton()) // Tant que l'état du bouton n'est pas LOW
 { 
  if (Fill_Factor>0.39 && Fill_Factor < 1 && Pmpp>55) // Si le Fill Factor est supérieur à 0.39 et le Pmpp supérieur à 55 mW
    {
      if (!incrementer) //Si incrementer est égal a faux alors
      {
        nb_valide ++; // augmenter le nombre de panneau solaire de plus 1 
        incrementer = true ; // Mettre la varianle incrementer a vraie
      }
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.display(); 
      display.print("FF=");
      display.print(Fill_Factor);   // Afficher le Fill Factor
      display.print("\nPmpp=");
      display.print(Pmpp);          // Afficher la puissance
      display.print("mW \n");
      display.display();
      display.print("Nb v = "); 
      display.print(nb_valide);
      display.print(" Nb x = "); 
      display.print(nb_pas_valide);
      display.display(); 

      // Déclarer les variables pour dessiner ✓
      int centerX = SCREEN_WIDTH / 2.2;
      int centerY = SCREEN_HEIGHT / 1.6;
      int size = 30;
      int thickness = 8;

      // Afficher ✓ pour indiquer que le panneau solaire est OK
      for (int i = -thickness/2; i <= thickness/2; i++) {
        display.drawLine(centerX - size/2, centerY + i, centerX, centerY + size/2 + i, SSD1306_WHITE); // branche courte
        display.drawLine(centerX, centerY + size/2 + i, centerX + size, centerY - size/3 + i, SSD1306_WHITE); // branche longue légèrement inclinée
      }

      display.display(); // Afficher ✓ sur l'écran
    }
  else if (Fill_Factor<0 || Fill_Factor>1) // Sinon si le Fill Factor est inférieur à 0 ou supérieur à 1
    {
      display.setCursor(0, 32);            // Placer le curseur 
      display.print("Relancer le test");   // Envoyer le texte sur l'écran pour relancer le test 
      display.display();                   // Afficher le texte
    }
  else 
    {
      if (!incrementer) //Si incrementer est égal a faux alors
      {
        nb_pas_valide ++; // augmenter le nombre de panneau solaire de plus 1 
        incrementer = true ; // Mettre la varianle incrementer a vraie
      }
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.display(); 
      display.print("FF=");
      display.print(Fill_Factor); // Afficher le Fill 
      display.print("\nPmpp=");
      display.print(Pmpp); 
      display.print("mW \n");     // Afficher la puissance
      display.print("Nb v = "); 
      display.print(nb_valide);
      display.print(" Nb x = "); 
      display.print(nb_pas_valide);
      display.display(); 

      // Déclarer les variables pour dessiner ☓
      int centerX = SCREEN_WIDTH / 2;
      int centerY = SCREEN_HEIGHT / 1.5;
      int halfSize = 15;   // moitié de la longueur de la croix
      int thickness = 6;   // épaisseur en pixels

      // Afficher ☓ pour indiquer que le panneau solaire est H.S
      for (int i = -thickness / 2; i <= thickness / 2; i++) {
        display.drawLine(centerX - halfSize, centerY - halfSize + i, centerX + halfSize, centerY + halfSize + i, SSD1306_WHITE);
        display.drawLine(centerX - halfSize, centerY + halfSize + i, centerX + halfSize, centerY - halfSize + i, SSD1306_WHITE);
      }
      display.display(); // Afficher ☓ sur l'écran
    }
    delay(100);
  }
  affichage_debut(); // Afficher l'écran de début
  delay(500);
}


/*=========================Fonction pour afficher le test de la connexion avec la charge=========================*/
void affichage_test_charge()
{
  String resultat_charge = test_charge(); // Récupérer le résultat de la fonction
  delay(1000);
  while (resultat_charge == "Err") // Tant que resultat_charge est égal à "Err"
  {
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32); // Placer le curseur
    display.print("verifier branchement charge"); // Envoyer le texte sur l'écran
    display.display(); // Afficher le texte

    delay(1000);
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32); // Placer le curseur
    display.print("Tentative de \nreconnexion..."); // Envoyer le texte sur l'écran  
    display.display(); // Afficher le texte
    delay(1000);
    resultat_charge = test_charge(); // Récupérer le résultat de la fonction
  }
  if (resultat_charge == "Siglent Technologies,SDL1020X-E,SDL13GCC900335,1.1.1.23R4") // Si resultat_charge est correct
  {
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32); // Placer le curseur 
    display.print("La charge est \nconnecte"); // Envoyer le texte sur l'écran
    display.display(); // Afficher le texte
  }  
}


/*=========================Fonction pour afficher le test de la connexion avec le multimètre=========================*/
void affichage_test_multi()
{
  String resultat_multi = test_multi(); // Récupérer le résultat de la fonction

  while (resultat_multi == "Err") // Tant que resultat_multi est égal à "Err"
  {
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32);  // Placer le curseur
    display.print("verifier branchement multimetre"); // Envoyer le texte sur l'écran
    display.display(); // Afficher le texte

    delay(1000);
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32);  // Placer le curseur
    display.print("Tentative de \nreconnexion..."); // Envoyer le texte sur l'écran  
    display.display(); // Afficher le texte
    resultat_multi = test_multi(); // Récupérer le résultat de la fonction
  }
  if (resultat_multi == "Siglent Technologies,SDM3045X,SDM34FBX5R1363,5.01.01.06R1")  // Si resultat_multi est correct
  {
      display.clearDisplay(); // Effacer l'écran
      display.setCursor(0, 32);// Placer le curseur
      display.print("Le multimetre est \nconnecte"); // Envoyer le texte sur l'écran
      display.display(); // Afficher le texte
  }
}
 


  

