---
title: "Creación de Módulos para Phyxio"
date: 2022-05-29T10:53:24+01:00
geekdocNav: false
tags:
- phyxio
- module
- development
description: "Manual de usuario para la creación de módulos para Phyxio"
image: "creating_phyxio_modules/phyxio-icon.png"
---


## Introducción

Phyxio es una aplicación distribuida formada por dos componentes clave: el servicio web y los módulos del
*runner*. Estos últimos se encargan de utilizar el **hardware de la máquina** (GPU, TPU, cámaras, etc) para ofrecer
servicios de reconocimiento de un ejercicio en concreto.

Este manual explica el proceso de **desarrollo** de un módulo de Phyxio.


## Requisitos

Para poder seguir con este manual, se asume que los siguientes **paquetes Debian** están correctamente instalados:

* `phyxio-module-runner`: se encarga de **lanzar/parar** los módulos que Phyxio necesite para la realización
  de cierto ejercicio. Será necesario para probar el módulo que estemos desarrollando.

* `phyxio-module-creator`: herramienta de gestión para la **creación de módulos** Phyxio. Si estás leyendo esta
  documentación, es posible que ya tengas ese paquete instalado.


## Arquitectura de los módulos

Un módulo es básicamente una aplicación que corre dentro del hardware del Totem, y que se **comunica con Phyxio**
por medio de una o varias interfaces Ice. Puede ser un programa binario escrito en C++, C#, un script en Python, o
cualquier otro lenguaje para el que existan bindings de [ZeroC Ice](https://zeroc.com/products/ice). La herramienta
 `phyxio-module-creator` y, por ende, este manual asumen que el módulo será escrito en **Python**.

Un módulo se compone básicamente de las siguientes partes:

* **Binario**: este es el programa principal que se ejecutará cuando Phyxio lo indique. Se encarga de procesar
  el vídeo, de la lectura del hardware, las comunicaciones con el *runner* y con phyx.io, etc.

* **Interfaz ECI**: forma la interfaz web que se mostrará *durante* la realización del ejercicio. La ECI (
  *Embedded Controller Interface*) está formada por un fichero HTML, que cargará una guía de estilos en CSS
  y una aplicación en Javascript.

{{< image src="basic-architecture.png" width="100%" >}}
<br></br>

La herramienta `phyxio-module-creator` genera la jerarquía y el **código mínimo necesario** para un módulo básico.
A partir de ahí, el desarrollador podrá aportar la funcionalidad que necesite.


## Creación de un módulo

{{< note primary "Nota" >}}
Este manual explicará el proceso de creación de un módulo que analizará la **posición de las manos**
del usuario y detectará cuando coloca una de ellas en un punto determinado de la pantalla. Se empleará OpenCV y/o
mediapipe, aunque los detalles técnicos del proceso de detección no se discutirán aquí.
{{< /note >}}

Para **crear un módulo** de Phyxio, llamado `move-to-point`, ejecuta el siguiente comando en una terminal:

{{< shell >}}
phyxio-mod-admin -c move-to-point
{{< /shell >}}

El anterior comando **generará un directorio**, del mismo nombre que el módulo, con la estructura básica necesaria.
Algo similar a lo siguiente:

{{< shell >}}
$ tree
move-to-point
├── Makefile
├── module.json
└── src
    ├── bin
    │   └── move-to-point.py
    ├── eci
    │   ├── calibration.html
    │   ├── css
    │   │   └── style.css
    │   ├── index.html
    │   └── js
    │       └── app.js
    │       └── calibration.js
    ├── module.conf
    └── move-to-point.ice
{{< /shell >}}

La **función** de cada uno de esos archivos se detalla a continuación:

* El fichero `Makefile` contiene las reglas necesarias para algunas tareas rutinarias. En particular, permite
**compilar las interfaces** Slice a Javascript, para ser usadas dentro de la aplicación JS. También tiene algunas
otras reglas específicas para la creación de paquetes Debian y de artefactos de mender, pero no se usarán en este
 manual. **No es necesario editar este fichero**.

* `module.json` sirve para especificar la **configuración básica** del módulo. Se indica el nombre del módulo,
`name`; su nombre en clave, o `codename`, cuyo valor se empleará al registrar el módulo dentro de Phyxio; la ruta
relativa (`module_dir`) donde se encuentran el binario y la interfaz ECI; el nombre del binario (`exec`); y
una serie de parámetros de configuración para la ECI (en especial, el `proxy` que deberá usar para comunicarse
con el binario). Los valores por defecto suelen ser adecuados, y **no es necesario cambiar nada** (al menos de
momento).

* `src/bin/move-to-point.py` es el binario del módulo, que será **lo que se ejecute** cuando Phyxio lo indique.
Este fichero contiene el código mínimo necesario para un módulo Python, pero es posible reescribirlo por
completo, usando cualquier otro lenguaje compatible. Más adelante comentaremos su contenido paso a paso.

* El directorio `src/eci` contiene la **interfaz web** que se empotrará dentro de phyx.io cuando se esté realizando
un ejercicio que use este módulo, o cuando se realize el proceso de calibración. También hablaremos después
de su contenido más detalladamente.

* `src/module.conf` especifica la configuración de ZeroC Ice que usará el binario para crear su *Communicator* y
el adaptador de objectos. Los valores especificados por defecto son suficiente, por lo que **tampoco es necesario
cambiar nada** (salvo requisitos muy concretos).

* `src/move-to-point.ice` define la interfaz ZeroC Ice que se empleará en la comunicación entre el binario y la
interfaz ECI (aplicación web). Por defecto, la interfaz hereda de `ECIClient`, pero está pensada para **ser extendida
con nuevos métodos**, según sea necesario. Comentaremos su uso más adelante.


### Instalación

Para que el módulo recién creado pueda usarse, debe **instalarse en el sistema**. La aplicación
`phyxio-mod-admin` también se encarga de ello. Basta con indicarle el nombre del módulo (desde el directorio
en dónde esté), con la opción `-i`. Esto **copiará el módulo completo** a su ruta correspondiente. A veces, sin
embargo, y sobre todo durante el desarrollo, es conveniente tener el módulo instalado (para probarlo) y a su
vez, poder seguir realizando modificaciones en él. A tal efecto existe la opción `-s`, que le indica a
`phyxio-mod-admin` que debe realizar un **enlace simbólico** (en vez de una copia completa).

{{< note type="danger" title="¡Atención!" >}}
Si usas la opción `-s`, asegúrate de que **otros usuarios pueden acceder** a la carpeta donde tienes el módulo (y
a todos sus ancestros). De otra forma, el servicio no podrá cargar el módulo correctamente. Además, dado que
se crea un enlace simbólico a la ruta local, recuerda que no debes cambiarle el nombre a esa carpeta, o moverla de
sitio, pues eso **romperá el enlace simbólico**, y dejará de funcionar.
{{< /note >}}

Por tanto, si queremos instalar nuestro módulo, y seguir trabajando con él, ejecutaremos el siguiente
comando:

{{< note type="warning" title="¡Importante!" >}}
Es necesario ejecutarlo con `sudo`, pues la aplicación modificará rutas del sistema.
{{< /note >}}

{{< shell >}}
sudo phyxio-mod-admin -i -s move-to-point
{{< /shell >}}

Si deseas desinstalar, usa la opción `-u` de la misma forma:

{{< shell >}}
sudo phyxio-mod-admin -u move-to-point
{{< /shell >}}

Al instalar un nuevo módulo (o desinstalarlo), es necesario **reiniciar el servicio** que lo usa (para que
actualice sus registros). Así pues, ejecuta el siguiente comando después de la (des)instalación:

{{< shell >}}
sudo systemctl restart phyxio-module-runner.service
{{< /shell >}}

Y, para comprobar que la instalación (y la carga) se han realizado satisfactoriamente, puedes ver el log
del servicio con el siguiente comando:

{{< shell >}}
journalctl -f -u phyxio-module-runner.service

...
INFO:ModuleRunner: Loading module /usr/share/phyxio-module-runner/modules/move-to-point/module.json
INFO:ModuleRunner: - module named 'move-to-point' correctly loaded!
...
{{< /shell >}}


## Esquema general de funcionamiento

Cuando el usuario visita la web de [phyx.io](https://phyx.io?fmode=totem), en la **vista de totem**, entra en una
rutina y pulsa sobre *Comenzar*, se abre una página de **carga** para el primer ejercicio de la rutina. Esta
página corre una aplicación en JavaScript que contacta con un servicio llamado `module-runner` (usando ZeroC Ice),
y le indica que ejecute el binario del módulo específico para ese ejercicio.

El `module-runner`, al iniciar, registra todos los módulos instalados (leyendo el fichero `module.json` de cada
uno). Cuando la página de carga le indica el nombre del módulo (usando su `codename`), este lo busca
en su registro, obtiene la ruta al binario, y **lo ejecuta**. Además, lanza un servidor web con la interfaz ECI
del módulo, y **notifica** a la página de carga que está disponible.

La página de carga (también llamada cliente), al recibir la notificación del *runner*, **carga la web ECI** del
módulo y se queda esperando más eventos desde el *runner*. A partir de este momento, el control lo toma el módulo
del ejercicio, tanto la aplicación que corre en la ECI, como el binario que ha ejecutado el *runner*.


## Flujo de ejecución del binario

{{< note type="primary" title="Nota" >}}
Si bien es cierto que llamamos *binario* a la aplicación principal del módulo, no tiene porqué ser un
binario como tal (algo compilado). De hecho, **la mayoría de las veces**, hablamos de un script en Python. No
obstante, seguiremos usando esta notación en aras de la simplicidad.
{{< /note >}}

El binario del módulo que se ha generado (el fichero `src/bin/move-to-point.py`) consta de una aplicación Ice
(`Ice.Application`), con su *communicator* correspondiente, y un adaptador de objetos:

{{< code python >}}
class MoveToPoint(Ice.Application):
    def run(self, args):
        if not self._check_args(args):
            return -1

        # create communicator and adapter
        ic = self.communicator()
        adapter = ic.createObjectAdapter("MoveToPoint.Adapter")
        adapter.activate()
{{< /code >}}

Además, **se crea un proxy** a un objeto que reside en el *runner* y que sirve para notificarle cambios en el
estado del módulo:

{{< code python >}}
        # get proxy to runner service
        runner = Phyxio.ModuleHandlerPrx.checkedCast(
            ic.stringToProxy(self.args.runner_prx))
{{< /code >}}

En concreto, se usará el método `ready()` para indicar al *runner* que el módulo ha arrancado
correctamente y está activo, y `finish()` para notificar la finalización del ejercicio, y mandar un reporte
de resultados.

Por otro lado, también se crea **un sirviente** que será registrado en el adaptador de objetos, y estará a
disposición de la ECI. La interfaz de ese sirviente es `Phyxio.ECIObserver`, y tiene como objetivo permitir la
comunicación desde la ECI con el módulo. En particular, la interfaz enviará al módulo el proxy de otro
 **observador**, que se empleará en la comunicación entre el módulo y la ECI. De esta forma, se establece
un canal de comunicación bidireccional entre ambas partes. La interfaz de ese observador está definida en el
fichero slice que se ha generado (en este caso, `move-to-point.ice`) y se puede modificar según sea necesario.

En la librería de componentes, hay una implementación genérica del `ECIObserver`, llamada `PhyxioECIObserverI`,
que es la que se usa en el código generado por `phyxio-mod-admin`:

{{< code python >}}
        # add observer servant, to receive client proxy
        servant = PhyxioECIObserverI(self.args, runner)
        prx = adapter.add(servant, Ice.stringToIdentity("MoveToPoint"))
        log.info(f" Module proxy: {prx}")
{{< /code >}}

Por último, la aplicación **notifica su estado** y ejecuta el **bucle de eventos** del *communicator*. De
ser necesario, es posible reemplazar este por otro bucle de eventos (lo veremos más tarde). El código
relacionado es el siguiente:

{{< code python >}}
        # notify runner that we are ready, and wait events
        runner.readyAsync()
        self.callbackOnInterrupt()
        ic.waitForShutdown()
        ic.destroy()
{{< /code >}}

Todo el código discutido hasta ahora ha sido generado por la herramienta `phyxio-mod-admin`, y **no es necesario
realizar modificaciones** en él (salvo que se indique lo contrario). También existen algunos puntos, marcados
con sendos comentarios, donde es probable que necesites realizar cambios. Según progresemos en el ejemplo,
los iremos viendo.


### Argumentos del ejercicio

La clase principal del binario que estamos analizando contiene un método llamado `_check_args()`, que se encarga
de parsear los argumentos de la línea de órdenes. Estos argumentos (que los aporta el *runner*) se obtienen
de dos sitios diferentes: 1) la **configuración interna** del runner (parámetros comunes a todos los módulos)
y 2) las opciones de **configuración del ejercicio** (determinada por el *trainer* al crearlo).

