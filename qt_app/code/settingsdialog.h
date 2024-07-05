#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    QString getServerAddress() const;
    QString getClientId() const;
    QString getSubscriptionTopic() const;
    void setServerAddress(const QString &address);
    void setClientId(const QString &id);
    void setSubscriptionTopic(const QString &topic);

private slots:
    void accept() override;

private:
    QLineEdit *serverAddressInput;
    QLineEdit *clientIdInput;
    QLineEdit *subscriptionTopicInput;

    QString serverAddress;
    QString clientId;
    QString subscriptionTopic;
};

#endif // SETTINGSDIALOG_H
