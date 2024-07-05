#include "settingsdialog.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
     setWindowTitle("MQTT设置");

    QFormLayout *formLayout = new QFormLayout();

    serverAddressInput = new QLineEdit(this);
    clientIdInput = new QLineEdit(this);
    subscriptionTopicInput = new QLineEdit(this);

    formLayout->addRow(new QLabel("服务器地址", this), serverAddressInput);
    formLayout->addRow(new QLabel("客户端ID", this), clientIdInput);
    formLayout->addRow(new QLabel("订阅主题", this), subscriptionTopicInput);

    QPushButton *okButton = new QPushButton("确定", this);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(okButton);

    setLayout(mainLayout);
}

void SettingsDialog::accept() {
    serverAddress = serverAddressInput->text();
    clientId = clientIdInput->text();
    subscriptionTopic = subscriptionTopicInput->text();
    QDialog::accept();
}

QString SettingsDialog::getServerAddress() const {
    return serverAddress;
}

QString SettingsDialog::getClientId() const {
    return clientId;
}

QString SettingsDialog::getSubscriptionTopic() const {
    return subscriptionTopic;
}

void SettingsDialog::setServerAddress(const QString &address) {
    serverAddress = address;
    serverAddressInput->setText(address);
}

void SettingsDialog::setClientId(const QString &id) {
    clientId = id;
    clientIdInput->setText(id);
}

void SettingsDialog::setSubscriptionTopic(const QString &topic) {
    subscriptionTopic = topic;
    subscriptionTopicInput->setText(topic);
}










