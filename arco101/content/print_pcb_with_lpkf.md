---
title: "Cómo imprimir un PCB con una máquina LPKF"
date: 2019-03-01T10:49:04+01:00
tags:
- docs
- hugo
- LPKF
- PCB
draft: false

image: "print_pcb_with_lpkf/lpkf.jpg"
description: "En esta sección se verá como imprimir con una máquina LPKF un PCB previamente diseñado"
---

# Introducción

Las máquinas de prototipado para Printed Circuit Board (PCB), como las fabricadas por la empresa LPKF, permiten al usuario la impresión física de una placa previamente diseñada, siendo la solución predominante en la creación de prototipos. Estas máquinas imprimen el footprint de un diseño en una placa recubierta de cobre, creando los pads y aislando las pistas del resto del cobre.

Para realizar el proceso de impresión se debe hacer uso de las siguientes herramientas:

- Máquina de prototipado para PCB LPKF
- Software **CircuitCam**
- Software **BoardMaster**
- Herramientas de la máquina LPKF
- Placa revestida de cobre de la cual se imprimirá y cortará nuestro PCB

A lo largo de la sección se verá paso por paso como operar con estas herramientas para obtener un PCB a partir de un diseño desarrollado con programas como KiCad o DesignSpark.

# Preparación del diseño con CircuitCam

Antes de empezar a imprimir el circuito, hay que convertir los archivos resultantes del diseño en un formato entendible al software controlará la máquina (BoardMaster) y con la información necesaria para su impresión. Este es el objetivo del software **CircuitCam**, el cual es un sistema de fabricación asistida por ordenador para PCBs y otro tipo de tecnologías. Al abrir el software se nos presentará una interfaz similar a la siguiente.

{{< polaroid src="CCamiface.png" caption= "Interfaz del software CircuitCam" >}}

A la izquierda se puede observar un recuadro rojo, este recuadro es **muy importante** ya que contiene **seis iconos que se corresponde con los seis pasos ordenados** que se deben seguir para preparar los archivos y que se explican a continuación.

## Importar Archivos

El primer icono corresponde con la importación de los archivos resultado del diseño. En este paso hay que seleccionar aquellas capas que se van a usar para la impresión, como norma general serán necesarios cuatro archivos *Gerber* correspondientes a las capas top, bottom, de contorno y de taladrado, resultado del diseño previo de la placa. Estos archivos suelen ser generados como plots del diseño y tienen extensión `.gbr`, salvo la capa de taladrado que posee extensión **drl**.

Una vez importados estos archivos, la aplicación nos preguntará por la correspondencia de cada uno de los archivos importados con las capas de la placa. En la ventana que aparece, en la columna Layer/Template se le tiene que que asignarl a cada archivo su capa correspondiente:

- Archivo de la capa top - TopLayer
- Archivo de la capa bottom - BottomLayer
- Archivo de la capa del contorno - BoardOutline
- Archivo de la capa de taladrado - DrillPlated

En algunos casos es posible que a la capa de taladrado se le deba cambiar la escala para que coincida con el resto de las capas. Si la escala es la incorrecta, este error será muy visible en el software ya que se podrá como una capa queda más pequeña o más grande que el resto. Para la mejor comprensión de este paso, a continuación se presenta una imagen de ejemplo con la correspondencia entre capas.

{{< polaroid src="CCamTemp.png" caption="Ejemplo de correspondencia de archivos con las capas" >}}

## Indicar parámetro del contorno

Esta tarea está asociada al segundo icono, y su objetivo es el de señalar al software como queremos que se realice el contorno del PCB al cortarlo de la placa original. 

En este paso hay que indicar al software que a la hora realizar el corte, la máquina debe dejar cuatro pequeñas franjas sin cortar, esto es debido a que si el corte se realizase completo, cuando la placa fabricada quede totalmente libre, la broca seguiría en contacto con ella pudiendo ocasionar que el PCB salga volando. Para evitarlo se le tiene que decir al software que deje cuatro uniones en los laterales (4 edges), además se fijará la anchura de corte del contorno a 1 mm (Tab Width) aunque es recomendable ponerla a una anchura menor, para una vez finalizada la impresión, extraer el PCB con mayor facilidad.

{{< polaroid src="CCamBorder.png" caption="Selección del contorno de la placa" >}}

