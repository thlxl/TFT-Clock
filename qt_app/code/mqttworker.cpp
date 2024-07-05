// MqttWorker.cpp
#include "mqttworker.h"

MqttWorker::MqttWorker(MyMqttClient *client, QObject *parent)
    : QObject(parent), client(client) {}

void MqttWorker::process() {
    client->consume_messages();
    emit finished();
}
