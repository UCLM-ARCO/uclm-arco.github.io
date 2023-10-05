#!/usr/bin/python3

from mijia import LumiGateway


class ListDevices:
    def __init__(self):
        # create the gateway
        gw = LumiGateway()
        gw.start()

        for dev in gw.get_devices().values():
            print(dev)


if __name__ == "__main__":
    ListDevices()
