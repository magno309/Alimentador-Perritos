#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

  // Credenciales de Internet
const char* ssid     = "INFINITUM4Y4F_2.4";
const char* password = "FuckoffimbetterthanU$8";

  // Cosas del dominio            
const char* server = "fida-mil.somee.com";

  //Otras cosas
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED and contconexion < 50){
    ++contconexion;
    delay(500);
    Serial.print(.);
  }
  if(contconexion < 50)
    Serial.println("\nWiFi Conectado :D");
  else
    Serial.println("\nError de conexion :c");
}

void loop() {
  delay(20000); // 20seg

  float peso = 7.8;
  enviarPeso(peso);
  

}

void enviarPeso(float peso){
  WiFiClient cliente;
  if(cliente.connect(server, 80)){
    Serial.print("Conectando al cliente");
    int idDisp = 1;   // Este es el identificador del dispositivo
    String url = "ESPToDB.aspx?accion=actCont&idDisp="+ idDisp +"&pesoAct=" + peso;
    cliente.print(url);
    delay(1000);
  }
  cliente.stop();
}




