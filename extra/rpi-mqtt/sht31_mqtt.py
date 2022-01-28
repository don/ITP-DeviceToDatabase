#!/usr/bin/env python3

# Raspberry Pi with 
#  * SHT31 wired to SDA and SLC
#  * LED wired to pin 18
# For MQTT see https://pypi.org/project/paho-mqtt/#usage-and-api
# SHT-31 data sheet https://www.mouser.com/datasheet/2/682/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital-971521.pdf


import paho.mqtt.client as mqtt
from time import sleep
import smbus
import RPi.GPIO as GPIO
import config

led_pin = 18
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(led_pin, GPIO.OUT)

# Get I2C bus
bus = smbus.SMBus(1)
  
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

    # SHT31 address, 0x44
    bus.write_i2c_block_data(0x44, 0x2C, [0x06])

    # SHT31 address, 0x44(68)
    # Read data back from 0x00(00), 6 bytes
    # Temp MSB, Temp LSB, Temp CRC, Humididty MSB, Humidity LSB, Humidity CRC
    data = bus.read_i2c_block_data(0x44, 0x00, 6)

    # Convert the data
    temp = data[0] * 256 + data[1]
    temperature_c = -45 + (175 * temp / 65535.0)
    temperature_f = -49 + (315 * temp / 65535.0)
    humidity = 100 * (data[3] * 256 + data[4]) / 65535.0
    
    print(
        "Temp: {:.1f} F / {:.1f} C    Humidity: {:.1f}% ".format(
            temperature_f, temperature_c, humidity
        )
    )    
        
    # format values to 1 decimal place while publishing
    client.publish(f"itp/{config.device_id}/temperature", f"{temperature_f:.1f}")
    client.publish(f"itp/{config.device_id}/humidity", f"{humidity:.1f}")

    sleep(20)
