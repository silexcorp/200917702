#Crea 2 discos del mismo tamaño (60 Mb) pero en diferentes unidades
mkdisk -Size::60 -unit::M -path::/home/archivos/primerpractica/ -name::D1.dsk
mkdisk -Size::60 -unit::M -path::/home/archivos/primerpractica/ -name::D2.dsk
#Crear 3 particiones del mismo tamaño en el primer disco con ajustes diferentes
#Todas son de 15 Mb (La extendida es de 30 Mb)
#Tipo::		Primaria	Primaria	Extendida	Logica
#Ajuste::	Primer 		Mejor		-			Peor
#Nombre::	PRI 1		PRI 2		EXT			LOG 1
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D1.dsk -type::P -fit::F -name::PRI1
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D1.dsk -fit::B -name::PRI2
fdisk -Size::30 -unit::M -path::/home/archivos/primerpractica/D1.dsk -type::E -name::EXT
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D1.dsk -type::L  -name::LOG1
#Crear las mismas  particiones en el segundo disco
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D2.dsk -type::P -fit::F -name::PRI1
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D2.dsk -fit::B -name::PRI2
fdisk -Size::30 -unit::M -path::/home/archivos/primerpractica/D2.dsk -type::E -name::EXT
fdisk -Size::15 -unit::M -path::/home/archivos/primerpractica/D2.dsk -type::L  -name::LOG1
#Verificar generacion de ids al montar
#Debería generar los ids:: vda1, vdb1, vda2, vdb2
#vda para el d2.dsk y vdb para d1.dsk
mount -path::/home/archivos/primerpractica/D2.dsk -name::PRI1
mount -path::/home/archivos/primerpractica/D1.dsk -name::PRI1
mount -path::/home/archivos/primerpractica/D2.dsk -name::LOG1
mount -path::/home/archivos/primerpractica/D1.dsk -name::LOG1
#Reporte de las particiones del disco
#Debe crear la carpeta de reportes
rep -name::disk -path::/home/archivos/primerpractica/reportes/Reporte1Disco2.png -id::vda1
rep -name::disk -path::/home/archivos/primerpractica/reportes/Reporte1Disco1.png -id::vdb1

rep -name::mbr -path::/home/archivos/primerpractica/reportes/Reporte1MBR2.png -id::vda1
rep -name::mbr -path::/home/archivos/primerpractica/reportes/Reporte1MBR1.png -id::vdb1


#Crea el sistema de archivos LWH en todas las particiones
mkfs -id::vda1
Mkfs -id::vdb1
Mkfs -id::vda2
Mkfs -id::vdb2


#rep
rep -name::sb -id::vda1 -path::/home/archivos/primerpractica/reportes/Reporte1Disco1SB.png
rep -name::bm_avd -id::vda1 -path::/home/archivos/primerpractica/reportes/Reporte1Disco1AVD.txt
rep -name::bm_dd -id::vda1 -path::/home/archivos/primerpractica/reportes/Reporte1Disco1DD.txt
rep -name::bm_inode -id::vda1 -path::/home/archivos/primerpractica/reportes/Reporte1Disco1Ti.txt
rep -name::bm_block -id::vda1 -path::/home/archivos/primerpractica/reportes/Reporte1Disco1BD.txt


rep -name::avds -path::/home/archivos/primerpractica/reportes/Reporte1Disco1AVDSS.png -id::vda1

#mkdir y mkfile con -p tiene que crear las carpetas padres que no existan
mkdir -id::vda1 -path::/home/user/mia/p1 -p

