#include <PubSubClient.h>
#include <ESP8266WiFi.h>

IPAddress server(192, 168, 0, 56); // IP de la raspberry Pi

const char* ssid     = "SSID"; // Your ssid
const char* pass = "PASSWORD"; // Your Password
const char* host = "Enchufe2"; // nombre del entorno

int relay = 2;

#define BUFFER_SIZE 100

WiFiClient wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& pub) {
    if(pub.payload_string() == "on")
    {
      digitalWrite(relay, LOW); // en caso de que el modulo rele funcione al reves, cambiarl LOW por HIGH
    }
    else
    {
      digitalWrite(relay, HIGH); // en caso de que el modulo rele funcione al reves, cambiarl HIGH por LOW
    }  
}

void setup() 
{
  pinMode(relay,OUTPUT);
  digitalWrite(relay, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  client.set_callback(callback);
  if (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  return;
  Serial.println("WiFi connected");
  }
  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("ESP8266: Enchufe2")) {
        client.publish("outTopic",(String)"hello world, I'm "+host);
        client.subscribe(host+(String)"/#");
      }
    }
    if (client.connected())
      client.loop();
  }  
}
