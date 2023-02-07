#include <WiFi.h>

const char* ssid = "ESP32";
const char* password = "teste_automatiki";

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP da rede criada: ");
  Serial.println(IP);
}

void loop() {
  // Seu código aqui
}