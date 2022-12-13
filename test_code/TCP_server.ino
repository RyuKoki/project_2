#include<ESP8266WiFi.h>

//#define WIFI_STA_NAME "crwu"
//#define WIFI_STA_PASS "ry123456"

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  const char* WIFI_NAME = "R_kojiko";        // Wifi's Name
  const char* WIFI_PASS = "ryu123456";  // Wifi's Password
  WiFi.mode( WIFI_STA );              // define WiFi in station mode
  WiFi.begin( WIFI_NAME, WIFI_PASS ); // connect WiFi
  // try to connect the WiFi
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(1000);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  // connect WiFi successful
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("!!!!!!!!!! WiFi Connected !!!!!!!!!!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      delay(1);
    }
    client.stop();
    Serial.println("client disonnected");
  }
  delay(10);
}

//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(115200);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  Serial.println("Hello World");
//  delay(1000);
//}