Cuando terminemos de configurar los parámetros habra que pulsar sobre el botón **Run** ya que si pulsamos sobre el botón OK los cambios no surtirán efecto. Para comprobar que todo ha salido correctamente, sobre la representación virtual de la placa, debería poder verse un contorno amarillo que rodea la placa entera excepto por los cuatro puntos que hemos indicado que no deben ser cortados.

## Indicar cortes internos

El tercer icono correspondiente al tercer paso es utilizado para seleccionar cortes internos. Lo más probable es que el diseño que se esté realizando no tenga cortes internos, por lo que es posible que incluso el icono esté desactivado. En ese caso este paso simplemente será ignorado y se pasará al siguiente. 

## Realizar limpieza de cobre

El cuarto paso, está destinado a la limpieza total de cobre en algunas zonas de la placa. Si el usuario lo desea, puede seleccionar ciertas zonas de la placa a las que se les quitará todo el cobre de la superficie. La realización de este paso se deja a elección del usuario puesto que este paso no es obligatorio y puede ser omitido.

## Aislamiento de las capas

El quinto paso se encarga de aislar las capas que forman nuestra placa. Este paso es tan sencillo como pulsar sobre el quinto icono y esperar a que la tarea finalice.

## Exportar los archivos

Llegamos al último paso del proceso con CircuitCam. Al igual que los últimos pasos, este es un paso muy sencillo ya que nos encargaremos simplemenre de exportar el proyecto y generar el archivo `.LMD`, con el que podrá operar BoardMaster y podremos imprimir nuestro diseño. Debemos indicar una ruta y un nombre del archivo para que el archivo `LMD` sea almacenado.

# Interfaz de BoardMaster

BoardMaster es el software que nos permitirá comunicarnos y manejar la máquina LPKF durante todo el proceso de impresión. Antes de comenzar a explicar este proceso, es necesario explicar algunas de las funcionalidades más importantes de este software, así como familiarizar al usuario con la interfaz que presenta.

Cuando BoardMaster se ejecuta presenta la interfaz que se puede ver a continuación. 

{{< polaroid src="interfazBM.png" caption="Interfaz BoardMaster" >}}

Como se puede observar en la imagen, en la interfaz hay algunos números señalando partes de la misma para simplificar su explicación. La utilidad de cada elemento es la siguiente.

1. Lista desplegable con los distintos pasos que se deben llevar a cabo para realizar la impresión.
2. Lista desplegable con los nombres y medidas de las herramientas que van a ser usadas para la impresión. Las herramientas son unas brocas que deben ser situadas al norte de la mesa de trabajo. Esta lista muestra el orden de las herramientas de izquierda a derecha, la herramienta con el número 1 será la que está situada más a la izquierda desde el punto de vista del usuario.
3. Representación virtual de las herramientas en la mesa de trabajo, a cada tipo de herramienta se la asigna un color que se corresponde con el color que posee la herramienta real.
4. **Botón de la cruceta** que permite situar el cabezal de la máquina en una coordenada concreta de la mesa.
5. **Botón de Parking** que nos permitirá extraer la mesa de trabajo para realizar cualquier operación, como por ejemplo colocar la placa de cobre o colocar las herramientas.
6. Estas **flechas de desplazamiento** permiten al usuario mover el cabezal con precisión. Las flechas de la izquierda permiten mover el cabezal en los ejes X e Y, las flechas de la derecha permiten moverlo en el eje Z. El número situado en el centro de las flechas permite indicar el número de unidades que se va a desplazar el cabezal por una pulsación.
7. Estos cuatro botones forman un conjunto que permite manipular el footprint de la placa. El botón de abajo a la izquierda permite mover la representación virtual de la placa a lo largo de la mesa de trabajo para elegir donde realizar la impresión, el botón de la derecha realiza copias de la placa lo cual será muy útil si por ejemplo se quiere realizar la impresión de más de una placa de nuestro diseño. Los botones de arriba son **botones de selección** de pads y pistas, el de la izquierda solo seleccionará aquellos pads y pistas que se le indiquen, el de la derecha en cambio seleccionará los pads y pistas que hemos indicado así como todo a lo que esté conectado.
8. **Botón Start/Stop**. Cuando se realice el proceso de impresión este botón se utilizará para iniciar o detener cualquiera de las etapas del proceso.
9. Representación virtual de la mesa de trabajo.
10. **Botones de adición y eliminación** de pads y pistas. Para el software BoardMaster, cuando queremos seleccionar algún componente del footprint no basta con los botones de selección, estos componentes despueś deben ser añadidos. Por ejemplo si quisiesemos imprimir un pad de la placa y simplemente lo seleccionamos el pad no será impreso dado que debe ser añadido. Los botones **+** y **-** añaden o eliminan una selección respectivamente, los botones **Alt+** y **Alt-** añaden o eliminan todos los componentes del footprint.

