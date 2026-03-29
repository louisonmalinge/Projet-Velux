#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include "ecran.h"
#include "Siglent.h"

// --- Configuration ---
const int eth_cs = 10;
const int eth_rst = 9;

// Correction de l'erreur : on définit le tableau MAC proprement
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
IPAddress siglentCharge(192, 168, 1, 102);
IPAddress siglentMulti(192, 168, 1, 101);
const int port = 5025;

// Objets globaux
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
EthernetClient clientCharge;
EthernetClient clientMulti;

void setup() {
    Serial.begin(9600);
    
    // Reset physique Ethernet
    pinMode(eth_rst, OUTPUT);
    digitalWrite(eth_rst, LOW); delay(200);
    digitalWrite(eth_rst, HIGH); delay(500);

    init_ecran();
    
    Ethernet.init(eth_cs);
    Ethernet.begin(mac, ip); // Utilisation du tableau mac défini plus haut
    
    Wire.begin();
    Serial.println(F("Systeme pret. Debut de la sequence."));
    delay(2000);
}

void loop() {
    // --- ÉTAPE 1 : Tension à vide (Voc) en mode CV ---
    Serial.println(F("Etape 1: Tension a vide (Voc) en mode CV"));
    
    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe OFF");
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:FUNC VOLT"); // Mode Tension (CV)
    
    // On regle sur une tension haute (ex: 30V) pour bloquer le courant
    // Assure-toi que cette valeur est superieure a ta source
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:VOLT:LEV 30.0"); 
    
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP OFF");
    
    // On laisse stabiliser puis on lit la tension lue par la CHARGE
    delay(500);
    String voc = interroger_instrument(clientCharge, siglentCharge, port, "MEAS:VOLT?");
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("ETAPE 1/3"));
    display.println(F("Voc (Mode CV @ 30V)"));
    display.setCursor(0,35);
    display.setTextSize(2); display.print(voc); display.println(F(" V"));
    display.display(); display.setTextSize(1);
    
    delay(5000); 

    // --- ÉTAPE 2 : Courant court-circuit (Isc) ---
    Serial.println(F("Etape 2: Court-circuit (Isc)"));
    // Le mode SHORT outrepasse le mode CV
    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe ON");
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:INP ON");
    
    String isc = interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?");
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("ETAPE 2/3"));
    display.println(F("Court-Circuit (Isc)"));
    display.setCursor(0,35);
    display.setTextSize(2); display.print(isc); display.println(F(" A"));
    display.display(); display.setTextSize(1);
    
    delay(5000); 

    // --- ÉTAPE 3 : Intensité sous 1.0V ---
    Serial.println(F("Etape 3: Mesure sous 2.0V"));
    set_charge_state(clientCharge, siglentCharge, port, ":SOURce:SHORt:STATe OFF");
    // On reste en mode VOLT mais on descend a 1.0V
    set_charge_state(clientCharge, siglentCharge, port, "SOUR:VOLT:LEV 2.0");
    
    String i1v = interroger_instrument(clientMulti, siglentMulti, port, "MEAS:CURR:DC?");
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("ETAPE 3/3"));
    display.println(F("Courant sous 2.0V"));
    display.setCursor(0,35);
    display.setTextSize(2); display.print(i1v); display.println(F(" A"));
    display.display(); display.setTextSize(1);

    delay(5000); 
}