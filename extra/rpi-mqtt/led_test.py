#!/usr/bin/env python3
# LED wired to pin 18

import RPi.GPIO as GPIO
import time

led_pin = 18
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(led_pin, GPIO.OUT)

for i in range(10):
    print("on")
    GPIO.output(led_pin, GPIO.HIGH)
    time.sleep(1)
    print("off")
    GPIO.output(led_pin, GPIO.LOW)
    time.sleep(1)