Si el uso de algún elemento de la interfaz no se ha entendido muy bien en esta explicación, durante el proceso de impresión todo quedará más claro. 

# Preparación de la impresión

Una vez generado el archivo `LMD` con CircuitCam se deben preparar tanto la máquina como el software BoardMaster para realizar el proceso de impresión.

Lo primero de todo es encender la máquina LPKF, muy importante, el programa **BoardMaster no debe ejecutarse hasta que la máquina esté encendida** con el objetivo de evitar cualquier problema de comunicación entre el software y la máquina. Una vez que nos aseguremos de que la máquina está encendida se procede a abrir BoardMaster. 


## Preparación de la máquina LPKF

La primera tarea será la de colocar las herramientas, con cuidado de no dañarlas, en su lugar correspondiente. Para ello, primero debemos observar las herramientas que el software considera que tiene la máquina (la lista desplegable de las herramientas nos será de gran ayuda) y colocar cada una en su lugar correspondiente. Si nuestro diseño requiere de alguna herramienta que no aparezca en la interfaz de BoardMaster tenemos dos alternativas, antes de iniciar el proceso de impresión añadir esa herramienta en el software y después la colocamos en la máquina, o también, en el momento que lleguemos a una etapa que requiera de esa herramienta, BoardMaster nos avisará y podremos realizar el cambio tanto en el software como en la máquina. Lo más importante es que en este paso las herramientas de la mesa virtual de BoardMaster y de la máquina LPKF sean las mismas y estén colocadas en su lugar correspondiente.

Una vez tengamos las herramientas colocadas hay que colocar la placa de cobre en la mesa, muy importante siempre que manejemos la placa el **uso de guantes**, ya que la grasa de las manos ensuciaría la placa provocando su oxidación. Si observamos detenidamente la placa de cobre, en los laterales tiene dos pequeños orificios que encajan perfectamente con la mesa, cogemos la placa con los guantes y la encajamos bien en la mesa comprobando que no se mueva. La colocación es muy importante, ya que si para la capa top y para la capa bottom, la placa no fuese colocada de la misma forma, las pistas no coincidirían con los pads produciendo un mal proceso de fabricación que puede concluir en una placa defectuosa.

{{< polaroid src="ToolsLPKF.jpg" caption="Ejemplo de herramientas colocadas en la máquina LPKF" >}}

## Preparación de BoardMaster

Para empezar hay que señalar el área de trabajo en la mesa virtual de la interfaz de BoardMaster, es decir vamos a indicar la zona en la que se va a mover el cabezal de la máquina durante la impresión. Para ello en la lista desplegable señalamos una herramienta (preferentemente una de marcado, se suelen identificar con el color naranja) y esperamoz a que el cabezal de la máquina la coja. A continuación, ya sea con el botón de la cruceta o con las flechas de desplazamiento, desplazamos el cabezal de la máquina a la zona de la mesa donde queramos situar la esquina inferior izquierda de nuestro área de trabajo. Una vez llegado al punto  deseado confirmamos seleccionando **Configuración->Material->Seleccionar Esquina Inferior**. Hacemos lo mismo para seleccionar la esquina superior derecha pero en este caso confirmamos con **Configuración->Material->Seleccionar Esquina Superior**. El área de trabajo se habrá convertido en un recuadro gris sobre la mesa virtual, este área representa ahora la zona de la placa donde podemos realizar impresiones. Muy importante que a la hora de hacer la selección observemos la zona que recorre el cabezal real de la máquina, ya que por ejemplo si seleccionamos una zona en la que ya se ha realizado el corte de una placa, la impresión fallaría.

{{< polaroid src="workArea.png" caption="Ejemplo de área de trabajo" >}}

Por último hay que importar el proyecto que se ha generado anteriormente con CircuitCam, seleccionamos **Archivo->Importar->LMD** e introducimos la ruta donde almacenamos el archivo con extensión `.LMD`. Una vez importado en la mesa de trabajo aparecerá un footprint de nuestro PCB, el cual podremos desplazar a nuestro gusto por la zona de trabajo.


