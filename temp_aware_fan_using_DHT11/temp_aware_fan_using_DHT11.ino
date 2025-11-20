#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ---------- WiFi ----------
const char* ssid = "Redmi 13 5G";      
const char* password = "123456789"; 

// ---------- MQTT ----------
const char* mqtt_server = "10.138.35.82"; 
WiFiClient espClient;
PubSubClient client(espClient);

// ---------- DHT11 ----------
#define DHTPIN 4      // GPIO4 connected to DHT11 data pin
#define DHTTYPE DHT11 // Using DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

// ---------- L298N ----------
#define ENA 13   // Fan speed control (PWM)
#define IN1 12
#define IN2 14

// ---------- Variables ----------
bool autoMode = true;
bool fanStatus = false;
float tempThreshold = 30.0; // temperature in °C to turn ON fan

// ---------- WiFi Setup ----------
void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ---------- MQTT Reconnect ----------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32FanClient")) {
      Serial.println("connected");
      client.subscribe("fanSystem/command"); // topic for manual control
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// ---------- Handle MQTT Messages ----------
void callback(char* topic, byte* message, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(msg);

  if (msg == "ON") {
    autoMode = false;
    fanOn();
  } else if (msg == "OFF") {
    autoMode = false;
    fanOff();
  } else if (msg == "AUTO") {
    autoMode = true;
  }
}

// ---------- Fan Control ----------
void fanOn() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(ENA, 255); // full speed (8-bit = 0–255)
  fanStatus = true;
  Serial.println("Fan ON");
}

void fanOff() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(ENA, 0);   // stop
  fanStatus = false;
  Serial.println("Fan OFF");
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);

  // DHT
  dht.begin();

  // Fan pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // ✅ New API (ESP32 Core v3.x)
  ledcAttach(ENA, 1000, 8);  // pin, freq=1kHz, resolution=8bit (0–255)

  // WiFi + MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ---------- Loop ----------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C  Humidity: ");
  Serial.print(hum);
  Serial.println("%");

  if (autoMode) {
    if (temp > tempThreshold) {
      fanOn();
    } else {
      fanOff();
    }
  }

  // publish to MQTT
  client.publish("fanSystem/temperature", String(temp).c_str());
  client.publish("fanSystem/humidity", String(hum).c_str());
  client.publish("fanSystem/status", fanStatus ? "ON" : "OFF");

  delay(2000); // every 2 seconds
}