#apuntadores directos para vda1
mkdir -id::vda1 -path::/home/user/mia/p1/a1
mkdir -id::vda1 -path::/home/user/mia/p1/a2
mkdir -id::vda1 -path::/home/user/mia/p1/a3
mkdir -id::vda1 -path::/home/user/mia/p1/a4
mkdir -id::vda1 -path::/home/user/mia/p1/a5
mkdir -id::vda1 -path::/home/user/mia/p1/a6
mkdir -id::vda1 -path::/home/user/mia/p1/a7
mkdir -id::vda1 -path::/home/user/mia/p1/a8
mkdir -id::vda1 -path::/home/user/mia/p1/a9
mkdir -id::vda1 -path::/home/user/mia/p1/a10
mkdir -id::vda1 -path::/home/user/mia/p1/a11
mkdir -id::vda1 -path::/home/user/mia/p1/a12
mkdir -id::vda1 -path::/home/user/mia/p1/a13
mkdir -id::vda1 -path::/home/user/mia/p1/a14
#apuntadores directos para vdb1
mkdir -id::vdb1 -path::/home/user/mia/p1/a1
mkdir -id::vdb1 -path::/home/user/mia/p1/a2
mkdir -id::vdb1 -path::/home/user/mia/p1/a3
mkdir -id::vdb1 -path::/home/user/mia/p1/a4
mkdir -id::vdb1 -path::/home/user/mia/p1/a5
mkdir -id::vdb1 -path::/home/user/mia/p1/a6
mkdir -id::vdb1 -path::/home/user/mia/p1/a7
mkdir -id::vdb1 -path::/home/user/mia/p1/a8
mkdir -id::vdb1 -path::/home/user/mia/p1/a9
mkdir -id::vdb1 -path::/home/user/mia/p1/a10
mkdir -id::vdb1 -path::/home/user/mia/p1/a11
mkdir -id::vdb1 -path::/home/user/mia/p1/a12
mkdir -id::vdb1 -path::/home/user/mia/p1/a13
mkdir -id::vdb1 -path::/home/user/mia/p1/a14
#apuntadores indirectos para vda2
mkdir -id::vda2 -path::/home/user/mia/p1/b1
mkdir -id::vda2 -path::/home/user/mia/p1/b2
mkdir -id::vda2 -path::/home/user/mia/p1/b3
mkdir -id::vda2 -path::/home/user/mia/p1/b4
mkdir -id::vda2 -path::/home/user/mia/p1/b5
mkdir -id::vda2 -path::/home/user/mia/p1/b6
mkdir -id::vda2 -path::/home/user/mia/p1/b7
mkdir -id::vda2 -path::/home/user/mia/p1/b8
mkdir -id::vda2 -path::/home/user/mia/p1/b9
mkdir -id::vda2 -path::/home/user/mia/p1/b10
mkdir -id::vda2 -path::/home/user/mia/p1/b11
#apuntadores indirectos para vdb2
mkdir -id::vdb2 -path::/home/user/mia/p1/b1
mkdir -id::vdb2 -path::/home/user/mia/p1/b2
mkdir -id::vdb2 -path::/home/user/mia/p1/b3
mkdir -id::vdb2 -path::/home/user/mia/p1/b4
mkdir -id::vdb2 -path::/home/user/mia/p1/b5
mkdir -id::vdb2 -path::/home/user/mia/p1/b6
mkdir -id::vdb2 -path::/home/user/mia/p1/b7
mkdir -id::vdb2 -path::/home/user/mia/p1/b8
mkdir -id::vdb2 -path::/home/user/mia/p1/b9
mkdir -id::vdb2 -path::/home/user/mia/p1/b10
mkdir -id::vdb2 -path::/home/user/mia/p1/b11



#Validar que no cree dos veces el mismo directorio
mkdir -id::vda1 -path::/home/user/mia/p1/a13

#Archivo de 4 bloques directos tamaño :: 12*100 :: 100
mkfile -id::vda1 -path::/home/user/mia/p1/a1.txt -size::99
#Archivo que utiliza 7 blqoues de tamaño :: 7*25 =175
mkfile -id::vda1 -path::/home/user/mia/p1/a2.txt -size::174
#Archivo que utiliza 20 bloques, de tamaño :: 20*25 =500
mkfile -id::vda1 -path::/home/user/mia/p1/a3.txt -size::499
#Archivo que utiliza 30 bloques, de tamaño ::30 * 25 = 750
mkfile -id::vda1 -path::/home/user/mia/p1/a4.txt -size::749



