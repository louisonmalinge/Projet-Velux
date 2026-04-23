#include <Arduino.h> // inclure librairie arduino
#include "Ecran.h"   // inclure fichier Ecran .h
#include "Siglent.h" // inclure fichier Siglent .h
#include "Bouton.h"


void setup_ecran() 
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
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
  display.clearDisplay();
  display.setCursor(0, 32);
  display.setTextSize(1);
  display.print("Appuyer sur le bouton");  // Envoyer le texte sur l'écran  
  display.display(); // Afficher le texte
  display.setCursor(0, 0);
}

void test_en_cours()
{
 display.clearDisplay(); // Effacer l'écran
 display.setCursor(0, 32);
 display.setTextSize(1);
 display.print("Le test est en cours"); // Envoyer le texte sur l'écran
 display.display(); // Afficher le texte
}


void afficher_Fill_Factor()
{
  initialiserBouton();
  //bool Valeur_Bouton = traiterEtatBouton();
  Variable_Fill_Factor result = calculer_Fill_Factor();
  float Fill_Factor = result.FF;  // Récupérer la valeur du Fill Factor
  float Pmpp = result.Pmpp;      // Récupérer la valeur du Pmpp
  display.clearDisplay(); // Effacer l'écran

 while (traiterEtatBouton()) 
 { 
  
  Serial.print("\n");
  Serial.print(Pmpp);
  Serial.print("\n");
  Serial.print(Fill_Factor);

 
  if (Fill_Factor>0.39 && Pmpp>55)
    {
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.display(); 
      display.print("FF=");
      display.print(Fill_Factor);
      display.print("\nPmpp=");
      display.print(Pmpp); 
      display.print("mW \n");
      display.display();
      display.setTextSize(3); 
      display.setCursor(40, 25);
      display.print("OK \n");
      display.display();
    }
  else if (Fill_Factor<0 && Fill_Factor>1)
    {
      display.setCursor(0, 32);
      display.print("Relancer le test"); // Envoyer le texte sur l'écran
      display.display(); // Afficher le texte
    }
  else 
    {
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.display(); 
      display.print("FF=");
      display.print(Fill_Factor);
      display.print("\nPmpp=");
      display.print(Pmpp); 
      display.print("mW \n");
      display.display();
      display.setTextSize(3); 
      display.setCursor(40, 25);
      display.print("H.S");
      display.display();
    }
    delay(100);
  }
  affichage_debut();
  delay(500);
}

void affichage_test_charge()
{
  String resultat_charge = test_charge();
  delay(1000);
  while (resultat_charge == "Err")
  {
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32);
    display.print("verifier branchement charge"); // Envoyer le texte sur l'écran
    display.display(); // Afficher le texte

    delay(1000);
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32);
    display.print("Tentative de \n reconnexion..."); // Envoyer le texte sur l'écran  
    display.display(); // Afficher le texte
    delay(1000);
    resultat_charge = test_charge();
  }
  if (resultat_charge == "Siglent Technologies,SDL1020X-E,SDL13GCC900335,1.1.1.23R4")
  {
    display.clearDisplay(); // Effacer l'écran
    display.setCursor(0, 32);
    display.print("La charge est \n connecte"); // Envoyer le texte sur l'écran
    display.display(); // Afficher le texte
  }  
}


void affichage_test_multi()
{
  String resultat_multi = test_multi();

   while (resultat_multi == "Err")
  {
        display.clearDisplay(); // Effacer l'écran
        display.setCursor(0, 32);
        display.print("verifier branchement multimetre"); // Envoyer le texte sur l'écran
        display.display(); // Afficher le texte

        delay(1000);
        display.clearDisplay(); // Effacer l'écran
        display.setCursor(0, 32);
        display.print("Tentative de \n reconnexion..."); // Envoyer le texte sur l'écran  
        display.display(); // Afficher le texte
       resultat_multi = test_multi();
  }
 if (resultat_multi == "Siglent Technologies,SDM3045X,SDM34FBX5R1363,5.01.01.06R1")
  {
      display.clearDisplay(); // Effacer l'écran
      display.setCursor(0, 32);
      display.print("Le multimetre est \n connecte"); // Envoyer le texte sur l'écran
      display.display(); // Afficher le texte
  }
}

void panneau_solaire_branche()
 {
  float res = mesure_resistance();
  while (res == 0)
  {
    display.clearDisplay();
    display.setCursor(0, 32);
    display.print("Veuillez brancher le \n panneau solaire");
    display.display();
    res = mesure_resistance();
  }
    display.clearDisplay();
    display.setCursor(0, 32);
    display.print("Panneau Solaire \n Branche");
    display.display();
    delay(2000);
    display.clearDisplay();
 }

 
 


  

