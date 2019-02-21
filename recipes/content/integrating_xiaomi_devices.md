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

Now, you can disconnect your WiFi from the internet (if you want), and close the "Mi Home" app. You are now ready to use the library.

# Connection to the Gateway
# Receiving events
# Sending actions
