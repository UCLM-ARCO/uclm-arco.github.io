#!/usr/bin/python3

import sys
import Ice
import time
from datetime import datetime, timedelta

Ice.loadSlice("clock.ice -I. --all")
import st
import recipe


class ClockI(recipe.Clock):
    def __init__(self):
        self.observer = None
        self.set(time.strftime("%H:%M:%S"))

    def set(self, value, source=None, current=None):
        self.seconds = sum(
            x * int(t) for x, t in zip([3600, 60, 1], value.split(":"))
        )
        if source is not None:
            print("time set to '{}' by {}".format(value, source))

    def linkto(self, observer, current):
        ic = current.adapter.getCommunicator()
        prx = ic.stringToProxy(observer)
        self.observer = st.IStringPrx.uncheckedCast(prx)
        print("new observer set: '{}'".format(str(prx)))

    def publish_time(self, elapsed):
        if self.observer is None:
            return

        self.seconds += elapsed
        dt = datetime(1, 1, 1) + timedelta(seconds=self.seconds)
        now = dt.strftime("%H:%M:%S")

        self.observer.set(now, "clock")
        print("publish time: '{}'".format(now))


class ClockServer(Ice.Application):
    def run(self, args):
        ic = self.communicator()
        adapter = ic.createObjectAdapterWithEndpoints(
            "Adapter", "tcp -h 127.0.0.1 -p 1234"
        )
        adapter.activate()

        servant = ClockI()
        proxy = adapter.add(servant, ic.stringToIdentity("Clock"))
        print("proxy ready: '{}'".format(proxy))

        last = time.time()
        while True:
            try:
                time.sleep(5)
                elapsed = int(time.time() - last)
                last = time.time()
                servant.publish_time(elapsed)
            except KeyboardInterrupt:
                break


if __name__ == "__main__":
    ClockServer(1).main(sys.argv)
