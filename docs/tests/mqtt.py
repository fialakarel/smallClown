#!/usr/bin/python3

import paho.mqtt.client as mqtt
from time import sleep
from datetime import datetime


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    # client.subscribe("$SYS/#")
    # client.subscribe("/devices/ESP8266/debug")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(str(datetime.now()) + " " + msg.topic + " " + str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.82", 1883, 60)
client.loop_start()
#client.publish("/devices/ESP8266/command", payload="test", qos=0, retain=False)

while True:
    print("\n\n\n")
    print(str(datetime.now()) + " Sending ...")
    client.publish("/devices/5C:CF:7F:D6:55:D6/command",
                   payload='{ "cmd": "test", "test": "ok"}', qos=0, retain=False)
    print(str(datetime.now()) + " Sent ...")
    sleep(1)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
# client.loop_forever()
