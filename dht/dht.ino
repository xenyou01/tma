#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
//#include <DHT11.h>

#define dhtPin 2
#define type DHT11

DHT dht(dhtPin, type);
 
long duration, distance;

//const char* ssid = "DarkMachine";
//const char* password = "pedsonpro";
const char* ssid = "Meno Soap 7515";
const char* password = "#Xenyou01091993;";
const char* mqtt_server = "192.168.178.56";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char buf1[50];
char buf2[50];

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}
void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float hu = dht.readHumidity();
    if(isnan(hu)){
      Serial.println("Failed to read the Humidity");
    }else{
      int h = static_cast<int>(hu);
      Serial.print("Publish Humidity: ");
      sprintf(buf1, "%d", h);
      Serial.print(buf1);
      if(client.publish("sensor/humidity", buf1)){
        Serial.println("  Success");
      }else{
        Serial.println("  Failed!!");
      }
    }
    float temp = dht.readTemperature();
    if(isnan(temp)){
      Serial.println("Failed to read the Temperature");
    }else{
      int t = static_cast<int>(temp);
      Serial.print("Publish Temperature: ");
      sprintf (buf2, "%d", t);
      Serial.print(buf2);
      if(client.publish("sensor/temperature", buf2)){
        Serial.println("  Success");
      }else{
        Serial.println("  Failed!!");
      }
    }
  }
}