Utilizando la función `get_global_parser()` de la librería *tools*, se crea un parser (en concreto, un
[ArgumentParser](https://docs.python.org/3/library/argparse.html#argumentparser-objects)) que ya tiene
configuradas las opciones comunes. En el método `_check_args()` puedes añadir cualquier opción particular que
tu módulo necesite. A modo de ejemplo, vamos a hacer que `move-to-point` acepte un argumento
opcional para indicar si limitamos el punto dónde el usuario debe mover la mano a una parte del espacio: derecha,
izquierda, arriba o abajo. Para ello, añadimos un argumento al parser, `--limit-area`, de la siguiente forma:

{{< code python >}}
    def _check_args(self, args):
        parser = get_global_parser()

        parser.add_argument("--limit-area",
            choices=["left", "right", "top", "bottom"],
            help="limit area to specified section")
{{< /code >}}

A partir de ese momento, la aplicación podrá acceder al valor de ese parámetro opcional desde
`args.limit_area`.


### *Tracker*&nbsp;de manos

El módulo que genera `phyxio-mod-admin` por defecto está orientado al **análisis de vídeo** mediante
técnicas de visión por computador. Esto implica que está pensado para ser usado con un componente de vídeo
que procese los *frames* de entrada, realice las operaciones pertinentes, y escriba los *frames* resultantes a
un sumidero de vídeo. Y esa es precisamente la funcionalidad que aporta la clase `VideoComponent` de la librería
de componentes.

`VideoComponent` se encarga de **inicializar los dispositivos de vídeo**, tanto de entrada como de salida. Además,
implementa un **bucle de eventos** específico, que realiza las siguientes acciones:

* **Captura un frame** del dispositivo de vídeo de entrada.
* Lo **invierte** horizontalmente (para obtener una imagen especular).
* Llama al método `on_frame()`, pasando el frame como argumento.
* **Escribe** en el dispositivo de vídeo de salida el frame retornado por `on_frame()`.

{{< note type=primary title=Nota >}}
`VideoComponent` también implementa **otros métodos** de soporte para notificar cambios a la
ECI, detener el bucle de eventos, etc. Los iremos viendo según sea necesario.
{{< /note >}}

Para usar el `VideoComponent`, es necesario definir un nuevo componente que herede de él. Por ejemplo,
para el módulo `move-to-point`, definiremos la clase `HandPositionDetector`, en cuyo constructor crearemos los
objetos necesarios para realizar el tracking de las manos (usando
[MediaPipe Hands](https://google.github.io/mediapipe/solutions/hands.html)):

{{< code python >}}
import mediapipe as mp
mp_draw = mp.solutions.drawing_utils
mp_draw_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

from libs.components import VideoComponent

class HandPositionDetector(VideoComponent):
    def __init__(self, args, servant):
        super().__init__(args, observer=servant)
        self._target_pos = TargetPositionIterator(args)

        self._hands = mp_hands.Hands(
            max_num_hands=1,
            model_complexity=0,
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5)
{{< /code >}}

Como parámetros del constructor, tenemos los argumentos que procesó el parser de la línea de órdenes (`args`),
y el sirviente que creamos en la aplicación principal (la instancia de `PhyxioObserver`, `servant`). El
constructor de la clase `VideoComponent` acepta un parámetro opcional más llamado `has_video_out` que indica si
deseamos proporcionar feedback de vídeo al usuario o no. Por defecto es `True`, pero si le pasamos `False`, **no
se creará** el dispositivo de vídeo de salida (lo cual podría ser útil para módulos que procesan el vídeo pero
no necesitan mostrarlo al usuario).

El método más importante que **debemos sobreescribir** de la clase `VideoComponent` es `on_frame`. Acepta
un único parámetro, un frame de vídeo, y debe devolver el mismo frame (quizá modificado), o uno similar, con
las **mismas características** (tamaño, bpp, etc.).

{{< note type=warning title=¡Importante! >}}
Este método lo invocará el bucle de eventos **con cada frame** del dispositivo de entrada de vídeo que se capture,
por lo que el procesamiento debe ser el mínimo imprescindible, para evitar latencia o *frame dropping*.
{{< /note >}}

En el ejemplo que estamos desarrollando, este método realizará lo siguiente:
* Dibujar un círculo en la **posición objetivo**
* Hacer el *tracking* de una mano y obtener su **centro geométrico**
* Comparar ambos (posición objetivo y centro geométrico), y si hay superposición, **actualizar el objetivo** a
  una nueva posición, e **incrementar** (y notificar a la ECI) el número de iteraciones realizadas.

Su implementación sería la siguiente:

{{< code python >}}
    def on_frame(self, image):
        # draw target position
        self._target_pos.draw(image)

        # detect hands, and exit if no hand detected
        # NOTE: to improve performance, mark the image as not writeable (pass by reference)
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = self._hands.process(image)
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if results.multi_hand_landmarks is None:
            return image

        # get hand center, and draw "a ball"
        hand_center = self._get_hand_center(image, results.multi_hand_landmarks[0])
        cv2.circle(image, hand_center, 32, ORANGE, cv2.FILLED, cv2.LINE_AA)

        # check if center is inside target
        if self._target_pos.match(hand_center):
            self._target_pos.next()
            self.iter_add()

        # draw the hand annotations on the image (only for debug)
        if self._is_debug:
            self._draw_hand_skel(image, results)

        return image
{{< /code >}}

También he definido en la misma clase un par de métodos auxiliares, que se emplean en `on_frame`. Uno es el
encargado de obtener el centro geométrico de la mano:

{{< code python >}}
    def _get_hand_center(self, image, landmarks):
        # https://google.github.io/mediapipe/solutions/hands.html#hand-landmark-model
        x = (landmarks.landmark[5].x + landmarks.landmark[17].x + landmarks.landmark[0].x) / 3
        y = (landmarks.landmark[5].y + landmarks.landmark[17].y + landmarks.landmark[0].y) / 3

        # landmark positions are normalized, return global positions
        return int(x * image.shape[1]), int(y * image.shape[0])
{{< /code >}}

Y el otro se encarga de dibujar los landmarks que el módulo `Hands` ha detectado (es decir, el "esqueleto"
de la mano). Esto se usa sólo a modo de depuración, y no se llamará a menos que, al definir el ejercicio en
phyx.io se incluya el flag `--debug-info`:

{{< code python >}}
    def _draw_hand_skel(self, image, results):
        for hand_landmarks in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(image, hand_landmarks,
                mp_hands.HAND_CONNECTIONS,
                mp_draw_styles.get_default_hand_landmarks_style(),
                mp_draw_styles.get_default_hand_connections_style())
{{< /code >}}

En la clase anterior, también he instanciado un objeto encargado de generar al azar la **posición objetivo** a la
que el usuario tendrá que mover la mano (el `TargetPositionIterator`). Por razones de completitud, he aquí
su código:

{{< code python >}}
import random
import cv2

ORANGE  = (0, 95, 253)    # BGR
GREEN   = (50, 194, 134)  # BGR
GRAY    = (100, 100, 100) # BGR
WHITE   = (255, 255, 255) # BGR

class TargetPositionIterator:
    def __init__(self, args, radius=64):
        self._w = args.video_width
        self._h = args.video_height
        self._r = radius
        self._limit = args.limit_area

        self._current = (0, 0)
        self.next()

    def draw(self, image):
        x, y = self._current
        cv2.circle(image, (x+1, y+1), self._r, GRAY, 3, cv2.LINE_AA)
        cv2.circle(image, (x, y), self._r, WHITE, 3, cv2.LINE_AA)

    def match(self, position):
        dx = abs(position[0] - self._current[0])
        dy = abs(position[1] - self._current[1])
        return dx < self._r and dy < self._r

    def next(self):
        # (0,0) is at top-left corner
        x1, y1 = self._r, self._r
        x2, y2 = self._w - self._r, self._h - self._r

        if self._limit == "top":
            y2 = self._h / 2
        elif self._limit == "bottom":
            y1 = self._h / 2
        elif self._limit == "left":
            x2 = self._w / 2
        elif self._limit == "right":
            x1 = self._w / 2

        x = random.randrange(x1, x2)
        y = random.randrange(y1, y2)
        self._current = (x, y)
{{< /code >}}

### Modificación del bucle de eventos

Hasta ahora, nuestro módulo utilizaba el bucle de eventos de Ice para mantenerse activo. Sin embargo, al
implementar un `VideoComponent`, es necesario **reemplazarlo** por el que trae el componente de vídeo.
La modificación se debe hacer en el método `run` de la aplicación principal (en el caso que nos ocupa, la
clase `MoveToPoint` que hereda de `Ice.Application`). Los pasos para realizar esta modificación son:

1. Crear una **instancia** del `VideoComponent`:

{{< code python >}}
        self._detector = HandPositionDetector(self.args, servant)
{{< /code >}}

2. Reemplazar el **bucle de eventos** de Ice por el suyo:

{{< code python >}}
        # ic.waitForShutdown()
        self._detector.run()
{{< /code >}}

3. **Detener el detector** cuando se produce una interrupción. Básicamente es llamar al método `stop` cuando el
*runner* solicita que nuestro módulo debe detenerse (por ejemplo, porque el usuario ha pulsado sobre el botón
*stop*). Este código debe ir en el método `interruptCallback`:

{{< code python >}}
    def interruptCallback(self, sig):
        self._detector.stop()
{{< /code >}}


## Interfaz de usuario (ECI)

La interfaz empotrada de control (*Embedded Control Interface*) es la parte que phyx.io cargará en el **cliente
web**, y supone la parte visual y de interacción con el usuario del módulo. Consta inicialmente de un fichero
en HTML (`src/eci/index.html`) con la plantilla que necesitamos para funcionar, y una aplicación JavaScript
(`src/eci/js/app.js`) que implementa la interfaz Ice que hayamos definido, y la inicialización mínima necesaria.

La interfaz que genera la herramienta `phyxio-mod-admin` está compuesta de:

* **Feedback de vídeo** empotrado a pantalla completa. Este es el vídeo de salida que genera el módulo.
* **Contador** de tiempo e iteraciones (realizadas y objetivo).
* Vídeo del **modelo** de prueba del ejercicio, para indicar al usuario qué debe hacer.
* Panel de **información** sobre la tarea a realizar.

Si existe algún componente que no queramos mostrar, simplemente **borramos** el código HTML correspondiente. Por
ejemplo, en `move-to-point` no tendremos vídeo de muestra del ejercicio, por ello, eliminamos las siguientes líneas
de código del fichero HTML (no es necesario cambiar nada de la aplicación en JavaScript).

{{< code html >}}
<!-- -----------------------------------------------------------
     video from trainer model sample
-->
<div id="model-video">
  <video id="mv-elem" poster="/shared/images/model-poster.png" autoplay loop muted></video>
</div>
{{< /code >}}


## Compilando y probando

Antes de poder probar el módulo recién creado, y siempre que realicemos modificaciones en la interfaz
Ice, será necesario compilar el slice. Para ello, ejecuta el siguiente comando:

{{< shell >}}
make compile-slices
{{< /shell >}}

Esto generará un fichero .js con los skeletons/stubs para nuestro cliente JavaScript. Y dado que en un paso
anterior ya hicimos la instalación del módulo, y reiniciamos el `module-runner`, no es necesario hacer nada
más para poder usarlo. Simplemente, registra el módulo en phyx.io (usando su `condename`), y crea un
ejercicio que lo use, añádelo a una rutina y asignala a un usuario de prueba. Accede a la página del totem, y
¡prueba tu módulo!

{{< note type=primary title=Nota >}}
Recuerda que necesitas permiso para **acceder a la cámara**. Por ello, si usas una ventana de incógnito o es la
primera vez que lo usas, el navegador te mostrará un mensaje solicitando acceso. Por otro lado, también necsitarás
registrar los **certificados TLS** que usa el *runner*, por lo que también habrás tenido que ejecutsar
`phyxio-install` en tu usuario local.
{{< /note >}}


## Soporte para calibración

Algunos ejercicios están ligados a ciertos **parámetros** que son específicos **de cada usuario** que los realiza.
Esta información se ha de obtener previamente, en un proceso de calibración. En esta etapa, el ejercicio se ejecuta
de forma similar a como lo haría normalmente, pero la lógica se centra en obtener del usuario los parámetros
pertinentes, notificándoselos a Phyxio para que los proporcione en próximas ejecuciones.

No todos los módulos requieren de calibración, y de hecho está **desactivada por defecto**. Para habilitarla, o
bien al crear un ejercicio, o bien desde la vista de edición, se ha de marcar la casilla *Calibración*:

{{< image src="enable-calibration.png" width="100%" >}}

Una vez marcada, nuestro módulo podrá recibir dos argumentos relacionados:

* `--calibrate`, que le indicará que debe arrancar el módulo en modo calibración.
* `--calibration-data`, que proporciona los datos de calibración almacenados para este usuario. **No es necesario**
  parsear esta opción, pues ya lo hace el componente de vídeo. En su lugar, tendremos una propiedad llamada
  `_calibration` con los parámetros correctos.

{{< note type=primary title=Nota >}}
Los datos de calibración son específicos por cada para **ejercicio / usuario**, y por tanto, están vinculados
a las opciones de configuración del ejercicio. No son únicos para cada módulo.
{{< /note >}}


### Componente de vídeo para calibración

Cuando el binario recibe el argumento `--calibrate`, debe ejecutarse en modo calibración, con el objetivo de
obtener del usuario los parámetros requeridos. Queda bajo el **criterio del desarrollador** implementar los
mecanismos necesarios para conseguirlo. Por ejemplo, podría simplemente **mostrar una interfaz web** (sin vídeo),
con varios controles que el usuario debe ajustar (con el ratón, o el *touchscreen*). Una vez ajustados, al
pulsar sobre un botón "guardar" se notifica a Phyxio que la calibración ha concluido.

Otra forma de calibrar un ejercicio concreto (especialmente los que usan vídeo), es indicar al usuario que
haga las mismas acciones que haría normalmente, pero en vez de esperar a que llegue a un umbral específico, lo
que hace el módulo es **registrar los rangos máximos** a los que el usuario es capaz de llegar.

Utilizaremos una aproximación mixta para añadir soporte de calibración al ejercicio del ejemplo `move-to-point`.
El parámetro que vamos a calibrar es el **radio del círculo** que se pinta como objetivo. Y para ello, diseñamos
un nuevo componente de vídeo, `HandPositionCalibrator`, que heredará de `HandPositionDetector` para reutilizar
algunos métodos ya implementados.

{{< code python >}}
class HandPositionCalibrator(HandPositionDetector):
    def __init__(self, args, servant):
        super().__init__(args, servant)
        self._target_pos = TargetPositionCalibrator(args)

        # set initial values of calibration
        self._update_calibration()

    def on_frame(self, image):
        # draw target position
        self._target_pos.draw(image)

        # detect hands, and exit if no hand detected
        # NOTE: to improve performance, mark the image as not writeable (pass by reference)
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = self._hands.process(image)
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        if results.multi_hand_landmarks is None:
            return image

        # get hand center, and draw "a ball"
        hand_center = self._get_hand_center(image, results.multi_hand_landmarks[0])
        cv2.circle(image, hand_center, 32, ORANGE, cv2.FILLED, cv2.LINE_AA)

        # update target radius
        if self._target_pos.update(hand_center) and self._event_observer is not None:
            self._update_calibration()

        # draw the hand annotations on the image (only for debug)
        if self._is_debug:
            self._draw_hand_skel(image, results)

        return image

    def _update_calibration(self):
        self._event_observer.record_calibration(target_radius=self._target_pos.radius)
        self._event_observer.sync_calibration()
{{< /code >}}

La implementación es **similar al tracker** que antes definimos. Los métodos interesantes aquí son los
relacionados con la calibración, que están en el objeto `_event_observer`.

* `record_calibration()`, encargado de notificar al sirviente los nuevos parámetros que deseamos almacenar
  como datos de calibración. Se utiliza un diseño tipo clave-valor, usando los nombres de los parámetros de la
  función como clave. En el ejemplo, los datos almacenados serán: `{"target_radius": x}`, siendo `x` el valor
  actual del radio (que lo tenemos en `self._target_pos.radius`). No es necesario indicar todos los parámetros
  al mismo tiempo; podríamos, por ejemplo, llamar de nuevo a la función con otro dato (algo como
  `record_calibration(date=now)`), y ambos registros se conservarían, tanto `target_radius` como `date`.

* `sync_calibration()`, que envía a la ECI **todos los datos** de calibración almacenados hasta ahora. Queda por
  cuenta del desarrollador cómo se representarán en la interfaz.

Por cuestiones de completitud, he aquí de nuevo la implementación de la clase que gestiona el *target position*,
llamada `TargetPositionCalibrator`:

{{< code python >}}
class TargetPositionCalibrator:
    def __init__(self, args, min_radius=64):
        self._w = args.video_width
        self._h = args.video_height
        self._min_r = min_radius

        self.radius = min_radius

        self._handler_r = 16
        self._pos = (self._w // 2 - self.radius, self._h // 2 - self.radius)

    def draw(self, image):
        # outter target circle
        x, y = self._pos
        cv2.circle(image, (x + 1, y + 1), self.radius, GRAY, 3, cv2.LINE_AA)
        cv2.circle(image, self._pos, self.radius, WHITE, 3, cv2.LINE_AA)

        # handler point
        cv2.circle(image, (x + self.radius, y), self._handler_r, GREEN, cv2.FILLED, cv2.LINE_AA)

    def update(self, position):
        """Returns whether registered calibration values have changed or not."""
        dx = abs(position[0] - (self._pos[0] + self.radius))
        dy = abs(position[1] - self._pos[1])
        if dx < self._handler_r * 2 and dy < self._handler_r * 2:
            self.radius = max(self._min_r, abs(position[0] - self._pos[0]))
            return True
        return False
{{< /code >}}

Por último, un **paso importante** es instanciar el nuevo componente de vídeo. Como la aplicación principal es
la misma, y queremos seguir dando soporte al ejercicio que teníamos hecho, debemos crear un componente u otro en
función del argumento `--calibrate`:

{{< code python >}}
class MoveToPoint(Ice.Application):
    def run(self, args):

        ...

        # create our hand detector
        if self.args.calibrate:
            self._detector = HandPositionCalibrator(self.args, servant)
        else:
            self._detector = HandPositionDetector(self.args, servant)
{{< /code >}}

Otra posible implementación habría supuesto utilizar el mismo componente de vídeo, pero implementar
internamente la lógica que hiciera una función u otra, en función del valor de `args.calibrate`.


### Interfaz ECI de calibración

Cuando se ejecuta el módulo en modo de calibración, el fichero HTML que se cargará no será `index.html`, como
hasta ahora, sino `calibration.html`, que reside en la misma carpeta. Esta interfaz no contie los contadores
de tiempo o de iteraciones, sino que muestra al usuario los **valores actuales** de calibración. También hay
un botón que permite determinar cuando la calibración ha concluido, e indicar al *backend* que deben almacenarse
los valores recogidos.

Al igual que la interfaz principal, la de calibración también cuenta con *feedback* de vídeo, y un *placeholder*
para los **vídeos modelo** del ejercicio, por si fueran necesarios para la calibración. En el ejemplo que estamos
desarrollando no hay modelos, por lo que elimino esta sección de la plantilla:

{{< code html >}}
    <!-- -----------------------------------------------------------
         video from trainer model sample
    -->
    <div id="model-video">
      <video id="mv-elem" poster="/shared/images/model-poster.png" autoplay loop muted></video>
    </div>
{{< /code >}}

Por otro lado, sí deseo mostrar el **valor actual del parámetro** que quiero almacenar en la calibración (a saber,
el radio del círculo objetivo). Para ello existe la sección `stats`. Esta formada por una lista de pares,
*nombre-valor*, donde el primer elemento (`<span>`) es el nombre, y el segundo (`<h1>`) es el valor. Para el
ejemplo, la modificamos de la siguiente forma:

{{< code html >}}
      <!-- -----------------------------------------------------------
           current calibration parameters (modify to suit your needs)
      -->
      <div class="stats">
        <span>Target Radius:</span>
        <h1 id="target-radius">0</h1>
      </div>
{{< /code >}}

Ahora será necesario actualizar ese elemento, `#target-radius`, con el valor que nos proporciona el módulo al
sincronizar. Para ello, necesitamos modificar el fichero JavaScript que contiene el código relacionado con la
interfaz ECI, que es diferente del principal (`app.js`). En este caso, se llama `calibration.js`, y el método
que nos interesa es `syncCalibration`, que recibe un objeto con todos los datos de calibración almacenados para
este ejercicio y este usuario, según lo hayamos definido en el módulo con `record_calibration`. Así, en nuestro
ejemplo, tendremos disponible directamente el valor usando la clave `target_radius`:

{{< code js >}}
   // this is an RMI invocation (common iface)
   syncCalibration(data) {
      data = JSON.parse(data);
      $('#target-radius').text(data.target_radius);
   }
{{< /code >}}

¡Y listo! Aunque hay más cosas que podemos modificar, para este ejemplo, tenemos todo lo necesario para iniciar
el proceso de calibración de nuestro módulo.


### Uso de datos de calibración

Una vez que el ejercicio está **correctamente calibrado**, es necesario realizar una última modificación en el
componente principal (no el de calibración) para que use los datos convenientemente. La calibración está
disponible en el atributo `_calibration` de la clase `VideoComponent` y sus derivadas. Es un diccionario con las
claves y valores que hayamos registrado.

Para el ejemplo que nos atañe, he modificado la creación de la instancia `TargetPositionIterator` para pasarle
el radio que se indica en la calibración. Dado que la clase se reutiliza, y que en modo calibración no hay
`_calibration`, el código es un poco más complejo, pero podría haber sido simplemente
`TargetPositionIterator(args, self._calibration["target_radius"])`:

{{< code python >}}
        kwargs = {}
        if self._calibration is not None:
            kwargs["radius"] = self._calibration["target_radius"]
            self._target_pos = TargetPositionIterator(args, **kwargs)
{{< /code >}}

Con ese cambio, ya deberías poder cambiar el tamaño del círculo objetivo en el módulo `move-to-point`.