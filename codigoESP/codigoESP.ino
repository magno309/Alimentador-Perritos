#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// Credenciales de Internet
const char* ssid     = "ARRIS-A692";
const char* password = "3AAAA137575F5418";

// Cosas del dominio
const char* server = "fida-mil.somee.com";

//Otras cosas
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(115200);
  /*Serial.println("");
  WiFi.begin(ssid, password);
  int contconexion = 0;
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50)
    Serial.println("\nWiFi Conectado :D");
  else
    Serial.println("\nError de conexion :c");*/
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(20000); // 20seg

  float peso = 7.8;
  enviarPeso(peso);


}

void enviarPeso(float peso) {
  WiFiClient cliente;
  if (cliente.connect(server, 80)) {
    Serial.print("Conectando al cliente");
    int idDisp = 1;   // Este es el identificador del dispositivo
    String url = "ESPToDB.aspx?accion=actCont&idDisp=" + idDisp + "&pesoAct=" + peso;
    cliente.print(url);
    delay(1000);
  }
  cliente.stop();
}
