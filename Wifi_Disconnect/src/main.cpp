#include <Arduino.h>

#include "secrets/wifi.h"
#include "wifi_connect.h"
#include <WiFiClientSecure.h>
#include "ca_cert.h"

#include "secrets/mqtt.h"
#include <PubSubClient.h>

#include <Ticker.h>

namespace
{
    const char *ssid = WiFiSecrets::ssid;
    const char *password = WiFiSecrets::pass;
    const char *echo_topic = "esp32/echo_test";
    unsigned int publish_count = 0;
    uint16_t keepAlive = 15;    // seconds (default is 15)
    uint16_t socketTimeout = 5; // seconds (default is 15)
}

WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);

Ticker mqttPulishTicker;

void mqttPublish()
{
    Serial.print("Publishing: ");
    Serial.println(publish_count);
    mqttClient.publish(echo_topic, String(publish_count).c_str(), false);
    publish_count++;
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("From %s:  ", topic);
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void mqttReconnect()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT connection...");
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        if (mqttClient.connect(client_id.c_str(), MQTT::username, MQTT::password))
        {
            Serial.print(client_id);
            Serial.println(" connected");
            mqttClient.subscribe(echo_topic);
        }
        else
        {
            Serial.print("MTTT connect failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 1 seconds");
            delay(1000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    setup_wifi(ssid, password);
    tlsClient.setCACert(ca_cert);

    // mqttClient.setKeepAlive(keepAlive); // To see how long mqttClient detects the TCP connection is lost
    // mqttClient.setSocketTimeout(socketTimeout); // To see how long mqttClient detects the TCP connection is lost

    mqttClient.setCallback(mqttCallback);
    mqttClient.setServer(MQTT::broker, MQTT::port);
    mqttPulishTicker.attach(1, mqttPublish);
}

void loop()
{
    if (!mqttClient.connected())
    {
        mqttReconnect();
    }
    mqttClient.loop();
}
