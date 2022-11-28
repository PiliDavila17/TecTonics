// Codigo que se utilizó para comprobar la conexion del NodeMCU 
// tomado del manual NodeMCU

#include <ESP8266WiFi.h>

const char* ssid = "CAMBIAR POR EL SSID";
const char* password = "CAMBIAR POR LA CONTRASEÑA";


int led = 0; //
WiFiServer server (80);
void setup () {
Serial.begin (115200);
delay (10);
pinMode (led, OUTPUT) ;
digitalWrite (led, LOW);
Serial.println ();
Serial.println ();
Serial.print ("Connecting to ");
Serial.println (ssid);
WiFi.begin (ssid, password);
while (WiFi.status () != WL_CONNECTED) {
delay (500);
Serial.print (".");
}
Serial.println ("");
Serial.println ("WiFi connected");
server.begin () ;
Serial.println ("Server started");
Serial.print ("Use this URL to connect: ");
Serial.print ("http://");
Serial.print (WiFi.localIP());
Serial.println ("/");
}
void loop () {
WiFiClient client = server.available() ;
if (!client) {
  return;
  }
Serial.println("new client");
while(!client.available()){
  delay(1);
}
String request = client. readStringUntil ('\r');
Serial.println (request);
client. flush ();
int value = LOW;
if (request.indexOf ("/LED-ON") != -1) {
digitalWrite (led, HIGH);
value = HIGH;
}
if (request.indexOf ("/LED-OFF") != -1) {
 digitalWrite (led, LOW);
value= LOW;
 }

client.println ("TTP/1.1 200 OK");
client.println ("Content-Type: text/html");
 client.println("");
 client.println ("<! DOCTYPE HTML>");
 client.println ("<html>");

 client.print ("Led Status: ");

if (value == HIGH){
client.print ("ON");
}
else {
client.print ("OFF");
}
client.println("");
client.println ("<a href=\"/LED-ON\"\"›<button>LED On </button></a>");
client.println("<a href=\"/LED-OFF\"\"><button>LED Off </button></a>");
client.println("</html>");

delay (1);
Serial.println ("Client disonnected");
Serial.println("");
}
