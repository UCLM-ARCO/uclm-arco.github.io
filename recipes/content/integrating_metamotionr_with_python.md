---
title: "Integrating MbientLab MetaMotionR sensors with Python"
date: 2020-03-30T23:20:43+02:00
draft: false

image: "integrating_metamotionr_with_python/metamotionr.jpeg"
description: "A recipe to learn how to use the MetaMotion library, and read data from the sensors of the MetaMotionR board usin Python"
---

# Overview

MbientLab is a manufacture of different wearable sensors, an example of this sensors
are the **Meta** sensors family, which is formed by different wearable devices like
**MetaTracker**, **MetaMotionC** and **MetaMotionR**. The Meta family devices are formed
by different sensors, like Accelerometers, Gyroscopes, Barometes, etc... In this case, 
we are going to see how to use a Python library for the MetaMotionR, what enable the user 
to read the sensors of this device. The measures that can be read with the Library are 
the **acceleration**, **motion**, **tap**, **step counter** by an Accelerometer and 
the **rotation** by a Gyroscope.

# Ingredients

In order to follow the next recipe you will need the this ingredients:

* A **MetaMotionR** device
* The linux packages `bluez-utils`, `build-essential`, `libboost-all-dev`, `libbluetooth-dev` which
  enables the Bluetooth communication and provides the necessary compilers
