/*
 *  ESP8266 NodeMCU MQTT Example
 *  https://circuits4you.com
 *  -Manoj R. Thakur
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
    
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
// Temperature value
float temp;

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

#define LED LED_BUILTIN

#define bouton 13

//Enter your wifi credentials
const char* ssid = "JulLeSang";  
const char* password = "12345679";

//Enter your mqtt server configurations
const char* mqttServer = "192.168.2.150";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
const char* mqttUser = "BUI"; //User
const char* mqttPassword = "pi"; //Password
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  delay(1000);
  pinMode(LED,OUTPUT);
  pinMode(bouton,INPUT);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    //if (client.connect("ESP8266", mqttUser, mqttPassword )) {
    if (client.connect("ESP8266")) {
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
 
  client.publish("esp/DBJ/prise2/AE", "Hello from ESP8266");
  client.subscribe("esp/DBJ/prise2/AE");
  client.subscribe("esp/DBJ/prises/AE");
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
   Serial.print(message);
  if(message == "off") {digitalWrite(LED,LOW);}   //LED off
  if(message == "on") {digitalWrite(LED,HIGH);} //LED on

 client.subscribe("esp/DBJ/prises/AE");
   if(message == "alloff") {digitalWrite(LED,LOW);}   //LED off
   if(message == "allon") {digitalWrite(LED,HIGH);} //LED on

 int ledState = digitalRead(LED);
 int buttonState = digitalRead(bouton);
 
    // if the state has changed, increment the counter
    if (ledState == HIGH) {
      client.publish("esp/DBJ/prise1/etat", "La LED est éteinte"); //
      // if the current state is HIGH then the button
      // went from off to on:
    } else {
      // if the current state is LOW then the button
      // went from on to off:
      client.publish("esp/DBJ/prise1/etat", "La LED est allumée"); //
    }
        // if the state has changed, increment the counter
  
  Serial.println();
  Serial.println("-----------------------");  
}

void loop() {
}
}
