import sys
import Ice

Ice.loadSlice("interface.ice")
import Example

class Client(Ice.Application):
    def run(self, args):
        ic = self.communicator()

        # 1. Use remote object's IDM address
        address = ic.stringToIdentity("10AA02")

        # 2. Send the invocation through the IDM router
        remote = ic.propertyToProxy("IDM.Router.Proxy")
        remote = remote.ice_identity(address)
        remote = Example.HelloPrx.uncheckedCast(remote)

        remote.sayIt()

if __name__ == "__main__":
    Client().main(sys.argv)
