#include <WiFi.h>
const char* WIFI_NAME = "Project2_eiei"; // Wifi's Name
const char* WIFI_PASS = "RyuRy@2001";    // Wifi's Password
WiFiClient wifiClient;

#include <PubSubClient.h>
const char* mqtt_server   = "192.168.8.101";  // IP of the MQTT broker
const char* ldr_topic     = "home/livingroom/ldr";
const char* mqtt_username = "project_test";
const char* mqtt_password = "project_test";
const char* clientID      = "client_livingroom";
PubSubClient client(mqtt_server, 1883, wifiClient);

#define LDR_PIN (36)

void connect_MQTT() {
  Serial.print("Conneting to ... ");
  WiFi.begin( WIFI_NAME, WIFI_PASS ); // connect WiFi
  // try to connect the WiFi
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  // connect WiFi successful
  Serial.println("");
  Serial.println("!!!!!!!!!! WiFi Connected !!!!!!!!!!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  } else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

void setup() {
  Serial.begin(115200);
  connect_MQTT();
}

void loop() {
  Serial.setTimeout(2000);

  int intensity = analogRead(LDR_PIN);
  Serial.print("LDR values >> ");
  Serial.println(intensity);

  String LDR_string = "LDR: "+String((int)intensity);
  client.publish(ldr_topic, String("From KidBright #27 "+String(intensity)).c_str());
//  if (client.publish(ldr_topic, String("From KidBright #27 "+String(intensity)).c_str())) {
//    Serial.println("LDR value sent!!!!!!!!!");  
//  } else {
//    Serial.println("Failed to send. Reconnecting to MQTT Broker and trying again");
//    client.connect(clientID, mqtt_username, mqtt_password);
//    delay(10);
//    client.publish(ldr_topic, String(intensity).c_str());
//  }
//  client.disconnect();
  delay(1000*10);
}
