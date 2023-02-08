#include <WiFi.h>
#include <WebServer.h>

const int LED = 2;

const char *ssid = "ESP32_AP";
const char *password = "password";

WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("ESP32 IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    String html = "<h1>ESP32 Web Server</h1>";
    html += "<p><a href='/ledOn'><button>Ligar LED</button></a></p>";
    html += "<p><a href='/ledOff'><button>Desligar LED</button></a></p>";
    server.send(200, "text/html", html);
  });

  server.on("/ledOn", []() {
    digitalWrite(LED, HIGH);
    String html = "<h1>ESP32 Web Server</h1>";
    html += "<p><a href='/ledOn'><button>Ligar LED</button></a></p>";
    html += "<p><a href='/ledOff'><button>Desligar LED</button></a></p>";
    server.send(200, "text/html", html);
  });

  server.on("/ledOff", []() {
    digitalWrite(LED, LOW);
    String html = "<h1>ESP32 Web Server</h1>";
    html += "<p><a href='/ledOn'><button>Ligar LED</button></a></p>";
    html += "<p><a href='/ledOff'><button>Desligar LED</button></a></p>";
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}
