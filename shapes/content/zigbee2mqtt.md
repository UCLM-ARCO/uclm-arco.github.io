---
title: "Monitoring System"
date: 2021-02-08T10:25:16+01:00
draft: true
tags: 
- monitoring
- zzh! 
- zigbee2mqtt  
- grafana
- influxdb
- telegraf
- smart sensors

image: "zigbee2mqtt/icon.png"
description: "A recipe for configuring Zigbee2mqtt."
---

## Overview
Zigbee2mqtt is an open-source project that enables the interconnection of different-brand devices with Zigbee connectivity. It can also be integrated with Home Assistant thanks to the bidirectional message relay from the network to MQTT. Additionally, we need a zigbee adapter, this is the zig-a-zig-ah! (zzh!).

## Ingredients

In order to follow this recipe you will need:

- A suported Zigbee adapter (zzh!).
- One or more suported Zigbee device/s.
- A Raspberry Pi 4 or **RPi4**.
- The `shapes-monitoring-service` package, which will configure ZigBee2MQTT, Mosquitto, InfluxDB, Telegraf and Grafana, available in [ARCO package repository](https://uclm-arco.github.io/debian/).
- Grafana APT repository, the latest OSS release, available in [download link](https://grafana.com/docs/grafana/latest/installation/debian/).

## Configuring zzh!

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

## Configuring ZigBee2MQTT
Download the Z-Stack coordinator firmware from https://github.com/Koenkk/Z-Stack-firmware. The firmware you'll need can be found under `coordinator/Z-Stack_3.x.0/bin/CC26X2R_coordinator_<date>.zip`. Download and extract this. 

To burn this on your zzh! go to the directory `cc2538-bsl-master` and run:
{{<shell>}}
pi@raspberrypi:~$ ./cc2538-bsl.py -p /dev/<port> -evw <firmware_zigbee>.hex
{{</shell>}}

The port in my case is ttyUSB0, as shown in section *Drivers for CH341*, so don't forget to change to the port used on your machine.

In case you want to erase the flash, run:
{{<shell>}}
pi@raspberrypi:~$ ./cc2538-bsl.py -p /dev/<port> -e
{{</shell>}} 

**Warning!** Before running, all you have to do is to look if your port is different from ttyUSB0, in this case you have to change it in the `docker-compose.yml` file in the `/etc/compose` directory.

## Configuring `shapes-monitoring-service`
The monitoring system, or `shapes-monitoring-service`, is a service running always in our device with `systemd`, we can see its status with the following command:
{{<shell>}}
pi@raspberrypi:~$ systemctl status shapes-monitoring.service
● shapes-monitoring.service - Service for monitoring smart sensors
     Loaded: loaded (/lib/systemd/system/shapes-monitoring.service; enabled; vendor preset: enabled)
     Active: active (exited) since Thu 2021-05-13 10:55:14 CEST; 21min ago
    Process: 20484 ExecStart=/usr/bin/docker-compose up -d (code=exited, status=0/SUCCESS)
   Main PID: 20484 (code=exited, status=0/SUCCESS)

may 13 10:54:56 maria-GP63-Leopard-8RD systemd[1]: Starting Service for monitoring smart sensors...
may 13 10:54:56 maria-GP63-Leopard-8RD docker-compose[20484]: Creating network "compose_default" with the default driver
may 13 10:54:56 maria-GP63-Leopard-8RD docker-compose[20484]: Creating mosquitto ...
may 13 10:54:56 maria-GP63-Leopard-8RD docker-compose[20484]: Creating zigbee2mqtt ...
may 13 10:54:56 maria-GP63-Leopard-8RD docker-compose[20484]: Creating influxdb    ...
may 13 10:55:07 maria-GP63-Leopard-8RD docker-compose[20484]: [153B blob data]
may 13 10:55:14 maria-GP63-Leopard-8RD docker-compose[20484]: [43B blob data]
may 13 10:55:14 maria-GP63-Leopard-8RD systemd[1]: Finished Service for monitoring smart sensors.
{{</shell>}}

This service runs ZigBee2MQTT, Mosquitto, InfluxDB and Telegraf with docker. If you want to stop the service run the command `systemctl stop shapes-monitoring-service`.

**Optional.** When synchronizing the sensors you can rename the devices in the configuration file in the `zigbee2mqtt-data` folder, which is located in the `/etc/compose` directory. An example would be as follows:
{{<staticCode "zigbee2mqtt-data/configuration.yaml">}}

In addition, this package automates the provision of Grafana with data stored in InfluxDB. To create the correct files, `shapes-monitoring-service` must know the database connection parameters, so we must edit its configuration file with `sudo shapes-monitoring-service -e`. A configuration can be:
{{<staticCode "configuration.conf">}}

Then execute the next command to create the database in InfluxDB: 
{{<shell>}}
pi@raspberrypi:~$ sudo shapes-monitoring -u
Connected to http://localhost:8086 version 1.8.5
    InfluxDB shell version: 1.6.4
    > create database zigbee2mqtt
    > use zigbee2mqtt
    Using database zigbee2mqtt
    > create user telegraf with password 'telegraf' with all privileges
    > grant all on zigbee2mqtt to telegraf
    > exit
Database checked!
Done updating!
{{</shell>}}

This will provision our local Grafana instance. If we check [http://localhost:3000](http://localhost:3000) we will see Grafana UI, and following on the side menu *Dashboards > Manage* we can see our provisioned dashboards:
{{<image src="grafana_ui.png">}}

To see what files where provisioned and the generated dashboards information use:
{{<shell>}}
pi@raspberrypi:~$ sudo shapes-monitoring -s
INFO: Datasource provider file at /etc/grafana/provisioning/datasources/shapes-monitoring-datasource.yaml
INFO: Dashboard provider file at /etc/grafana/provisioning/dashboards/shapes-monitoring-dashboard.yaml
INFO: Found dashboard definition at /var/lib/grafana/dashboards/shapes-monitoring-dashboard.json:
INFO: 	Name: smart-sensors
INFO: 	UID: fFBmCCrMz
INFO: Available panels:
INFO: 	Humidity
INFO: 	Temperature
INFO: 	Motion
INFO: 	Door/Window
{{</shell>}}

**NOTE:** If you want grafana to be started on boot, execute `sudo systemctl enable grafana-server.service`.

Once the service `shapes-monitoring-service` is started and the database has been configured, you must run the shapes-monitoring command. This command executes an mqtt client that transforms the values true and false by 1 and 0 in the motion and door and window sensors to stores them in the database. The output of this command would look like this:
{{<shell>}}
pi@raspberrypi:~$ shapes-monitoring-client
Connected to MQTT Broker localhost:1883 with return code 0!
Received {"battery":100,"linkquality":165,"occupancy":true,"voltage":3025} from zigbee2mqtt/motion_xiaomi/SENSOR topic!
Sensor data: {'battery': 100, 'linkquality': 165, 'occupancy': 1, 'voltage': 3025}
Sensor data published!

Connected to MQTT Broker localhost:1883 with return code 0!
Received {"battery":97,"humidity":41.72,"linkquality":168,"pressure":947.7,"temperature":19.62,"voltage":2995} from zigbee2mqtt/temperature_humidity_xiaomi/SENSOR topic!
Received {"battery":100,"contact":true,"linkquality":168,"voltage":3005} from zigbee2mqtt/door_window_xiaomi/SENSOR topic!
Sensor data: {'battery': 100, 'contact': 1, 'linkquality': 168, 'voltage': 3005}
Sensor data published!

Connected to MQTT Broker localhost:1883 with return code 0!
Received {"battery":100,"contact":false,"linkquality":183,"voltage":3005} from zigbee2mqtt/door_window_xiaomi/SENSOR topic!
Sensor data: {'battery': 100, 'contact': 0, 'linkquality': 183, 'voltage': 3005}
Sensor data published!
{{</shell>}}

To view the data transmitted by the sensors go to the Smart Sensors dashboard and you will see something similar to the following image:
{{<image src="smart-sensors_dashboard.png" width="100%">}}