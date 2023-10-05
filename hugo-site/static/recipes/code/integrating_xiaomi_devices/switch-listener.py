#!/usr/bin/python3

from mijia import LumiGateway


class SwitchListener:
    def __init__(self):
        # create the gateway
        gw = LumiGateway()
        gw.start()

        # get the switch device
        devices = gw.get_devices(filter="switch")
        switch = list(devices.values())[0]
        switch.on_click(self.on_click_event)

        # wait for incomming events
        gw.wait_until_break()

    def on_click_event(self, event, dev):
        print("- event: {}, dev: {}".format(event, dev))


if __name__ == "__main__":
    SwitchListener()
