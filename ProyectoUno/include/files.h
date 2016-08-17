#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/*
 * Limintaciones */
#define limBloquesXInodo    4380
#define capacidadCarpeta    17518
#define capacidadArchivo    280320






int part_mont;
int cont;
int contador_extendidas;
int contador_primarias;
int contador_Logicas;
int contador_montadas;

char ultima_letra;
int contador_letra;


int existen_logicas;
int cont_log;
int contar_log;
int contar_ebr;


char reporte_mbr1[5000];
char reporte_mbr2[10000];
char reporte_mbr3[10000];
char reporte_mbr4[10000];
char reporte_mbr5[10000];
char reporte_ebr[10000];
char reporte_ebr2[10000];
char reporte_ebr3[10000];

char reporte_disco[10000];
char reporte_disco2[10000];
char reporte_disco3[10000];
char reporte_disco4[10000];
char reporte_disco5[50000];


char id_[10];
int id_inodo_mv;
int inodo_ids2;

char reporte_inodo[250000];
char reporte_inodo2[250000];
char reporte_inodo3[500000];

char reporte_bloque[250000];
char reporte_bloque2[250000];
char reporte_bloque3[500000];





/* Descripción de la
 * Estructura SUPER BLOQUE
 * Contiene info dela config
 * del Sistema de Archivos
 */
struct SUPERB{
    int sb_arbol_virtual_count;                 //Cantidad de estructuras en el árbol virtual de directorio
    int sb_detalle_directorio_count;            //Cantidad de estructuras en el detalle de directorio
    int sb_inodos_count;                        //Cantidad de i-nodos en la tabla de i-nodos
    int sb_bloques_count;                       //Cantidad de bloques de datos
    int sb_free_arbol_virtual_count;            //Cantidad de estructuras en el árbol virtual de directorio libres
    int sb_free_detalle_directorio_count;       //Cantidad de estructuras en el detalle de directorio libres
    int sb_free_bloques_count;                  //Cantidad de bloques de datos libres
    int sb_free_inodes_count;                   //Cantidad de i-nodos en la tabla de i-nodos libres
    time_t sb_mtime;                            //Ultima fecha en el que el sistema fue montado
    time_t sb_umtime;                           //Ultima fecha en que el sistema fue desmontado
    int sb_mnt_count;                           //Indica cuantas veces se ha montado el sistema
    int sb_magic;                               //Valor que identifica al sistema de archivos, tendrá el valor del número de Carnet del Estudiante
    int sb_avd_size;                            //Tamaño de una estructura del árbol virtual de directorio
    int sb_detalle_directorio_size;             //Tamaño de una estructura de un detalle de directorio
    int sb_inode_size;                          //Tamaño de una estructura de un i-nodo
    int sb_block_size;                          //Tamaño de una estructura de un bloque de datos
    int sb_ap_avd;                              //Apuntador al inicio del árbol virtual de directorio
    int sb_ap_bitmap_avd;                       //Apuntador al inicio del bitmap del árbol virtual de directorio
    int sb_ap_detalle_directorio;               //Apuntador al inicio del detalle de directorio
    int sb_ap_bitmap_detalle_directorio;        //Apuntador al inicio del bitmap de detalle de directorio
    int sb_ap_tabla_inodo;                      //Apuntador al inicio de la tabla de i-nodos
    int sb_ap_bitmap_tabla_inodo;               //Apuntador al inicio del bitmap de la tabla de i-nodos
    int sb_ap_bloque;                           //Apuntador al inicio del bloque de datos
    int sb_ap_bitmap_inodo;                     //Apuntador al inicio del bitmap de la tabla
    int sb_ap_log;                              //Apuntador al inicio del log o bitácora
    int sb_ap_copia_super_bloque;               //Apuntador al inicio de la copia del super bloque
    int sb_first_free_bit_avd;                  //Primer bit libre en el bitmap árbol de directorio
    int sb_first_free_bit_detalle_directorio;   //Primer bit libre en el bitmap detalle de directorio
    int sb_first_free_bit_tabla_inodo;          //Primer bit libre en el bitmap de i-nodo
    int sb_first_free_bit_bloques;              //Primer bit libre en el bitmap de bloque de datos
};







/*
 *  ÁRBOL VIRTUAL DE DIRECTORIO
 */
struct AVD{
    time_t avd_fecha_creacion;
    char avd_name_directorio[16];
    int  avd_ap_array_subdirectorios[6];
    int  avd_ap_detalle_directorio;
    int  avd_ap_avd;
};
/* BITMAP ÁRBOL VIRTUAL DE DIRECTORIO */
struct BITMAPAVD{
    char state;
};


