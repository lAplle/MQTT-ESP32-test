#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "x";
const char* password = "x";
const char* mqtt_server = "x";

const int connectingLed = 27;
const int connectedLed = 12;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(connectingLed, OUTPUT);
  pinMode(connectedLed, OUTPUT);

}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to " + String(ssid));
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    Serial.println("");
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [" + String(topic) + "]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  digitalWrite(connectedLed, LOW);
  digitalWrite(connectingLed, HIGH);
  delay(1000);
  digitalWrite(connectingLed, LOW);
}

void reconnect() {

    if (client.connect("ESP32Client")) {
      client.subscribe("test/topic");
      Serial.println("Subscribed to topic '" + String(topic) +);
    } else {
      Serial.println("failed, rc=" + String(client.state()));
      Serial.println("Trying again in 5 seconds...");
      delay(5000);
    }
    if (!client.connected()) {
    Serial.println("Reconnecting...");
    }
}

void loop() {
  while (!client.connected()) {
    reconnect();
  } 
  digitalWrite(connectedLed, HIGH);
  client.loop();
}