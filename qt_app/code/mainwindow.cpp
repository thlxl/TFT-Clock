#include <string>
#include <sstream>
#include <QThread>
#include <QString>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mylabel.h"
#include "colorgradientslider.h"
#include "mqttworker.h"



// const std::string SERVER_ADDRESS	{ "mqtt://192.168.1.114:1883" };
// const std::string CLIENT_ID		{ "DesktopAPP" };
// const std::string TOPIC 			{ "/sensor/temperature" };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),pixmaps(10),numberImages(10)
{
    ui->setupUi(this);

    MyLabel *mylabel_1 = new MyLabel(ui->widget_2);
    mylabel_1->point_size = 5;
    mylabel_1->x1 = 4;
    mylabel_1->x2 = 4;
    mylabel_1->y1 = 6;
    mylabel_1->y2 = 14;
    mylabel_1->setGeometry(177,40,20,160);

    MyLabel *mylabel_2 = new MyLabel(ui->widget_2);
    mylabel_2->point_size = 5;
    mylabel_2->x1 = 4;
    mylabel_2->x2 = 4;
    mylabel_2->y1 = 6;
    mylabel_2->y2 = 14;
    mylabel_2->setGeometry(364,40,20,160);

    MyLabel *mylabel_3 = new MyLabel(ui->widget_3);
    mylabel_3->point_size = 2;
    mylabel_3->x1 = 2;
    mylabel_3->x2 = 2;
    mylabel_3->y1 = 4;
    mylabel_3->y2 = 4;
    mylabel_3->setGeometry(57,30,10,21);

    MyLabel *mylabel_4 = new MyLabel(ui->widget_3);
    mylabel_4->point_size = 2;
    mylabel_4->x1 = 0;
    mylabel_4->x2 = 0;
    mylabel_4->y1 = 4;
    mylabel_4->y2 = 4;
    mylabel_4->setGeometry(114,30,10,21);

    ui->widget_3->setEnabled(false);

    ColorGradientSlider *slider = new ColorGradientSlider(this);
    ui->horizontalLayout_2->addWidget(slider);
    connect(slider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);

    mqttDialog = new SettingsDialog(this);
    serverAddress = "";
    clientId = "";
    subscriptionTopic = "";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_clicked()//连接按钮
{
    if(serverAddress == "" || clientId == ""){
        qDebug() << "please input correct server url or client ID";
        return;
    }
    client = std::make_unique<MyMqttClient>(serverAddress.toStdString(), clientId.toStdString(), MQTTVERSION_5);

    connect(client.get(), &MyMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(client.get(), &MyMqttClient::timeStyle, this, &MainWindow::onTimeStyle);

    if(client->connect() == 0){
        return;
    }

    if(topicsList[0] == ""){
        cout << "Please subscribe MQTT topic!" << endl;
        return;
    }
    
    for(auto topic : topicsList){
        if(client->subscribe(topic.toStdString(), 0) == 0){
            return;
        }
    }

    // 创建 Worker 对象
    MqttWorker *worker = new MqttWorker(client.get());

    // 创建并设置 QThread
    QThread *thread = new QThread;
    worker->moveToThread(thread);

    // 连接信号和槽
    connect(thread, &QThread::started, worker, &MqttWorker::process);
    connect(worker, &MqttWorker::finished, thread, &QThread::quit);
    connect(worker, &MqttWorker::finished, worker, &MqttWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    // 启动线程
    thread->start();
}

void MainWindow::on_pushButton_2_clicked()//MQTT设置按钮
{
    mqttDialog->setServerAddress(serverAddress);
    mqttDialog->setClientId(clientId);
    mqttDialog->setSubscriptionTopic(subscriptionTopic);

    if (mqttDialog->exec() == QDialog::Accepted) {
        serverAddress = mqttDialog->getServerAddress();
        clientId = mqttDialog->getClientId();
        subscriptionTopic = mqttDialog->getSubscriptionTopic();
        topicsList = subscriptionTopic.split(' ');

        qDebug() << "服务器地址:" << serverAddress;
        qDebug() << "客户端ID:" << clientId;
        qDebug() << "订阅主题:" << subscriptionTopic;
    }
}

void MainWindow::onMessageReceived()
{
    ui->label_8->setText(QString::fromStdString(client->topicMessages["/sensor/tem"]) + "℃");
    ui->label_10->setText(QString::fromStdString(client->topicMessages["/sensor/hum"]) + "%");
    // QImage image(gImage_0, 80, 160, QImage::Format_RGB16);

    // // 将 QImage 转换为 QPixmap 并显示在 QLabel 中
    // QPixmap pixmap = QPixmap::fromImage(image);

    vector<int> time_nums = extractNumbers(client->topicMessages["/time"]);

    if(pixmaps.isEmpty() || (time_nums.size() < 6)){
        return;
    }
    ui->label->setPixmap(pixmaps[time_nums[0]]);
    ui->label_2->setPixmap(pixmaps[time_nums[1]]);
    ui->label_3->setPixmap(pixmaps[time_nums[2]]);
    ui->label_4->setPixmap(pixmaps[time_nums[3]]);
    ui->label_5->setPixmap(pixmaps[time_nums[4]]);
    ui->label_6->setPixmap(pixmaps[time_nums[5]]);
}

void MainWindow::onTimeStyle()
{
    cout << "/time/style: " << client->topicMessages["/time/style"] << endl;
    if(client->topicMessages["/time/style"] == "image1"){
        numberImages[0] = gImage_0;
        numberImages[1] = gImage_1;
        numberImages[2] = gImage_2;
        numberImages[3] = gImage_3;
        numberImages[4] = gImage_4;
        numberImages[5] = gImage_5;
        numberImages[6] = gImage_6;
        numberImages[7] = gImage_7;
        numberImages[8] = gImage_8;
        numberImages[9] = gImage_9;
    }else if (client->topicMessages["/time/style"] == "image2")
    {
        numberImages[0] = gImage2_0;
        numberImages[1] = gImage2_1;
        numberImages[2] = gImage2_2;
        numberImages[3] = gImage2_3;
        numberImages[4] = gImage2_4;
        numberImages[5] = gImage2_5;
        numberImages[6] = gImage2_6;
        numberImages[7] = gImage2_7;
        numberImages[8] = gImage2_8;
        numberImages[9] = gImage2_9;     
    }
    

    QImage image;
    for(int i = 0; i < 10; i++){       
        image = QImage(numberImages[i], 80, 160, QImage::Format_RGB16);
        pixmaps[i] = QPixmap::fromImage(image);  
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool isEnabled = (arg1 == Qt::Checked);
    ui->widget_3->setEnabled(isEnabled);
}

void MainWindow::onSliderValueChanged(int value)
{
    QColor color;
    color.setHsv(value, 255, 255); // Set HSV color

    int r = color.red();
    int g = color.green();
    int b = color.blue();

    rgb24bit = (r << 16) | (g << 8) | b;


    std::string payload = std::to_string(rgb24bit);
    mqtt::message_ptr pubmsg = mqtt::make_message("/LED/rgb", payload);
    pubmsg->set_qos(1);
    if((client == NULL) || (!client->isConnected)){
        cout << "Please connect MQTT server!" << endl;
        return;
    }
    client->publish(pubmsg);
}

std::vector<int> MainWindow::extractNumbers(const std::string& str) {
    std::vector<int> numbers;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ',')) {
        try {
            int num = std::stoi(token);
            numbers.push_back(num);
        } catch (const std::invalid_argument& e) {
            // Handle invalid numbers or non-integer strings here if needed
            std::cerr << "Invalid number: " << token << std::endl;
        }
    }

    return numbers;
}
void MainWindow::on_pushButton_5_clicked()//断开按钮
{
    if((client == NULL) || (!client->isConnected)){
        cout << "Not connect MQTT server!" << endl;
        return;
    }
    client->disconnect();
}


void MainWindow::on_pushButton_3_clicked()//时钟风格按钮
{
    std::string payload = "switch";
    mqtt::message_ptr pubmsg = mqtt::make_message("/time/style", payload);
    pubmsg->set_qos(1);
    if((client == NULL) || (!client->isConnected)){
        cout << "Please connect MQTT server!" << endl;
        return;
    }
    client->publish(pubmsg);
}


void MainWindow::on_pushButton_clicked()//校时设置按钮
{

}

