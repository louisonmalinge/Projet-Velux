#include <Arduino.h> // inclure la librairie Arduino.h
#include <Wire.h>    // inclure la librairie Wire.h
#include "Siglent.h" // inclure le fichier Siglent.h


/*=========================Déclarer variables pour connexion Ethernet=========================*/

#define eth_cs 10 // Définir le Chip Select sur la pin 10 de l'Arduino   
#define eth_rst 9 // Définir le reset sur la pin 9 de l'Arduino 

extern byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Définir l'adresse MAC de l'Arduino    
extern IPAddress ip(192, 168, 1, 100);                      // Définir l'adresse IP de l'Arduino 
extern IPAddress siglentCharge(192, 168, 1,102);           // Indiquer l'adresse IP de la charge
extern IPAddress siglentMulti(192, 168, 1, 101);            // Indiquer l'adresse IP du multimètre
extern const int port = 5025;                                // Définir le port 

EthernetClient clientCharge; // Déclarer la charge comme client
EthernetClient clientMulti;  // Déclarer le multimètre comme client

/*=========================Fonction pour interroger un appareil SIGLENT=========================*/
String interroger_instrument(EthernetClient &client, IPAddress ip, int port, String commande)
{
    if (!client.connected()) client.connect(ip, port);     // Si le client n'est pas connecté, on tente de se connecter
    if (client.connected()) // Si la connexion est établie
    {
        client.println(commande);  // Envoyer la commande 
        
        unsigned long start = millis();  // Mémoriser l'heure de départ
       
        while (!client.available() && millis() - start < 2000); // Attendre des données ou 2 secondes
        if (client.available()) // Si des données sont disponibles
        {
            String resp = client.readStringUntil('\n');  // Lire jusqu'au retour à la ligne
            resp.trim();  // Supprimer les espaces et retours superflus
            return resp;  // Retourner la réponse
        }
    }
    return "Err";  // Si pas de connexion ou pas de réponse
}


/*=========================Fonction pour configurer un appareil SIGLENT=========================*/
void set_charge_state(EthernetClient &client, IPAddress ip, int port, String cmd)
{
    if (!client.connected()) client.connect(ip, port); // Si le client n'est pas connecté
    if (client.connected()) {     // Si la connexion est établie
        client.println(cmd);      // Envoyer la commande
        delay(150);               // Délai pour traitement de la commande
    }
}

/*=========================Fonction pour initialiser les protocoles de communication=========================*/
void init_com()  
{
    // Reset physique Ethernet
    pinMode(eth_rst, OUTPUT);        // Mettre la pin reset en sortie
    digitalWrite(eth_rst, LOW); 
    delay(200);                      // Mettre la pin à LOW
    digitalWrite(eth_rst, HIGH); 
    delay(500);                      // Mettre la pin à HIGH

    Ethernet.init(eth_cs);           // Initialiser Ethernet avec le Chip Select
    Ethernet.begin(mac, ip);         // Initialiser Ethernet avec MAC et IP
    
    Wire.begin();                    // Initialiser le bus I2C
    delay(1000);
}


/*=========================Fonction pour mesurer la tension à vide=========================*/ 
float mesure_voc() 
{
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:FUNC VOLT"); // Mode tension continue
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP OFF");   // Désactiver mode imposer tension
    delay(500);
    String Voc = interroger_instrument(clientCharge, siglentCharge, port, "MEAS:VOLT?"); // Lire tension
    float Tension_a_vide = Voc.toFloat(); // Convertir en float
    delay(100);
    return Tension_a_vide; // Retourner la valeur
}


/*=========================Fonction pour mesurer l'intensité de court-circuit=========================*/ 
float mesure_isc()
{

    delay(2000);
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP ON");             // Activer mode imposer tension
    delay(1000);
    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe ON"); // Activer mode court-circuit
    delay(1000);

    interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?"); // Lire intensité
    delay(1000);
    String Isc = interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?"); // Lire intensité
    delay(1000);

    float Intensite_de_cc = Isc.toFloat(); // Convertir en float
    delay(100);

    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe OFF"); // Désactiver mode court-circuit
    float courant_isc_mA = (Intensite_de_cc *1000); // Conversion en mA
    return courant_isc_mA;
}


/*=========================Fonction pour mesurer/calculer la puissance en mW=========================*/ 
float mesure_Pmpp()
{
    float Pmpp_Max = 0;       // Puissance maximale
    float Pmpp_actuel = 0;    // Puissance actuelle

    set_charge_state(clientCharge, siglentCharge, port, "SOUR:FUNC VOLT");  // Mode tension continue
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP ON");     // Activer imposer tension

    for (float vtest = 0.5; vtest < 30.0; vtest += 0.5) // Balayage tension
    {
        set_charge_state(clientCharge, siglentCharge, port, "SOUR:VOLT:LEV " + String(vtest)); // Imposer tension
        delay(250);

        String intensite = interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?"); // Lire courant
        float courant = intensite.toFloat();
        float courant_mA = courant * 1000;
        float Pmpp_actuel = vtest * courant; // Calcul puissance
        delay(100);
        
        if (Pmpp_actuel > Pmpp_Max) // Si puissance actuelle > puissance max
        {
            Pmpp_Max = Pmpp_actuel;
            delay(100);
        }
        else if(Pmpp_actuel < Pmpp_Max) // Sinon si puissance actuelle < puissance max
        {
            break; // Arrêter boucle
        }
    }
    float Pmpp_Max_mW = Pmpp_Max * 1000; // Conversion en mW
    return Pmpp_Max_mW;
}


/*=========================Fonction pour calculer le Fill Factor=========================*/ 
Variable_Fill_Factor calculer_Fill_Factor()
{
    float Isc=0;
    Variable_Fill_Factor result;
    float Voc = mesure_voc();       // Mesurer Voc
    Isc = mesure_isc();       // Mesurer Isc
    result.Pmpp = mesure_Pmpp();    // Mesurer Pmpp
    return result;
}


/*=========================Fonction pour tester la communication avec la charge=========================*/ 
String test_charge()
{
    String TC = interroger_instrument(clientCharge, siglentCharge, port, "*IDN?");
    return TC;
}


/*=========================Fonction pour tester la communication avec le multimètre=========================*/
String test_multi()
{
    String TM = interroger_instrument(clientMulti, siglentMulti, port, "*IDN?");
    return TM;
}
