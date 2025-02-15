import machine
import time

led_pin = machine.Pin(5, machine.Pin.OUT)

while True:
    led_pin.value(1)
    time.sleep(2)
    led_pin.value(0)
    time.sleep(2)