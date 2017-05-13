#include <Arduino.h> 

/* ***** USAGE *****

// include 
#include "ktLibs/ktBLED.cpp"

ktBLED led = ktBLED();

led.on();

*/
  
class ktBLED{
    public:
	// blink LED
	void blink(int blink_delay) {
	  this->on();
	  delay(blink_delay);
	  this->off();
	}

	// builtin LED on
	void on() {
	  digitalWrite(LED_BUILTIN, LOW);
	}
	
	// builtin LED off
	void off() {
	  digitalWrite(LED_BUILTIN, HIGH);
	}
};
   