struct ARCHIVE{
    char    dd_file_nombre[16];         //Maneja el nombre del archivo
    int     dd_file_ap_inodo;           //Apuntador al inodo
    time_t  dd_file_date_creacion;      //Fecha de creación del archivo (dd/mm/yyyy hh:mm)
    time_t  dd_file_date_modificacion;  //Fecha de modificación del archivo (dd/mm/yyyy hh:mm)
};
/*
 *  DETALLE DE DIRECTORIO
 */
struct DD{
    struct ARCHIVE dd_archive_1;       //Estructura con información del archivo
    struct ARCHIVE dd_archive_2;       //Estructura con información del archivo
    struct ARCHIVE dd_archive_3;       //Estructura con información del archivo
    struct ARCHIVE dd_archive_4;       //Estructura con información del archivo
    struct ARCHIVE dd_archive_5;       //Estructura con información del archivo
    int     dd_ap_detalle_directorio;//Si se quiere ingresar un sexto archivo y no hay espacio enesta
                                    //estructura de detalle directorio, apunta a otraestructura de detalle de directorio.
};
/* BITMAP DE DETALLE DE DIRECTORIO */
struct BITMAPDD{
    char state;
};


/*
 *  TABLA DE INODOS
 */
struct TI{
    int i_count_inodo;          //Número de i-nodo
    int i_size_file;            //Tamaño del archivo
    int i_count_bloques_asignados;//Número de bloques asignados
    int i_array_bloques[4];     //Arreglo de 4 apuntadores a bloques de datos para guardarel archivo.
    int i_ap_indirecto;         //Un apuntador indirecto por si el archivo ocupa más de 4bloques de datos,
                                //para el manejo de archivos de tamaño“grande”.
};

/* BITMAP DE TABLA DE INODOS */
struct BITMAPTI{
    char state;                 //Mapa de bit para verificar elEstado del i-nodo
};



/*
 *  BLOQUE DE DATOS
 */
struct BD{
    char bd_data[25];           //Contiene la información del archivo
};
/* BITMAP DE BLOQUE DE DATOS */
struct BITMAPBD{
    char state;                 //Mapa de bit para verificar los bloquesDe datos,
};





/* LOG BITACORA */
struct LOG{
    char log_tipo_operacion;    //El tipo de operación a realizars
    int  log_tipo;              //Si es archivo (0), si es directorio(1)
    char log_nombre[16];        //Nombre archivo o directorio
    char log_contenido[25];         //Si hay datos contenidos
    time_t log_fecha;           //Fecha de transacción
};







//ESTRUCUTURAS FASE 2
typedef struct{

     int s_inodes_count;//numero total de inodods
     int s_blocks_count;//numero total de bloques
     int s_free_blocks_count;//numero de bloques libre
     int s_free_inodes_count;//numero de inodos libres
     char s_mtime[25];//ultima fecha que se monto
     char s_umtime[25];
     int s_mnt_count;//cantidad de veces montado
     char s_magic[6];
     int s_inode_size;
     int s_block_size;
     int s_first_ino;
     int s_first_blo;
     int s_bm_inode_star;
     int s_bm_block_star;
     int s_inode_start;
     int s_block_start;
}Super_Bloque;

typedef struct{
     char inodo[1];
}Bmp_Inodos;

typedef struct{
     char bloque[1];
}Bmp_Bloques;

typedef struct{
     int id;
     int i_uid;
     int i_gid;
     int i_size;
     char i_atime[25];
     char i_ctime[25];
     char i_mtime[25];
     int i_block[15];//apuntadores directos e indirectos
     char i_type[1];
     int i_perm;

}Tabla_Inodos;

typedef struct{

     char b_name[12];
     int b_inodo;

}Datos_Carpeta;

typedef struct{

     Datos_Carpeta carpetas[4];

}Bloques_Carpeta;

typedef struct{
     char b_content[64];
}Bloques_Archivo;






typedef struct SUPERB SUPERB;
typedef struct AVD AVD;
typedef struct BITMAPAVD BITMAPAVD;
typedef struct ARCHIVE ARCHIVE;
typedef struct DD DD;
typedef struct BITMAPDD BITMAPDD;
typedef struct TI TI;
typedef struct BITMAPTI BITMAPTI;
typedef struct BD BD;
typedef struct BITMAPBD BITMAPBD;
typedef struct LOG LOG;

//extern void mkfs_disco(char *r_id, char *r_type, char *r_add, char *r_unit);

#endif // FILES_H
