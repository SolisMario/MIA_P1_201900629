# Manual de Usuario

## Aplicación de Comandos

<div style="text-align: justify"> 
La aplicacion presentada hace uso exclusivo de una consola, a excepción de los reportes que genera, estos son presentados con  Graphviz. Esta no cuenta con menús, sino que se utilizarán comandos. No distinguirá entre mayúsculas y minúsculas. La aplicación será capaz de leer desde standard input (stdin) y escribir a standard output (stdout) de ser necesario. Hay parámetros obligatorios y opcionales. Solo se puede colocar un comando por línea.
<br>
<br>
Si se utiliza un parámetro que no está especificado en este documento, debe mostrar un mensaje de error. Se utilizarán espacios en blanco para separar cada parámetro. Si se necesita que algún valor lleve espacios en blanco se encerrará entre comillas (" "). <b>Los parámetros pueden venir en cualquier orden.</b>
</div>

## Administración de Discos

<div style="text-align: justify">
Estos comandos permitirán crear archivos que simularán discos duros en los que se podrá formatear más adelante con el sistema de archivos ext2 o ext3. Estos comandos estarán disponibles desde que se inicia el programa.</div>

#### 1. MKDISK

<div style="text-align: justify">
Este comando creará un archivo binario que simulará undisco duro, estos archivos binarios tendrán la extensión <b>disk</b> y su contenido al inicio será 0. Deberá ocupar físicamente el tamaño indicado por los parámetros, (no importa que el sistema operativo no muestre el tamaño exacto). Recibirá el nombre del archivo que simulará el disco duro y tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-size | Obligatorio | Este parámetro recibirá un número que indicará el tamaño del disco a crear. Debe ser positivo y mayor que cero, si no se mostrará un error.
-f | Opcional | Indicará el ajuste que utilizará el disco para crear las particiones dentro del disco Podrá tener los siguientes valores: <br>BF: Indicará el mejor ajuste (Best Fit) <br>FF: Utilizará el primer ajuste (First Fit) <br>WF: Utilizará el peor ajuste (Worst Fit) <br>Ya que es opcional, se tomará el primer ajuste si no está especificado en el comando. Si se utiliza otro valor que no sea alguno de los anteriores mostrará un mensaje de error.
-u | Opcional | Este parámetro recibirá una letra que indicará las unidades que utilizará el parámetro size. Podrá tener los siguientes valores: <br><b>k </b>que indicará que se utilizarán Kilobytes (1024 bytes) <br><b>m</b> en el que se utilizarán Megabytes (1024 * 1024 bytes) <br>Este parámetro es opcional, si no se encuentra se creará un disco con tamaño en Megabytes. Si se utiliza otro valor debe mostrarse un mensaje de error.
-path | Obligatorio | Este parámetro será la ruta en el que se creará el archivo que representará el disco duro. Si las carpetas de la ruta no existen deberán crearse.

#### 2. RMDISK

<div style="text-align: justify">Este parámetro elimina un archivo que representa a un disco
duro mostrando un mensaje de confirmación para eliminar.
Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta en el que se eliminará el archivo que representará el disco duro. Si el archivo no existe, debe mostrar un mensaje de error.

#### 3. FDISK

