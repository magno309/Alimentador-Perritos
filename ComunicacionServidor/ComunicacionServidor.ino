#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid     = "ARRIS-A692";
const char* password = "3AAAA137575F5418";

const char* nombreServidor = "http://fida-mil.somee.com/ESPToDB.aspx";

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

void loop() {
  leerCantidadComida();
  if (WiFi.status() == WL_CONNECTED) {
    enviarCantidadComida();
    //consultarDespacho();
  } else {
    Serial.println("Sin conexión a Internet!");
  }
  delay(10000);
}


void leerCantidadComida() {
  cantidadComida = 1002;
}

void enviarCantidadComida() {
  HTTPClient http;
  String datos_a_enviar = "?accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida);
  http.begin(nombreServidor);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int codigo_respuesta = http.POST(datos_a_enviar);
  Serial.println("Respuesta del servidor: " + String(codigo_respuesta));
  if (codigo_respuesta > 0) {
    if (codigo_respuesta == 200) {
      //String cuerpo_respuesta = http.getString();
      //Serial.println("Respuesta del servidor: " + cuerpo_respuesta);
    } else {
      Serial.println("Error con código " + codigo_respuesta);
    }
  }
  http.end();
  /*WiFiClient cliente;
    if (cliente.connect(nombreServidor, 80)) {
    Serial.println("Cliente conectado!");
    String postStr = "?accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida);
    postStr += "\r\n\r\n";
    cliente.print("POST /ESPToDB.aspx HTTP/1.1\n");
    cliente.print("Host: http://fida-mil.somee.com\n");
    cliente.print("accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida) + "\n");
    cliente.print("Content-Type: application/x-www-form-urlencoded\n");
    cliente.print("Content-Length: ");
    cliente.print(postStr.length());
    cliente.print("\n\n");
    cliente.print(postStr);
    delay(1000);
    }
    cliente.stop();*/
}
