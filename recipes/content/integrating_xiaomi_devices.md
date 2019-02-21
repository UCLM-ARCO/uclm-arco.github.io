---
title: "Integrating Xiaomi Devices"
date: 2019-02-20T08:05:08+01:00
tags:
- recipe
- xiaomi
- smart home
- python
draft: true

image: "integrating_xiaomi_devices/mijia-kit.jpg"
description: "This recipe will show you how to actuate and receive events from the Mijia Smart Home kit (using <b>Python</b> and the <code>mijia-smart-home</code> library)."
---

# Overview

Xiaomi (or Aqara or Mijia) has a family of products intended to be part of a Smart Home environment that everyone can install and use. They are beautiful and simple, but the associated *cloud app* sometimes does **not** fullfill our requirements (for instance, **cloudless control**).

Thus, here we present a **Python library** that you could use to interact with those devices, *changing* its state and *receiving events* from their sensors. Let's review what you will need to follow up.

{{< polaroid src="mijia-kit-2.jpg" caption="Xiaomi/Mijia smart home kit" >}}

# Ingredients

For this recipe, you will need the following items:

* A **Mijia Gateway**, and the devices you want to control (PIR, push button, magnet sensor, ...)
* A **WiFi** access point (you may also use your own Smartphone AP)
* An updated Debian/Ubuntu **Linux** box
* **Python 3** installed and ready
* [Pike](http://pike.esi.uclm.es/)'s repository for library package

For this last requirement, and if you already have Pike's repository configured on your system, just install the library:

{{< shell >}}
$ sudo apt install mijia-smart-home
{{< /shell >}}

# Setting up the environment

The library you just installed uses what is called the [Lan Communication API](https://aqara.gitbooks.io/lumi-gateway-lan-communication-api/content/), which is an open API that allows **third-party** applications to control Mijia devices. It should be enabled on the Gateway, as it's disabled by default.

So, your first step should be to **install** the "Mi Home" application on your smartphone. Now, plug the Mijia Gateway and wait for a chinese message. It should now **blink on yellow**. If not, press the button for 5 seconds and release. It should *beep*, say the message and blink on yellow. This state is the **"binding"** state. Now, open the "Mi Home" app on your smartphone.

If required, login or register. Then, press the **upper-right plus** button. It will show an scan procedure, and find the connected Mijia Gateway. Press it when found.

{{< polaroid src="01-plus-button.png" caption="Adding Gateway to Mi Home" >}}

Now, it will show you a dialog to enter the **WiFi credentials**. The Mi Home app wil send this information to the Gateway, so it can also connect to your AP. Just select your SSID and provide the AP password. Note that this AP **must have** Internet conectivity for this step to be completed. But just for this step, once the Gateway is configured, you can let the AP isolated from the Internet.

Press "next", and the app will **finish the setup** procedure. You may also hear a message from the Gateway saying that the connection is ready.

Next, select the Gateway in the app, and open its "Settings", and press on "About".

{{< polaroid src="02-settings-about.png" caption="Gateway 'settings' > 'about' " >}}

It will show you an screen with a **link to the forum**, and the "Plug-in version" at the **bottom**. Press that label ("Plug-in version") until you see a message, and more options appear.

{{< polaroid src="03-hidden-settings.png" caption="Enabling the hidden settings" >}}

There, you should see now the option "Wireless communication protocol". Tap it. Here you can **enable** the protocol, and regenerate a **new password**. Do both of them, and **store the password**, as you will need it later. Press OK when finish.

{{< polaroid src="04-enable-proto.png" caption="Enabling the 'Wireles Communication Protocol'" >}}

Now, you can disconnect your WiFi from the internet (if you want), and close the "Mi Home" app. You are now **ready** to use the library.

# Connection to the Gateway

The `mijia-smart-home` library provides some objects that **represents** the real-life devices. You can call these object's methods to **attach handlers** that will be called when an event arrives, or call directly a method to **change the state** of some device.

So, the first object you should create is the Gateway itself. Import the library and create a `LumiGateway()` instance. Then, call the method `start()`, which does the following:

* Starts a **discovery** stage, to find the Gateway.
* Setups a listener thread, to process **incoming** events from it.

{{< code py >}}
from mijia import LumiGateway

gw = LumiGateway()
gw.start()
{{< /code >}}

{{% note "danger" %}}
Remember that your host machine (where you run this script) should be connected to the **same network** that the Aqara Gateway.

Also, note that the **protocol to discover** a new Gateway implies the use of IP multicast messages, so you need IP multicast support. If there is any **problem**, try to add the WiFi interface as a route for multicast messages:

<hr>
<code>$ sudo route add -net 224.0.0.0 netmask 224.0.0.0 wlan0</code >
{{% /note %}}

The `LumiGateway`'s constructor has **some parameters** but we will review them later. When the `start()` returns, you can inspect the list of **linked devices** to the Gateway (both online and offline).

{{< code py >}}
for dev in gw.get_devices().values():
    print(dev)
{{< /code >}}

Each device is of an **specific class**, having its specific methods, but all of them have an `sid`, which is the device identifier, and a `status`, which indicates if the device is **online** and the Gateway acknowledges it or not. The rest of the interface will be analyzed in following sections.

So, the **full code** for this part would be:

{{< staticCode "list-devices.py" >}}

Running that script in a **well configured** environment, should yield something like this:

{{< shell >}}
$ ./list-devices.py
INFO:root:using 192.168.8.157 for multicast membership
WARNING:root:using not ciphered key (password not provided)
<Plug, sid: 158d0002699f12, status: unknown, enabled: False, inuse: False>
<Magnet, sid: 158d0002520b26, status: unknown, open: None>
<Switch, sid: 158d00027b8eff, status: unknown, pressed: False>
<WeatherV1, sid: 158d0002c8f54e, status: unknown, temp: 10000, humi: 0, press: 0>
<Unsupported Device, sid: d0cf5efffeccf1dc, status: unknown>
<Motion, sid: 158d0002567d32, status: unknown, elapsed: None>
<SensorCube, sid: 158d00029a8fa6, status: unknown>

{{< /shell >}}

# Receiving events
# Sending actions
