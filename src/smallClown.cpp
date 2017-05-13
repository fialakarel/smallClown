#include <Arduino.h>

#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DallasTemperature.h>

// include own libs
#include "ktLibs/ktOTA.cpp"
#include "ktLibs/ktBLED.cpp"
#include "ktLibs/ktWiFi.cpp"
#include "ktLibs/ktDebug.cpp"
//#include "ktLibs/ktMQTT.cpp"

#define KEEPALIVE_LED_INTERVAL 10000

// WiFi settings
const char* ssid = "ssid";
const char* password = "pass";
String MAC_ADDR = WiFi.macAddress();

// MQTT server
const char* mqtt_server = "192.168.1.1";
int mqtt_port = 1883;
const char* mqtt_clientID = &MAC_ADDR[0];   // set mqtt_clientID to MAC

// Channels
String statuschannel_temp = "/devices/" + MAC_ADDR + "/status";     // build string
const char* statusChannel = &statuschannel_temp[0];
String commandchannel_temp = "/devices/" + MAC_ADDR + "/command";   // build string
const char* commandChannel = &commandchannel_temp[0];

// other variables
long lastBlink = millis();

// system variables
int serial_speed = 115200;
ktBLED led = ktBLED();
ktDebug debug = ktDebug();

// OTA variables
int ota_port = 8266;                        // Port defaults to 8266
const char* ota_hostname = &MAC_ADDR[0];    // Hostname defaults to esp8266-[ChipID]
const char* ota_password = "salvatore";     // Password for OTA updates

// classes
WiFiClient espClient;
PubSubClient client(espClient);


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[");
  Serial.print(topic);
  Serial.print("] -> [");
  Serial.print((const char*)payload);
  Serial.println("]");

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject((const char*) payload);   // parse JSON
  if (!root.success()) {                                        // Test if parsing succeeds.
    Serial.println("parseObject() failed");
  }

  Serial.print("cmd: ");
  Serial.println((const char*)root["cmd"]);

  Serial.println();
  led.blink(1000);
}

void mqtt_reconnect() {
  Serial.print("Attempting MQTT connection ... ");
  // Attempt to connect
  if (client.connect(mqtt_clientID, statusChannel, 0, true, "offline")) {
    Serial.println("[connected]");
    // Once connected, publish an announcement...
    client.publish(statusChannel, "online", true);
    // ... and resubscribe
    client.subscribe(commandChannel);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" waiting for 0.5 seconds");
    delay(500);
  }
}

void logic_readtemp() {
  OneWire  oneWire(5);  // on pin 5 (a 4.7K resistor is necessary)
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  const char* ch = "/temperature/bedroom";
  char charVal[10];
  dtostrf(temp, 8, 2, charVal); //
  client.publish(ch, charVal);
  //Serial.println(temp);
}

void alive() {
  //blink on-board led to signalize that the board is alive
  if ((millis() - lastBlink) > KEEPALIVE_LED_INTERVAL) {
    //led.blink(10);
    lastBlink = millis();
    logic_readtemp();
  }
}

void loop_handler() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    mqtt_reconnect();
  }
  client.loop();
  alive();
}

void pir() {
  pinMode(4, INPUT);
  int state = digitalRead(4);
  Serial.println(state);
  delay(500);
}

void relay() {
  pinMode(0, OUTPUT);
  Serial.println("HIGH");
  digitalWrite(0, HIGH);
  delay(2000);
  Serial.println("LOW");
  digitalWrite(0, LOW);
  delay(2000);
}

void relay_on_pir() {
  pinMode(4, INPUT);
  int state = digitalRead(4);
  Serial.println(state);
  pinMode(0, OUTPUT);
  digitalWrite(0, state);
  delay(10);
}

void l() {
  pinMode(4, OUTPUT);
   for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(4, fadeValue);         
    delay(100);                            
  } 
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    analogWrite(4, fadeValue);         
    delay(100);
  } 
}


void setup_mqtt() {
  Serial.print("Setting up MQTT ...");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
  Serial.println(" [DONE]");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  led.on();
  Serial.begin(serial_speed);
  Serial.println();              //Clear some garbage that may be printed to the serial console
  Serial.print("\n\n\rBooting ");
  ktWiFi(ssid, password);
  ktOTA(ota_hostname, ota_password, ota_port);    // setup OTA updates
  setup_mqtt();
  debug.startup(MAC_ADDR, mqtt_server, mqtt_port, statusChannel, commandChannel);
  lastBlink = millis();
  Serial.println("\n\n\rReady");
  led.off();
}


void loop() {
  loop_handler();
  //pir();
  //relay();
  //relay_on_pir();
  //l();
}



