#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "LIVE TIM_9BD2_2G";
const char* password = "7qmdtfuceq";

#define LED 2
#define LAMPADA 26

// Informações de conexão WiFi
  const char* SSID = "LIVE TIM_9BD2_2G"; //"LIVE TIM_F200_2G";
  const char* PASSWORD = "7qmdtfuceq"; //"xpbcn8u37r";

// Informações do servidor MQTT
const char* BROKER_MQTT = "192.168.1.251";
const int BROKER_PORT = 1883;

const char* mqttUser = "";
const char* mqttPassword = "";

#define ID_MQTT "CENTRAL"
#define TOPIC_SUBESCRIBE_LED "publish/led"
#define TOPIC_SUBESCRIBE_LAMPADA "publish/lampada"
#define TOPIC_PUBLISH_LED "subescribe/led"
#define TOPIC_PUBLISH_LAMPADA "subescribe/lampada"


WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);

WiFiServer server(80);

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

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(LAMPADA, OUTPUT);

  conectarWifi();
  Serial.println(WiFi.localIP());
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Click <a href=\"/LED_H\">here</a> to turn the LED on pin 2 on.<br>");
            client.print("Click <a href=\"/LED_L\">here</a> to turn the LED on pin 2 off.<br>");

            client.print("Click <a href=\"/LAMPADA_H\">here</a> to turn the lampada on pin 26 on.<br>");
            client.print("Click <a href=\"/LAMPADA_L\">here</a> to turn the lampada on pin 26 off.<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /LED_H")) {
          //digitalWrite(LED, HIGH);
          if (MQTT.connected()) {
            MQTT.publish(TOPIC_PUBLISH_LED, "led_1");
          }
          else {
            Serial.println("Aparelho não conectado a rede");
          }
        }
        if (currentLine.endsWith("GET /LED_L")) {
          //digitalWrite(LED, LOW);
          if (MQTT.connected()) {
            MQTT.publish(TOPIC_PUBLISH_LED, "led_0");
          }
          else {
            Serial.println("Aparelho não conectado a rede");
          }
        }

        if (currentLine.endsWith("GET /LAMPADA_H")) {
          //digitalWrite(LAMPADA, HIGH);
          if (MQTT.connected()) {
            MQTT.publish(TOPIC_PUBLISH_LAMPADA, "lampada_1");
          }
          else {
            Serial.println("Aparelho não conectado a rede");
          }
        }
        if (currentLine.endsWith("GET /LAMPADA_L")) {
          //digitalWrite(LAMPADA, LOW);
          if (MQTT.connected()) {
            MQTT.publish(TOPIC_PUBLISH_LAMPADA, "lampada_0");
          }
          else {
            Serial.println("Aparelho não conectado a rede");
          }
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }

  mantemConexoes();

  MQTT.loop();
}