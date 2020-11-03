#include <WiFi.h>
#include <WiFiClient.h>
//#include <HTTPClient.h>

const char* ssid     = "ARRIS-A692";
const char* password = "3AAAA137575F5418";

//const char* ssid     = "PUERTASELECTRICAS";
//const char* password = "NNsFKa4RN5";

//char host[50];
//char strHost[] = "http://fida-mil.somee.com/ESPToDB.aspx";
//char strUrl[] = "/ESPToDB.aspx";

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
float cantidadComida;

unsigned long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  leerCantidadComida();
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      //enviarCantidadComida();
      consultarActivados();
    } else {
      Serial.println("Sin conexión a Internet!");
    }
  }
}


void leerCantidadComida() {
  cantidadComida = 154331;
}

/*void enviarCantidadComida() {
  //Usando HTTPClient.h
  HTTPClient http;
  http.begin("http://fida-mil.somee.com/ESPToDB.aspx?accion=actCont&idDisp=" + String(idDispensador) + "&pesoAct=" + String(cantidadComida));
  int httpResponseCode = http.GET();
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  if (httpResponseCode > 0) {
    if (httpResponseCode == 200) {
      // Todo salió bien
      Serial.println("Comida en el dispensador: " + String(cantidadComida) + "Kg.");
    }
  }
  http.end();
  }*/

void consultarActivados() {
  //Usando HTTPClient.h
  /*HTTPClient http;
    http.begin("http://fida-mil.somee.com/consultas.asmx?op=dActivos");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST("");
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
    if (httpResponseCode == 200) {
      // Todo salió bien
      Serial.println(http.getString());
    }
    }
    http.end();*/
  //Usando WiFiClient.h
  WiFiClient client;
  /*
  if (client.connect("http://fida-mil.somee.com/consultas.asmx", 80)) {
    client.println("GET ?op=dActivos HTTP/1.0");
    client.println();
    while (client.available()) {
      Serial.println(client.readStringUntil('\r'));
    }
  } else {
    Serial.println("Fallo en la conexión!");
  }*/
  //INSERTADO POR ISABEL .-. 
  
  if(client.connect("fida-mil.somee.com", 80)){
    client.println(F("POST /consultas.asmx HTTP/1.1"));
    client.println(F("Host: fida-mil.somee.com"));
    client.println(F("Content-Type: application/soap+xml; charset=utf-8"));
    client.println(F("Content-Length: 500")); // No esoty segura el valor del content-lenght
    //client.println(F("Connection: close")); // Creo que esto no va
    //client.println(F("SOAPAction: \"http://toponet.zapto.org/logactivation\""));  // ni esto
    client.println();
    client.println(F("<?xml version="1.0" encoding="utf-8"?>"));
    client.println(F("<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">"));
    client.println(F("<soap12:Body>"));
    client.println(F("<dActivos xmlns="http://tempuri.org/">"));
    client.println(F("<idDispensador>string</idDispensador>"));
    client.println(F("</dActivos>"));
    client.println(F("</soap12:Body>"));
    client.println(F("</soap12:Envelope>"));
  } else {
  }
  
}
