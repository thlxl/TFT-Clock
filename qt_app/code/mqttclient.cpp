#include "mqttclient.h"

// const string SERVER_ADDRESS	{ "mqtt://192.168.1.114:1883" };
// const string CLIENT_ID		{ "DesktopAPP" };
// const string TOPIC 			{ "/sensor/temperature" };

// const int  QOS = 1;

//mqtt::create_options createOpts(MQTTVERSION_5);

MyMqttClient::MyMqttClient(const string& server_addr, const string& client_id, int mqtt_version)
    : cli(make_unique<mqtt::async_client>(server_addr, client_id, mqtt::create_options(mqtt_version)))
    , connOpts(mqtt::connect_options_builder()
        .properties({
            {mqtt::property::SESSION_EXPIRY_INTERVAL, 604800}
        })
        .clean_session(false)
        .finalize())
{
    cli->set_connection_lost_handler([](const std::string&) {
        cout << "*** Connection Lost ***" << endl;
    });

    cli->set_disconnected_handler([](const mqtt::properties&, mqtt::ReasonCode reason) {
        cout << "*** Disconnected. Reason: " << reason << " ***" << endl;
    });
}

MyMqttClient::~MyMqttClient()
{
    disconnect();
}

int MyMqttClient::connect() {
    try {
        // Start consumer before connecting to make sure to not miss messages
        cli->start_consuming();

        // Connect to the server
        cout << "Connecting to the MQTT server..." << flush;
        auto tok = cli->connect(connOpts);

        // Getting the connect response will block waiting for the connection to complete.
        auto rsp = tok->get_connect_response();

        // Make sure we were granted a v5 connection.
        if (rsp.get_mqtt_version() < MQTTVERSION_5) {
            cout << "Did not get an MQTT v5 connection." << endl;
            return 0;
        }

        cout << "OK" << endl;

        // If there is no session present, then we need to subscribe
        if (!rsp.is_session_present()) {
            cout << "Session not present on broker. Subscribing." << endl;
        }

        isConnected = true;
        return 1;
    } catch (const mqtt::exception& exc) {
        cerr << "\n  " << exc << endl;
        return 0;
    }
}

int MyMqttClient::subscribe(const string& topic, int qos) {
    try {
        qos_ = qos;
        cli->subscribe(topic, qos)->wait();
        subscribed_topics.push_back(topic);
        return 1;
    } catch (const mqtt::exception& exc) {
        cerr << "\n  " << exc << endl;
        return 0;
    }
}

int MyMqttClient::disconnect() {
    try {
        if (cli->is_connected()) {
            cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
            cli->stop_consuming();
            cli->disconnect()->wait();
            cout << "OK" << endl;
        } else {
            cout << "\nClient was disconnected" << endl;
        }
        isConnected = false;
        return 1;
    } catch (const mqtt::exception& exc) {
        cerr << "\n  " << exc << endl;
        return 0;
    }
}

void MyMqttClient::consume_messages() {
    cout << "Waiting for messages on subscribe topic"  << endl;
    while (true) {
        auto msg = cli->consume_message();
        if (!msg) break;

        string topic = msg->get_topic();
        string payload = msg->to_string();

        topicMessages[topic] = payload;

        //cout << msg->get_topic() << ": " << msg->to_string() << endl;
        if(msg->get_topic() == "/time/style"){
            emit timeStyle();
        }
        emit messageReceived();
    }
}

int MyMqttClient::publish(mqtt::message_ptr pubmsg)
{
    try{
        if(cli->publish(pubmsg)){
            cout << "Publish Success" <<endl;
            return 1;
        }else{
            cout << "Publish timed out" <<endl;
            return 0;
        }
    }catch(const mqtt::exception &exc){
        cout << "Error publishing message:" << exc.what() << endl;
        return 0;
    }
    
}

/////////////////////////////////////////////////////////////////////////////

// int mqttclient_connect(void)
// {
// 	// Create a client using MQTT v5
// 	mqtt::create_options createOpts(MQTTVERSION_5);
// 	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID, createOpts);

// 	auto connOpts = mqtt::connect_options_builder()
// 		.properties({
// 			{mqtt::property::SESSION_EXPIRY_INTERVAL, 604800}
// 	    })
// 		.clean_session(false)
// 		.finalize();

// 	try {
// 		cli.set_connection_lost_handler([](const std::string&) {
// 			cout << "*** Connection Lost ***" << endl;
// 		});

// 		cli.set_disconnected_handler([](const mqtt::properties&, mqtt::ReasonCode reason) {
// 			cout << "*** Disconnected. Reason: " << reason << " ***" << endl;
// 		});

// 		// Start consumer before connecting to make sure to not miss messages

// 		cli.start_consuming();

// 		// Connect to the server

// 		cout << "Connecting to the MQTT server..." << flush;
// 		auto tok = cli.connect(connOpts);

// 		// Getting the connect response will block waiting for the
// 		// connection to complete.
// 		auto rsp = tok->get_connect_response();

// 		// Make sure we were granted a v5 connection.
// 		if (rsp.get_mqtt_version() < MQTTVERSION_5) {
// 			cout << "Did not get an MQTT v5 connection." << endl;
// 			exit(1);
// 		}

// 		// If there is no session present, then we need to subscribe, but if
// 		// there is a session, then the server remembers us and our
// 		// subscriptions.
// 		if (!rsp.is_session_present()) {
// 			cout << "Session not present on broker. Subscribing." << endl;
// 			cli.subscribe(TOPIC, QOS)->wait();
// 		}

// 		cout << "OK" << endl;

// 		// Consume messages
// 		// This just exits if the client is disconnected.
// 		// (See some other examples for auto or manual reconnect)

// 		cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;

// 		while (true) {
// 			auto msg = cli.consume_message();
// 			if (!msg) break;
// 			cout << msg->get_topic() << ": " << msg->to_string() << endl;
// 		}

// 		// If we're here, the client was almost certainly disconnected.
// 		// But we check, just to make sure.

// 		if (cli.is_connected()) {
// 			cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
// 			cli.stop_consuming();
// 			cli.disconnect()->wait();
// 			cout << "OK" << endl;
// 		}
// 		else {
// 			cout << "\nClient was disconnected" << endl;
// 		}
// 	}
// 	catch (const mqtt::exception& exc) {
// 		cerr << "\n  " << exc << endl;
// 		return 1;
// 	}

//  	return 0;
// }
