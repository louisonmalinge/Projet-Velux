#include <SPI.h>
#include <Ethernet.h>
#include  <Wire.h>
#include  "Ecran.h" // inclure fichier Ecran .h
#include "Siglent.h" // inclure fichier Siglent.h


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int bouton = A1;  
int etat = 0 ; 

void setup() {
  Serial.begin(9600);

 
  pinMode(bouton, INPUT_PULLUP);
  
  
  init_eth();
  setup_ecran(); 
  delay(2000);
  

}

void loop() 
{
  int etat_bouton = digitalRead(bouton);
  Serial.print(etat_bouton);
  if (etat_bouton == LOW)
  {
    etat = 1;
  }


  switch(etat)
  {

  case 1:
  afficher_resultat_Voc();
  delay(2000);
  etat = 2;
  delay(2000);
  break ;

  case 2:
  afficher_resultat_Isc();
  delay(2000);
  etat = 3;
  delay(2000);
  break ;

  case 3:
  afficher_resultat_Pmpp();
  delay(5000);
  etat=0;
  break ;

  default : 
  affichage_debut();
  break;

  }

}

  

  


