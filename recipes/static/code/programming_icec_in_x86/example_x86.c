#include <stdio.h>
#include <IceC/IceC.h>
#include <IceC/platforms/x86/TCPEndpoint.h>

#include "st.h"


void st_IBoolI_set(st_IBoolPtr self, Ice_Bool v, Ice_String sourceAddr)
{
    printf("The new value is %s\n", v ? "true" : "false");
    fflush(NULL);
}

int main()
{
    Ice_Communicator ic;
    Ice_ObjectAdapter adapter;
    st_IBool servant;
    char *endp = "tcp -h 127.0.0.1 -p 10000";
    char *servant_identity = "ServantIBool";

    Ice_initialize(&ic);
    TCPEndpoint_init(&ic);

    Ice_Communicator_createObjectAdapterWithEndpoints(&ic, "Adapter", endp, &adapter);
    Ice_ObjectAdapter_activate(&adapter);

    st_IBool_init(&servant);
    Ice_ObjectAdapter_add(&adapter, (Ice_ObjectPtr)&servant, servant_identity);

    printf("Proxy ready: '%s -e 1.0 -o:%s'\n", servant_identity, endp);
    fflush(NULL);

    Ice_Communicator_waitForShutdown(&ic);
    return 0;
}
