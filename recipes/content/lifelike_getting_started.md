---
title: "Lifelike: getting started using ST interfaces"
date: 2019-03-05T16:20:02+01:00
tags:
- recipe
- smart home
- lifelike
- st
- domotics
draft: false

image: "lifelike_getting_started/smart-home.jpg"
description: "First steps of using the <code>ST</code> interfaces and Lifelike&trade; tools."
---

# Overview

Lifelike is a **platform** for building *Smart Home* solutions the easy way. It uses very simple interfaces, with a `push` model, to **acquire** sensor information and also to **change** the state of actuators. This recipe will analyze **those interfaces** and how to use them.

# Ingredients

In order to follow this recipe, you will need to satisfy the following **requirements**:

* The `lifelike` package, available at [Pike](http://pike.esi.uclm.es/)'s repository.
* **Python** skills (search in [python.org](https://www.python.org/) for tutorials if needed).
* Basic understanding of the [ZeroC](https://zeroc.com/products/ice) Ice midleware.
* Debian/Ubuntu **Linux** and a terminal.

If you **already added** the Pike repository to your system, then just install `lifelike`:

{{< shell >}}
$ sudo apt install lifelike
{{< /shell >}}

# `ST` description

The `ST` module provides a set of **interfaces**, written in [ZeroC Slice IDL](https://doc.zeroc.com/ice/3.7/the-slice-language), specifically designed to be **simple**, and aimed to build sensors and actuators for a smart environment.

The **definition** of the module is as follows:

{{< staticCode "st.ice" >}}

{{% note primary %}}
Please, **don't download** this interface for deployment. It is provided by the `lifelike` package (usually at `/usr/share/slice/st`). Use it instead.
{{% /note %}}

It has an interface for every **basic type** (`bool`, `byte`, `float`, `string`...), with a single method, `set`, which accepts two arguments:

* `value`, of a specific type (determined by the interface).
* `sourceAddr`, a string with the source address of the **caller**.

These interfaces could be **implemented by actuators** and **used by sensors**. For instance, a door controller would implement the `IBool` interface to lock/unlock the door. Or a temperature sensor could use the `IFloat` interface to **publish** its temperature to some subscriber.

The last interface, `Linkable` is used to *register* an observer to some *observable* object. For instance, the temperature sensor could implement this interface, so when a service wants to **receive the temperature**, it could call to `linkto()` with a proxy (or a valid IDM address) to a servant that implements the **same interface** that the temperature sensor uses (in the former example, `IFloat`). The sensor usually will **store that proxy**, and whenever it wants to publish the temperature, it calls the `set()` method on it.

Let's use them on a **very simple** example.

# Minimal Service

Imagine that you want to create a **clock service**. It has a method that allows the user to **change** the current time, and it also **publishes** the updated time every five seconds.

This clock service will implement the interface `IString` to set the current hour. Also, it implements the `Linkable` interface, so other services could attach theirs observers. And it will use the same interface `IString` to publish the updated time.

# Minimal Client

# Minimal Observer

# Using the `st-client` tool

