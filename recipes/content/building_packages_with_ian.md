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

Some of the mos relevant commandas that we are going to use are `ian create`, `ian build` and `ian upload`.

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

# Preparing the environment
Before build and upload your package you need to do the following tasks

## Generate a GPG key

Before building the packages, is necessary to generate a `gpg key`. Your package will be signed with your gpg key, and your key will be signed with a repository key, thanks to that
everybody who install the package could check that it belongs to the repository.

In order to create your key you must execute the next command and answer the quetions properly:
{{< shell >}}
$ gpg --full-generate-key
{{< /shell >}}

{{% note warning %}}
Once you have generated the key is very important dont forget the password and dont lose the key, you can expoert your key to a file with the command `$ gpg --export-secret-keys -a <huella_digital> > ~/<nombre-fichero>.asc`.
When you generate your key is recommended a **RSA** with a key size of **4096** bits
{{% /note %}}

## Configure ian

Once generated the gpg key is time to configure `ian`. This step is as simple as modify the file `~.config/ian/config` with the next content (this information have to be the same as the one given to your key).

{{< code config >}}
DEBFULLNAME="Jesús Fernández-Bermejo Ruiz"
DEBEMAIL=jesus.fdezbermejo@gmail.com
DEBSIGN_KEYID=<huella_GPG>
DEBREPO_URL=debrepo@debrepo/shared
EDITOR="emacs -nw"
{{< /code >}}

## Configure dupload

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
{{< /shell >}}

# Building the package

Now, is time to build your package. The first step is create a directory with the named of your package (for example `example-package`) where you are going to save the source files of your program. In this example case, we are going to package a python program that consists in a file named `example.py`.

Once you moved your source code, you have to create a `Makefile`. This `Makefile` must contains two directives, **build** and **install**, build is an optinal directive and contains the actions needed to compile the software, in the other hand, the directive install contains the actions neede to install the executable file of your program.

Due to python is an interpreted programming language, the compilation is not needed, so only the **install directive is mandatory**. The Makefile would be the following.

{{< code make >}}
DESTDIR ?= ~

install:
	install -vd $(DESTDIR)/usr/bin
	install -v -m 555 example.py $(DESTDIR)/usr/bin/example

{{< /code >}}

The directive install performs the following tasks. Firstly if the directory `~/usr/bin` doesn't exist it is created, after that, it copies the python file to the specified directory with the properly permissions (read and execute permissions), converting the word **example** in a command that execute the `example.py` program.

After the Makefile is created, the following step is execute the `ian create` command, this command will create a folder named `debian` with a default information about the package. Some files in this debian directory, like `changelog` and `control` can be modified with the desired information.

Finally executing the command `ian build`, all the files of your package will be created.

# Uploading the package










