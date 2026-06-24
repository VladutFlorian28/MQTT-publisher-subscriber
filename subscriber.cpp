#include <iostream>
#include <string>
#include <wiringPi.h>
#include "MQTTClient.h"

using namespace std;

const string ADDRESS = "tcp://mqtt.eclipseprojects.io:1883";
const string TOPIC = "proiect/vlad/date";

// Definire pini pentru matricea 5x7
int coloane[5] = { 4, 5, 6, 12, 13 };

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    string payload = (char*)message->payload;

    // Extragem doar cifra (pana la separatorul |)
    size_t pos = payload.find('|');
    string cifra = payload.substr(0, pos);

    cout << "--- Mesaj receptat ---" << endl;
    cout << "Cifra de afisat pe matrice: " << cifra << endl;

    // Exemplu de utilizare WiringPi in C++
    digitalWrite(coloane[0], HIGH);
    delay(200);
    digitalWrite(coloane[0], LOW);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    // Initializare WiringPi (sintaxa ramane ca in C)
    if (wiringPiSetupGpio() == -1) {
        cout << "Eroare initializare GPIO!" << endl;
        return -1;
    }

    for (int p : coloane) pinMode(p, OUTPUT);

    MQTTClient client;
    MQTTClient_create(&client, ADDRESS.c_str(), "SubscriberCPP", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_connect(client, &conn_opts);
    MQTTClient_subscribe(client, TOPIC.c_str(), 1);

    cout << "Subscriber C++ pornit. Astept date..." << endl;
    while (true) {
        // Programul ruleaza pana la inchiderea manuala
    }

    return 0;
}