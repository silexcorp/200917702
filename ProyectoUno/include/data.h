#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "errno.h"

/*
 * Variables globales de comandos */

#define MKDISK "mkdisk"
#define RMDISK "rmdisk"
#define FDISK  "fdisk"
#define MOUNT  "mount"
#define UMOUNT "umount"
#define DF     "df"
#define DU     "du"
#define MKFS   "mkfs"
#define FS     "fs"
#define FS2    "2fs"
#define FS3    "3fs"
#define JOURNALING    "JOURNALING"

#define EXIT   "exit"
#define HELP   "help"
#define CLEAR  "clear"
#define WAIT   "wait"
#define STATE  "state"

/* Constantes de comparacion */
#define PATH    "path"
#define SIZE    "size"
#define UNIT    "unit"
#define TYPE    "type"
#define NAME    "name"
#define FIT     "fit"
#define DELETE  "delete"
#define NAME    "name"
#define ADD     "add"
#define ID      "id"
#define FULL    "full"
#define FAST    "fast"
#define BF      "bf"
#define FF      "ff"
#define WF      "wf"
#define ID      "id"
#define MMBR    "mbr"
#define DDISK   "disk"
#define USR     "usr"   //SEGUNDA FASE
#define PWD     "pwd"
#define GRP     "grp"
#define UGO     "ugo"
#define R       "r"
#define CCONT   "cont"
#define CONTENT "content"
#define P       "p"
#define IDDEST  "iddest"
#define DEST    "dest"
#define RUTA    "ruta"

/* Admin de discos */
#define MKFS    "mkfs"

/* ADMIN de usuarios
 * y grupos         */
#define LOGIN   "login"
#define LOGOUT  "logout"
#define MKGRP   "mkgrp"
#define RMGRP   "rmgrp"
#define MKUSR   "mkusr"
#define RMUSR   "rmusr"
#define ROOT    "root"
#define USER    "user"

/* ADMIN de carpteas,
 * archivos y permisos */
#define CHMOD   "chmod"
#define MKFILE  "mkfile"
#define CAT     "cat"
#define REM     "rem"
#define EDIT    "edit"
#define REN     "ren"
#define MKDIR   "mkdir"
#define CP      "cp"
#define MV      "mv"
#define FIND    "find"
#define CHOWN   "chown"
#define CHGRP   "chgrp"

/* Reportes */
#define REP     "rep"
#define INODE   "inode"
#define BLOCK   "block"
#define BM_INODE "bm_inode"
#define BM_BLOCK "bm_block"
#define TREE    "tree"
#define SB      "sb"
#define FFILE   "file"
#define LS      "ls"

#define BM_AVD  "bm_avd"
#define BM_DD   "bm_dd"
#define DD_FILE "dd_file"
#define AVDS    "avds"
#define LOGG    "log"

/*
#define S_AVD   "s_avd"
#define S_DD    "s_dd"
#define S_TI    "s_ti"
#define S_BD    "s_sd"
#define S_LOG   "s_log"
#define S_SB    "sb"
*/

/* Script */
#define EXEC   "exec"

/* Colores */
#define NEGRO   30
#define ROJO    31
#define VERDE   32
#define AMARILLO 33
#define AZUL    34
#define MAGENTA 35
#define CYAN    36
#define BLANCO  37
#define DEFECTO  0


//#define NULL  nullptr
//extern int login;
int inde;
char buffer[1];      //bit utilizado para guardar 0's
char fecha[16];      //fecha actual
DIR *directorio;              //variable directorio global
FILE* graph;
char *ubicacion_archivo;
extern void limpiar_cadena(char *cadena);
extern void concatenar_char(char* s, char c);
extern void crear_carpeta(char *carpeta);
extern int verificar_carpeta(char *carpeta);
extern int existe_archivo(char* file);
extern void analizar_entrada(char *entrada);
extern void establecer_nombre(char *path,char *nombre);
extern void establecer_nombre_dot(char *path,char *nombre);
extern void establecer_destino(char *path,char *nombre, char *ext);
extern time_t get_time();
extern void set_time(time_t t);
extern void settime();
extern void quitar_espacio(char s[], char c);
extern void quitar_espacios_en_blanco(char s[], char c);
extern int convert(char *num);

extern void login_system(char *r_usr, char *r_pwd);

extern void suc(char *info, char *why);
extern void war(char *where, char *why);
extern void err(char *where, char *why);
extern void exe(char *what, char *why);
extern void inf(char *what);
extern char *get_name(char *disco);
extern char *get_path(char *disco);
extern char *get_ext(char *disco);
extern int get_size(char *informacion);

/*
char *nueva_cadena;
char *temporal;
//char cadena[300] = "vda1 vda2 vda3";
nueva_cadena = strtok (final,"");
int esta = 0;
while (nueva_cadena != NULL){
    if(1){
        temporal = strtok(nueva_cadena, " ");
        if(temporal){
            // Verifica el primer dato o split/word
            //printf("        %s\n", temporal);
            arr[esta] = malloc(sizeof(temporal)+1);
            strcpy(arr[esta], temporal);esta++;
        }
        while(temporal){
            temporal = strtok(NULL, " ");
            if(temporal){
                //printf("        %s\n",temporal);
                arr[esta] = malloc(sizeof(temporal)+1);
                strcpy(arr[esta], temporal);esta++;
            }else{
                break;
            }
        }
    }
    nueva_cadena = strtok (NULL, "");
}*/



#endif // DATA_H
