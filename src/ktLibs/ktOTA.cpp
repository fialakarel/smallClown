#ifndef HEADER_KTOTA

#define HEADER_KTOTA
 
#include <ArduinoOTA.h>

/* ***** USAGE *****

// include 
#include "ktLibs/ktOTA.cpp"

// in setup loop
ktOTA(ota_hostname, ota_password, ota_port);    // setup OTA updates

// in loop
ArduinoOTA.handle();	// handle updates
*/
  
class ktOTA{
    public:
	ktOTA(const char* ota_hostname, const char* ota_password, int ota_port) {
	  Serial.print("Setting up OTA ...");
	  ArduinoOTA.setPort(ota_port);
	  ArduinoOTA.setHostname(ota_hostname);
	  ArduinoOTA.setPassword(ota_password);
	  ArduinoOTA.onStart([]() {
	    Serial.println("\n\n\n\n\rStart");
	  });
	  ArduinoOTA.onEnd([]() {
	    Serial.println("\nEnd");
	  });
	  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	    //blinkLed(100);
	  });
	  ArduinoOTA.onError([](ota_error_t error) {
	    Serial.printf("Error[%u]: ", error);
	    if      (error == OTA_AUTH_ERROR)     { Serial.println("Auth Failed");    }
	    else if (error == OTA_BEGIN_ERROR)    { Serial.println("Begin Failed");   }
	    else if (error == OTA_CONNECT_ERROR)  { Serial.println("Connect Failed"); }
	    else if (error == OTA_RECEIVE_ERROR)  { Serial.println("Receive Failed"); }
	    else if (error == OTA_END_ERROR)      { Serial.println("End Failed");     }
	  });
	  ArduinoOTA.begin();
	  Serial.println(" [DONE]");
	}
};
   
#endif


