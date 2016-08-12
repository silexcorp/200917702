#Crea 2 discos del mismo tamaño (60 Mb) pero en diferentes unidades
mkdisk -Size::60 -unit::M -path::"/home/archivos/primera practica/" -name::D1.dsk
mkdisk -Size::60 -unit::M -path::"/home/archivos/primera practica/" -name::D2.dsk

#Crear 3 particiones del mismo tamaño en el primer disco con ajustes diferentes
#Todas son de 15 Mb (La extendida es de 30 Mb)
#Tipo::		Primaria	Primaria	Extendida	Logica
#Ajuste::	Primer 		Mejor		-			Peor
#Nombre::	PRI 1		PRI 2		EXT			LOG 1
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D1.dsk" -type::P -fit::F -name::PRI1
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D1.dsk" -fit::B -name::PRI2
fdisk -Size::30 -unit::M -path::"/home/archivos/primera practica/D1.dsk" -type::E -name::EXT
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D1.dsk" -type::L  -name::LOG1

#Crear las mismas  particiones en el segundo disco
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D2.dsk" -type::P -fit::F -name::PRI1
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D2.dsk" -fit::B -name::PRI2
fdisk -Size::30 -unit::M -path::"/home/archivos/primera practica/D2.dsk" -type::E -name::EXT
fdisk -Size::15 -unit::M -path::"/home/archivos/primera practica/D2.dsk" -type::L  -name::LOG1

#Verificar generacion de ids al montar
#Debería generar los ids:: vda1, vdb1, vda2, vdb2
#vda para el d2.dsk y vdb para d1.dsk
mount -path::"/home/archivos/primera practica/D2.dsk" -name::PRI1
mount -path::"/home/archivos/primera practica/D1.dsk" -name::PRI1
mount -path::"/home/archivos/primera practica/D2.dsk" -name::LOG1
mount -path::"/home/archivos/primera practica/D1.dsk" -name::LOG1

#Reporte de las particiones del disco
#Debe crear la carpeta de reportes
rep -name::disk -path::"/home/archivos/primera practica/reportes/Reporte1 Disco2.png" -id::vda1
rep -name::disk -path::"/home/archivos/primera practica/reportes/Reporte1 Disco1.png" -id::vdb1

rep -name::mbr -path::"/home/archivos/primera practica/reportes/Reporte1 MBR2.png" -id::vda1
rep -name::mbr -path::"/home/archivos/primera practica/reportes/Reporte1 MBR1.png" -id::vdb1
