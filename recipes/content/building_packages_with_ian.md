---
title: "Building Packages with Ian"
date: 2019-05-13T10:38:15+02:00
tags:
- debian
- packages
- ian
draft: false

image: "building_packages_with_ian/deb-package.png"
description: "Recipe explaining how to build and upload a debian package to a package repository"
---

# Overview

**Ian** is a very useful tool that facilitates the task of builiding and uploading packages to a repository. Building packages in debian could be a very annoying task, thanks to Ian, you can build your package barely with a few commands.

Some of the mos relevant commandas that we are going to use are `ian create`, `ian build` and `ian upload`. In this recipe the focus will be put in how to build a package, at the end of the recipe a link you can find a link to a recipe that explain how tu upload a package to the arco repository.

# Ingredients

In order to follow this recipe, you will need to satisfy the following requirements:

- The `gpg` and `ssh` package.
- The `ian` package.
- A Debian/Ubuntu Linux.
- Basic knowledge of Linux and Makefiles.

In order to install the tool `ian` you must execute the following command.

{{< shell >}}
$ sudo apt install ian
{{< /shell >}}

# Configure Ian
<!-- ## Generate a GPG key

Before building the packages, is necessary to generate a `gpg key`. Your package will be signed with your gpg key, and your key will be signed with a repository key, thanks to that
everybody who install the package could check that it belongs to the repository.

In order to create your key you must execute the next command and answer the quetions properly:
{{< shell >}}
$ gpg --full-generate-key
{{< /shell >}}

{{% note warning %}}
Once you have generated the key is very important dont forget the password and dont lose the key, you can expoert your key to a file with the command `$ gpg --export-secret-keys -a <huella_digital> > ~/<nombre-fichero>.asc`.
When you generate your key is recommended a **RSA** with a key size of **4096** bits
{{% /note %}} -->

The first step after the installation of ian is to configure it. This step is as simple as modify the file `~.config/ian/config` with the next content. AS you can see, is required a gpg key, to sign the packages built.

{{< code config >}}
DEBFULLNAME="Jesús Fernández-Bermejo Ruiz"
DEBEMAIL=jesus.fbermejo@gmail.com
DEBSIGN_KEYID=<gpg_key>
DEBREPO_URL=debrepo@debrepo/shared
EDITOR="emacs -nw"
{{< /code >}}

<!-- ## Configure dupload

**Dupload** is a tool used to upload a package to a repository, when you execute `ian upload`, ian uses this tool to upload the package. Once installed, in order to configure this package you must create the file `~/.dupload.conf` with the following content:

{{< code config >}}
package config;

$default_host = "debrepo";

$cfg{'debrepo'} = {
    fqdn => "debrepo",
    login => "repouser",
    method => "scpb",
    incoming => "/shared/incoming/",

    # The dinstall on ftp-master sends emails itself
    dinstall_runs => 1,
};

$postupload{'changes'} = 'ssh debrepo "reprepro -V -b /shared processincoming sid-process"';

1;  # DO NOT remove this line!
{{< /code >}}

## Generate a SSH key

In order to upload your package to the repository you need to connect to it by ssh, so you need a ssh key. To create the key you must execute the next command.

{{< shell >}}
$ ssh-keygen -b 4096
{{< /shell >}} -->

# Preparaing the building

Now, is time to build your package. The first step is create a directory with the named of your package (for example `example-package`) where you are going to save the source files of your program. In this example case, we are going to package a python program that consists in a file named `example.py`.

Once you moved your source code, you have to create a `Makefile`. This `Makefile` must contains two directives, **build** and **install**, build is an optinal directive and contains the actions needed to compile the software, in the other hand, the directive install contains the actions needed to install the executable file of your program.

Due to python is an interpreted programming language, the compilation is not needed, so only the install directive is mandatory. The Makefile would be the following.

{{< code make >}}
DESTDIR ?= ~

install:
	install -vd $(DESTDIR)/usr/bin
	install -v -m 555 example.py $(DESTDIR)/usr/bin/example

{{< /code >}}

The directive install performs the following tasks. Firstly if the directory `~/usr/bin` doesn't exist it is created, after that, it copies the python file to the specified directory with the properly permissions (read and execute permissions), converting the word **example** in a command that execute the `example.py` program.

After the Makefile is created, the following step is execute the `ian create` command, this command will create a folder named `debian` with a default information about the package. In the `debian` directory is saved all the information required to build the package, and is formed by different files and directories, some of them optionals. The most important files are the following:

## control

This file saves all the information that help other tools to manage the package. An example of a control file can be found bellow.

{{< code bash >}}
Source: metamotion
Section: python
Priority: optional
Maintainer: Jesús Fernández-Bermejo Ruiz <jesus.fruiz@uclm.es>
Build-Depends: debhelper (>= 12), quilt, python3, libbluetooth-dev, dh-python
Standards-Version: 4.3.0

Package: metamotion
Architecture: all
Depends: ${misc:Depends}, python3, ${python3:Depends}
Description: Python library to control MetaMotionR devices
 This package present a useful library that enable the users
 to request differente data from the MetaMotionR sensor, like
 the acceleration and rotation of the sensor.
{{< /code >}}

In the first seven lines, is added the control information of the package. The firs line indicates the name of the source package, the second the section inside the distribution where the package can be found, some section examples are devel for programming tool, main for open source programs, non-free for propietary code... etc. The tird line describes the relevance of the package for the user, the fourth the maintainer of the package and the fifth the list of packages needed to build our package.

In the second paragraph, the first line is the name of the binary package, the second the architectures where the package can be compile, the third line is a list of packages needed for our package instalation, if some of that packages are not installed, our packaged will not be installed. In the fourth line, is added a short and a long description.

## copyright

This file contains information about the resources, license and copyright of the original sources of the package, in a simple use, isn't necessary to modify this file.

## changelog

This is a mandatory file with a special format, and also a good place where document all the changes on your source code. A simple example of a `changelog` is the following

{{< code bash >}}
gentoo (0.9.12-1) unstable; urgency=low

 * Initial Release. Closes: #12345
 * This is my first Debian package.
 * Adjusted the Makefile to fix $(DESTDIR) problems.

-- Josip Rodin <joy-mg@debian.org>  Mon, 22 Mar 2010 00:37:31 +0100
{{< /code >}}

In the first line, and in this order, is presented the name of the package, its version, the distribution where the pacakge can be installed (should be unstable) and the urgency. Following, you can see a log entry with the changes for the new version of the package and at the end, information about the author of the changes and a timestamp.

## rules

This is a `Makefile` that will be executed at the time to build the package. 


# Building the package
Finally and once all the information is added to debian directory, builduing the package is as simple as executing the command `ian build -c`. After this command, all the files of your package will be created. If you want to know more about ian, the command `ian help` will tell you all the options that this tool have. If you want to know more about building debian packages I recommend you to read the [debian maintainer's guide](https://www.debian.org/doc/manuals/maint-guide/) and specially the fourth chapter.

# Uploading the package

For the package upload, you can find all the information about how to upload a package to the ARCO's repository [here](https://github.com/UCLM-ARCO/debian/wiki/Upload-through-local-repo). You need to have a GitHub account with permission. 








