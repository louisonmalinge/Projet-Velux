#ifndef SIGLENT_H 
#define SIGLENT_H
#include <Ethernet.h> // inclure la librairie Ethernet


void init_com(); // Déclarer la fonction pour initialiser les communications
String interroger_instrument(EthernetClient &client, IPAddress ip, int port, String commande); // Déclarer la fonction pour interroger un appareil SIGLENT
void set_charge_state(EthernetClient &client, IPAddress ip, int port, String cmd); // Déclarer la fonction pour configurer un appareil SIGLENT
String test_charge(); // Déclarer la fonction pour tester la communication avec la charge
String test_multi(); // Déclarer la fonction pour tester la communication avec le multimètre
float mesure_voc(); // Déclarer la fonction pour mesurer la tension à vide
float mesure_isc(); // Déclarer la fonction pour mesurer l'intensité de court-circuit
float mesure_Pmpp(); // Déclarer la fonction pour mesurer la puissance maximale
float mesure_resistance(); // Déclarer la fonction pour tester le branchement du panneau solaire
/*===========Déclarer une structure avec 2 variables==============*/
struct Variable_Fill_Factor 
{
    float Pmpp;
    float FF;
};
Variable_Fill_Factor calculer_Fill_Factor();  // Déclarer la fonction pour calculer le Fill Factor

#endif
