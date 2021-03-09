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
It's possible to run Zigbee2MQTT in a Docker container using the official Zigbee2MQTT Docker image. For this purpose, `docker-compose.yml` file in the zigbee2mqtt folder must contain the following:
{{<staticCode "docker-compose.yml">}}

And the `configuration.yaml` file in the zigbee2mqtt-data folder must contain the following:
{{<staticCode "zigbee2mqtt-data/configuration.yaml">}}

To facilitate execution, you can create a Makefile.
{{<staticCode "Makefile">}}

These files are provided in the directory `shapes/static/code/zigbee2mqtt`. 

**Warning!** Before running, all you have to do is to look if your port is different from ttyUSB0, in this case you have to change it in the `docker-compose.yml` file in the zigbee2mqtt folder and the `configuration.yaml` files in the zigbee2mqtt-data folder.

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
Two types of sensors have been used for this example: a smart plug and a smart motion sensor. First of all, you must pair the device to zzh! To do this you should see the led flashing on the device, then the device is ready for pairing. Once this step is done you will see on your terminal the events produced by the sensors.

In the case of the smart plug, the following events are displayed on our terminal indicating that the device has been connected and configured. In addition, the last two events show whether the plug has been turned on or off by the user.
{{<shell>}}
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: Device '0xec1bbdfffea21a34' joined
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xec1bbdfffea21a34","ieee_address":"0xec1bbdfffea21a34"},"type":"device_joined"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: Starting interview of '0xec1bbdfffea21a34'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xec1bbdfffea21a34","ieee_address":"0xec1bbdfffea21a34","status":"started"},"type":"device_interview"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":{"friendly_name":"0xec1bbdfffea21a34"},"type":"device_connected"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:27: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"interview_started","meta":{"friendly_name":"0xec1bbdfffea21a34"},"type":"pairing"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:45: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xec1bbdfffea21a34","ieee_address":"0xec1bbdfffea21a34"},"type":"device_announce"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:45: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"announce","meta":{"friendly_name":"0xec1bbdfffea21a34"},"type":"device_announced"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: Successfully interviewed '0xec1bbdfffea21a34', device has successfully been paired
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: Device '0xec1bbdfffea21a34' is supported, identified as: Lidl Silvercrest smart plug (EU, CH, FR, BS, DK) (HG06337)
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"definition":{"description":"Silvercrest smart plug (EU, CH, FR, BS, DK)","exposes":[{"features":[{"access":7,"description":"On/off state of the switch","name":"state","property":"state","type":"binary","value_off":"OFF","value_on":"ON","value_toggle":"TOGGLE"}],"type":"switch"},{"access":1,"description":"Link quality (signal strength)","name":"linkquality","property":"linkquality","type":"numeric","unit":"lqi","value_max":255,"value_min":0}],"model":"HG06337","supports_ota":false,"vendor":"Lidl"},"friendly_name":"0xec1bbdfffea21a34","ieee_address":"0xec1bbdfffea21a34","status":"successful","supported":true},"type":"device_interview"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: Configuring '0xec1bbdfffea21a34'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"interview_successful","meta":{"description":"Silvercrest smart plug (EU, CH, FR, BS, DK)","friendly_name":"0xec1bbdfffea21a34","model":"HG06337","supported":true,"vendor":"Lidl"},"type":"pairing"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:16:51: Successfully configured '0xec1bbdfffea21a34'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:17:10: MQTT publish: topic 'zigbee2mqtt/0xec1bbdfffea21a34', payload '{"linkquality":147,"state":"ON"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:17:12: MQTT publish: topic 'zigbee2mqtt/0xec1bbdfffea21a34', payload '{"linkquality":144,"state":"OFF"}'
{{</shell>}}

In the case of the intelligent motion sensor, as with the plug, the events that the sensor has been connected and configured correctly are displayed. In this case the last two events indicate whether or not there is presence in the room, being *occupancy* true if the sensor detects motion and false otherwise. 
{{<shell>}}
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: Device '0xbc33acfffe35ac11' joined
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11"},"type":"device_joined"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: Starting interview of '0xbc33acfffe35ac11'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11","status":"started"},"type":"device_interview"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":{"friendly_name":"0xbc33acfffe35ac11"},"type":"device_connected"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:23: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"interview_started","meta":{"friendly_name":"0xbc33acfffe35ac11"},"type":"pairing"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:24: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11"},"type":"device_announce"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:26:24: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"announce","meta":{"friendly_name":"0xbc33acfffe35ac11"},"type":"device_announced"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:11: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"linkquality":183}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:14: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"linkquality":186}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:15: Successfully interviewed '0xbc33acfffe35ac11', device has successfully been paired
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:15: Device '0xbc33acfffe35ac11' is supported, identified as: Lidl Silvercrest smart motion sensor (HG06335)
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:15: MQTT publish: topic 'zigbee2mqtt/bridge/event', payload '{"data":{"definition":{"description":"Silvercrest smart motion sensor","exposes":[{"access":1,"description":"Indicates whether the device detected occupancy","name":"occupancy","property":"occupancy","type":"binary","value_off":false,"value_on":true},{"access":1,"description":"Indicates if the battery of this device is almost empty","name":"battery_low","property":"battery_low","type":"binary","value_off":false,"value_on":true},{"access":1,"description":"Indicates whether the device is tampered","name":"tamper","property":"tamper","type":"binary","value_off":false,"value_on":true},{"access":1,"description":"Remaining battery in %","name":"battery","property":"battery","type":"numeric","unit":"%","value_max":100,"value_min":0},{"access":1,"description":"Link quality (signal strength)","name":"linkquality","property":"linkquality","type":"numeric","unit":"lqi","value_max":255,"value_min":0}],"model":"HG06335","supports_ota":false,"vendor":"Lidl"},"friendly_name":"0xbc33acfffe35ac11","ieee_address":"0xbc33acfffe35ac11","status":"successful","supported":true},"type":"device_interview"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:15: Configuring '0xbc33acfffe35ac11'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:15: MQTT publish: topic 'zigbee2mqtt/bridge/log', payload '{"message":"interview_successful","meta":{"description":"Silvercrest smart motion sensor","friendly_name":"0xbc33acfffe35ac11","model":"HG06335","supported":true,"vendor":"Lidl"},"type":"pairing"}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:17: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"linkquality":183}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:19: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"linkquality":180}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:28: Successfully configured '0xbc33acfffe35ac11'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:38: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"battery_low":false,"linkquality":177,"occupancy":false,"tamper":false}'
zigbee2mqtt    | Zigbee2MQTT:info  2021-03-09 19:27:46: MQTT publish: topic 'zigbee2mqtt/0xbc33acfffe35ac11', payload '{"battery":100,"battery_low":false,"linkquality":183,"occupancy":true,"tamper":false}'
{{</shell>}}
