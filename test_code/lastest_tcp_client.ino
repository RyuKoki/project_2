#include <WiFi.h>      // import library

const char* ssid = "crwu";         //ค่าของ wifi (SSID ของตัวเอง)
const char* password = "ry123456"; //ค่าของ wifi (password ของตัวเอง)

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); //try to connect wifi
   
  while (WiFi.status() != WL_CONNECTED){ //รอการเชื่อมต่อ
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); //แสดงหมายเลข IP ของเรา
  
  Serial.println("Connect to TCP Server");
  while (!client.connect("192.168.43.23",8000)){ //เชื่อมต่อกับ tcp Server
    Serial.print(".");
    delay(500);
  } 
  Serial.println("Connecting Success"); 
}


void loop() {
  while(client.available()){
    uint8_t  data = client.read();
    Serial.write(data);
  }
  if(Serial.available()){
    client.print(Serial.readString());
    
  }
    delay(1000);
}
