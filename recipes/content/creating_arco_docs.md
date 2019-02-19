---
title: "Cómo crear documentación"
date: 2019-02-18T14:25:28+01:00
tags:
- recipe
draft: true

image: "create-document.png"
description: "En esta receta se describen los pasos necesarios para usar el <b>repositorio de documentación</b> de Arco."
---

# Introducción

La documentación del grupo Arco se genera mediante una herramienta de gestion de **sitios estáticos**, llamada [Hugo](https://gohugo.io/), y se mantiene en una serie de repositorios. Para añadir o editar cualquier contenido, se ha de **conocer** esta estructura, y se deben utilizar las herramientas **adecuadas**.

El objetivo de esta receta es proporcionar una **visión global**, y algunos mecanismos para llevar a cabo la tarea de documentar. Si necesitas más información, puedes visitar la página de [documentación de Hugo](https://gohugo.io/getting-started/).

# Configuración del entorno

Antes de nada, para poder llevar a cabo esta receta necesitarás:

* Acceso al repositorio de documentación
* Una instalación **actualizada** de GNU/Linux
* Manejo **básico** de la terminal

Para empezar, instala los paquetes necesarios:

{{< shell >}}
$ sudo apt install git hugo
{{< /shell >}}

Recuerda que la versión de Hugo que instales ha de ser `>= 0.40`, lo que puedes comprobar con el siguiente comando:

{{< shell >}}
$ hugo version
{{< /shell >}}

Para poder trabajar con la documentación de Arco, en primer lugar **descárgate el repositorio**:

{{< shell >}}
$ git clone git@bitbucket.org:arco_group/doc.git
{{< /shell >}}

En la raíz del repositorio encontrarás diferentes **directorios**, cada uno específico de una **sección** de la documentación. Así, por ejemplo, el directorio `idm` contiene la documentación del proyecto IDM, y será accesible desde la URL `https://site.url/idm/`. Para más información sobre esta estructura, ve a [Como crear una nueva sección](#cómo-crear-una-nueva-sección).

También deberías ver el directorio `hugo-site`, que contiene la **configuración** del sitio, algunos ficheros estáticos comunes, plantillas genéricas, etc. Para poder generar y desplegar la documentación, es necesario **sincronizar** el contenido de los directorios de cada sección con el proyecto Hugo. Para ello, se proporciona un `Makefile` con algunas reglas:

* `hugo-clean`: **elimina** el contenido del proyecto Hugo, y reestablece la estructura mínima necesaria.
* `hugo-sync`: **copia** el contenido de las secciones al proyecto Hugo.
* `keep-sync`: lanza un servicio que monitoriza los ficheros de las secciones, y **sincroniza** el proyecto Hugo si hay algún cambio.
* `sync`: regla que ejecuta las tres anteriores.
* `run-server`: lanza el **servidor de desarrollo** de Hugo, que permite previsualizar el contenido generado mientras realizas cambios.

Dado esto, el *workflow* general que deberías seguir para modificar la documentación es el siguiente:

0. **Actualizar** el repositorio: `git pull`
0. Lanzar el servicio para **sincronizar** el proyecto Hugo: `make sync`
0. En otra terminal, ejecutar el **servidor** de desarrollo: `make run-server`
0. Abrir en el navegador la **URL**: [http://localhost:1313](http://localhost:1313)
0. **Editar**, y revisar que el resultado es el esperado.
0. Hacer un **commit** con todos los cambios.


# Cómo editar un documento existente

Lo primero que debes hacer es **determinar** qué fichero debes editar. Podemos distinguir principalmente dos tipos de contenido: plantillas (en formato `.html`) y documentos en [Markdown](https://daringfireball.net/projects/markdown/) (con la extensión `.md`). Cada fichero `.md` se **renderizará** con una plantilla determinada.

Igualmente, cada **sección** (o subsección) del sitio se renderiza con una plantilla específica. El fichero de cabecera de cada sección (el `_index.md`) puede especificar con qué plantilla se renderizará (con el parámetro `layout`). Por ejemplo, la sección `idm`, tiene `layout: "home"` en su fichero de cabecera, por lo que esa sección se renderizará con la plantilla `home.html`, situada en el directorio `idm/layouts`.

Y si lo que quieres es editar **contenido específico** de una entrada (no de una sección), sólo debes buscar el fichero Markdown correspondiente. Por ejemplo, el contenido de la entrada `http://site.url/idm/examples/install/` lo proporciona el fichero `/idm/content/examples/install.md`.

Si quieres añadir una **imagen**, copiala al directorio `static/images` de la sección correspondiente, y haz referencia a ella directamente. Por ejemplo:

{{< code "md" >}}
![image](/recipes/images/create-document.png)
{{< /code >}}

# Cómo crear una nueva sección

Para crear una nueva sección, se han creado algunas **herramientas específicas** que se encargar de copiar las plantillas y configurar el proyecto adecuadamente.

Antes de nada, para hacerlas disponibles, carga el fichero `arco-doc.sh` de la siguiente forma:

{{< shell >}}
$ . arco-doc.sh
{{< /shell >}}

A continuación, ejecuta el script `ad-create-section`, indicándole el **nombre** de la nueva sección, en minúsculas. Te pedirá más información, como el **tipo** de sección que estás creando, a elegir entre las siguientes:

* `project`: un proyecto oficial del grupo, como *Citisim* o *Hesperia*.
* `internal-project`: proyectos no financiados directamente, en los que se desarrolla algún producto software horizontal.
* `recipes`: documentación genérica sobre procesos y otros usos.

La herramienta también te pedirá una **breve descripción**, que se mostrará en la portada de la página de documentación. Un ejemplo de ejecución sería este:

{{< shell >}}
$ ad-create-section arco-talks
Please, provide the type of section.
Choose from [project, internal-project, recipes, ...]
- recipes
And a brief description? (intro when finish)
- Posts with information about the Arco Talks: topic, date and maybe even the slides!
Creating section dirs...
Populating with initial content...
Adding section to automated tools...
Done.

- You can modify the given parameters in 'arco-talks/content/_index.md'
- Also, don't forget to provide an icon (arco-talks/images/arco-talks-logo.png).
{{< /shell >}}

Al finalizar, habrá creado un nuevo directorio con el nombre de la sección (`arco-talks` en el ejemplo), con la siguiente **estructura**:

{{< shell >}}
$ tree arco-talks/
arco-talks/
├── content
│   └── _index.md
├── layouts
│   └── partials
│       └── header.html
└── static
    └── images

5 directories, 2 files
{{< /shell >}}

El directorio `content` se encargará de almacenar todo el **contenido** de la nueva sección, ficheros Markdown y subsecciones. En `layouts` se guardarán las **plantillas** específicas para tu contenido. Si no añades ninguna plantilla, se utilizará alguna genérica del sitio. Por último, en `static` estarán todos los **ficheros estáticos** (css, js, imágenes, código, ejemplos, etc.) de esta sección.

Al crear la nueva sección, también se habrá modificado el `Makefile` para que las reglas de sincronización tengan en cuenta la nueva entrada. Si tenías el servicio de *sync* o el servidor de desarrollo de Hugo en marcha, necesitas **reiniciarlos** para aplicar los cambios. Simplemente páralos y vuelve a ejecutar `make sync` y `make run-server`. Si vuelves a abrir la página principal de la documentación, deberías poder ver tu nueva sección. Por ejemplo, algo así:

![](/recipes/images/new-section.png)

Como ves, es necesario hacer algunos retoques. Por ejemplo, quizá quieras cambiar el nombre que se muestra aquí (parámetro `title`), o la ruta del icono (parámetro `icon`). Para ello, edita el fichero `_index.md` de tu sección (`[SECTION]/content/_index.md`). El fichero generado para el ejemplo tiene este contenido:

{{< code cpp "arco-talks/content/_index.md" >}}
---
type: "recipes"
title: "Arco-talks"
description: "Posts with information about the Arco Talks: topic, date and maybe even the slides!"
icon: "arco-talks/images/arco-talks-logo.png"
---
{{< /code >}}

En este ejemplo, cambiaré el título, y **reemplazaré** el icono por defecto con uno específico de esta sección. Quedaría algo como esto:

![](/recipes/images/new-section-changes.png)

Si pulsas sobre el botón "*View*" (es decir, si navegas con el *browser* a la sección que acabas de crear), verás un **documento vacío**, con un mensaje animándote a modificar la **cabecera** de tu sección. La cabecera es un fichero HTML (un *partial* de Hugo) que se incluirá al renderizar todas las páginas de esta sección. Se encuentra en `[SECTION]/layouts/partials/header.html`. El contenido será algo similar a esto:

{{< code html "header.html" >}}
{{ partial "header.html" . }}

This is your section ({{ .Section }}) header.
Customize it.
{{< /code >}}

La primera línea se usa para incluir la cabecera del sitio, **no la modifiques**. El resto puedes cambiarlo según tus **necesidades**. Este es el lugar adecuado para cargar un fichero CSS específico de esta sección, añadir un título, etc. Por ejemplo, la cabecera de la sección `arco-talks` sería similar a esta:

{{< code html "arco-talks/layouts/partials/header.html" >}}
{{ partial "header.html" . }}

<link rel="stylesheet" href="/{{ .Section }}/css/style.css">

<div class="box">
  <a href="/">
    <img src="/{{ .Section }}/images/arco-talks-logo.png"
         style="padding: 0 25px; width: 110px" />
  </a>
  <h1 style="display: inline; font-weight: bold; font-size: 2.3em; padding-top: 10px">
    Arco Talks</h1>
</div>

<hr style="border: 1px solid #333333">
<br>
{{< /code >}}

Por otro lado, es importante notar que la **plantilla por defecto** para una sección recién creada será la que hay definida en el tema del *site*. Es decir, si no creas una plantilla nueva o se lo indicas en el [Front Matter](https://gohugo.io/content-management/front-matter/) de tu sección, la plantilla que usará será `hugo-site/themes/default/layouts/_default/section.html`. Esta plantilla creara una lista con las páginas de tu sección, lo cual, combinado con la cabecera antes descrita, debería ser suficiente.

Sin embago, es posible que necesites **más flexibilidad**, o simplemente quieras crear la página de sección directamente en HTML. Para ello, será necesario que crees una plantilla específica. De entre las diversas formas de hacerlo, la **más sencilla** es indicar explícitamente en el fichero `_index.md` de la sección qué plantilla usará (parámetro `layout`). Por ejemplo:

{{< code cpp "arco-talks/content/_index.md" >}}
---
[...]
icon: "arco-talks/images/arco-talks-logo.png"

layout: "home"
---
{{< /code >}}

Ahora deberás **crear la plantilla**, con el nombre indicado y extensión `.html`, en el directorio `layouts` de la sección. Es importante notar que esta plantilla no forma parte de una de nivel superior, por lo que **tendrás que incluir** los partials `header.html`, y `footer.html` del sitio. Puedes utilizar el siguiente fragmento como punto de partida:

{{< code html "arco-talks/layouts/home.html" >}}
{{ partial (print .Section "/header.html") . }}

{{ .Content }}

{{ range .Data.Pages }}
  {{ .Render "card" }}
{{ end }}

{{ partial "footer.html" . }}
{{< /code >}}


# Como añadir una nueva entrada

# Cómo publicar los cambios