<div style="text-align: justify">Este comando administra las particiones en el archivo que representa al disco duro. Deberá mostrar un error si no se pudo realizar la operación solicitada sobre la partición, especificando por qué razón no pudo crearse (Por espacio, por restricciones de particiones, etc.). No se considerará el caso de que se pongan parámetros incompatibles, por ejemplo, en un mismo comando fdisk llamar a delete y add. La estructura de cada disco se explicará más adelante. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-size | Obligatorio al crear | Este parámetro recibirá un número que indicará el tamaño de la partición a crear. Debe ser positivo y mayor a cero, de lo contrario se mostrará un mensaje de error.
-u | Opcional | Este parámetro recibirá una letra que indicará las unidades que utilizará el parámetro size. Podrá tener los siguientes valores: <br><b>B:</b> indicará que se utilizarán bytes. <br><b>K:</b> indicará que se utilizarán Kilobytes(1024 bytes) <br><b>M:</b>indicará que se utilizarán Megabytes(1024 * 1024 bytes). <br>Este parámetro es opcional, si no se encuentra se creará una partición en Kilobytes. Si se utiliza un valor diferente mostrará un mensaje de error.
-path |  Obligatorio | Este parámetro será la ruta en la que se encuentra el disco en el que se o creará la partición. Este archivo ya debe existir, si no se mostrará un error.
-type | Opcional | Indicará que tipo de partición se creará. Ya que es opcional, se tomará como primaria en caso de que no se indique. Podrá tener los siguientes valores: <br><b>P:</b> en este caso se creará una partición primaria. <br><b>E:</b> en este caso se creará una partición extendida. <br><b>L:</b> Con este valor se creará una partición lógica. <br>Las particiones lógicas sólo puedenestar dentro de la extendida sin sobrepasar su tamaño. Deberá tener en cuenta las restricciones de teoría de particiones: <br>La suma de primarias y extendidas debe ser como máximo 4. Solo puede haber una partición extendida por disco. No se puede crear una partición lógica si no hay una extendida. <br>Si se utiliza otro valor diferente a los anteriores deberá mostrar un mensaje de error.
-f | Opcional | Indicará el ajuste que utilizará la partición para asignar espacio. Podrá tener los siguientes valores: <br><b>BF:</b> Indicará el mejor ajuste (Best Fit) <br><b>FF:</b> Utilizará el primer ajuste (First Fit) <br><b>WF:</b> Utilizará el peor ajuste (Worst Fit) <br>Ya que es opcional, se tomará el peor ajuste si no está especificado en el comando. Si se utiliza otro valor que no sea alguno de los anteriores mostrará un mensaje de error.
-delete | Opcional | Este parámetro indica que se eliminará una partición. Este parámetro se utiliza junto con -name y -path. Se deberá mostrar un mensaje que permita confirmar la eliminación de dicha partición. <br>Si la partición no existe deberá mostrar error. Si se elimina la particiónextendida, deben eliminarse las particiones lógicas que tenga adentro. <br>Recibirá los siguientes valores: <br><b>Fast:</b> Esta opción marca como vacío el espacio en la tabla de particiones. <br><b>Full:</b> Esta opción además marcar como vació el espacio en la tabla de particiones, rellena el espacio con el carácter \0. Si se utiliza otro valor diferente, mostrará un mensaje de error.
-name |Obligatorio| Indicará el nombre de la partición. El nombre no debe repetirse dentro o de las particiones de cada disco. Si se va a eliminar, la partición ya debe existir, si no existe debe mostrar un mensaje de error.
-add| Opcional |Este parámetro se utilizará para agregar o quitar espacio de la partición. Puede ser positivo o negativo. Tomará el parámetro units para las unidades a agregar o eliminar. <br>En el caso de agregar espacio, deberá comprobar que exista espacio libre después de la partición. En el caso de quitar espacio se debe comprobar que quede espacio en la partición (no espacio negativo).

#### 4. MOUNT

<div style="text-align: justify">Este comando montará una partición del disco en el sistema
e imprimira en consola un resumen de todas las particiones
montadas actualmente.<BR>
Cada partición se identificará por un id que tendrá la
siguiente estructura utilizando el número de carnet:<br>
*Últimos dos dígitos del Carnet + Numero + Letra<br>
Por cada partición montada, deberá imprimir una línea en consola con siguiente formato: <br><b>Path del disco utilizado|Nombre de la partición|ID de la partición montada.</b> <br>El número será el misma para particiones en el mismo disco y la letra diferente para particiones en el mismo disco. (NOTA: Este Comando Debe Realizar el montaje en memoria ram no debe escribir esto en el disco) Los parámetros admitidos por este comando son:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta en la que se encuentra el disco que se montará en el sistema. Este archivo ya debe existir.
-name | Obligatorio | Indica el nombre de la partición a cargar. Si no existe debe mostrar error.

