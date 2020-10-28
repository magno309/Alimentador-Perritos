#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid     = "ARRIS-A692";
const char* password = "3AAAA137575F5418";

String nombreServidor = "http://fida-mil.somee.com/ESPToDB.aspx";

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
  String datos_a_enviar = "accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida);
  http.begin(nombreServidor);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int codigo_respuesta = http.POST(datos_a_enviar);
  if (codigo_respuesta > 0) {
    Serial.println("Respuesta del servidor: " + codigo_respuesta);
    if (codigo_respuesta == 200) {
      String cuerpo_respuesta = http.getString();
      Serial.println("Respuesta del servidor: " + cuerpo_respuesta);
    } else {
      Serial.println("Error con código " + codigo_respuesta);
    }
    http.end();
  }
}

void consultarDespacho() {
  HTTPClient http;
  String accion = nombreServidor + "?accion=consultarActivados&idDisp=" + String(idDispensador);
  String lectura = httpGETRequest(accion);
  Serial.println(lectura);
  JSONVar objeto = JSON.parse(lectura);

  if (JSON.typeof(objeto) == "undefined") {
    Serial.println("Error al parsear entrada!");
    return;
  }

  Serial.print("Objeto JSON = ");
  Serial.println(objeto);
}

String httpGETRequest(String serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
