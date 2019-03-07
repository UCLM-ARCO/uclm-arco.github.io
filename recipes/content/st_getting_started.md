---
title: "Smart Transducer: getting started"
date: 2019-03-05T16:20:02+01:00
tags:
- recipe
- smart home
- st
- domotics
draft: false

image: "st_getting_started/smart-home.jpg"
description: "First steps of using the <code>ST</code> interfaces and tools."
---

# Overview

Smart-Transducer is a **platform** for building *Smart Home* solutions the easy way. It uses very simple interfaces, with a `push` model, to **acquire** sensor information and also to **change** the state of actuators. This recipe will analyze **those interfaces** and how to use them.

# Ingredients

In order to follow this recipe, you will need to satisfy the following **requirements**:

* The `smart-transducer` package, available at [Pike](http://pike.esi.uclm.es/)'s repository.
* **Python** skills (search in [python.org](https://www.python.org/) for tutorials if needed).
* Basic understanding of the [ZeroC](https://zeroc.com/products/ice) Ice midleware.
* Debian/Ubuntu **Linux** and a terminal.

If you **already added** the Pike repository to your system, then just install `smart-transducer`:

{{< shell >}}
$ sudo apt install smart-transducer
{{< /shell >}}

# `ST` description

The `ST` module provides a set of **interfaces**, written in [ZeroC Slice IDL](https://doc.zeroc.com/ice/3.7/the-slice-language), specifically designed to be **simple**, and aimed to build sensors and actuators for a smart environment.

The **definition** of the module is as follows:

{{< staticCode "st.ice" >}}

{{% note primary %}}
Please, **don't download** this interface for deployment. It is provided by the `smart-transducer` package (usually at `/usr/share/slice/st`). Use it instead.
{{% /note %}}

It has an interface for every **basic type** (`bool`, `byte`, `float`, `string`...), with a single method, `set`, which accepts two arguments:

* `value`, of a specific type (determined by the interface).
* `sourceAddr`, a string with the source address of the **caller**.

These interfaces could be **implemented by actuators** and **used by sensors**. For instance, a door controller would implement the `IBool` interface to lock/unlock the door. Or a temperature sensor could use the `IFloat` interface to **publish** its temperature to some subscriber.

The last interface, `Linkable` is used to *register* an observer to some *observable* object. For instance, the temperature sensor could implement this interface, so when a service wants to **receive the temperature**, it could call to `linkto()` with a proxy (or a valid IDM address) to a servant that implements the **same interface** that the temperature sensor uses (in the former example, `IFloat`). The sensor usually will **store that proxy**, and whenever it wants to publish the temperature, it calls the `set()` method on it.

Let's use them on a **very simple** example.

# A (not-so) minimal service

Imagine that you want to create a **clock service**. It has a method that allows the user to **change** the current time, and it also **publishes** the updated time every five seconds.

This clock service will implement the interface `IString` to set the current hour. Also, it implements the `Linkable` interface, so other services could attach their observers. And it will use the same interface `IString` to publish the updated time.

So, first, let's create a **custom interface** that inherits `IString` and `Linkable`. This way, we may have only one servant. The slice would be:

{{< staticCode "clock.ice" >}}

Now, we can implement the servant. The method `IString.set()` will be used to set the current time, so the given value must have **some format**. Let's assume that it will be in the form "hh:mm:ss", where *hh* is the hour in 24 hours format, *mm* is the minute and *ss* is the second. The code may be:

{{< code py >}}
def set(self, value, source=None, current=None):
    self.seconds = sum(
        x * int(t) for x, t in zip([3600, 60, 1], value.split(":"))
    )
    if source is not None:
        print("time set to '{}' by {}".format(value, source))
{{< /code >}}


On the other hand, the method `Linkable.linkto()` will provide the (stringfied) **proxy** to some object that will implement the `IString` interface, so we **store** it for later use:

{{< code py >}}
def linkto(self, observer, current):
    ic = current.adapter.getCommunicator()
    prx = ic.stringToProxy(observer)
    self.observer = st.IStringPrx.uncheckedCast(prx)
    print("new observer set: '{}'".format(str(prx)))
{{< /code >}}

The last part is to create the **server** that will hold the **adapter**, instantiate this servant, and register it on the adapter. It will also run the **event loop**: once every five seconds, it will call a method on the servant to **publish** the time to its observer (if any). The whole `clock-server.py` code is:

{{< staticCode "clock-server.py" >}}

Run it and you should see something like this:

{{< shell >}}
$ ./clock-server.py
proxy ready: 'Clock -t -e 1.1:tcp -h 127.0.0.1 -p 1234 -t 60000'
{{< /shell >}}

# A minimal client

You can also write a **custom client**, which in this case is **very straightforward**. Just create the proxy, and call it using the given arguments. It will be used to **change** the clock time, so you must call it using the clock's proxy and the desired time, in the correct format.

The whole client code may be:

{{< staticCode "clock-client.py" >}}

If you run it, you should see a message on the server telling you that it changed its time:

{{< shell >}}
client $ ./clock-client.py 'Clock -t:tcp -h 127.0.0.1 -p 1234' "23:59:46"

server output:
[...]
time set to '23:59:46' by time-master
{{< /shell >}}

# A minimal observer

In order to receive time publications from the server, we would need an **observer**. It is just a common servant of the proper type (in this case `IString`) that we will register on the server using the `Linkable` interface. We don't need to do any special thing in the servant, so:

{{< code py >}}
class ClockObserverI(st.IString):
    def set(self, value, source, current):
        print("time event: '{}', by {}".format(value, source))
{{< /code >}}

As always, we need to register this servant on an adapter. We then use the resulting proxy as the argument for the `linkto` method. The whole observer would be:

{{< staticCode "clock-observer.py" >}}

**Run** the observer and it should start receiving new events:

{{< shell >}}
$ ./clock-observer.py 'Clock -t:tcp -h 127.0.0.1 -p 1234'
Proxy ready: 'Clock -t -e 1.1:tcp -h 127.0.0.1 -p 1235 -t 60000'
Subscribed to clock, waiting events...
time event: '23:59:51', by clock
time event: '23:59:56', by clock
[...]

server output:
[...]
new observer set: 'Clock -t -e 1.1:tcp -h 127.0.0.1 -p 1235 -t 60000'
publish time: '23:59:51'
publish time: '23:59:56'
[...]
{{< /shell >}}

# Using the `st-client` tool

To invoke any `st` interface, you can use the `st-client` tool, provided in the `smart-transducer` package. So, to change the clock time, just call it with the proxy and the desired value (run `st-client -h` for more options):

{{< shell >}}
$ st-client -t string -p "Clock -t:tcp -h 127.0.0.1 -p 1234" "12:23:34"
{{< /shell >}}