#### 5. UMOUNT

<div style="text-align: justify">Desmonta una partición del sistema. Se utilizará el id que se le asignó a la partición al momento de cargarla. Recibirá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-id | Obligatorio | Especifica el id de la partición que se desmontará. Si no existe el id deberá mostrar un error.

#### 6. MKFS

<div style="text-align: justify">DEste comando realiza un formateo completo de la partición, se formateará como ext2 por defecto si en caso el parámetro fs no está definido. También creará un archivo en la raíz llamado users.txt que tendrá los usuarios y contraseñas del sistema de archivos. La estructura de este archivo se explicará más adelante.</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-id | Obligatorio | Indicará el id que se generó con el comando mount. Si no error. existe mostrará Se utilizará para saber la partición y el disco que se utilizará para hacer el sistema de archivos.
-type | Opcional | Indicará que tipo de formateo se realizará. Podrá tener los siguientes valores: <br><b>Fast:</b> en este un caso se realiza un formateo rápido <br><b>Full:</b> en este un caso se formateo realizará completo.  <br>Ya que es opcional, se tomará como un formateocompleto si no especifica esta opción.
-fs | Opcional | se Indica el sistema de archivos a formatear. Podrá tener los siguientes valores: <br><b>2fs:</b> Para el sistema EXT2 <br><b>3fs:</b> Para el sistema EXT3 <br>Por defecto será ext2.

## Administracion de Usuarios y Grupos

<div style="text-align: justify">Este archivo será un archivo de texto, llamado users.txt guardado en el sistema ext2/ext3 de la raíz de cada partición. Existirán dos tipos de registros, unos para grupos y otros para usuarios. Un id 0 significa que el usuario o grupo está eliminado, el id de grupo o de usuario irá aumentando según se vayan creando usuarios o grupos. Tendrá la siguiente estructura: <br>GID, Tipo, Grupo <br>UID, Tipo, Grupo, Usuario, Contraseña <br>El estado ocupará una letra, el tipo otra, el grupo ocupará como máximo 10 letras al igual que el usuario y la contraseña. <br>Al inicio existirá un grupo llamado root, un usuario root y una contraseña (123) para el usuario root.</div>

#### 1. LOGIN

<div style="text-align: justify">Este comando se utiliza para iniciar sesión en el sistema. No se puede iniciar otra sesión sin haber hecho un logout antes, si no, debe mostrar un mensaje de error indicando que debe cerrar sesión. Recibirá los Siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-user | Obligatorio | Especifica el nombre del usuario que iniciará sesión. Si no se encuentra mostrará un mensaje indicando que el usuario no existe. *distinguirá mayúsculas de minúsculas.
-pwd | Obligatorio | Indicará la contraseña del usuario que iniciara sesión. Si no coincide debe mostrar un mensaje de autenticación fallida. <br>*distinguirá entre mayúsculas y minúsculas.
-id | Obligatorio | Indicará el id de la partición montada de la cual van a iniciar sesión. De lograr iniciar sesión todas las acciones se realizarán sobre este id.


#### 2. LOGOUT

<div style="text-align: justify">Este comando se utiliza para cerrar sesión. Debe haber una sesión activa anteriormente para poder utilizarlo, si no, debe mostrar un mensaje de error. <b>Este comando no recibe parámetros.</b></div>


#### 3. MKGRP

<div style="text-align: justify">Este comando creará un grupo para los usuarios de la partición y se guardará en el archivo users.txt de la partición, este comando solo lo puede utilizar el usuario <b>root. Si otro usuario lo intenta ejecutar, deberá mostrar un mensaje de error</b>, si el grupo a ingresar ya existe deberá mostrar un mensaje de error. Distinguirá entre mayúsculas y minúsculas. Recibirá los Siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-name | Obligatorio | Indicará el nombre que tendrá el grupo

#### 4. RMGRP

