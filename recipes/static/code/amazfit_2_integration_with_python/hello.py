#!/usr/bin/python3

from amazfit import Core2

# chip = Core2.find(timeout=10)
# if chip is None:
#     print("ERROR: no device found!")

chip = Core2(address="f1:4f:bb:6a:b5:79")

chip.connect()
