#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "MQTTClient.h"

using namespace std;

const string ADDRESS = "tcp://mqtt.eclipseprojects.io:1883";
const string TOPIC = "proiect/vlad/date";

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    int nr_maxim;
    float temporizare;

    cout << "Introduceti numarul maxim: ";
    cin >> nr_maxim;
    cout << "Introduceti temporizarea (secunde): ";
    cin >> temporizare;

    MQTTClient_create(&client, ADDRESS.c_str(), "PublisherCPP", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        cerr << "Eroare la conectare MQTT!" << endl;
        return -1;
    }

    for (int i = 1; i <= nr_maxim; i++) {
        // Cream mesajul care contine cifra si temporizarea
        string payload = to_string(i) + "|" + to_string(temporizare);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = (void*)payload.c_str();
        pubmsg.payloadlen = (int)payload.length();
        pubmsg.qos = 1;

        MQTTClient_publishMessage(client, TOPIC.c_str(), &pubmsg, NULL);
        cout << "Trimis prin MQTT: " << payload << endl;

        // Temporizare folosind biblioteca standard C++
        this_thread::sleep_for(chrono::milliseconds((int)(temporizare * 1000)));
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}