---
type: "recipes"
title: "ARCO 101"
description: "Getting started documentation for <b>newcomers</b> to the ARCO Research Group!"
icon: "arco101/images/arco-logo-a.png"
---

## ¡Bienvenido a ARCO!

<br>
En esta sección encontrarás la documentación necesaria para ponerte al día en las mecánicas, proyectos, herramientas y procedimientos que usamos en el
grupo.

Si estas viendo esta información ya has contactado con alguien de nuestro grupo y te ha pasado este enlace. Esa persona
será la que te sirva de guía, pueda resolver tus dudas y será **tu mentor**. El objetivo de esta página es que todos **ahorremos
tiempo y puedas ser productivo lo antes posible**.

Puedes revisar las secciones en el orden que quieras, pero te recomendamos empezar por el principio. Y si tienes alguna duda, **¡pregunta!** . Estamos disponibles en los laboratorios / despachos del grupo, así como en los diferentes canales de [Slack](https://arcotalk.slack.com). `¡Happy hacking!`

La mayoría de los equipos de desarrollo y producción del grupo ARCO son máquinas con GNU/Linux y es donde
más comodo nos sentimos, si no estás familiarizado con este entorno será una de tus primeras cosas que deberías aprender.
 Hay muchos recursos en la web para aprender este tipo de entornos, ponemos algunos para que te hagas una idea de las herramientas que usamos:

  * Entre las distribuciones que mas nos gustan están [Debian](https://www.debian.org/doc/index.es.html) y [Ubuntu](https://help.ubuntu.com/stable/ubuntu-help/index.html), [en este enlace](https://kali.training/topic/introduction-to-apt/) puedes encontrar una buena introducción con ejercicios para que te sientas cómodo gestionando paquetes en estas distribuciones (secciones 8.1.1 y 8.1.2). Esa documentación precisamente pertenece a otra distribución (KALI) por lo que la parte de los repositorios está centrada en esa distribución, aunque vale como concepto, los repositorios de debian y ubuntu son diferentes. **Asegurate de sentirte cómodo instalando/desinstalando paquetes, manejando la consola de comandos, gestionando usuarios de forma básica, moviendote en los directorios/archivos, los permisos de usuarios/archivos, etc.**

  * Todo lo que generes con nosotros debe estar bajo un sistema de control de versiones (actualimente git). Si no estas familiarizado con git y quieres ver de forma simple el flujo de trabajo, [aquí puedes verlo] (https://rogerdudler.github.io/git-guide/).

  * Creamos repositorios en [bitbucket](bitbucket.org) y [github](github.com), **Crea una cuenta en esas web con tu dirección de correo UCLm (@uclm.es o @alu.uclm.es).**

  * El usuario que te asignemos será, por defecto, una máquina GNU/Linux. **Tu mentor se ocupará de crearte un usuario en esa máquina.**

  * Dejamos libre elección en la herramienta de edición de los archivos, si no tienes ninguna preferida, [Visual Studio Code](https://code.visualstudio.com/es) es una buena elección.

  * Usamos [slack](slack.com) para discusiones/coordinarnos. **Crea una cuenta en Slack con tu dirección de correo UCLM (@uclm.es o @alu.uclm.es**.

  * **Escribe un correo al responsable de sistemas (David.Villa@uclm.es) indicando**:
    * Tus usuarios en bitbucket, github y slack.
    * Las cuentas de correo que has usado para crear esas cuentas (aunque aconsejamos usar la de la UCLM).
    * Quién es tu mentor y ponle en copia del correo.
    * En qué proyecto vas a trabajar.

Aunque hay variaciones por distintos motivos (normalmente el proyecto específico), intentamos seguir unas pautas a la hora de codificar, estructurar los repositorios y generar documentación. Salvo indicación contraria del director del proyecto:

* Python es uno de nuestros lenguajes favoritos, si te integras en un proyecto python seguramente ya tiene un repositorio, sigue trabajando con esa estructura. Si vas a crear un módulo esta es una [estructura clásica de un proyecto python](https://docs.python-guide.org/writing/structure/) que te puede servir como referencia. Como guía de estilo de programación, y siempre usando el sentido común, una buena guía es [Pep 8](https://pep8.org/)

* Nunca subas repositorio un fichero generado a partir de otro (binarios, .o , ejecutables, pngs, etc.). Sobre todo al principio ten especial cuidado con esto.

* Lo que subas al repositorio, al menos debe compilar.

* Haz commits/push frecuentes (al menos uno diario).

* Para escribir recetas concretas sobre procedimientos de configuración, herramientas,  documentación técnica, etc sigue [estas instrucciones](https://arcogroup.bitbucket.io/recipes/creating_arco_docs/)


Otros procedimientos muy importantes a tener en cuenta:

* Tu mentor debe dar tu nombre completo al conserje para que puedas recoger la llave del laboratorio.
* No te olvides de cerrar la puerta del laboratorio con llave siempre que se quede vacío.
* Ten tu puesto de trabajo limpio y ordenado.
* Usa auriculares (con un volumen razonable) si te pones música, para no molestar a tus compañeros.
* Si necesitas algún tipo de equipamiento adicional, habla con tu mentor. No cojas nada de otro equipo/proyecto, ni siquiera temporalmente, salvo que tu mentor te de permiso.

Consulta el [manual de procedimientos del grupo](https://bitbucket.org/arco_group/arco.policy/wiki/Home) para información más detallada.
