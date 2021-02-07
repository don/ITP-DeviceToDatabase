#!/usr/bin/env python3

# Raspberry Pi with 
#  * DHT22 wired to pin 4
#  * LED wired to pin 18
# For MQTT see https://pypi.org/project/paho-mqtt/#usage-and-api

import paho.mqtt.client as mqtt
from time import sleep
import board
import adafruit_dht
import RPi.GPIO as GPIO
import config

led_pin = 18
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(led_pin, GPIO.OUT)

# Initial the dht device, with data pin connected to:
dhtDevice = adafruit_dht.DHT22(board.D4)
# dhtDevice = adafruit_dht.DHT22(board.D4, use_pulseio=False)

def on_connect(client, userdata, flags, result_code):
    print("Connected ", str(result_code))  # expecting 0
    client.publish(f"presence/connected/{config.device_id}", config.device_id)
    client.subscribe(f"itp/{config.device_id}/led")

def on_message(client, userdata, msg):
    message = msg.payload.decode('utf-8').lower()
    print(message)
    if message == "on":
        GPIO.output(led_pin, GPIO.HIGH)
    elif message == "off":
        GPIO.output(led_pin, GPIO.LOW)

client = mqtt.Client(config.device_id)
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(config.mqtt_user, config.mqtt_password)
client.tls_set()
client.connect(config.mqtt_broker, config.mqtt_port, keepalive=60)

client.loop_start()

while True:
    try:
        temperature_c = dhtDevice.temperature
        temperature_f = temperature_c * (9 / 5) + 32
        humidity = dhtDevice.humidity
        print(
            "Temp: {:.1f} F / {:.1f} C    Humidity: {}% ".format(
                temperature_f, temperature_c, humidity
            )
        )    
        
        # format values to 1 decimal place while publishing
        client.publish(f"itp/{config.device_id}/temperature", f"{temperature_f:.1f}")
        client.publish(f"itp/{config.device_id}/humidity", f"{humidity:.1f}")

    except RuntimeError as error:
        # Errors happen fairly often, DHT's are hard to read, just keep going
        print(error.args[0])
    except Exception as error:
        dhtDevice.exit()
        raise error

    sleep(20)
