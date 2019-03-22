---
title: "Programming IceC in X86"
date: 2019-03-22T13:08:00+01:00
tags:
- recipe
- icec
- x86
- C
- make
draft: false

# image: "your-recipe-image.png"
description:  "Description, step by step, of how to program and compile an IceC program to the x86 architecture"
---

# Overview

IceC is an object oriented communication middleware, written in C/C++, with a low use of the resources. This middlware is thought to small microcontrollers with limited resources, but is compatible with different architectures, in this example you are going to see how to program IceC in the architecture x86.

# Ingredients

In order to follow the next recipe you will need the following requirements:

- The [icec](https://arcogroup.bitbucket.io/icec/) and [smart-transducer](https://arcogroup.bitbucket.io/recipes/st_getting_started/) packages available at [Pike](http://pike.esi.uclm.es/)'s repository.
- Basic knowledge of **C** and the tool **make**.
- A Debian/Ubuntu **Linux** distribution.

To install the required packages you have to execute the next command.

{{< shell >}}
$ sudo apt install icec smart-tranducer
{{< /shell >}} 

# An IceC server in x86

The server will consist in a an object that implement the `IBool` interface of the `st.ice` module, located in the path `/usr/share/slice/st/`.

The first step of the program must be implement the `set` function of the interface, in this example case, the implementation will be print a message with the new value in the `stdout`.

{{< code c >}}
void st_IBoolI_set(st_IBoolPtr self, Ice_Bool v, Ice_String sourceAddr)
{
    printf("The new value is %s\n", v ? "true" : "false");
    fflush(NULL);
}
{{< /code >}} 

The work flow in an IceC server will always be the same:

1. Declare an ice communicator, an object adapter and the servants (an object that implements and ice interface) that the server is going to serve.
{{< code c >}}
Ice_Communicator ic;
Ice_ObjectAdapter adapter;
st_IBool servant;
{{< /code >}} 

2. Initialize the communicator and the endpoints of the communicator (`TCPEndpoint` or `UDPEndpoint`).
{{< code c >}}
Ice_initialize(&ic);
TCPEndpoint_init(&ic);
{{< /code >}} 

3. Create and activate the object adapter with their respective endpoints (ip address and port where the server is going to be listening). 
{{< code c >}}
Ice_Communicator_createObjectAdapterWithEndpoints(&ic, "Adapter", endp, &adapter);
Ice_ObjectAdapter_activate(&adapter);
{{< /code >}} 

4. Initialize the servants with their respective type.
{{< code c >}}
st_IBool_init(&servant);
{{< /code >}}

5. Add the servants to the object adapter.
{{< code c >}}
char *servant_identity = "ServantIBool";
Ice_ObjectAdapter_add(&adapter, (Ice_ObjectPtr)&servant, servant_identity);
{{< /code >}}

6. Print the apropiate proxies with the structure `"[identity] -e [ice_version] -[type_of_adapter]:[endpoints]"`.
{{< code c >}}
printf("Proxy ready: '%s -e 1.0 -o:%s'\n", servant_identity, endp);
{{< /code >}}

7. Wait for calls to objects
{{< code c >}}
Ice_Communicator_waitForShutdown(&ic);
{{< /code >}}


When you print the proxy is very important to consider some issues. Firstly IceC **only support** the version 1.0, if you want to communicate an Ice program with another written with IceC both have to work with the 1.0 version. Secondly, if you declare a tcp adapter you have to print a tcp adapter, the different types of adapter are **-o(oneway)** **-t(twoway)** for TCP and **-d(datagram)** for UDP.

After organize the code, your program should look like this one:

{{< staticCode "example_x86.c" >}}

# Compile the program with IceC

To simplify the task of compiling, it's going to be used t a `Makefile` with the tool `make`.

The first task to do is define the variables that are going to be used in the compilation process, in this case we must define the compiler, some paths and the compilation flags that we will need.

{{< code >}}
ICEC_SRC = /usr/src/IceC
DIRSLICE = /usr/share/slice/st/
TARGET = example_x86
SLICE = st

CC = gcc
CFLAGS += -I$(ICEC_SRC) #Flag to include the source code of IceC
CFLAGS += -Wall -Wextra -pedantic -Wno-variadic-macros -Wno-unused-parameter \
	   -Wno-unused-function
{{< /code >}} 

Following we are going to used a powerfull tool of make, `vpath`. Thanks to it, we can indicate a set of directories where make will search for source files or prerequisites that are missing. In order to use IceC, it's necessary to obtain the object code of certain source files, with `vpath` the folder where that files are located can be indicated.

{{< code >}}
vpath %.c $(ICEC_SRC)
vpath %.c $(ICEC_SRC)/platforms/x86
{{< /code >}}

Finally, we must include al the rules with their respective prerequisites to compile our program.

{{< code >}}
all: $(SLICE).h $(TARGET)

$(SLICE).h: 
	slice2c $(DIRSLICE)$(SLICE).ice

$(TARGET): $(TARGET).c IceC.o IceUtil.o TCPEndpoint.o port.o
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(DIREXE)$(TARGET)

clean: 
	find -name "*~" -print -delete
	find -name "*.o" -print -delete
	$(RM) $(TARGET) $(SLICE).h
{{< /code >}}

As you can see, the two main files that are produce with this Makefile are the **header file** (in this case `st.h`) with the IceC interfaces and the executable. In order to obatin the executable, we have to link our source code with some **object files of IceC**. Because of the previous `vpath`, `make` will see the .o files like a prerequisites and will search the .c files in the paths that we indicated producing the object files and compiling the program.

The entire Makefile will be the following.

{{< staticCode "Makefile" >}}

# Executing the server

Once developed the `Makefile`, compile and execute the program is as simple as execute the next commands in your shell.

{{< shell >}}
$ make
$ make run
{{< /shell >}}

If you want to test your server you can use the `st-client` of the `smart-transducer` package.

{{< shell >}}
$ st-client -t bool -p "ServantIBool -e 1.0 -o:tcp -h 127.0.0.1 -p 10000" 1
{{< /shell >}}

If all is correct the server must show the next message.

{{< shell >}}
Proxy ready: 'ServantIBool -e 1.0 -o:tcp -h 127.0.0.1 -p 10000'
The new value is true
{{< /shell >}}













 
