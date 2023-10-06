#!/usr/bin/python3

import time
from amazfit import Core2

chip = Core2.find(timeout=10)
if chip is None:
    print("ERROR: no device found!")

chip.connect()
chip.pair()

chip.read_info()
print(
    f"Device Information:\n"
    f"- name: {chip.name}\n"
    f"- address: {chip.address}\n"
    f"- serial number: {chip.serial_number}\n"
    f"- hardware version: {chip.hardware_rev}\n"
    f"- software version: {chip.software_rev}"
)

print(f"- steps: {chip.steps}")

def on_steps_callback(steps):
    print(f"- steps changed, new value: {steps}")

chip.on_steps(on_steps_callback)

print(f"- axis: {chip.orientation}")

def on_orientation_callback(axis):
    print(f"- orientation changed, new value: {axis}")

chip.on_orientation(on_orientation_callback)

while True:
    try:
        chip.fire_orientation_read()
        time.sleep(2)
    except KeyboardInterrupt:
        break


