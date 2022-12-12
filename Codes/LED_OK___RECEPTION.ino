/*
 *  ESP8266 NodeMCU MQTT Example
 *  https://circuits4you.com
 *  -Manoj R. Thakur
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED 16
 
//Enter your wifi credentials
const char* ssid = "Android";  
const char* password = "bui12345";

//Enter your mqtt server configurations
const char* mqttServer = "192.168.88.167";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
const char* mqttUser = "pi"; //User
const char* mqttPassword = "pi"; //Password
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  delay(1000);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);
 
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
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
 
}
 
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
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
  
 int ledState = digitalRead(LED);
 
    // if the state has changed, increment the counter
    if (ledState == HIGH) {
      client.publish("esp/state", "La LED est allumée"); //
      // if the current state is HIGH then the button
      // went from off to on:
    } else {
      // if the current state is LOW then the button
      // went from on to off:
      client.publish("esp/state", "La LED est éteinte"); //
    }

  Serial.println();
  Serial.println("-----------------------");  
}

 
void loop() {
  client.loop();
}