* The `metawear` package, a library created for **MbientLab** to read sensors measures
* The `metamotion` library available in [pike][http://pike.esi.uclm.es/]
* Basic knwoledge of `Python3` and `Linux`

In order to install al the dependencias you must execute the next commands

{{<shell>}}
sudo apt install bluez-utils build-essential libboost-all-dev libblueetooth-dev
pip3 install metawear --upgrade
sudo apt install metawear
{{</shell>}}

In the following sections, we are going to explain the two main funcionalities of the library,
read a value from a sensor, and attach a handler to a value produced by the sensor. Almost
every measure of the sensor can be readed, but not all the measures support the use of a
handler.

# Connecting and Reading data from the device

In this section, we are going to explain how you can read the battery from
a MetaMotionR board. The first step is import the `metamotion` library in the 
following way.

{{<code py>}}
#!/usr/bin/python3

from metamotion import MetaMotion
{{</code>}}

{{% note "warning" %}}
In order to **save battery**, the chip is in sleep mode. You must **press** the 
button of the chip before execute the programa to assure the correct operation.
{{% /note %}}

Once you import the library you must instantiate your chip. In order to do that 
you must invocate the constructor `MetaMotion` with the **MAC address** of the
chip, which can be observer in the label of the device. Once the sensos is instanitated
you must call the `connect()` method in order to establish the Bluetooth connection

{{<code py>}}
sensor = MetaMotion("E9:75:41:AF:11:AE")
sensor.connect()
{{</code>}}

Finally you can read any value of the chip, in this case we are going to read the remanent percentage
battery invoking the method `read_battery` of the class. This method returns a struct that is formed 
by two fields, `voltage`, which represents the voltage that receives the chip, and `battery` which 
is the percentage of battery that the device has.

{{<code py>}}
measure = sensor.read_battery()
print(measure.battery)
{{</code>}}

Before the end of the program is important to invoke the `disconnect` method, in order
to disable the sensors and close the communication.

{{<code py>}}
sensor.disconnect()
{{</code>}}

The full code is listed here below

{{<code py>}}
#!/usr/bin/python3

from metamotion import MetaMotion

sensor = MetaMotion("E9:75:41:AF:11:AE")
sensor.connect()

measure = sensor.read_battery()
print(measure.battery)

sensor.disconnect()
{{</code>}}


{{% note "danger" %}}
In some cases, when you are doing the bluetooth connection with this library, the program may 
throw a `buffer overflow detected` error. The source of this error is part of the `metawear` module,
which is a wrapper of the C++ library with the same name created by the manufacturer **MbientLab**, 
and is used to make the connection in the `MetaMotion` library. I'm still working in the solution for this 
problem, if you experience this error, press the button of your sensor and execute yor program again
(sorry for the inconvenience)
{{% /note %}}

# Read a measure from a sensor

Like it was said before, the MetaMotionR device is formed by several sensors. The most important sensors, 
and those to which the library gives access are the **Acceleromter** and the **Gyro** sensor. In order
to access to the measures of this sensors, a `MetaMotion` instance has two attributes representing it, 
`accelerometer` and `gyroscope`. Following, we are going to see an example in which we are going to read
the **acceleration**.

In order to read the acceleration, the first step is to active and initialize the accelerometer. 
For this purpose you must invoke the method `setup_accelerometer` in order
to active and initialize the accelerometer sensor. If you don't do this, and you try to
access the `accelerometer` sensor, an exception will be raised.

{{<code py>}}
sensor.setup_accelerometer()
{{</code>}}

Once initialized the sensor, you can read the acceleration invoking the method form the `accelerometer`
`read_acceleration`.

{{<code py>}}
sensor.accelerometer.read_acceleration()
{{</code>}}

The full code is listed below

{{<code py>}}
#!/usr/bin/python3

from metamotion import MetaMotion

sensor = MetaMotion("E9:75:41:AF:11:AE")
sensor.connect()
sensor.setup_accelerometer()

measure = sensor.accelerometer.read_acceleration()
print(measure)

sensor.disconnect()
{{</code>}}

# Add a handler to a measure

With the MetaMotion library you can read any values produced by the sensor, but this library 
also provides the funcionality to subscribe to a measure and attach a handler. In this way,
if the sensor which we are subscribed send a value, the handler will be executed and will
process the data with the user function.

In this section we are going to see an example where we add a handler to an accelertion sample.

The first step is to declare the handler that will process the data, in this case the handler
is going to print the data. 

{{<code py>}}
def handler(data):
    print(f"The acceleration is: {data.x} {data.y} {data.z}")
{{</code>}}

Once the handler is defined, you must attach it to the acceleration produce by the sensor, so that
when an acceleration measure is received, the user function will be executed. The acceleration is
produced by an **accelerometer** in the **MetaMotionR** device, for this reason, to add the handler
you must call the `on_acceleration` method of the `accelerometer` attribute in your instance of the
`MetaMotion` class. To avoid the end of the program, the method `wait_until_break` must be called.
The full program can be seen below.

{{<code py>}}
#!/usr/bin/python3

from metamotion import MetaMotion

def handler(data):
    print(f"The acceleration is: {data.x} {data.y} {data.z}")

sensor = MetaMotion("E9:75:41:AF:11:AE")
sensor.connect()

measure = sensor.accelerometer.on_acceleration(handler)
sensor.wait_until_break()

sensor.disconnect()
{{</code>}}

# Using a logger

An interesting funcionality of the MetaMotionR devices is the posibility to create loggers. A logges
make the users ables to register a set of data from a sensor in the device, and when they want download
all this data. This is very useful if you don't want to communicate with the sensor in each measure. In
the following example, we are going to create a logger for a **rotation** measure

As in all previous examples, the first step is to made the connection with the sensor. Once the connection
is made, and beacuse we are going to log a measure from the **Gyro**, we must invoke the method `setup_logger`
from the `gyroscope` attribute to initialize the logger. After that, you must invoke the method `start_logging`
in order to start to save the data.

{{<code py>}}
sensor.gyroscope.setup_logger()
sensor.start_logging()
{{</code>}}

Stop the logging is as simple as execute the method `stop_logging`.

{{<code py>}}
sensor.stop_logging()
{{</code>}}

Following, to get the data previously logged, you should invoke the method `subscribe_logged_data` from the **Gyro**
and then invoke the method `download_data`. After this, the library will begin to download the log, to assure that all the data
is downloaded before the program ends you can invoke the `wait_until_download_completed` method. Finally, you can read the complete
log in the variable `rotation_log` from the `gyroscope`.

{{<code py>}}
sensor.stop_logging()
sensor.gyroscope.subscribe_logged_data()
sensor.download_logs()

sensor.gyroscope.wait_until_download_completed()
print("The rotation is ", sensor.gyrocope.rotation_log)
{{</code>}}

The full code is listed bellow

{{<code py>}}
#!/usr/bin/python3

import sys
import time
from metamotion import MetaMotion

sensor = MetaMotion("E9:75:41:AF:11:AE")
sensor.connect()

sensor.gyroscope.setup_logger()
sensor.start_logging()

time.sleep(4) #Wait 4 seconds to let the sensor log data

sensor.stop_logging()
sensor.gyroscope.subscribe_logged_data()
sensor.download_logs()

sensor.gyroscope.wait_until_download_completed()
print("The rotation is ", sensor.gyrocope.rotation_log)

sensor.disconnect()
{{</code>}}

You can use as many logs as you want, but you must follow the next workflow:

1. Connect the sensor
2. Setup the necessary logger in their respective sensor from the chip
3. Start the logging
4. Wait a desired amount of time
5. Stop the logging
6. Subscribe to all the logged data by sensor
7. Download the logs
8. wait for each sensor to download its log data

# MetaMotion API 

For more information you can see the API reference where you can find how to read
all the measures available and the different methods to attach handlers:

* [MetaMotion API Reference](/api/bosch-sensor/)