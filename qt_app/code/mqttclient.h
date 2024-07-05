#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include <map>
#include <QObject>
#include "mqtt/async_client.h"

using namespace std;

class MyMqttClient : public QObject{
    Q_OBJECT

public:
    MyMqttClient(const string& server_addr, const string& client_id, int mqtt_version);
    ~MyMqttClient();
    int connect();
    int subscribe(const string& topic, int qos);
    int disconnect();
    void consume_messages();
    int publish(mqtt::message_ptr pubmsg);

    map<string, string> topicMessages;

    bool isConnected = false;

signals:
    void messageReceived();
    void timeStyle();

private:
    std::unique_ptr<mqtt::async_client> cli;
    mqtt::connect_options connOpts;
    vector<string> subscribed_topics;
    int qos_;
};

#endif // !MQTTCLIENT_H