---
title: "Integrating Xiaomi Mi Band 4 devices with smart mirror"
date: 2020-12-14T16:17:39+01:00
draft: false
tags:
- xiaomi
- miband4
- python
- magic mirror
- grafana
- bluetooth

image: "integrating_miband_with_smart_mirror/icon.png"
description: "A recipe to learn how to collect data from Xiaomi Mi Band 4 devices and visualize it in a smart mirror."
---

## Overview

Mi Band 4 is the most popular and best-selling smart band of the famous IT company Xiaomi. It provides several information about our physical activity like number of steps or heart rate. This device is linked with our smartphone, which access to its data using Bluetooth LE. Using this mechanism, and with a Raspberry Pi transformed into a *smart mirror*, we can monitorize this information and present it with graphs and other intuitive forms of displaying its progression through time.

## Ingredients

In order to follow this recipe you will need:

* A Xiaomi Mi Band 4 device or **MB4**.
* A Raspberry Pi 4 or **RPi4**.
* A PostgreSQL database with remote connections allowed. It could be located on the *cloud* or run locally with [the proper configuration](https://support.plesk.com/hc/en-us/articles/115003321434-How-to-enable-remote-access-to-PostgreSQL-server-on-a-Plesk-server-).
* The `miband-dc` package on your RPi4, which is our data collector, available in [ARCO package repository](https://uclm-arco.github.io/debian/).
* The `miband-grafana` package on your PC, which will configure Grafana, also available in [ARCO package repository](https://uclm-arco.github.io/debian/).
* Basic knowledge of `linux`.

## Configuring `miband-dc`

The data collector, or `miband-dc`, is a service running *always* in our device with `systemd`, we can see its status with the following command:

{{<shell>}}
pi@raspberry:~/$ systemctl status miband-dc.service
● miband-dc.service - miband-dc service
     Loaded: loaded (/lib/systemd/system/miband-dc.service; enabled; vendor preset: enabled)
     Active: active (running) since Wed 2020-12-02 19:08:34 CET; 4s ago
   Main PID: 22377 (miband-dc)
      Tasks: 3 (limit: 9362)
     Memory: 12.6M
     CGroup: /system.slice/miband-dc.service
             └─22377 /usr/bin/python3 /usr/bin/miband-dc -d devices.csv -c settings.json

dic 02 19:08:34 raspberry systemd[1]: Started miband-dc service.
{{</shell>}}

But it needs configuration to work properly.

### Database

The service needs a database to store the collected data. It reads the connection parameters from the file `/usr/share/miband-dc/settings.json`. An example is:

{{<staticCode "settings.json">}}

It is really important that the database user has, at least, CONNECT and INSERT privileges. 

### Devices

At start, it will not have any device configured, so add them to `/usr/share/miband-dc/devices.csv`. For example we have this configuration:

{{<staticCode "devices.csv">}}

In order to get the device token, you can use [Huami token script](https://github.com/argrento/huami-token) and then paste it to the file. To sum up:

1. Link your MB4 to Xiaomi MiFit app.
2. Use huami_token script to get the token with your linked account credentials.
3. Do not reset your MB4 nor unlink it from your account.

### Restart service

After all wanted modifications, restart the service and check its status:

{{<shell>}}
pi@raspberry:~/$ sudo systemctl restart miband-dc.service
pi@raspberry:~/$ systemctl status miband-dc.service
● miband-dc.service - miband-dc service
     Loaded: loaded (/lib/systemd/system/miband-dc.service; enabled; vendor preset: enabled)
     Active: active (running) since Wed 2020-12-02 19:14:47 CET; 7s ago
   Main PID: 23417 (miband-dc)
      Tasks: 3 (limit: 9362)
     Memory: 12.6M
     CGroup: /system.slice/miband-dc.service
             └─23417 /usr/bin/python3 /usr/bin/miband-dc -d devices.csv -c settings.json

dic 02 19:14:47 raspberry bash[23417]: INFO:mi band 1: init dev, mac: E3:49:01:ED:00:A6, static-id: None
dic 02 19:14:47 raspberry bash[23417]: DEBUG:TaskRunner: remaining tasks: 0
dic 02 19:14:47 raspberry bash[23417]: INFO:TaskRunner: run task: <Task: DiscoverTask>
dic 02 19:14:47 raspberry bash[23417]: INFO:DiscoverTask: start discovery process...
dic 02 19:14:54 raspberry bash[23417]: INFO:DiscoverTask: - discovered 1 devices
dic 02 19:14:54 raspberry bash[23417]: INFO:DiscoverTask: - got device: <MiBandDev, mac: E3:49:01:ED:00:A6, status: online>
dic 02 19:14:54 raspberry bash[23417]: DEBUG:TaskRunner: remaining tasks: 0
dic 02 19:14:55 raspberry bash[23417]: INFO:TaskRunner: run task: <Task: RunFunctionTask-update_static>
dic 02 19:14:55 raspberry bash[23417]: INFO:mi band 1: connecting to 'E3:49:01:ED:00:A6'...
{{</shell>}}

With the service running, the daemon is storing the MB4 data on the database provided.

## Configuring `miband-grafana`

This package exists to automate the Grafana provision with the data stored by `miband-dc`. In order to create the correct files, `miband-grafana` must know the connection parameters of the database, so we must modify its configuration file `/usr/share/miband-grafana/configuration.conf`. For example:

{{<staticCode "configuration.conf">}}

It is really important that the database user has **only** CONNECT and SELECT privileges, because the queries executed by Grafana cannot be filtered so any user logged on Grafana UI with admin rights could manipulate the database.

Then execute:

{{<shell>}}
user@pc:~/$ sudo miband-grafana -u
{{</shell>}}

This will provision our local Grafana instance. If we check [http://localhost:3000](http://localhost:3000) we will see Grafana UI, and following on the side menu *Dashboards > Manage* we can see our provisioned dashboards:

{{<image src="grafanaui-dashboards.png">}}

To see what files where provisioned and the generated dashboards information use:
{{<shell>}}
user@pc:~/$ sudo miband-grafana -s
{{</shell>}}

## Setting up the smart mirror

To transform our RPi4 in a smart mirror we will use [Magic Mirror](https://magicmirror.builders/). Following its [installation instructions](https://docs.magicmirror.builders/getting-started/installation.html), execute:

{{<shell>}}
pi@raspberry:~/$ curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
pi@raspberry:~/$ sudo apt install -y nodejs
pi@raspberry:~/$ git clone https://github.com/MichMich/MagicMirror
pi@raspberry:~/$ cd MagicMirror
pi@raspberry:~/MagicMirror/$ npm install
pi@raspberry:~/MagicMirror/$ cp config/config.js.sample config/config.js
{{</shell>}}

Once this is done, it is time to add our Grafana module for Magic Mirror, called [MMM-GrafanaEmbed](https://bitbucket.org/arco_group/mmm-grafanaembed):

{{<shell>}}
pi@raspberry:~/MagicMirror/$ cd modules
pi@raspberry:~/MagicMirror/modules/$ git clone https://bitbucket.org/arco_group/mmm-grafanaembed.git MMM-GrafanaEmbed
{{</shell>}}

Finally, we change Magic Mirror configuration `/MagicMirror/config/config.js` adding MMM-GrafanaEmbed parameters on `modules` list. For example:

{{<code js>}}
modules: [
    // Rest of modules configuration
    {
        module: 'MMM-GrafanaEmbed',
        position: 'bottom_center',
        config: {
            host: "192.168.0.22",   // Our PC IP address
            dash_id: 'c9b38467-5d9a-3c27-918d-cfa1bd575db2',    // Dashboard uid gotten from miband-grafana
            dash_name: 'mi-band-data-user-1',    // Dashboard name gotten from miband-grafana
            panel_id: 11    // Panel id for number of steps gotten from miband-grafana
        }
    }
]
{{</code>}}

Then we run the following command and we will bring our Grafana panel to Magic Mirror:

{{<shell>}}
pi@raspberry:~/MagicMirror/$ npm run start
{{</shell>}}

{{<image src="magicmirror-configured.png">}}

## References

* [Grafana](https://grafana.com/)
* [Magic Mirror](https://magicmirror.builders/)
* miband-dc(1)
* [`miband-grafana` documentation](https://bitbucket.org/arco_group/miband-grafana/src/master/README.md)
* [MMM-GrafanaEmbed documentation](https://bitbucket.org/arco_group/mmm-grafanaembed/src/master/README.md)