#Crear archivo con contenido, pegar p1.txt en la carpeta archivos
mkfile -id::vda1 -path::/home/user/mia/p1/a5.txt -cont::/home/archivos/p1.txt
cat -id::vda1 -path::/home/user/mia/p1/a5.txt

#Validar que no cree el mismo archivo dos veces
mkfile -id::vda1 -path::/home/user/mia/p1/a4.txt -size::749

#Crear carpetas padres
mkfile -id::vda1 -path::/docs/archivos/documentos/doc.txt -p -size::16

#Copiar y mover
mkdir -id::vdb1 -path::/home/user/mia/copiar -p
mkdir -id::vdb1 -path::/home/user/mia/mover -p
cp -id::vda1 -path::/home/user/mia/p1 -iddest::vdb1 -pathdest::/home/user/mia/copiar
cp -id::vda1 -path::/home/user/mia/p1 -iddest::vdb1 -pathdest::/home/user/mia/copiar
mv -id::vdb1 -path::/home/user/mia/copiar/p1 -iddest::vdb1 -pathdest::/home/user/mia/mover
mv -id::vdb1 -path::/home/user/mia/copiar/p1 -iddest::vdb1 -pathdest::/home/user/mia/mover

#Reporte de archivos creados y de todo el sistema
find -id::vda1 -Path::/home/user -name::b*
find -id::vda1 -Path::/home/user -name::?1
find -id::vda1 -Path::/ -name::*

#Apuntadores indirectos y validacion
cat -id::vda1 -path::/home/user/mia/p1/ax.txt
cat -id::vda1 -path::/home/user/mia/p1/a2.txt


#REPORTES
rep -name::sb -Path::/home/archivos/reportes/sb.png -id::vda1
rep -name::file -Path::/home/archivos/reportes/filea3.png -ruta::/home/user/mia/p1/a3.txt -id::vda1
rep -name::dd_file -Path::/home/archivos/reportes/dd_file.png -ruta::/home/user/mia/p1/ -id::vda1
rep -name::tree -Path::/home/archivos/reportes/tree.png -id::vda1
rep -name::bm_avd -Path::/home/archivos/reportes/bm_avd01.png -id::vda1
rep -name::bm_dd -Path::/home/archivos/reportes/bm_dd.png -id::vda1
rep -name::bm_inode -Path::/home/archivos/reportes/bm_inode.png -id::vda1
rep -name::bm_block -Path::/home/archivos/reportes/bm_block.png -id::vda1

#Renombrar
ren -id::vdb1 -path::/home/user -name::user2
ren -id::vda1 -path::/home/user/mia/p1/c1 -name::nuevo
find -id::vda1 -path::/ -name::*
find -id::vdb1 -path::/ -name::*

#Eliminar
rem -id::vdb2 -path::/home
rem -id::vda2 -path::/home
find -id::vdb1 -path::/ -name::*


mkfile -id::vda1 -path::/1.txt -size::32
mkfile -id::vda1 -path::/2.txt -size::32
mkfile -id::vda1 -path::/3.txt -size::32
mkfile -id::vda1 -path::/4.txt -size::32
mkfile -id::vda1 -path::/5.txt -size::32


#Mas espacio
edit -id::vda1 -path::/1.txt -size::80
edit -id::vda1 -path::/3.txt -size::80
edit -id::vda1 -path::/5.txt -size::80


#menos espacio
edit -id::vda1 -path::/2.txt -size::0
edit -id::vda1 -path::/4.txt -size::0

#Editar con contenido
edit -id::vda1 -path::/2.txt -cont::/home/archivos/p1.txt
cat -id::vda1 -path::/2.tx