# Proceso de impresión

Todo el entorno, tanto la máquina LPKF como BoardMaster está preparado para iniciar la impresión, ahora debemos ejecutar, en orden, cada una de las étapas de la lista desplegable para realizar el proceso. Nosotros solo debemos preocuparnos en seleccionar la étapa de la lista, añadir los componentes del footprint, pulsar el botón Start y esperar a que finalice, el software sabrá que herramienta elegir y como operar para cada una de las tareas. Si no entiendes alguna indicación del proceso ve a [Interfaz de BoardMaster](#interfaz-de-boardmaster). El procedimiento de la impresión será el siguiente:

1. **MarkingDrills**: Primer paso del proceso de impresión, su objetivo es el de marcar los taladros, para que llegada la hora de taladrar el taladro no resbale y los pads y pistas se
desplacen. Se seleccionan todos los pads y pistas con el botón Alt+ y se pulsa el botón Start. Una vez termine el proceso, se puede observar si el proceso se ha realizado correctamente pulsando sobre el botón parking y viendo el resultado en la placa. 
2. **DrillingPlated**: En este paso la broca atravesará la placa y creará los pads. Dado que tampoco es una tarea muy delicada podemos añadir todos los pads con el botón Alt+ e iniciar el proceso con Start.
3. **Info! Galvanic Plating**: Esta es una etapa informativa, lo que nos está diciendo es que si vamos a hacer el galvanizado de los pads es el momento idóneo para hacerlo. Este paso puede ser omitido.
4. **Read_Bottom**: Esta de nuevo es otra etapa informativa que nos está diciendo que preparemos la capa bottom para ser impresa.
5. **MillingBottom**: Aquí la máquina realizará las pistas que conectan los pads de la capa bottom. Para comprobar que la máquina está calibrada, con el botón de selección de la derecha (el que seleccionaba unicamente aquello que indicabamos) se deben indicar pequeñas pistas aisladas que rodean los pads de la placa, añadirlos con el botón + y realizar la impresión. Si tras la impresión se ve que la pista está limpia se pulsa sobre el botón Alt+ para realizar la impresión completa de las pistas, en caso contrario habrá que calibrar la máquina.
6. **Read_Top**: En esta etapa se nos está indicando que debemos darle la vuelta a la placa, puesto que vamos a imprimir la capa top. Para ello no hay que olvidarse de ponerse los guantes, darle la vuelta a la placa y encajarla bien de nuevo en la mesa para que el proceso sea lo más preciso posible.
7. **MillingTop**: Con este paso se llega a los momentos más críticos de la impresión. Al girar la placa se puede dar el caso de que las pistas no correspondan a la perfección con las de la capa bottom, pudiendo ocasionar malas conexiones. Para solventar este problema, se puede proceder como en el caso anterior imprimiendo pequeñas pistas aisladas que rodean pads y comprobando su impresión. Si las pistas no coinciden bien con el pad se debe calibrar la máquina modificarndo el origen de la misma. Con este fin se pulsa sobre **Configuración->Selecciones de Máquina**, en la ventana que aparece pulsamos sobre Desbloquear, y mediante prueba y error, modificamos las coordenadas del Home a la vez que imprimimos pequeñas pistas aisladas hasta que las pistas coincidan correctamente con los pads.
8. **DrillingUnplated**: Etapa omitida.
9. **CuttingInside**: En esta etapa la máquina realizará los cortes internos de la placa. Lo más probable es que la placa a fabricar no tenga cortes internos por lo que esta etapa puede omitirse.
10. **CuttingOutside**: Última etapa en el software. En esta etapa la máquina realizará el corte externo, dejando los espacios sin cortar que indicamos en CircuitCam.

{{< polaroid src="changeHome.png" caption="Ventana de modificación del home" >}}


Cuando los pasos hayan finalizado, pulsamos el botón parking para extraer la mesa de trabajo, y con los guantes, coger la lámina de PCB y extraer la/s placa/s impresa/s de la placa original. Una vez extraídas se las aplica una laca protectora y se liman los bordes para poder cogerla con las manos desnudas. 

Para finalizar, se debe dejar el entorno lo más limpio posible por lo que es importante extraer las herramientas y apagar la máquina. El proceso de impresión ha finalizado.



