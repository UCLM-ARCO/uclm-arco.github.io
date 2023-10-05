import sys
import Ice

# 1. Load IDM slice
Ice.loadSlice("/usr/share/slice/idm/idm.ice")
import IDM

Ice.loadSlice("interface.ice")
import Example

class HelloI(Example.Hello):
    def sayIt(self, current):
        print("Hello World!")

class Server(Ice.Application):
    def run(self, args):
        ic = self.communicator()

        # 2. Add servant with a valid IDM address
        address = ic.stringToIdentity("10AA02")

        adapter = ic.createObjectAdapter("Adapter")
        proxy = adapter.add(HelloI(), address)
        adapter.activate()

        # 3. Register proxy on IDM router
        router = ic.propertyToProxy("IDM.Router.Proxy")
        router = IDM.RouterPrx.checkedCast(router)
        router.adv(str(proxy))

        print("Ready, waiting events...")
        self.shutdownOnInterrupt()
        ic.waitForShutdown()

if __name__ == "__main__":
    Server().main(sys.argv)
