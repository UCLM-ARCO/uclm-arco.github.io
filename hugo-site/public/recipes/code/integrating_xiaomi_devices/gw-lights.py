#!/usr/bin/python3

import sys
import time
from mijia import LumiGateway


class GwLights:
    def __init__(self, password):
        # create the gateway
        gw = LumiGateway(passwd=password)
        gw.start()

        for i in range(0, 255):
            gw.set_light_color(i, 255-i, 0, 100)
            time.sleep(0.05)

if __name__ == "__main__":
    if len(sys.argv) <= 2:
        print("Usage: {} <password>".format(sys.argv[0]))

    GwLights(sys.argv[1])
