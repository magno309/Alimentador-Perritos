#include <WiFi.h>

// Credenciales de Internet
//const char* ssid     = "INFINITUM4Y4F_2.4";
//const char* password = "FuckoffimbetterthanU$8";

const char* ssid     = "ARRIS-A692";
const char* password = "3AAAA137575F5418";

// Puerto de servidor web
WiFiServer server(80);

// HTTP Request
String header;

// Estado del LED
String output2State = "off";
String output4State = "off";

//Estado del motor
String motorState = "off"; //off,fwd,back

// Salidas LED
const int output2 = 2;
const int output4 = 4;

//Salidas Motor
const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 14;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  pinMode(output2, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  // Conectarse a Internet
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
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // INCIO DE CONTROL
            //On - Off GPIO 2
            if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, LOW);
              //On - off GPIO 4
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
            // On - off Motor (GPIO 27)
            else if (header.indexOf("GET /27/fwd") >= 0) {
              motorState = "fwd";
              Serial.println("Moving Forward");
              digitalWrite(motor1Pin1, LOW);
              digitalWrite(motor1Pin2, HIGH);
            } else if (header.indexOf("GET /27/back") >= 0) {
              Serial.println("Moving Backwards");
              motorState = "back";
              digitalWrite(motor1Pin1, HIGH);
              digitalWrite(motor1Pin2, LOW);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("Motor stopped");
              motorState = "off";
              digitalWrite(motor1Pin1, LOW);
              digitalWrite(motor1Pin2, LOW);
            }
            //FIN DE CONTROL
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Display current state, and ON/OFF buttons
            client.println("<p>GPIO 2 - State " + output2State + "</p>");
            // If the output2State is off, it displays the ON button
            if (output2State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<p>GPIO 4 - State " + output4State + "</p>");
            if (output4State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<p>Motor - State " + motorState + "</p>");
            client.println("<p><a href=\"/27/fwd\"><button class=\"button\">Motor Foward</button></a></p>");
            client.println("<p><a href=\"/27/back\"><button class=\"button\">Motor Backwards</button></a></p>");
            client.println("<p><a href=\"/27/off\"><button class=\"button\">Motor Off</button></a></p>");
            /*if (output27State == "off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">Motor ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">Motor OFF</button></a></p>");
            }*/
            //Fin de documento HTML
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
