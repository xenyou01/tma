#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define echoPin 2 // Echo Pin
#define trigPin 0 // Trigger Pin
 
long duration, distance; // Duration used to calculate distance

const char* ssid = "XenYou";
const char* password = "Nngpedson1";
//const char* ssid = "DarkMachine";
//const char* password = "pedsonpro";

const char* mqtt_server = "192.168.178.56";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char buf[50];

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    //Calculate the distance (in cm)
    distance = duration/58.2;
   Serial.print("Publish Distance: ");
   sprintf(buf, "%d", distance);
   Serial.print(buf);
   if(client.publish("sensor/distance", buf)){
     Serial.println("  Success");
   }else{
     Serial.println("  Failed!!");
   }
  }
}
