#include <DHT.h>
#include <DHT_U.h>


#include <ESP8266WiFi.h>
#define LED1 D2 //LED WIFI
#define LED2 D3 //LED SABER QUE FUNCIONA
#define LED3 D5 //LED BASE DATOS
String ssid = "iPhone de Pili";
String password = "010117mklp";


String host = "172.20.10.7";
const int port = 80;
const int watchdog = 5000;
unsigned long previousMillis = millis();

DHT dht(5,DHT11);//clase DHT nombre_obj(pin, TipoSensor)
float temp, hum; //Declaras las variables en que guardaras la información


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED3, OUTPUT);

  //Inicia Serial
  Serial.begin(115200); //a qué velocidad nos conectamos al puerto
  Serial.println("\n");

  //Conexión wifi
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){ //Ciclo, solo se sale si se conecta a wifi o si excede el número de intentos
    delay(500);
    Serial.println(".");
  }
  Serial.println("");

  //INICIALIZA SENSOR
  dht.begin();
  
}

void loop() {
  digitalWrite(LED2,HIGH);
  
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("No conectado a WiFi");
    delay(1000);
    digitalWrite(LED1,LOW);

  }
  else{
    Serial.println("Conectado a WiFI");
    delay(1000);
    digitalWrite(LED1,HIGH);
    
  }

  //Sensor
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  
  Serial.println("Temperatura: " + String(temp));
  Serial.println("Humedad: " + String(hum) + "%");

   unsigned long currentMillis = millis();
    
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("Fallo al conectar a la base de datos");
      digitalWrite(LED3,LOW);
      return;
    }
    else{
      digitalWrite(LED3,HIGH);
    }

    String url = "/IOT_PROYECTO/index.php?temp=";
    url += temp;
    url += "&hum=";
    url += hum;
    
    // Enviamos petición al servidor
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    // Leemos la respuesta del servidor
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  
  delay(5000);

  

}
