#!/usr/bin/env python

# Measure illuminance (in lux) and RGB levels
# Raspberry Pi with Pimorini Enviro Phat
# https://learn.pimoroni.com/tutorial/sandyj/getting-started-with-enviro-phat

import paho.mqtt.client as mqtt
from envirophat import light
from envirophat import leds 
from time import sleep

device_id="raspberrypi"

def on_connect(client, userdata, flags, rc):
    print("Connected ", str(rc));
    client.publish("presence/connected/" + device_id, device_id)
    client.subscribe("itp/" + device_id + "/led")

# note: when the LED on, it affects the light sensor readings
def on_message(client, userdata, msg):
    message = str(msg.payload).lower()
    if message == "on":
        leds.on() 
    elif message == "off":
        leds.off()

client = mqtt.Client(device_id)
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("dev2db", "public")
client.tls_set()
client.connect("dev2db.cloud.shiftr.io", 8883, 60)
client.loop_start()

while True:
    illuminance = light.light()
    red, green, blue = light.rgb()
    color = "#%02x%02x%02x" % (red, green, blue)
    print(red, green, blue, color)
    client.publish("itp/" + device_id + "/red", red)
    client.publish("itp/" + device_id + "/green", green)
    client.publish("itp/" + device_id + "/blue", blue)
    client.publish("itp/" + device_id + "/color", color)
    client.publish("itp/" + device_id + "/illuminance", illuminance)
    sleep(20)


