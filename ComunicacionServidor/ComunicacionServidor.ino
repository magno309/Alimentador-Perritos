#include <WiFi.h>
#include <WiFiClient.h>

//const char* ssid     = "ARRIS-A692";
//const char* password = "3AAAA137575F5418";

const char* ssid     = "PUERTASELECTRICAS";
const char* password = "NNsFKa4RN5";

char host[50];
char strHost[] = "fida-mil.somee.com";
char strUrl[] = "/ESPToDB.aspx";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;

String output2State = "off";
const int output2 = 2;

void setup() {
  pinMode(output2, OUTPUT);
  Serial.begin(115200);
  // Conectarse a Internet
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

unsigned int idDispensador = 1;
unsigned int cantidadComida;

unsigned long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  leerCantidadComida();
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      enviarCantidadComida();

    } else {
      Serial.println("Sin conexión a Internet!");
    }
  }
}


void leerCantidadComida() {
  cantidadComida = 1002;
}

void enviarCantidadComida() {
  WiFiClient client;
  //strHost.toCharArray(host, strHost.length());
  if (client.connect(strHost, 80)) {
    Serial.println("Cliente conectado!");
    String datos = "accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida);
    client.print(String("POST ") + strUrl + " HTTP/1.1" + "\r\n" +
                 "Host: " + strHost + "\n\r" +
                 "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                 "Content-Length: " + datos.length() + "\r\n" +
                 "\r\n" + datos);
    delay(10);
    Serial.println("Enviando al servidor...");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Cliente fuera de tiempo!");
        client.stop();
        return;
      }
    }
  } else {
    Serial.println("Error al conectar el cliente!");
  }
  client.stop();
}
