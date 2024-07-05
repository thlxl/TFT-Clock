#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "mqttclient.h"
#include "settingsdialog.h"
#include "lcd_number.h"
#include "lcd_number2.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getServerAddress() const { return serverAddress; }
    QString getClientId() const { return clientId; }
    QString getSubscriptionTopic() const { return subscriptionTopic; }
    std::vector<int> extractNumbers(const std::string& str);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void onMessageReceived();
    void onTimeStyle();

    void on_checkBox_stateChanged(int arg1);
    void onSliderValueChanged(int value);

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MyMqttClient> client;
    QString serverAddress;
    QString clientId;
    QString subscriptionTopic;
    QStringList topicsList;
    SettingsDialog* mqttDialog;
    QVector<QPixmap> pixmaps;
    QVector<const unsigned char *> numberImages;
    int sec_ones, sec_tens, min_ones, min_tens, hour_ones, hour_tens;
    int rgb24bit;
};
#endif // MAINWINDOW_H
