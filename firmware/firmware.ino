//#include <configfile.h>

#include <WebSocketsClient.h>  // download and install from https://github.com/Links2004/arduinoWebSockets
#include <SocketIOclient.h>
#include <HTTPClient.h>
#include "wifiset.h"



SocketIOclient socketIO;


void messageHandler(uint8_t* payload) {
  StaticJsonDocument<64> doc;
  DeserializationError error = deserializeJson(doc, payload);
 
  if (error) {
    Serial.println(error.f_str());
    return;
  }

  String messageKey = doc[0];
  bool value = doc[1];
  
  if (messageKey == "buttonState1")   
  {
    digitalWrite(12, value);
  }
  if (messageKey == "buttonState2")   
  {
    digitalWrite(14, value);
  }

}




void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.println("Disconnected!");
      break;

    case sIOtype_CONNECT:
      Serial.printf("Connected to url: %s%s\n", SERVER, payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;

    case sIOtype_EVENT:
      
      for (int i=0; i<length; i++){
      Serial.print((char) payload[i]);
      }
      messageHandler(payload);

      break;
  }
}


void setup() {
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);

  Serial.begin(9600);

  System_init();

  // server address, port and URL
  socketIO.begin(SERVER, 4001, "/socket.io/?EIO=4");

  socketIO.onEvent(socketIOEvent); 
}

void loop() {
  socketIO.loop();
}