#include <Arduino.h> // inclure librairie Arduino.h
#include  <Wire.h>  // inclure librairie Wire.h
#include "Siglent.h" // inclure fichier Siglent .h



extern const int eth_cs = 10; // Définir chip select sur pin 10 de l'arduino   
extern const int eth_rst = 9; // Définir reset sur pin 9 de l'arduino 

extern byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Définir adresse Mac de l'arduino    
extern IPAddress ip(192, 168, 1, 100); // Définir l'adresse IP de l'arduino 
extern IPAddress siglentCharge(192, 168, 1, 102); // Indiquer l'adresse IP de la charge
extern IPAddress siglentMulti(192, 168, 1, 101);  // Indiquer l'adresse IP du multimetre
extern const int port = 5025; // Définir port 

EthernetClient clientCharge; // Déclarer la charge comme un client
EthernetClient clientMulti;  // Déclarer le multimètre comme un client


 String interroger_instrument(EthernetClient &client, IPAddress ip, int port, String commande) {
    if (!client.connected()) client.connect(ip, port);
    
    if (client.connected()) {
        client.println(commande);
        unsigned long start = millis();
        while (!client.available() && millis() - start < 2000);
        
        if (client.available()) {
            String resp = client.readStringUntil('\n');
            resp.trim();
            return resp;
        }
    }
    return "Err";
}


// Fonction pour configurer la charge
void set_charge_state(EthernetClient &client, IPAddress ip, int port, String cmd) {
    if (!client.connected()) client.connect(ip, port);
    if (client.connected()) {
        client.println(cmd);
        delay(150); 
    }
}

 void init_eth()  {
    
    // Reset physique Ethernet
    pinMode(eth_rst, OUTPUT); // Mettre la pin rst en sortie
    digitalWrite(eth_rst, LOW); delay(200); // Mettre la pin en niveau bas 
    digitalWrite(eth_rst, HIGH); delay(500); // Mettre la pin en niveau haut  

    Ethernet.init(eth_cs);   
    Ethernet.begin(mac, ip); // Utilisation du tableau mac défini dans Siglent.h
    
    Wire.begin();
    Serial.println(F("Systeme pret. Debut de la sequence."));
    delay(2000);
 
}

 
float mesure_voc() 
{
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:FUNC VOLT"); // Envoyer information à la charge de se mettre en tension continue
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP OFF");   // Désactiver le mode imposer une tension
    delay(500);

    String Voc =  interroger_instrument(clientCharge, siglentCharge, port, "MEAS:VOLT?"); // Demander à la charge sa tension
    Serial.println(Voc); // Ecrire dans le moniteur la tension voc
    float Tension_a_vide = Voc.toFloat(); //convertir la réponse string en float
    delay(500);
    Serial.println(Tension_a_vide);
    return Tension_a_vide; // Retourner la valeur de la tension a vide 
}

float mesure_isc()
{
    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe ON"); // Activer le mode courant de court-circuit
    delay(500);
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP ON"); // Activer le mode imposer une tension 
    delay(1000);

    String Isc =  interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?"); // Demander au mutlimètre son intensité
    Serial.println(Isc); // Ecrire dans le moniteur l'intenisté Isc'
    float Intensite_de_cc = Isc.toFloat() ; //convertir la réponse string en float
    
    delay(1000);

    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe OFF"); // Désactiver le mode courant de court-circuit
    float courant_isc_mA = (Intensite_de_cc *1000);
    Serial.println(courant_isc_mA);
    return courant_isc_mA; // Retourner la valeur de l'intensité de court-circuit
}

float mesure_Pmpp()
{
    float vtest ; // déclarer variable vtest 
    float Pmpp_Max = 0;
    float Pmpp_actuel = 0;
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:FUNC VOLT");  // Envoyer information à la charge de se mettre en tension continue
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP ON"); // Activer le mode imposer une tension 
    for ( float vtest = 0.5; vtest < 30.0 ; vtest +=0.5) // Tant que vtest n'est pas égal a voc alors augmenter vtest de 0.5
    {
        set_charge_state(clientCharge, siglentCharge, port, "SOUR:VOLT:LEV " + String(vtest)); // Imposer une tension qui augmente en fonction de vtest
        delay(500);
        String intensite = interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?"); // Demander mesure de l'intensité du mutlimètre
        delay(100);

        float courant = intensite.toFloat() ; // Convertir la mesure en float
        float courant_mA = (courant * 1000);
        Serial.print("tension = ");
        Serial.print(vtest); // écrire tension imposé
        Serial.print("\n");


        Serial.print("courant = ");
        Serial.print(courant_mA); // écrire courant mesuré
        Serial.print("\n");

        float Pmpp_actuel = vtest * courant; // Calcule de la puissance
        Serial.print("Puissance = ");
        Serial.print(Pmpp_Max *1000); // Ecrire puissance calculé
        Serial.print("\n");
        delay(500);
        
    if (Pmpp_actuel>Pmpp_Max) // Si la puissance actuel est supérieur a la puissance maximun alors
        {
            Pmpp_Max = Pmpp_actuel ; // incrémenter la puissance maximun de la puissance actuel
            delay(500);
        }
    else if(Pmpp_actuel<Pmpp_Max) // Sinon, Si la puissance acutel est inférieur à la puissance maximun alors
        {
        Serial.print("Stop");
        break; // Arrêter la boucle
        }
    }
    float Pmpp_Max_mW = (Pmpp_Max*1000);
    return Pmpp_Max_mW;
}


Variable_Fill_Factor calculer_Fill_Factor()
{
    Variable_Fill_Factor result;
    float Voc = mesure_voc();
    float Isc = mesure_isc();
    result.Pmpp = mesure_Pmpp();
    result.FF = (result.Pmpp/ (Voc * Isc ));
    return result ;
}

String test_charge()
{
    String TC = interroger_instrument(clientCharge, siglentCharge, port, "*IDN?");
    Serial.print("\nIdentification charge =");
    Serial.print(TC);
    return TC;
}

String test_multi()
{
    String TM = interroger_instrument(clientMulti, siglentMulti, port, "*IDN?");
    Serial.print("\n Identification Mutlimètre =");
    Serial.print(TM);
    return TM;
}

float mesure_resistance()
{
    String res =  interroger_instrument(clientCharge, siglentCharge, port, ":MEAS:RES?");   
    float resistance = res.toFloat();
    return resistance ;
}




