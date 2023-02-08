#include <sMQTTBroker.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>

sMQTTBroker broker;
AsyncWebServer server(80);

unsigned long Time;
unsigned long freeRam;

IPAddress local_IP(192, 168, 1, 251);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "LIVE TIM_9BD2_2G"; //"LIVE TIM_F200_2G";
const char* password = "7qmdtfuceq"; //"xpbcn8u37r";
const unsigned short mqttPort = 1883;

void otaEsp() {
  ArduinoOTA.setHostname("Broker32");

  ArduinoOTA.onStart([]() {
    String type;
    if(ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    }
    else {
      type = "filesystem";
    }

    Serial.println("Start updating" + type);
  })
  .onEnd([] () {
    Serial.println("\nEnd");
  });
}

void setup() {
  Serial.begin(115200);

  Serial.println("Tentando fixar IP");
  if(!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao conectar");
  }
  else {
    Serial.println("Conectado com sucesso");
  }

  Serial.println("Tentando conectar a rede");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");
  Serial.print("IP address:\t");

  Serial.println(WiFi.localIP());

  otaEsp();

  broker.init(mqttPort);

  // Define a rota para a página inicial
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(200, "text/html", "<h1>Bem-vindo ao servidor local</h1>");
});

// Inicia o servidor
server.begin();
}

void loop() {
  ArduinoOTA.handle();
  broker.update();

  if(millis() - Time > 1000) {
    Time=millis();
    if(ESP.getFreeHeap() != freeRam) {
      freeRam = ESP.getFreeHeap();
      Serial.print("RAM: ");
      Serial.println(freeRam);
    }
  }
}

