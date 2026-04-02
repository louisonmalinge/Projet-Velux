#ifndef SIGLENT_H 
#define SIGLENT_H
#include <Ethernet.h> // inclure librairie ethernet


void init_eth() ; // Fonction initialiser ethernet

String interroger_instrument(EthernetClient &client, IPAddress ip, int port, String commande); // Fonction pour récupérer une information d'un appareil de mesure

void set_charge_state(EthernetClient &client, IPAddress ip, int port, String cmd); // Fonction pour envoyer une information a l'appareil de mesure

float mesure_voc(); // Fonction pour mesurer la tension a vide

float mesure_isc(); // Fonction pour mesurer l'intensité de court-circuit

float mesure_Pmpp(); // Fonction pour mesurer la puissance maximun

float calcul_Fill_Factor();

#endif