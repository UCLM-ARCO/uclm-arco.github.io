---
title: "Amazfit 2 Dynamic Core: Integration with Python"
date: 2020-01-28T09:49:09+01:00
tags:
- sensor
- python
- bluetooth
- amazfit
draft: false

image: "amazfit_2_integration_with_python/amazfit-2-chip.jpg"
description:  "Recipe explaining how to connect and use the Amazfit 2 Smart Chip device using Python."
---


# Overview

Under the Amazit branch name, there are a lot of devices. Among them, Xiaomi
sells 'smart' shoes (or sneakers) that have a **device** inside which measures
some variables and also **counts the steps**. The device could be purchased
alone, and is known as *Amazfit 2 Smart Chip* or *Dynamic Core*. In this recipe,
we will use a **Python** library to connect to that device, and acquire the
provided information.


# Ingredients

Of course, to follow this recipe you will need **the device** itself, but also
the following **packages** (assuming that you are on GNU/Linux):

* `python3-bluez`: generic support for **Bluetooth**
* `python3-gattlib`: specific support for BLE and **GATT** protocols. More
  information on [bitbucket.org/OscarAcena/pygattlib](https://bitbucket.org/OscarAcena/pygattlib/src/default/#markdown-header-installation)
* `amazfit2-dynamic-core`: specific **library** for this device

The easiest way to install all these packages is using the
[pike](http://pike.esi.uclm.es/) Debian repository. To install it:

{{<shell>}}
wget -qO- http://pike.esi.uclm.es/add-pike-repo.sh | sudo sh
sudo apt update
{{</shell>}}

Then, install with `apt` the needed packages:

{{<shell>}}
sudo apt install python3-bluez python3-gattlib amazfit2-dynamic-core
{{</shell>}}


# Discovering and Connecting

The **first step** you would do in order to check if your device is ready for
connection (has enough battery, is awake, is near your Bluetooth transceiver,
etc.) is a **discovery**.

{{% note "danger" %}}

In order to **save battery**, the Smart Chip will go to sleep very frecuently.
In sleep mode, you could not discover it, neither connect to it. To wake up your
device, just **shake it** a little. <br><br>

{{% /note %}}

So, first things first, **import** the Smart Chip library into your script.
There is only one interesting class, called `Core2`, so:

{{<code py>}}
#!/usr/bin/python3

from amazfit import Core2
{{</code>}}

Now, you can use the method `find_all()` (which is a `@classmethod`) to perform
the **discovery**. It will try to find all the BLE (Bluetooth Low Energy)
devices near you, and then **filter them** by its vendor ID. Because this is an
endless operation, you can specify the **maximum time** you want to wait for
responses (by default is **5 seconds**), using the argument `timeout`.

`find_all()` will return a **list** with the found devices. A similar operation
is `find()`, which only returns the **first result** in the list:

{{<code py>}}
chip = Core2.find(timeout=10)
if chip is None:
    print("ERROR: no device found!")
{{</code>}}

Next step is **connect** to the device. This will create a communication
"channel" between your computer and the chip, used to read sensor values,
receive notifications, etc. For this purpose, call the method `connect()`, which
does not have any argument:

{{<code py>}}
chip.connect()
{{</code>}}

If it **returns** normally, the connection has been **stablished**. If something
went wrong (maybe the device is asleep...), it will raise a `RuntimeError` with
a message about the problem.


# About pairing

After the connection has been stablished, you can run a *bonding* or *pairing*
process. It will be used to **exchange a cyphering** key with the device, which
is the way to validate client's identity. Also, it will **reset the step counter**
to zero.

Although in many devices this is a critical part (otherwise, the device will
disconnect or ignore your requests), on the Smart Chip **is not required** to
read sensor values or receive notifications. If you need it, just use the method
`pair()`, which does not accept any parameter:

{{<code py>}}
chip.pair()
{{</code>}}

During this stage, the chip will **blink a red led** to indicate that you must
**shake** the device in that moment. If you don't do it, then the pairing will
**fail**. Otherwise, it will be successful, and you can read the exchanged key in
the property `key`. Store it for later usage:

{{<code py>}}
print(f"Pairing key: {chip.key}")
{{</code>}}


# Reading Static Information

The Smart Chip holds some specific **static information** that barely changes,
like the device **address**, its name, product id, vendor id, and so on. All
this data could be read using the method `read_info()`. This method will contact
with the device and **retrieve** all those fields. Then, you can access each one
using its specific property. For a complete list of the fields, please see the
[API
reference](/api/amazfit_2_dynamic_core/#span-class-api-func-read-info-span). The
following is an example of use:

{{<code py>}}
chip.read_info()
print(
    f"Device Information:\n"
    f"- name: {chip.name}\n"
    f"- address: {chip.address}\n"
    f"- serial number: {chip.serial_number}\n"
    f"- hardware version: {chip.hardware_rev}\n"
    f"- software version: {chip.software_rev}"
)
{{</code>}}

# Reading Step Count

There are some other values that could be read from the device, but are not
considered static information because they change **along the time**: the step
counter and the device orientation. Let's review first the steps.

The Amazfit library exposes the **step counter** as a property, `steps`. Each
time is accessed, the library will read it **from the device** (no caching is done).
So, as an example:

{{<code py>}}
print(f"- steps: {chip.steps}")
{{</code>}}

Now, you may put the device inside your shoes and **walk**. Then, run the program
again, which should return a different result.

Of course, it is very cumbersome (and battery draining) to **poll** the value
every few seconds. For this reason, you can use a push-notification system. The
library will call a user-configured **callback** each time the counter changes.
To setup this callback, use the method `on_steps()`, which accepts a single
parameter: the callback function with the following signature:

{{<code py>}}
def on_steps_callback(steps)
{{</code>}}

Of course, this push mechanism is **asynchronous**, which means that once the
callback is set, the `on_steps()` method will return, allowing you to **do other
things**. If you just want to wait for incoming events, you can create a waiting
*event loop*, or just call the library's builtin with `wait_until_break()`.

The following example will print **every new update** on the step counter:

{{<code py>}}
def on_steps_callback(steps):
    print(f"- steps changed, new value: {steps}")

chip.on_steps(on_steps_callback)
chip.wait_until_break()
{{</code>}}


# Reading Chip Orientation

Another dynamic value that could be read from the device is the **orientation**,
which are a pair of values representing the angle of (X, Y) axis (it is given as
a tuple). Once again, you retrieve it using a **property** that will be read from
the device each time is used:

{{<code py>}}
print(f"- axis: {chip.orientation}")
{{</code>}}

It will take **a little** to acquire this value, and could not be read very
frecuently.

{{% note "danger" %}}
And, what about the **meaning** of these two values? Well, I'm sorry to say that
here you are alone. You will need to figure it out, I'm still working on it...
<br><br>
{{% /note %}}

Again, you can also configure a **callback** to be called when this value has
changed (using the `on_orientation()` method), but, you are warned, no new
notifications will be received unless someone **fires a read request**, with the
method `fire_orientation_read()`. See the following example:

{{<code py>}}
print(f"- axis: {chip.orientation}")

def on_orientation_callback(axis):
    print(f"- orientation changed, new value: {axis}")

chip.on_orientation(on_orientation_callback)

while True:
    try:
        chip.fire_orientation_read()
        time.sleep(2)
    except KeyboardInterrupt:
        break
{{</code>}}


# References

This is a list of useful resources:

* [Amazfit 2 Chip API Reference](/api/amazfit_2_dynamic_core/)