<div style="text-align: justify">Este comando eliminará un grupo para los usuarios de la partición. Solo lo puede utilizar el usuario root, si lo utiliza alguien más debe mostrar un error. Recibirá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-name | Obligatorio | Indicará el nombre del grupo a eliminar. Si el grupo no se encuentra dentro de la partición debe mostrar un error.

#### 5. MKUSR

<div style="text-align: justify">Este comando crea un usuario en la partición. Solo lo puede ejecutar el usuario <b>root</b>, si lo utiliza otro usuario deberá mostrar un error. Recibirá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-usr | Obligatorio | Indicará el nombre usuario a crear, si existe, deberá mostrar error indicando que existe el usuario. <br><b>Máximo: 10 caracteres.</b>
-pwd | Obligatorio | Indicará la contraseña del usuario. <br><b>Máximo: 10 caracteres.</b>
-grp | Obligatorio | Indicará el grupo al que pertenecerá el usuario. <br>Debe de existir en la partición en la que se está creando el usuario, si no debe mostrar un mensaje de error. <br><b>Máximo: 10 caracteres.</b>

#### 5. RMUSR

<div style="text-align: justify">Este comando elimina un usuario en la partición. Solo lo puede ejecutar el usuario root, si lo utiliza otro usuario deberá mostrar un error. Recibirá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-usr | Obligatorio  |Indicará el nombre del usuario a eliminar. Si el usuario no se encuentra dentro de la partición debe mostrar un error.

## Administración de Carpetas Archivos:

<div style="text-align: justify">Estos comandos permitirán crear archivos y carpetas, así como editarlos, copiarlos, moverlos y eliminarlos.</div>

#### 1. TOUCH

<div style="text-align: justify">Este comando permitirá crear un archivo, el propietario será el usuario que actualmente ha iniciado sesión. Tendrá los permisos 664. El usuario deberá tener el permiso de escritura en la carpeta padre, si no debe mostrar un error. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo que se creará. Si lleva espacios en blanco deberá encerrarse entre comillas. Si ya existe debe mostrar un mensaje si se desea sobreescribir el archivo. Si no existen las carpetas padres, debe mostrar error, a menos que se utilice el parámetro r, que se explica posteriormente.
-r | Opcional | Si se utiliza este parámetro y las carpetas especificadas por el parámetro path no existen, entonces deben crearse las carpetas padres. Si ya existen, no deberá crear las carpetas. No recibirá ningún valor, si lo recibe debe mostrar error.
-size | opcional | Este parámetro indicará el tamaño en bytes del archivo, el contenido serán números del 0 al 9 cuantas veces sea necesario hasta cumplir el tamaño ingresado. Si no se utiliza este parámetro, el tamaño será 0 bytes. Si es negativo debe mostrar error.
-cont | Opcional | Indicará un archivo en el disco duro de la computadora que tendrá el contenido del archivo. Se utilizará para cargar contenido en el archivo. La ruta ingresada debe existir, si no mostrará unmensaje de error.
-stdin | Opcional | Este parámetro es mutuamente exclusivo con el parámetro cont. Parámetro booleano. Indica que el contenido del archivo a crear será obtenido de stdin (standard input).

#### 2. CAT

<div style="text-align: justify">Este comando permitirá mostrar el contenido del archivo, si el usuario que actualmente está logueado tiene acceso al permiso de lectura. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-filen | Obligatorio | Admitir como argumentos una lista de n ficheros que hay que enlazar. Estos se encadenarán en el mismo orden en el cual fueron especificados. Si no existe el archivo o no tiene permiso de lectura, debe mostrarse un mensaje de error.

#### 3. RM

