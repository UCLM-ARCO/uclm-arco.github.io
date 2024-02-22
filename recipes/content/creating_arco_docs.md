---
title: "Cómo crear documentación"
date: 2019-02-18T14:25:28+01:00
tags:
- recipe
- docs
- hugo
draft: false

image: "creating_arco_docs/create-document.png"
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
$ sudo apt install git hugo make inotify-tools
{{< /shell >}}

Recuerda que la versión de Hugo que instales ha de ser `>= 0.40`, lo que puedes comprobar con el siguiente comando:

{{< shell >}}
$ hugo version
{{< /shell >}}

Para poder trabajar con la documentación de Arco, en primer lugar **descárgate el repositorio**:

{{< shell >}}
$ git clone git@github.com:UCLM-ARCO/uclm-arco.github.io.git
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


## Imágenes

Si necesitas incluir **imágenes**, crea un directorio con el mismo nombre que tu entrada (pero **sin** extensión), dentro de `[SECTION]/static/images`. Por ejemplo, supongamos que estás editando el fichero `recipes/content/integrating_xiaomi_devices.md`. En este caso, el directorio a crear sería:

{{< shell >}}
$ mkdir recipes/static/images/integrating_xiaomi_devices
{{< /shell >}}

A continuación, incluye tus imágenes dentro de ese directorio, y haz **referencia** a ellas en tu contenido usando el [Shortcode](https://gohugo.io/content-management/shortcodes/) `image`, tal que así:

{{< code "md" >}}
{{</* image "filename.png" */>}}
{{< /code >}}

También puedes añadir una **imagen** que esté directamente en el directorio raiz, `static/images`, de la sección correspondiente. Para ellos, especifica en el shortcode el parámetro `relative="false"`. Por ejemplo:

{{< code "md" >}}
{{</* image src="filename.png" relative="false" */>}}
{{< /code >}}


## Código y ficheros de texto

El proceso para incluir el contenido de un fichero **estático** (quizá un script en Python, o un fichero de configuración de Ice) es muy similar al de las imágenes. Antes de nada, **crea el directorio específico** de tu entrada que contendrá estos ficheros en el directorio `static/code` de tu sección. Debe llamarse de la misma forma que tu página (pero sin la extension `.md`).

Por ejemplo, si quiero incluir un *snippet* en Python dentro del post `idm/content/hello_world.md`, crearé el directorio `idm/static/code/hello_world/`:

{{< shell >}}
$ mkdir idm/static/code/hello_world
{{< /shell >}}

A continuación, mete en ese directorio todo el código o ficheros estáticos que necesites. Para incluirlos en la entrada, puedes usar el [Shortcode](https://gohugo.io/content-management/shortcodes/) `staticCode`. Acepta dos parámetros: el **nombre del fichero**, y el **lenguage** (opcional, para cuestiones de marcado de sintáxis). Por ejemplo:

{{< code md >}}
{{</* staticCode "hello_world.py" */>}}
{{< /code >}}

Lo que debe producir un resultado similar a esto:

{{< staticCode "hello-world.py" >}}

Si lo que necesitas es incluir in **fragmento** de código *inline* (como los que hay en esta sección), usa el Shortcode `code`, indicándole el **lenguaje** del *snippet* que estás incluyendo. Por ejemplo, para mostrar código en C#, sería:

{{< code md >}}
{{</* code cs */>}}
class Service {
  private static int id = 123;
  public string name = "service-123";
}
{{</*/ code */>}}
{{< /code >}}

Lo que mostraría el siguiente resultado:

{{< image "template-cs-code.png" >}}

Y si lo que quieres es mostrar un comando por la terminal, el Shortcode a usar el `shell`:

{{< code md >}}
{{</* shell */>}}
$ cat file.txt
...
{{</*/ shell */>}}
{{< /code >}}

# Cómo crear una nueva sección

Para crear una nueva sección, se han creado algunas **herramientas específicas** que se encargan de copiar las plantillas y configurar el proyecto adecuadamente.

Antes de nada, para hacerlas disponibles, carga el fichero `arco-doc.sh` de la siguiente forma:

{{< shell >}}
$ . arco-doc.sh
{{< /shell >}}

A continuación, ejecuta el script `ad-create-section`, indicándole el **nombre** de la nueva sección, en minúsculas. Te pedirá más información, como el **tipo** de sección que estás creando, a elegir entre las siguientes:

* `project`: un proyecto oficial del grupo, como "citisim" o "hesperia".
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
    ├── code
    └── images

5 directories, 2 files
{{< /shell >}}

El directorio `content` se encargará de almacenar todo el **contenido** de la nueva sección, ficheros Markdown y subsecciones. En `layouts` se guardarán las **plantillas** específicas para tu contenido. Si no añades ninguna plantilla, se utilizará alguna genérica del sitio. Por último, en `static` estarán todos los **ficheros estáticos** (css, js, imágenes, código, ejemplos, etc.) de esta sección.

Al crear la nueva sección, también se habrá modificado el `Makefile` para que las reglas de sincronización tengan en cuenta la nueva entrada. Si tenías el servicio de *sync* o el servidor de desarrollo de Hugo en marcha, necesitas **reiniciarlos** para aplicar los cambios. Simplemente páralos y vuelve a ejecutar `make sync` y `make run-server`. Si vuelves a abrir la página principal de la documentación, deberías poder ver tu nueva sección. Por ejemplo, algo así:

{{< image "new-section.png" >}}

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

{{< image "new-section-changes.png" >}}

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

Por otro lado, es importante notar que la **plantilla por defecto** para una sección recién creada será la que hay definida en el tema del *site*. Es decir, si no creas una plantilla nueva o se lo indicas en el [Front Matter](https://gohugo.io/content-management/front-matter/) de tu sección, la plantilla que usará será `hugo-site/themes/default/layouts/_default/section.html`. Esta plantilla creará una lista con las páginas de tu sección, lo cual, combinado con la cabecera antes descrita, debería ser suficiente.

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


# Cómo añadir una nueva entrada

Hugo tiene un comando para crear nuevo contenido, y en función del tipo, copiará una plantilla u otra. Estas plantillas se llaman [Archetypes](https://gohugo.io/content-management/archetypes/), y se encuentran en el directorio `hugo-site/archetypes`. Así, para crear una nueva entrada, desde la raíz del repositorio, ejecuta el comando:

{{< shell >}}
$ hugo new recipes/content/integrating_xiaomi_devices.md
{{< /shell >}}

Esto creará un nuevo archivo, llamado `integrating_xiaomi_devices.md` dentro de la sección `recipes`, en el directorio de contenido (es decir en la ruta especificada). Hugo usará la **primera palabra** de la ruta para determinar el *Archetype* que empleará a modo de plantilla para la nueva entrada. Si abres el fichero recién creado, verás algo como esto:

{{< code cpp "recipes/content/integrating_xiaomi_devices.md" >}}
---
title: "Integrating Xiaomi Devices"
date: 2019-02-20T08:05:08+01:00
tags:
- recipe
draft: true

# image: "your-recipe-image.png"
description:  "Some description, used in recipe summary"
---

** Insert your recipe here! **

{{< /code >}}

Como puedes ver, es **una plantilla** específica de esta sección, de la cual partir para crear tu contenido, en Markdown. Si abres la página principal de la sección `recipes`, deberás ver una entrada como esta:

{{< image "new-recipe-summary.png" >}}

Ahora, simplemente **abre ese archivo** con tu editor favorito, y añade el contenido que necesites. Happy editing! :D

# Cómo publicar los cambios

Para publicar (o desplegar) los cambios que hagas, simplemente haz **commit** en el repositorio y sube los cambios al servidor. No debes preocuparte de nada más, un servicio **automático** se encargará de compilar y actualizar la página de documentación.

Es **muy recomendable**, sin embargo, que **sólo** subas versiones estables, que se puedan compilar con una versión reciente de Hugo. Para **verificar** si hay o no errores, ejecuta `make` en la raíz del repositorio y comprueba los mensajes de log. Deberías obtener una salida similar a esta:

{{< shell >}}
$ make
make -C hugo-site all
make[1]: se entra en el directorio '/home/oscar/repos/doc/hugo-site'
hugo -v --theme=default
INFO 2019/02/20 11:49:11 Using config file: /home/oscar/repos/doc/hugo-site/config.toml
Building sites … INFO 2019/02/20 11:49:11 syncing static files to /home/oscar/repos/doc/hugo-site/public/
INFO 2019/02/20 11:49:11 found taxonomies: map[string]string{"tag":"tags", "category":"categories"}

                   | EN
+------------------+----+
  Pages            | 31
  Paginator pages  |  0
  Non-page files   |  0
  Static files     | 65
  Processed images |  0
  Aliases          |  0
  Sitemaps         |  1
  Cleaned          |  0

Total in 25 ms
make[1]: se sale del directorio '/home/oscar/repos/doc/hugo-site'
{{< /shell >}}

Si no hay errores, es una **versión candidata** para ser publicada. Por último, ten en cuenta que en la versión pública no se incluyen los borradores, por lo que asegúrate de cambiar el parámetro `draft` a `false` en el [Front Matter](https://gohugo.io/content-management/front-matter/) de tus páginas.


# Cómo generar la referencia (API) de una librería en Python

Se han creado algunas **herramientas específicas** para generar la documentación de referencia de cualquier librería en **Python**. Esto incluye la descripción de las clases, las propiedades y los métodos (públicos o privados).

Para **entender mejor** como funciona, supongamos que tenemos una librería, llamada `mypylib` en el directorio `/tmp/mypylib`. El fichero `__init__.py` de ese módulo contiene lo siguiente:

{{< staticCode "mypylib/__init__.py" >}}

Para generar la documentación automáticamente, primero debes **cargar las herramientas** de `arco-doc`:

{{< shell >}}
$ . arco-doc.sh
{{< /shell >}}

Una vez hecho esto, tendrás disponible la utilidad `ad-gen-py-api`, que acepta como argumentos una **lista de los módulos** que quieres documentar, junto con algunas opciones más. En particular, nos interesan estas dos:

* `-c MODULE_DIR`, que sirve para indicarle la **ruta** donde está el módulo que quieres documentar (útil en caso de que la librería no esté instalada en el sistema).
* `--title TITLE`, para añadir un **título** a la documentación.

También es importante notar que `ad-gen-py-api` genera la documentación por la salida estándar, por lo que es necesario que **guardes** esa salida en el fichero `.md` correspondiente.

Así pues, el comando que usaríamos para documentar la librería de este **ejemplo** sería el siguiente:

{{< shell >}}
$ ad-gen-py-api -c /tmp --title "A Configurable Calculator" mypylib > api/content/mypylib.md
{{< /shell >}}

Lo que genera un documento Markdown con la referencia de la librería `mypylib`. Observa que la referencia se ha **guardado en la sección** `api` de la documentación. Si abres esa página con el navegador, deberías ver algo similar a esto:

{{< polaroid "generated-api.png" >}}

Como puedes observar, la herramienta ha incluido **sólo** aquellas clases o métodos que **tienen documentación**. Si una clase no incluye su *docstring* (por ejemplo, la clase `MyTools`), esa clase **se ignorará** completamente. Lo mismo sucede con los métodos, no se incluirán aquellos cuya *docstring* está **vacía** o es nula.

Por otro lado, si la clase hereda de otra, y quieres evitar que se incluya el *docstring* de un método en concreto, puedes **sobreescribirlo** e incluir una cadena vacía (algo similar a lo que ocurre en el ejemplo con el método `__init__`).

Por último, resaltar que la documentación generará un **índice** al principio con todas las **clases** documentadas, con enlaces a sus respectivas subsecciones.

.
