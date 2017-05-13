#include <Arduino.h> 

/* ***** USAGE *****

// include 
#include "ktLibs/ktDebug.cpp"

ktDebug debug = ktDebug();

debug.startup(MAC_ADDR, mqtt_server, mqtt_port, statusChannel, commandChannel);

*/
  
class ktDebug{
    public:
	// First debug info
	void startup(String MAC_ADDR, const char* mqtt_server, int mqtt_port,
			 const char* statusChannel, const char* commandChannel) {
	  Serial.print("MAC: ");
 	  Serial.println(MAC_ADDR);
 	  Serial.print("MQTT server: ");
 	  Serial.print(mqtt_server);
 	  Serial.print(":");
 	  Serial.println(mqtt_port);
 	  Serial.print("statusChannel: ");
 	  Serial.println(statusChannel);
 	  Serial.print("commandChannel: ");
 	  Serial.println(commandChannel);
	}
};
   
