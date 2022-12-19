/* sender.ino
 * reference: https://randomnerdtutorials.com/esp-now-many-to-one-esp32/
 */

#include <esp_now.h>
#include <WiFi.h>

// structure for sending data
typedef struct struct_msg {
  int id; // unique integer
  int x;
  int y;
} struct_msg;
struct_msg myData;

// structure for readings from each board
struct_msg board1;
struct_msg board2;
struct_msg board3;

// array with all board
struct_msg boardsStruct[3] = {board1, board2, board3};

// callback function when the data is received
void onDataRecv( const uint8_t * mac_addr, const uint8_t *incomingData, int len ) {
  char macStr[ 18 ];
  Serial.print( "Packet received from: " );
  snprintf( macStr, sizeof( macStr ), "%02x:%02x:%02x:%02x:%02x:%02x", 
            mac_addr[ 0 ], mac_addr[ 1 ], mac_addr[ 2 ], mac_addr[ 3 ], mac_addr[ 4 ], mac_addr[ 5 ]  );
  Serial.println( macStr );
  memcpy( &myData, incomingData, sizeof(myData) );
  Serial.printf( "Board ID %u: %u bytes\n", myData.id, len );
  // Update the structures with the new incoming data
  boardsStruct[ myData.id-1 ].x = myData.x;
  boardsStruct[ myData.id-1 ].y = myData.y;
  Serial.printf( "x value: %d \n", boardsStruct[myData.id-1].x );
  Serial.printf( "y value: %d \n", boardsStruct[myData.id-1].y );
  Serial.println();
}

void setup() {
  Serial.begin( 115200 );   // baud rate
  WiFi.mode( WIFI_STA );    // wifi station mode

  // initialize ESP-NOW
  if ( esp_now_init() != ESP_OK ) {
    Serial.println( "Error initializing ESP-NOW" );
    return;
  }

  // get recv packer info
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // access the variables for each board
  int board1x = boardsStruct[0].x;
  int board1y = boardsStruct[0].y;
  delay( 10000 );
}
