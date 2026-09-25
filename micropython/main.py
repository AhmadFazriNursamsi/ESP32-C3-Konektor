from machine import Pin
from time import sleep

# ESP32-C3 onboard LED (GPIO 8 pada board yang menggunakan LED di GPIO8)
led = Pin(8, Pin.OUT)

while True:
    led.value(1)
    print("LED ON")
    sleep(1)

    led.value(0)
    print("LED OFF")
    sleep(1)
