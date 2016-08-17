#ifndef DISK_H
#define DISK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <data.h>

/*
 * Estructura de la partición */
struct PARTITION{
    char part_status;   //Indica si la particion esta ACTIVA; Si:1 o No:0
    char part_type;     //Indica el tipo de particion, P:primario o E:extendida
    char part_fit;      //Tipo de ajuste de partición, B:Best, F:First, W:Worst
    int part_start;     //Indica en que byte del disco inicia la particion
    int part_size;      //Contiene el tamaño total de la particion en bytes
    char part_name[16]; //Nombre de la partición
};


/*
 * Estructura del MBR */
struct MBR{
    int mbr_tamanio;                 //Tamaño total del disco en bytes
    time_t mbr_fecha_creacion;       //Fecha y hora de creación del disco
    int mbr_disk_signature;          //Numero random, identifica de forma única a c/disco
    struct PARTITION mbr_partition_1;//Particion 1
    struct PARTITION mbr_partition_2;//Particion 2
    struct PARTITION mbr_partition_3;//Particion 3
    struct PARTITION mbr_partition_4;//Particion 4

};


/*
 * Estructura de la partición extendida */
struct EBR{
    char part_status;   //Indica si la partición esta activa o no
    char part_fit;      //Tipo de ajuste de la partición. B:Best, F:First, W:Worst
    int  part_start;    //Indica en que byte del disco inicia la partición
    int  part_size;     //Contiene el tamaño total de la partición en bytes
    int  part_next;     //Byte en el que está el proximo EBR. -1 si no hay siguiente
    char part_name[16]; //Nombre de partición
};

typedef struct{
    char id[10];
    char name[200];
    char path[200];
    char letra[1];
    char numero[1];
}MONTAJE;

MONTAJE particiones_montadas[100];
MONTAJE particiones_desmontadas[100];
typedef struct DISK DISK;
typedef struct MBR MBR;
typedef struct EBR EBR;
typedef struct PARTITION PART;



extern void crear_disco(int size, char unit, char *path, char *name);
extern void eliminar_disco(char *path, char *name);
extern void formatear_disco(char *path, char *name);
extern void analizar_comando_crear_eliminar(char *cmd, int estado);
extern void quitar_espacio(char s[], char c);
extern int comp(char *a,char *b);
extern void crear_carpeta(char *path);
extern int verificar_carpeta(char *path);

extern void crear_particion(char *path,int size,char unit, char type, char fit, char* name);
extern void montar_particion(char direccion[], char name[]);
extern void desmontar_particion(char id[]);
extern char letra(int n);

/* Funciones nuevas */
extern void Reporte_Disco(char id[],char path_img[],char name[]);
extern void Reporte_MBR_EBR(char id[],char path_img[],char name[]);



#endif // DISK_H
