//#include <ESP8266WiFi.h>
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(115200);
//  pinMode(LED_BUILTIN, OUTPUT);
//
//  const char* WIFI_NAME = "R_kojiko";        // Wifi's Name
//  const char* WIFI_PASS = "ryu123456";  // Wifi's Password
//  WiFi.mode( WIFI_STA );              // define WiFi in station mode
//  WiFi.begin( WIFI_NAME, WIFI_PASS ); // connect WiFi
//  // try to connect the WiFi
//  while ( WiFi.status() != WL_CONNECTED ) {
//    delay(500);
//    Serial.print(".");
//    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//  }
//  // connect WiFi successful
//  digitalWrite(LED_BUILTIN, HIGH);
//  Serial.println("");
//  Serial.println("!!!!!!!!!! WiFi Connected !!!!!!!!!!");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  WiFiClient client;
//
//  Serial.print("Connect.... ");
//  if (client.connect("192.168.88.229", 8000)) {
//    Serial.println("Connected");
//    while (client.connected()) {
//      if (client.available()) {
//        char c = client.read();
//        client.write(c);
//        Serial.write(c);
//      }
//      delay(1);
//    }
//    client.stop();
//    Serial.println();
//    Serial.println("Disconnect.");
//  } else {
//    Serial.println("fail.");
//  }
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//}

#include <ESP8266WiFi.h>
#define SERVER_PORT 8000            //ค่า port ที่ต้องการเชื่อมต่อ
IPAddress server_ip = {192,168,43,48};       //ค่า ip ของ Server (อย่าลืมแก้เป็น IP ของตัวเอง)
 
const char* ssid = "R_kojiko";         //ค่าของ SSID (อย่าลืมแก้เป็น ชื่อ SSID ของตัวเอง)
const char* password = "ryu123456"; //ค่าของ SSID (อย่าลืมแก้เป็น password ของตัวเอง)
 
WiFiServer server(SERVER_PORT); //สร้าง object server และกำหนด port ที่ต้องการเชื่อมต่อกับ server
WiFiClient client;              //สร้าง object client
 
void setup() 
{
    Serial.begin(115200);       //เปิดใช้ Serial
    WiFi.begin(ssid, password); //เชื่อมต่อกับ AP
   
     while (WiFi.status() != WL_CONNECTED)  //รอการเชื่อมต่อ
    {
            delay(500);
            Serial.print(".");
    }
   
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: "); 
    Serial.println(WiFi.localIP()); //แสดงหมายเลข IP
 
    Serial.println("Connect TCP Server");
    while (!client.connect(server_ip,SERVER_PORT)) //เชื่อมต่อกับ Server
    {
            Serial.print(".");
            delay(100);
    } 
    Serial.println("Success"); 
// ESP.wdtDisable();  //ปิด watch dog Timer
}
 
void loop() 
{
    while(client.available())   //ตรวจเช็ตว่ามี Data ส่งมาจาก Server หรือไม่
    {
            uint8_t  data =client.read();  //อ่าน Data จาก Buffer
            Serial.write(data); //แสดงผล Data ทาง Serial
    }
               client.println("Hello");     //ส่งค่าที่ได้รับกลับไปยัง Server
               delay(5000);
}