<div style="text-align: justify">Este comando permitirá eliminar un archivo o carpeta y todo su contenido, si el usuario que actualmente está logueado tiene acceso al permiso de escritura sobre el archivo y en el caso de carpetas, eliminará todos los archivos o subcarpetas en los que el usuario tenga permiso de escritura. Si no pudo eliminar un archivo o subcarpeta dentro de la carpeta por permisos, no deberá eliminar nada dentro de esa carpeta ni la carpeta como tal. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo o carpeta que se eliminará. Si lleva espacios en blanco deberá encerrarse entre comillas. Si no existe el archivo o no tiene permisos de escritura en la carpeta o en el archivo, debe mostrarse un mensaje de error. Si no pudo eliminar algún archivo o carpeta no deberá eliminar los padres.

#### 4. EDIT

<div style="text-align: justify">Este comando permitirá editar el contenido de un archivo para asignarle otro contenido. Funcionará si el usuario que actualmente está logueado tiene acceso al permiso de lectura y escritura sobre el archivo, si no debe mostrar error. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo o carpeta que se editará. Si lleva espacios en blanco deberá encerrarse entre comillas. Si no existe el archivo o no tiene permisos de escritura debe mostrarse un mensaje de error.
-cont | Obligatorio | Contiene la ruta a un archivo en el sistema operativo que contendrá el contenido que será Agregado a la edición.
-stdin | Opcional | Mutuamente excluyente con el parámetro cont. Parámetro booleano. Indica que el contenido a agregar al archivo será leído desde standard input (stdin)

#### 5. REN

<div style="text-align: justify">Este comando permitirá cambiar el nombre de un archivo o carpeta, si el usuario actualmente logueado tiene permiso de escritura sobre el archivo o carpeta. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo o carpeta al que se le cambiará el nombre. Si lleva espacios en blanco deberá encerrarse entre comillas. Si no existe el archivo o carpeta o no tiene permisos de escritura deberá mostrar un mensaje de error.
-name | Obligatorio | Especificará el nuevo nombre del archivo, debe verificar que no exista un archivo con el mismo nombre, de ser así debe mostrar un mensaje de error.

#### 6. MKDIR

<div style="text-align: justify">Este comando es similar a mkfile, pero no crea archivos, sino carpetas. El propietario será el usuario que actualmente ha iniciado sesión. Tendrá los permisos 664. El usuario deberá tener el permiso de escritura en la carpeta padre, si no debe mostrar un error. Tendrá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta de la carpeta que se creará. Si lleva espacios en blanco deberá encerrarse entre comillas. Si no existen las carpetas padres, debe mostrar error, a menos que se utilice el parámetro p.
-p | Opcional | Si se utiliza este parámetro y las carpetas padres en el path parámetro no existen, entonces deben crearse. Si ya existen, no realizará nada. No recibirá ningún valor, si lo recibe debe mostrar error.

#### 7. CP

<div style="text-align: justify">Este comando permitirá realizar una copia del archivo o carpeta y todo su contenido hacia otro destino.</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo o carpeta que se desea copiar. Si lleva espacios en blanco deberá encerrarse entre comillas. Debe copiar todos los archivos y carpetas con todo su contenido, a los cuales tenga permiso de lectura. Si no tiene permiso de lectura, no realiza la copia únicamente de ese archivo o carpeta. Muestra un error si no existe la ruta.
-dest | Obligatorio | Este parámetro será la ruta a donde se va a copiar el contenido. Debe tener permisos de escritura sobre esta carpeta, si no deberá mostrar un mensaje de error. De no existir la carpeta deberá mostrar un mensaje de error.

#### 8. MV

<div style="text-align: justify">Este comando moverá un archivo o carpeta y todo su contenido hacia otro destino. Si el origen y destino están dentro de la misma partición, solo cambiará las referencias, para que ya no tenga el padre origen sino, el padre destino, y que los padres de la carpeta o archivo ya no tengan como hijo a la carpeta o archivo que se movió. Solo se deberán verificar los permisos de escritura sobre la carpeta o archivo origen.</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path | Obligatorio | Este parámetro será la ruta del archivo o carpeta que se desea mover. Si lleva espacios en blanco deberá encerrarse entre comillas. <br>Debe mover todos los archivos y carpetas con todo su contenido, a los cuales tenga permiso de escritura. Si no tiene permiso de escritura, no realiza el movimiento Muestra un error si no existe la ruta.
-dest | Obligatorio | Este parámetro será la ruta de la carpeta a la que se moverá el archivo o carpeta. Debe tener permiso de escritura sobre la carpeta. Si lleva espacios en blanco deberá encerrarse entre comillas. Debe mostrar un mensaje de error si no tiene permisos para escribir o si la carpeta no existe.

