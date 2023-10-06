#!/usr/bin/python3

import sys
import Ice

Ice.loadSlice("clock.ice -I. --all")
import recipe


class ClockClient(Ice.Application):
    def run(self, args):
        if len(args) < 3:
            print("Usage: {} <proxy> <hh:mm:ss>".format(args[0]))
            return

        ic = self.communicator()

        clock = ic.stringToProxy(args[1])
        clock = recipe.ClockPrx.uncheckedCast(clock)
        clock.set(args[2], "time-master")


if __name__ == "__main__":
    ClockClient().main(sys.argv)
