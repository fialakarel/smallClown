#ifndef HEADER_KTWIFI

#define HEADER_KTWIFI
 
/* ***** USAGE *****

// include 
#include "ktLibs/ktWiFi.cpp"

// in setup loop
ktWiFi(ssid, password);    // setup WiFI
*/
  
class ktWiFi{
    public:
	ktWiFi(const char* ssid, const char* password) {
		  WiFi.mode(WIFI_STA);
		  WiFi.begin(ssid, password);
		  while (WiFi.status() != WL_CONNECTED) {
		    delay(500);
		    Serial.print(".");
		  }
		  Serial.print(" [DONE]\n\rWiFi online -- IP address: ");
		  Serial.println(WiFi.localIP());
	}
};
   
#endif


