// MqttWorker.h
#ifndef MQTTWORKER_H
#define MQTTWORKER_H

#include <QObject>
#include "mqttclient.h"

class MqttWorker : public QObject
{
    Q_OBJECT
public:
    explicit MqttWorker(MyMqttClient *client, QObject *parent = nullptr);

public slots:
    void process();

signals:
    void finished();

private:
    MyMqttClient *client;
};

#endif // MQTTWORKER_H
