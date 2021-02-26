---
title: "Zigbee2mqtt Configuration"
date: 2021-02-08T10:25:16+01:00
draft: false
tags: 
- home assistant
- zzh! 
- zigbee2mqtt  

image: "zigbee2mqtt/icon.png"
description: "A recipe for configuring Zigbee2mqtt."
---

## Overview
Zigbee2mqtt is an open-source project that enables the interconnection of different-brand devices with Zigbee connectivity. It can also be integrated with Home Assistant thanks to the bidirectional message relay from the network to MQTT. Additionally, we need a zigbee adapter, this is the zig-a-zig-ah! (zzh!).

## Ingredients

In order to follow this recipe you will need:

- A suported Zigbee adapter (zzh!)
- One or more suported Zigbee device/s
- A Raspberry Pi 4 or **RPi4** (optional)

## zzh! Configuration

### Drivers for CH341
First step in getting the zzh! set up is to ensure that the host computer has the right drivers for the CH341 installed. Plug your device and ensure that zzh is recognised with the command `dmesg`. If recognized, you will see something like:

{{<shell>}}
[ 2513.148412] usb 1-1: new full-speed USB device number 9 using xhci_hcd
[ 2513.298446] usb 1-1: New USB device found, idVendor=1a86, idProduct=7523, bcdDevice= 2.64
[ 2513.298451] usb 1-1: New USB device strings: Mfr=0, Product=2, SerialNumber=0
[ 2513.298455] usb 1-1: Product: USB Serial
[ 2514.187751] usbcore: registered new interface driver usbserial_generic
[ 2514.187757] usbserial: USB Serial support registered for generic
[ 2514.373835] usbcore: registered new interface driver ch341
[ 2514.373867] usbserial: USB Serial support registered for ch341-uart
[ 2514.373903] ch341 1-1:1.0: ch341-uart converter detected
[ 2514.374745] usb 1-1: ch341-uart converter now attached to ttyUSB0
{{</shell>}}

### Download cc2538-bsl
To run cc2538-bsl.py you need to have python and pip installed on your system. If you don't have them installed running the following commands should work: `sudo apt update && sudo apt-get install python3-pip`  

To download and extract cc2538-bsl run `wget -O cc2538-bsl.zip https://codeload.github.com/JelmerT/cc2538-bsl/zip/master && unzip cc2538-bsl.zip` in your terminal and to install the required dependencies run `sudo pip3 install pyserial intelhex`.

## Zigbee2mqtt
Download the Z-Stack coordinator firmware from https://github.com/Koenkk/Z-Stack-firmware. The firmware you'll need can be found under `coordinator/Z-Stack_3.x.0/bin/CC26X2R_coordinator_<date>.zip`. Download and extract this. 

To burn this on your zzh! go to the directory `cc2538-bsl-master` and run:
{{<shell>}}
user@user-pc:~$ ./cc2538-bsl.py -p /dev/<port> -evw <firmware_zigbee>.hex
{{</shell>}}

The port in my case is ttyUSB0, as shown in section *Drivers for CH341*, so don't forget to change to the port used on your machine.

In case you want to erase the flash, run:
{{<shell>}}
user@user-pc:~$ ./cc2538-bsl.py -p /dev/<port> -e
{{</shell>}}

### Running 
It's possible to run Zigbee2MQTT in a Docker container using the official Zigbee2MQTT Docker image. For this purpose, `configuration.yaml` file in the zigbee2mqtt folder must contain the following:
{{<staticCode "configuration.yaml">}}

And the `configuration.yaml` file in the zigbee2mqtt-data folder must contain the following:
{{<staticCode "zigbee2mqtt-data/configuration.yaml">}}

To facilitate execution, you can create a Makefile.
{{<staticCode "Makefile">}}

These files are provided in the directory `shapes/static/code/zigbee2mqtt`. 

**Warning!** Before running, all you have to do is to look if your port is different from ttyUSB0, in this case you have to change it in the `configuration.yaml` files in the zigbee2mqtt and zigbee2mqtt-data folders.

Then run: 
{{<shell>}}
user@user-pc:~$ make
{{</shell>}}

When started successfully, you will see something like:
{{<shell>}}
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:08: Logging to console and directory: '/app/data/log/2021-02-08.12-11-07' filename: log.txt
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:08: Starting Zigbee2MQTT version 1.17.1 (commit #99274c7)
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:08: Starting zigbee-herdsman (0.13.59)
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:10: zigbee-herdsman started
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:10: Coordinator firmware version: '{"meta":{"maintrel":1,"majorrel":2,"minorrel":7,"product":1,"revision":20210120,"transportrev":2},"type":"zStack3x0"}'
zigbee2mqtt    | Zigbee2MQTT:info  2019-11-09T13:04:03: Currently 0 devices are joined:
zigbee2mqtt    | Zigbee2MQTT:warn  2021-02-08 12:11:10: `permit_join` set to  `true` in configuration.yaml.
zigbee2mqtt    | Zigbee2MQTT:warn  2021-02-08 12:11:10: Allowing new devices to join.
zigbee2mqtt    | Zigbee2MQTT:warn  2021-02-08 12:11:10: Set `permit_join` to `false` once you joined all devices.
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:10: Zigbee: allowing new devices to join.
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:10: Connecting to MQTT server at mqtt://localhost
mosquitto      | 1612782670: New connection from 127.0.0.1:33918 on port 1883.
mosquitto      | 1612782670: New client connected from 127.0.0.1:33918 as mqttjs_6e1b6864 (p2, c1, k60).
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:11:10: Connected to MQTT server
{{</shell>}}

Now zigbee is ready listening for pairing devices.

### Example with lidl sensors
For this use we should install the lidl home app on the mobile phone. Once the app has been installed,  we paired the gateway and then we paired the device to the gateway, all these from app. 

If it has worked successfully, you will see something like:
{{<shell>}}
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:53:27: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"interview_started","meta":{"friendly_name":"0x680ae2fffe0a305f"},"type":"pairing"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:54:12: Device '0xbc33acfffe35ac11' joined
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:54:12: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11"},"type":"device_joined"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:54:12: Starting interview of '0xbc33acfffe35ac11'
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:54:12: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11","status":"started"},"type":"device_interview"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-02-08 12:54:12: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":{"friendly_name":"0xbc33acfffe35ac11"},"type":"device_connected"}'
{{</shell>}}