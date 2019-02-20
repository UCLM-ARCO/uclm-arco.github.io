---
date: "2017-07-05T14:48:22+02:00"
title: "How-to install IDM"
linktitle: "How-To install IDM"
weight: 0
draft: false

section: "examples"
---
<br>

IDM installation
----------------

**Note:** this post will show you how to install IDM on a **Debian**
based machine (like Ubuntu). The following instructions were tested on
Debian 9.0 (stretch) y Ubuntu 17.04 (zesty). If you use
[Vagrant](https://www.vagrantup.com/), you can download our
{{< staticLink "idm/code/install/Vagrantfile" VagrantFile >}} for both
setups.

Setting up Debian repositories
------------------------------

IDM is located on [ARCO's pike](http://pike.esi.uclm.es)
repository. To install it, run the following command:

{{< shell >}}
$ wget -qO- 'http://pike.esi.uclm.es/add-pike-repo.sh' | sudo sh
{{< /shell >}}

Installing ZeroC Ice for Python3
--------------------------------

With a **fresh** installation of Debian 9.0 or Ubuntu 17.04, you can
install ZeroC Ice for Python 3 directly:

{{< shell >}}
$ sudo apt-get install python3-zeroc-ice
{{< /shell >}}

For older versions or another systems, you can follow the official
installation instructions: [Ice
Distributions](https://zeroc.com/distributions/ice).

Installing IDM
--------------

Also, very straightforward:

{{< shell >}}
$ sudo apt-get install idm
{{< /shell >}}

Check that it works
-------------------

To check if your setup is working, download the following
{{< staticLink "idm/code/install/router.config" "configuration file:" >}}

{{< staticCode "router.config" sh >}}

And launch the IDM router using:

{{< shell >}}
$ idm-router --Ice.Config=router.config
WARNING:root: router controller not defined
WARNING:root: DUO advertisements disabled
INFO:root: queue Size: 10
INFO:root: router at '10AA01 -t -e 1.1:tcp -h 127.0.0.1 -p 6140 -t 60000'
INFO:root: waiting events...
{{< /shell >}}

If you see the same (or similar) result, then **it works!**

Using Vagrant
-------------

The provided {{< staticLink "idm/code/install/Vagrantfile" VagrantFile >}}
has two configured boxes, and comes bundled with a _bootstrap_ script
to install the required packages. Download it, and launch one of the
included configurations (```debian``` or ```ubuntu```):

{{< shell >}}
$ vagrant up debian
{{< /shell >}}

When finished, you can **enter** the box and check if IDM is correctly
installed (using the above instructions):

{{< shell >}}
$ vagrant ssh debian
[...]
{{< /shell >}}

<br>
