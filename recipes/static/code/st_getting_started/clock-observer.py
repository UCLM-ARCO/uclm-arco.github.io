#!/usr/bin/python3

import sys
import Ice
import time
import sched
from datetime import datetime, timedelta

Ice.loadSlice("clock.ice -I. --all")
import st
import recipe


class ClockObserverI(st.IString):
    def set(self, value, source, current):
        print("time event: '{}', by {}".format(value, source))


class ClockObserver(Ice.Application):
    def run(self, args):
        if len(args) < 2:
            print("Usage: {} <clock-server>".format(args[0]))
            return

        ic = self.communicator()
        adapter = ic.createObjectAdapterWithEndpoints(
            "Adapter", "tcp -h 127.0.0.1 -p 1235"
        )
        adapter.activate()

        servant = ClockObserverI()
        proxy = adapter.add(servant, ic.stringToIdentity("Clock"))
        print("Proxy ready: '{}'".format(proxy))

        server = ic.stringToProxy(args[1])
        server = st.LinkablePrx.uncheckedCast(server)
        server.linkto(str(proxy))
        print("Subscribed to clock, waiting events...")

        self.shutdownOnInterrupt()
        ic.waitForShutdown()


if __name__ == "__main__":
    ClockObserver().main(sys.argv)