### 9. FIND

<div style="text-align: justify">Este comando permitirá realizar una búsqueda por el nombre del archivo o carpeta. Por cada resultado, imprimirá una linea en consola con el siguiente formato: <br><b>nombre archivo|indice de inodo|tipo de archivo.</b> Donde el tipo de archivo puede ser folder o archivo. Permitirá los siguientes caracteres especiales:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-path |Obligatorio |Este parámetro será la ruta de la carpeta en el que se inicia la búsqueda, deberá buscar en todo su contenido. Si lleva espacios en blanco deberá encerrarse entre comillas.
-name | Obligatorio  | Debe tener permisos de lectura en los archivos que buscará. Indica el nombre de la carpeta o archivo que se desea buscar.

### 10. CHGRP

<div style="text-align: justify">Cambiará el grupo al que pertenece el usuario. Únicamente lo podrá utilizar el usuario root. Recibirá los siguientes parámetros:</div>

Parámetro | Categoría | Descripción
----------|----------|----------
-usr | Obligatorio | Especifica el nombre del usuario al que se le cambiará de grupo. Si no existe debe mostrar un error.
-grp | Obligatorio | Contendrá el nombre del nuevo grupo al que pertenecerá el usuario. Si no existe o está eliminadodebe mostrar un error.

### 11. PAUSE

<div style="text-align: justify">Este comando será solo la palabra “pause” no tiene atributos al ingresar este comando se pondrá en pausa solicitando que presione cualquier tecla para continuar. Este comando solamente tiene sentido si se encuentra dentro de un archivo script con el comando exec. Si se utiliza por si solo (sin exec) este comando no realizará ninguna acción.</div>

## Reportes

### Rerporte MBR

Mostrará tablas con toda la información del MBR, así como de los EBR que se pudieron haber creado.

### Reporte DISK 
Este reporte mostrará la estructura de las particiones, el mbr del disco y el porcentaje que cada partición o espacio libre tiene dentro del disco (La sumatoria de los porcentajes debe de ser 100%).

### Reporte INODE
Mostrará información de los inodos utilizados en modo tabla. Si no están utilizados no debe mostrarlos.

### Reporte BLOCK
Mostrará la información de todos los bloques utilizados. Si no estánutilizados no debe mostrarlos.

### Reporte TREE
Este reporte genera el árbol de el sistema ext2/ext3. Se mostrará toda la información de los inodos o bloques desde el inodo indicado. Este comando aceptará el parámetro root como un parámetro opcional. De no estar presente, se genera el reporte para todo el sistema de archivos. De indicarse el índice de un inodo, solo deberá generar el reporte del árbol utilizando el inodo inciado como raíz. Si el índice indicado con parámetro root no existe o está vacío, deberá mostar un mensaje de error y no generará el reporte. En este reporte no deben mostarse los bloques o inodos libres, únicamente se mostrarán los bloques que están siendo utilizados. Deberá ser como el siguiente (En este ejemplo no se ponen todos los datos, bloques y flechas por falta de espacio, se utilizaron bloques de carpeta con capacidad 2, bloques de apuntadores con capacidad 2 y bloques de archivo con capacidad 5):

### Reporte SB 
Muestra toda la información del superbloque en una tabla.

### Reporte FILE 
Este reporte muestra el nombre y todo el contenido del archivo especificado en el parámetro file. 

### Reporte LS 
Este reporte mostrará la información de los archivos y carpetas con permisos, propietario, grupo propietario, fecha de modificación, hora de modificación, tipo, fecha de creación.