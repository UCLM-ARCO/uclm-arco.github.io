---
title: "Cómo crear snapshots de vagrant"
date: 2019-08-23T07:52:05+02:00
tags:
- recipe
- vagrant
- devops
draft: false

image: "creating_vagrant_snapshots/vagrant-logo.png"
description:  "Esta receta explica como crear imágenes de un servicio para mantenerlo funcionando siempre."
---

# Ingredientes

Esta receta está pensada para ser usada con un demostrador o servicio que está virtualizado usando [Vagrant](https://www.vagrantup.com/). Partiremos asumiendo que el servicio está correctamente desplegado y funcionando.

# Crear el `box` de vagrant

Para crear el *box*, solo es necesario ejecutar el siguiente comando:

{{< shell >}}
$ vagrant package --output my-application.box
{{< /shell >}}

Esto generará un fichero con el nombre indicado, que podrá ser usado para **crear nuevas instancias** de este Vagrant. El fichero contiene el sistema entero tal y como esta en este momento, por lo que quizá ocupe más de 1 GiB (dependiendo del contenido de la VM).

Crea una carpeta en algún sitio, y ponle como nombre la fecha de hoy. Utilizaremos esa carpeta para guardar todo lo relacionado con esta instantánea. Mueve el `.box` a esa carpeta:

{{< shell >}}
$ mkdir 20190823
$ mv my-application.box 20190823
{{</ shell >}}

Ahora necesitarás **incluir** el `Vagrantfile`: cópialo en la carpeta que has creado. Y dado que el despliegue ya está hecho dentro de la VM, es necesario que **comentes** (o borres) las líneas relacionadas. En mi caso:

{{< code rb Vagrantfile >}}
...
# config.vm.provision :ansible, playbook: "provision.yaml"
...
{{</ code >}}

También deberás cambiar la **configuración relativa al box** que vas a emplear. Debes indicarle que será el que hay en esta misma carpeta (y que acabas de crear). Para ello, edita la siguiente línea (usa el nombre de tu `.box`):

{{< code rb Vagrantfile >}}
config.vm.box = "my-application.box"
{{</ code >}}

# Reunir las dependencias

Si bien es cierto que esta máquina no necesita despliegue, es probable que tengas algún **fichero compartido** en el host. Por ello, asegúrate de copiar al directorio donde está el `Vagrantfile` todos aquellos ficheros que necesites. Por ejemplo, si mi maquina necesita un fichero llamado `Makefile`, que asumo que se montará en `/vagrant` automáticamente, es necesario que lo copie al directorio destino:

{{< shell >}}
$ cp Makefile 20190823
{{</ shell >}}

Por otro lado, verifica que haces lo mismo con aquellas **carpetas** que se han **compartido de forma explícita**. Por ejemplo, si tienes un `synced_folder "src"` en tu `Vagrantfile`, copia ese directorio también (y si la ruta era relativa y en niveles superiores, actualiza el `Vagrantfile` como veas apropiado)

Una vez hecho esto, **comprueba que todo funciona** correctamente arrancando la máquina desde el directorio que has creado, y verificando que los servicios / dependencias se cumplen.

# Indicar que el servicio tiene *snapshots*

Lo siguiente que debes hacer es **guardar el directorio** que has creado en algún sitio accesible. De esa forma, estará disponible cuando haga falta. Y por último, debes indicar que este servicio está **disponible en una instantánea**. Para ello, añade al repositorio el siguiente fichero, y modifícalo para incluir la ruta adecuada:

{{< code txt SNAPSHOTED >}}
This repository (with its deployed services on Vagrant) has been SNAPSHOTED,
which means that there are one or more images with an already deployed state.
The goal of this is to get an always-running system, despite software updates or
source changes.

Please, see the following location to get the snapshots:

* ssh://arco.esi.uclm.es/users/oscar.acena/time-machine/my-application
{{</ code >}}

# Referencias

* [Vagrant: Creating a Base Box](https://www.vagrantup.com/docs/virtualbox/boxes.html)


