#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <Ethernet.h>

// Fonction pour interroger une valeur (Tension ou Courant)
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

#endif