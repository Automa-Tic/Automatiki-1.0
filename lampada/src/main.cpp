#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

#define pinLED 2
#define pinLAMPADA 26

// Informações de conexão WiFi
  const char* SSID = "LIVE TIM_9BD2_2G"; //"LIVE TIM_F200_2G";
  const char* PASSWORD = "7qmdtfuceq"; //"xpbcn8u37r";
WiFiClient wifiClient;

// Informações do servidor MQTT
const char* BROKER_MQTT = "192.168.1.251";
const int BROKER_PORT = 1883;

#define ID_MQTT "BCI02"
#define TOPIC_PUBLISH_LED "publish/led"
#define TOPIC_PUBLISH_LAMPADA "publish/lampada"
#define TOPIC_SUBESCRIBE_LED "subescribe/led"
#define TOPIC_SUBESCRIBE_LAMPADA "subescribe/lampada"
PubSubClient MQTT(wifiClient);

//Informações do esp32
bool led_status = false;
bool lampada_status = false;

//Funções 
void conectarWifi() {
  while (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Conectando-se a rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.print("Conectado com sucesso a rede: ");
  Serial.print(SSID);
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP());

}
void conectarMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if(MQTT.connect(ID_MQTT)) {
      Serial.print("Conectado ao broker: ");
      Serial.print("com sucesso.");
      Serial.println(" com sucesso.");

        MQTT.subscribe(TOPIC_SUBESCRIBE_LED);
        MQTT.subscribe(TOPIC_SUBESCRIBE_LAMPADA);
    }
    else {
      Serial.println("Falha ao conectar ao broker.");
      Serial.println("Nova tentativa de conexão em 1 segundo.");
      delay(1000);
    }
  }
}
void mantemConexoes() {
  if(!MQTT.connected()) {
    conectarMQTT();
  }

  conectarWifi();
}
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for(int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
    
    char c = (char)payload[i];
    msg += c;
  }

  if(msg == "led_1") {
    // lIGA O LED
    digitalWrite(pinLED, HIGH);

    // Publica o status do LED
    led_status = true;
  }
  if(msg == "led_0") {
    // Desliga o LED
    digitalWrite(pinLED, LOW);

    // Publica o status do LED
    led_status = false;
  }

  if(msg == "lampada_1") {
    // lIGA O LED
    digitalWrite(pinLAMPADA, HIGH);

    // Publica o status do LED
    lampada_status = true;
  }
  if(msg == "lampada_0") {
    // Desliga o LED
    digitalWrite(pinLAMPADA, LOW);

    // Publica o status do LED
    lampada_status = false;
  }

}


void setup() {
  pinMode(pinLAMPADA , OUTPUT);
  pinMode(pinLED , OUTPUT);
  Serial.begin(115200);

  conectarWifi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callback);
}

void loop() {
  mantemConexoes();

  MQTT.publish(TOPIC_PUBLISH_LED, led_status ? "ligada" : "desligada"); // Publica o status da lâmpada a cada segundo
  MQTT.publish(TOPIC_PUBLISH_LAMPADA, lampada_status ? "ligada" : "desligada"); // Publica o status da lâmpada a cada segundo

  MQTT.loop();
}


