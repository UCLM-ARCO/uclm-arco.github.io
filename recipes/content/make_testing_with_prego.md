---
title: "Make testing with Prego"
date: 2019-03-21T09:39:16+01:00
tags:
- recipe
- prego
- python
- testing
- integration
draft: false

image: "make_testing_with_prego/testing.jpeg"
description:  "A simple description of how to install and use prego to make system/integration tests"
---

# Overview

**Prego** is a library consisting on a set of clases and hamcrest matchers usefull to specify shell command interactions through files, environment variables, network ports.

The main concept in prego is the `Task()`. A task is a set of assertions with three different checkers:

1. **task.assert_that**, for single shot checking
2. **task.wait_that**, for polling recurrent checking
3. **task.command**, to run arbitrary shell command

A task also counts with two assertions asociated, `runnning()` and `terminated()`, which check if the task is working or has finished respectively.

Prego also counts with `context`, an object whose attribute may be aytomatically interpolated in command and filename paths. Some of the attributes are set as default values for `command()` parameters.

# Set up the environment

In this recipe the following elements are needed:

- The `python3-prego` package, both available at [Pike](http://pike.esi.uclm.es/)'s repository
- The `python3-nose`, `python3-hamcrest` packages 
- A Debian/Ubuntu Linux distribution
- Basic knowledge of python and shell comands

If you don't count with **Pike** in the repository list of your machine you **must add it**.

Following, you must install the packages previously mentioned with the next command.

{{< shell >}}
$ sudo apt install python3-prego python3-nose python3-hamcrest smart-transducer
{{< /shell >}}

# A simple example

In order to make a simple example we are going to use a tool named `ncat` that will permit us to emulate a system with a server and a client. To perform the testing with prego we must define tasks, in this case can be differentiated two different tasks, the server and the client. The purpose of our emulate system will be that the client will send the command `echo testing` to the server, what means that the server must show in the stdout the message *"testing"*. Once the funcionality of the system is clearly is time to make the test. 

First of all, the context object can be used to set default values. In this case, a variable **port** can be defined, representing the port where the server will be listening. To access the value of the variable you have to use the `$` character.

{{< code py >}}
prego.context.port = 2000
{{< /code >}}

The next step is create the server task. As you can see in the snippet of code, first the task is initialized, and in this case we say to prego to launch the task in background (`detach=true`), if we don't do it, the server will block the execution and the task of the client will never be executed. Once the task is defined, a command is assigned to it. The variable `cmd` will be usefull to check the stdout of the server. 

{{< code py >}}
server = prego.Task(desc='ncat server', detach=True)
cmd = server.command('ncat -l -p $port')
{{< /code >}}

The task of the client will be very similar, the main differences are that the client won't be detached, and obviously the command that will be assigned to the task will be different. 

{{< code py >}}
client = prego.Task(desc='ncat client')
client.wait_that(server, prego.running())
client.command('ncat -c "echo testing" localhost $port')
{{< /code >}} 

The task of the client make a polling checking with `wait_that`, thanks to this the task of the client won't be executed until the server is running.

Finally, is time to check if the server has received the message from the client. Thanks to **hamcrest** and with the variable `cmd` previously declared, we can compare the stdout of the server with an arbitrary `string`.

{{< code py >}}
server.assert_that(cmd.stdout.content, hamcrest.contains_string('testing'))
{{< /code >}} 

# Executing a test

The code of the test should look similar to this.


{{< staticCode "test.py" >}}

Execute the test is as simple as type the following in the shell.

{{< shell >}}
$ nosetests3 test.py
{{< /shell >}}

If all is correct and the test pass, your shell must show the message **OK**.

{{< shell >}}
.
----------------------------------------------------------------------
Ran 1 test in 0.228s

OK

{{< /shell >}}
 








