#Crea un disco de 3000 Kb en la carpeta home
Mkdisk -Size::3000 +unit::K -path::"/home/user/" \
-name::"Disco1.dsk"
#Se crearán carpetas si no existen
mkdisk -size::5 +unit::M -path::"/home/mis discos/" \
-name::"Disco3.dsk"
#Creará un disco de 10 Mb ya que no hay parámetro unit
Mkdisk -size::10 -path::"/home/mis discos/" \
-name::"Disco4.dsk"

#Elimina Disco4.dsk
rmDisk -path::"/home/mis discos/Disco4.dsk"

#Crea una partición primaria llamada Particion1 de 300 kb
#con el peor ajuste en el disco Disco1.dsk
fdisk -Size::300 -path::"/home/Disco1.dsk" -name::"Particion1"
#Crea una partición extendida dentro de Disco2 de 300 kb
#Tiene el peor ajuste
fdisk +type::E -path::"/home/Disco2.dsk" +Unit::K \
-name::"Particion2" -size::300
#Crea una partición lógica con el mejor ajuste, llamada Particion3,
#de 1 Mb en el Disco3
fdisk -size::1 +type::L +unit::M +fit::BF \
-path::"/mis discos/Disco3.dsk" -name::"Particion3"
#Intenta crear una partición extendida dentro de Disco2 de 200 kb
#Debería mostrar error ya que ya existe una partición extendida
#dentro de Disco2
fdisk +type::E -path::"/home/Disco2.dsk" -name::"Part3" \
+Unit::K -size::200
#Elimina de forma rápida una partición llamada Particion1 
fdisk +delete::fast -name::"Particion1" \
-path::"/home/Disco1.dsk"
#Elimina de forma completa una partición llamada Particion1 
fdisk -name::"Particion1" +delete::full \
-path::"/home/Disco1.dsk"
#Agrega 1 Mb a la partición Particion4 del Disco4.dsk
#Se debe validar que haya espacio libre después de la partición 
fdisk +add::1 +unit::M -path::"/home/mis discos/Disco4.dsk" \
-name::"Particion 4"

#Monta las particiones de Disco1.dsk
mount -path::"/home/Disco1.dsk" -name::"Part1" #id::vda1
mount -path::"/home/Disco2.dsk" -name::"Part1" #id::vdb1
mount -path::"/home/Disco3.dsk" -name::"Part2" #id::vdc1
mount -path::"/home/Disco1.dsk" -name::"Part2" #id::vda2
#Si se coloca el comando mount sin parametros mostrará en la consola las particiones montadas.
#id::vda1 -path::"/home/Disco1.dsk" -name::"Part1"
#id::vdb1 -path::"/home/Disco2.dsk" -name::"Part1"
#id::vdc1 -path::"/home/Disco3.dsk" -name::"Part2"
#id::vda2 -path::"/home/Disco1.dsk" -name::"Part2"

#Desmonta la partición con id vda1 (En Disco1.dsk) umount -id1::vda1
#Si no existe, se debe mostrar error umount -id1::vdx1
#Desmonta una lista de particiones. umount -id1::vda1 -id2::vdb2 -id3::vdc1

#/
Df +k
Df +h
Df +i +h

Du -h -n::5
Du -h +n::3 +path::"/home/usr"
DU -H

#Realiza un formateo rápido con ext3 de la "Particion 1" del #Disco1.dsk
mkfs -id::vda1 -type::fast
#Realiza un formateo completo con ext2 de Particion2 en Disco2.dsk
mkfs -id::vdb1 +FS::2fs
#Agrega 500 Kb a Particion3 en Disco3.dsk
mkfs -add::500 -id::vdc1
#Agrega 1 Mb a Particion3 en Disco3.dsk
Mkfs -add::1 -id::vdc1 \
-unit::M

 
