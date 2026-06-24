#include <iostream>
#include <string>
#include <unistd.h>
#include "MQTTClient.h"

using namespace std;

#define ADDRESS "tcp://localhost:1883"
#define TOPIC   "proiect/florin/date"

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, ADDRESS, "FlorinPublisherManual", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        cout << "Eroare la conectarea cu brokerul HiveMQ!" << endl;
        return -1;
    }

    // --- VALORI SETATE AUTOMAT ---
    int nr_maxim = 6;      // Va număra singur de la 1 la n <= 9
    float secunde = 5.0;    // Va trimite o cifră în fiecare secunda
    // -----------------------------

    cout << "=== PUBLISHER ACTIV (MOD AUTOMAT) ===" << endl;
    cout << "Trimit " << nr_maxim << " cifre cu pauza de " << secunde << " secunde." << endl;

    for (int i = 1; i <= nr_maxim; i++) {
        string mesaj = to_string(i);
        MQTTClient_publish(client, TOPIC, mesaj.length(), (void*)mesaj.c_str(), 1, 0, NULL);

        cout << "Am trimis: " << i << endl;

        // usleep folosește microsecunde: 1 secundă = 1.000.000
        usleep(secunde * 1000000);
    }

    cout << "Numărătoarea s-a încheiat cu succes!" << endl;
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}