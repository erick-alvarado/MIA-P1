# Manual de usuario
## Manejo de discos
## Ejecucion
La ejecucion de la aplicacion, puede realizarse mediante cualquier sistema operativo. Se debe utilizar el archivo.exe en caso de utilizar Windows y el archivo sin extension para ejecutar en linux. Estos pueden ser ubicados en la carpeta Main.

Para generar el ejutable en Linux se hace uso del siguiente comando, ubicandose previamente en el directorio Main:
```sh
g++ -o hola hola.cc
```
Para ejecutar un comando desde la consola hacemos uso de la siguiente sintaxis:
```sh
sudo ./hola comando
```
Por ejemplo, para ejecutar el comando exec:
```sh
sudo ./hola exec -path= /mnt/c/Users/alexa/OneDrive/Desktop/pruebas.txt
```

## Comandos
La aplicacion consta de comandos basicos de un manejador de discos. Estos pueden recibir multiples parametros obligatorios u opcionales. 
### MKDISK
Este comando crea un archivo binario que simula un disco duro. 
- Size
- F (Opcional)
- U (Opcional)
- Path

### RMDISK
Este comando elimina un archivo binario. 
- Path

### FDISK
Este comando administra las particiones en el archivo que representa el disco duro. Puede crear particiones de tipo primarias, extendidas y logicas. Teniendo como limite 4 primarias y 1 extendida por disco. Las particiones logicas no tienen limite alguno mas alla del espacio.

- Size
- Name
- Path
- F (Opcional)
- U (Opcional)
- Type (Opcional)
- Delete (Opcional)
- Add (Opcional)

### Mount
Este comando montara una particion del disco en el sistema e imprime una resumen de las particiones montadas actualmente.

- Name
- Path

### Unmount
Este comando desmonta una particion del sistema.

- Id

### MKFS
Este comando realiza un formateo completo de la particion, mediante los distintos sistemas de archivos siendo estos ext2 y ext3.
- Id
- Fs (Opcional)
- Type (Opcional)

### REP
Este comando es el encargado de generar reportes, puede recibir multiples instrucciones. Se encuentra habilitados los reportes disk y mbr.
- Name
- Path
- Id

### Exec
Este comando ejecuta multiples comandos desde un archivo especifico. 
- Path


