#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#ifndef WIFI_SSID
#error "WIFI_SSID is not defined! Please check secrets.ini"
#endif

#ifndef WIFI_PASSWORD
#error "WIFI_PASSWORD is not defined! Please check secrets.ini"
#endif

#ifndef MQTT_SERVER_IP
#error "MQTT_SERVER_IP is not defined! Please check secrets.ini"
#endif

#ifndef MQTT_TOPIC_CMD
#error "MQTT_TOPIC_CMD is not defined! Please check secrets.ini"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER_IP;

const char* topic_cmd = MQTT_TOPIC_CMD; 

#define LED_PIN 4 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected, IP: " + WiFi.localIP().toString());
}

void callback(char* topic, byte* payload, unsigned int length) {
    String msg = "";
    for (int i = 0; i < length; i++) msg += (char)payload[i];
    
    Serial.println("收到指令: " + msg);
    if (msg == "ON") digitalWrite(LED_PIN, HIGH);
    else if (msg == "OFF") digitalWrite(LED_PIN, LOW);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32_Robot_Client", "mqtt", "mqtt")) { 
            Serial.println("connected");

            String discoveryTopic = "homeassistant/light/mulsyse_led/config";
            String discoveryPayload = "{"
                "\"name\": \"LED\","
                "\"unique_id\": \"mulsyse_led_01\","
                "\"command_topic\": " + String(topic_cmd) + ","
                "\"payload_on\": \"ON\","
                "\"payload_off\": \"OFF\","
                "\"device\": {\"identifiers\": [\"mulsyse_01\"], \"name\": \"LED\"}"
            "}";
            
            client.publish(discoveryTopic.c_str(), discoveryPayload.c_str(), true);

            client.subscribe(topic_cmd); 
        } else {
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) reconnect();
    client.loop();
}