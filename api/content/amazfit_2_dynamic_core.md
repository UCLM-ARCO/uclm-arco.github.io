---
#
# WARNING: this is a generated file, modifications will be DISCARDED!!
#
# used command:
# ad-gen-py-api
#    --module-dir "." 
#    --title "Amazfit 2 Chip API Reference" 
#    --image "smart-chip-2.jpg" 
#    --description "Python library to connect with the Amazfit Smart Core 2" 
#    "amazfit" 
#

date: 2020-01-24T14:55:45+01:00
comment: "Documentation generated by Arco Docs Python API generator"
title: "Amazfit 2 Chip API Reference"
description: "Python library to connect with the Amazfit Smart Core 2"
image: "smart-chip-2.jpg"
---

# Amazfit 2 Chip API Reference

## class index:
* <a href="#anchor-class-Core2">Core2</a>

<hr>

# <span class="api-module">amazfit</span> API Documentation

## Class <span class="api-class">Core2</span> {#anchor-class-Core2}

This class provides access to the **Huami Smart Chip** (version 2),
which could be found on Amazfit or Xiaomi sneakers. Use this class to
discover a new device, connect and pair to it, retrieve device
information, orientation, steps, etc.

For some of the properties, it also supports a push mechanism which
allows to receive **notifications** when the property changes.


### <span class="api-prop">`battery`</span>
[*read-only property*] Holds the current **battery level**, in
percentage, of the device. It will retrieve it (synchronously) when used
(no caching).

**NOTE**: due to a **bug** in the device firmware, this characteristic
**may not** return any payload. In that case, `None` will be returned (an
error will also be logged).


### <span class="api-prop">`current_time`</span>
[*read-only property*] It returns the current **date and time**
configuration of the device, in **string** format. It will retrieve it
(synchronously) when used (no caching).


### <span class="api-prop">`orientation`</span>
[*read-only property*] Returns the **IMU orientation** readings of the
device. It is a **tuple** with two fields, one for each axis.

*FIXME: semantic of these fields is to be determined!*


### <span class="api-prop">`steps`</span>
[*read-only property*] Provides the current **step count** of the device. It
will retrieve it (synchronously) when used (no caching).


### <span class="api-func">`__init__`</span>

```py
def __init__(self, address, name='', key=None)
```

<br>Smart Chip (a.k.a **Dyanmic Core 2**) constructor. This is the basic
object you need to instantiate. It could be created **directly**
(providing the device address) or performing a **discovery** with the
`.find()` method (explained later).

**Parameters:**

* `address`, the **Bluetooth** MAC address of the device to connect. It
  should be in the form *aa:bb:cc:dd:ee:ff* (case insensitive).
* `name`, *[optional]* the name of the device. It also could be obtained
  later from the device itself.
* `key`: *[optional]* the encription **key** (AES/ECB) used to pair with
  the device. If not provided, it will be **generated** randomly.


### <span class="api-func">`connect`</span>

```py
def connect(self)
```

<br>Try to connect to the device, using the provided address. Returns when
then connection is **established**. If the device is not present (before
the *timeout* expires), it will raise an exception. This method **must**
be called **before** any other operation.


### <span class="api-func">`find`</span>

```py
def find(cls, timeout=5)
```

<br>**Classmethod** to search compatible devices. It will return the
**first** found.

**Parameters:**

* `timeout`: (default: 5), maximum **elapsed time** to wait for devices to
  answer discovery.


### <span class="api-func">`find_all`</span>

```py
def find_all(cls, timeout=5)
```

<br>**Classmethod** to search compatible devices. It will return **all** the
devices found.

**Parameters:**

* `timeout`: (default: 5), maximum **elapsed time** to wait for devices to
  answer discovery.


### <span class="api-func">`fire_orientation_read`</span>

```py
def fire_orientation_read(self)
```

<br>Forces a **new read** on the orientation characteristic. You should call
`on_orientation()` before, in order to setup a notification **callback**.


### <span class="api-func">`on_orientation`</span>

```py
def on_orientation(self, cb)
```

<br>Enables notifications on **orientation handle**, and configures a
callback to be called when a **new** notification arrives.

**Parameters:**

* `cb`: the **function or method** to be called. The signature should be
  as follows: `def callback([self,] axis)`

**NOTE:** the orientation characteristic **will not** issue new
notifications when the value has changed. To receive them, you should
force a new event with the `fire_orientation_read()` method.


### <span class="api-func">`on_steps`</span>

```py
def on_steps(self, cb)
```

<br>Enables notifications on **step readings**, and configures a callback to
be called when a **new** notification arrives.

**Parameters:**

* `cb`: the **function or method** to be called. The signature should be as
  follows: `def callback([self,] steps)`


### <span class="api-func">`pair`</span>

```py
def pair(self)
```

<br>Bond with this device. It will exchange the private key used to cipher
messages with the chip (although it is **not** necessary for every
operation). This procedure also will **reset the step counter** in the
device.


### <span class="api-func">`read_info`</span>

```py
def read_info(self)
```

<br>Retrieve all the **static information** provided by the device. After
calling this method, you can access the following attributes:

* `name`: the device **name** (if provided on constructor, it will be
  updated)
* `appearance`: a **number** which identifies the type of this device
  (like '*Generic Phone*', '*Sports Watch*' or '*Heart rate Sensor*').
* `privacy_flag`: indicates if this device has **privacy** enabled or
  disabled.
* `min_connection_int`: Preferred connection parameter, **BLE** minimum
  connection interval.
* `max_connection_int`: Preferred connection parameter, **BLE** maximum
  connection interval.
* `slave_latency`: Preferred connection parameter, **BLE** Slave
  latency.
* `timeout_mult`: Preferred connection parameter, **BLE** connection
  supervision timeout multiplier.
* `service_changed_start`: Start of affected **GATT** attribute handle
  range.
* `service_changed_end`: End of affected **GATT** attribute handle
  range.
* `system_id`: This is a 64-bit structure which consists of a 40-bit
  **manufacturer-defined** identifier concatenated with a 24 bit unique
  Organizationally **Unique** Identifier.
* `serial_number`: The serial number of this device.
* `hardware_rev`: Hardware revision number.
* `software_rev`: Software revision number.
* `vendor_src`: Bit which identifies the source of the Vendor ID:
  Bluetooth SIG or USB implementer.
* `vendor_id`: Identifies the product **vendor** from the namespace in
  the Vendor ID Source
* `product_id`: Manufacturer managed identifier for this product
* `product_ver`: Manufacturer managed **version** for this product


### <span class="api-func">`wait_until_break`</span>

```py
def wait_until_break(self)
```

<br>**Blocks** the calling thread execution until it receives a
`KeyboardInterrupt` (Ctrl+C) Use it (or use instead an **event loop**) to keep
the application **running** while you wait for incoming events.

