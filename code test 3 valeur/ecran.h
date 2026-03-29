#ifndef ECRAN_H
#define ECRAN_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
extern Adafruit_SSD1306 display; 

void init_ecran() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Erreur OLED"));
        for(;;); 
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void afficherMesure(String label, String valeur, String unite) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(label);
    display.setCursor(0, 20);
    display.setTextSize(2); // Plus gros pour la valeur
    display.print(valeur);
    display.setTextSize(1);
    display.print(" " + unite);
    display.display();
}

#endif