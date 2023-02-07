#include <WiFi.h>

const char* ssid = "ESP32";
const char* password = "teste_automatiki";

void setup() {
  Serial.println("Iniciando a placa");
  Serial.begin(115200);

  Serial.println("Tentando criar rede WiFi");
  WiFi.softAP(ssid, password);

  Serial.println("Rede WiFi criada");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP: ");
  Serial.println(IP);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASSWORD: ");
  Serial.println(password);
}

void loop() {
  // Seu código aqui
}