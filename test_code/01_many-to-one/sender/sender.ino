/* sender.ino
 * reference: https://randomnerdtutorials.com/esp-now-many-to-one-esp32/
 */

#include <esp_now.h>
#include <WiFi.h>

// receiver's MAC address
// MAC address' KID BRIGHT #26
uint8_t broadcastAddress[] = {0xB4, 0xE6, 0x2D, 0xA0, 0x2B, 0x85};

// structure for sending data
typedef struct struct_msg {
  int id; // unique integer
  int x;
  int y;
} struct_msg;
struct_msg myData;

// peer interface
esp_now_peer_info_t peerInfo;

// callback function when the data is sent
void onDataSent( const uint8_t *mac_addr, esp_now_send_status_t status ) {
  Serial.print( "\r\nLast Packet Send Status:\t" );
  Serial.println( status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail" );
}

void setup() {
  Serial.begin( 115200 );   // baud rate
  WiFi.mode( WIFI_STA );    // wifi station mode

  // initialize ESP-NOW
  if ( esp_now_init() != ESP_OK ) {
    Serial.println( "Error initializing ESP-NOW" );
    return;
  }

  // if ESP-NOW is successfully, show up the status's data
  esp_now_register_send_cb( onDataSent );

  // register peer
  memcpy( peerInfo.peer_addr, broadcastAddress, 6 );
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // connecting with target communication device or peer
  if ( esp_now_add_peer( &peerInfo ) != ESP_OK ){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // random data
  myData.id = 1;
  myData.x  = random( 0, 50 );
  myData.y  = random( 0, 50 );

  // send message using ESP-NOW
  esp_err_t result = esp_now_send( broadcastAddress, (uint8_t *) &myData, sizeof(myData) );

  // detecting status's sending message
  if ( result == ESP_OK ) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay( 10000 );
}
