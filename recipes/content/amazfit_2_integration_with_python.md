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

Under the Amazit branch name, there are a lot of devices. Among them, Xiamoi
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
read sensor values or receive notifications. It you need it, just use the method
`pair()`, which does not accept any parameter:

{{<code py>}}
chip.pair()
{{</code>}}

During this stage, the chip will **blink a red led** to indicate that you must
**shake** the device in that moment. If you don't do it, then the pairing will
**fail**. Otherwise, it will be successful, and you can read the exchanged key in
the property `key`. Store it for later usage.


# Reading Static Information

The smart chip holds some specific **static information** that barely changes,
like the device **address**, its name, product id, vendor id, and so on. All
this data could be read using the method `read_info()`. This method will contact
with the device and rea

# Reading Step Count
# Reading Chip Orientation
# References

* [Amazfit 2 Chip API Reference](/api/amazfit_2_dynamic_core/)
