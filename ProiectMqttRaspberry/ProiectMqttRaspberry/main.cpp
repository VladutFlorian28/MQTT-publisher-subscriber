#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "MQTTClient.h"

using namespace std;


#define ADDRESS "tcp://localhost:1883"
#define TOPIC   "proiect/florin/date"


int cifre[10][7][5] = {
    {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}}, // 0
    {{0,0,1,0,0}, {0,1,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,1,1,1,0}}, // 1
    {{1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}}, // 2
    {{1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {1,1,1,1,1}}, // 3
    {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {0,0,0,0,1}}, // 4
    {{1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {1,1,1,1,1}}, // 5
    {{1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}}, // 6
    {{1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {0,0,0,0,1}, {0,0,0,0,1}, {0,0,0,0,1}, {0,0,0,0,1}}, // 7
    {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}}, // 8
    {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}, {1,1,1,1,1}}  // 9
};

void deseneazaMatrice(int cifra) {
    if (cifra < 0 || cifra > 9) return;

    cout << "\n--- Matrice 7x5 pentru cifra " << cifra << " ---\n" << endl;
    for (int r = 0; r < 7; r++) {
        cout << "  "; 
        for (int c = 0; c < 5; c++) {
            if (cifre[cifra][r][c] == 1) cout << "# ";
            else cout << ". ";
        }
        cout << endl;
    }
    cout << "\n--------------------------------\n" << endl;
}

int mesajPrimit(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    string payload = (char*)message->payload;
    try {
        int cifra = stoi(payload);
        deseneazaMatrice(cifra % 10); 
    }
    catch (...) {
        cout << "Mesaj primit (nu e cifra): " << payload << endl;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, ADDRESS, "FlorinVisualizer", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, mesajPrimit, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        cout << "Eroare conectare MQTT!" << endl;
        return -1;
    }

    MQTTClient_subscribe(client, TOPIC, 1);
    cout << "=== VIZUALIZARE MATRICE 7x5 ACTIVA ===\nAstept cifre..." << endl;

    while (true) { usleep(100000); }
    return 0;
}