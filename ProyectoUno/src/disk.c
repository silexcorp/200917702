#include "disk.h"
#include "files.h"



void inicializar_sistema(){
    /*
     * Inicializar variables globales */
    ultima_letra = ' ';
    cont_log = 5;
}

/* Verifica archivo en el sistema */
void set_time(time_t t){
   //time_t t = time(0);
   struct tm *tcreacion = localtime(&t);
   strftime(fecha,16,"%d/%m/%y %H:%M",tcreacion);
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    CREAR DISCO
 /*
  * Crear discos con los siguientes parámetros */
void crear_disco(int size, char unit, char *path, char*name){
    /*
     * Creando disco con parametros obtenidos */
    exe("CREANDO DISCO:",name);
    /* Para un numero random */

    srand(time(NULL));
    MBR mbr;
    int size_disco = 0;
    buffer[0] = '\0';

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    mbr.mbr_fecha_creacion = get_time();
    mbr.mbr_disk_signature = rand()%1001;

    /*
     * Ver archivo y su ubicación */
    establecer_nombre(path,name);
    /*
     * Crear directorio o carpeta */
    crear_carpeta(path);
    /*
     * Verificar archivo, si no existe lo crea */
    FILE *f = fopen(ubicacion_archivo,"w");
    if(f!=NULL){
        //printf("Creando disco...\n");
        if(unit == 'K' || unit == 'k'){
            size_disco = size*1024;
        }else if(unit == 'M' || unit == 'm'){
            size_disco = size*1024*1024;
        }

        for(inde=0;inde<size_disco;inde++){
            fwrite (buffer, sizeof(buffer), 1, f);
        }
        rewind(f);

        mbr.mbr_tamanio = size_disco;
        fwrite(&mbr,sizeof(MBR),1,f);
        fflush(f);
        fclose(f);
        inf("disco creado exitosamente");

    }else{
        //fclose(f);
        err("al crear archivo","el archivo ya existente");
    }
   leerMBR(ubicacion_archivo);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    ELIMINAR DISCO
/*
 * Comando para eliminar algun disco creado */
void eliminar_disco(char *path, char *name){

    /*
     *  Iniciar comando de eliminacion */
    exe("ELIMINANDO DISCO:", name);

    /*
     * Ver archivo y su ubicación */
    establecer_nombre(path,name);
    /*
     * Solicitar confirmación de eliminacion */
    inf("escriba s/n para confirmar la eliminacion");
    char respuesta;
    do{
        scanf("%c",&respuesta);
        if(respuesta == 's' || respuesta == 'S'){
            /*
             * Verificar directroio, si no existe mostrará error */
            if(verificar_carpeta(path) == 0){
                if(remove(ubicacion_archivo) == 0){
                    inf("eliminando disco...");
                    inf("disco eliminado exitosamente");
                    break;
                }else{
                    err(name,"archivo no existente");
                    break;
                }
            }else{
                err(path,"carpete no existente");
                break;
            }
        }else if(respuesta == 'n' || respuesta == 'N'){
            war(name,"archivo no eliminado");
            break;
        }
    }while(respuesta != 's' || respuesta != 'S' || respuesta != 'n' || respuesta != 'N');
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    COMPROBAR DISCO

void leerMBR(char* path){
   MBR mbr;
   FILE *fp;
   fp=fopen(path,"rb");

   if(fp!=NULL)
   {
         fseek(fp,0,SEEK_SET);
         fread(&mbr,sizeof(MBR),1,fp);
         //printf("\n%s",mbr.mbr_fecha_creacion);
         printf("    SIZE: %d (bytes) \n",mbr.mbr_tamanio);
         printf("    SIGN: %d \n",mbr.mbr_disk_signature);

         if(mbr.mbr_partition_1.part_status == '1'){
             printf("\n      PARTICION 1:\n");
             printf("      NAME: %s \n",mbr.mbr_partition_1.part_name);
             printf("      SIZE: %d (bytes) \n",mbr.mbr_partition_1.part_size);
             printf("      START: %d \n",mbr.mbr_partition_1.part_start);
             printf("      STATE: %c \n",mbr.mbr_partition_1.part_status);
             printf("      TYPE: %c \n",mbr.mbr_partition_1.part_type);
             printf("      FIT: %c \n",mbr.mbr_partition_1.part_fit);
             if(mbr.mbr_partition_1.part_type == 'E'){
                 printf("\n         PARTICIONES EXTENDIDAS:\n");
                 verificar_ebr(path,inicio_particion_extendida(path));
             }
         }
         if(mbr.mbr_partition_2.part_status == '1'){
             printf("\n      PARTICION 2:\n");
             printf("      NAME: %s \n",mbr.mbr_partition_2.part_name);
             printf("      SIZE: %d (bytes) \n",mbr.mbr_partition_2.part_size);
             printf("      START: %d \n",mbr.mbr_partition_2.part_start);
             printf("      STATE: %c \n",mbr.mbr_partition_2.part_status);
             printf("      TYPE: %c \n",mbr.mbr_partition_2.part_type);
             printf("      FIT: %c \n",mbr.mbr_partition_2.part_fit);
             if(mbr.mbr_partition_2.part_type == 'E'){
                 printf("\n         PARTICIONES EXTENDIDAS:\n");
                 verificar_ebr(path,inicio_particion_extendida(path));
             }
         }
         if(mbr.mbr_partition_3.part_status == '1'){
             printf("\n      PARTICION 3:\n");
             printf("      NAME: %s \n",mbr.mbr_partition_3.part_name);
             printf("      SIZE: %d (bytes) \n",mbr.mbr_partition_3.part_size);
             printf("      START: %d \n",mbr.mbr_partition_3.part_start);
             printf("      STATE: %c \n",mbr.mbr_partition_3.part_status);
             printf("      TYPE: %c \n",mbr.mbr_partition_3.part_type);
             printf("      FIT: %c \n",mbr.mbr_partition_3.part_fit);
             if(mbr.mbr_partition_3.part_type == 'E'){
                 printf("\n         PARTICIONES EXTENDIDAS:\n");
                 verificar_ebr(path,inicio_particion_extendida(path));
             }
         }
         if(mbr.mbr_partition_4.part_status == '1'){
             printf("\n      PARTICION 4:\n");
             printf("      NAME: %s \n",mbr.mbr_partition_4.part_name);
             printf("      SIZE: %d (bytes) \n",mbr.mbr_partition_4.part_size);
             printf("      START: %d \n",mbr.mbr_partition_4.part_start);
             printf("      STATE: %c \n",mbr.mbr_partition_4.part_status);
             printf("      TYPE: %c \n",mbr.mbr_partition_4.part_type);
             printf("      FIT: %c \n",mbr.mbr_partition_4.part_fit);
             if(mbr.mbr_partition_4.part_type == 'E'){
                 printf("\n         PARTICIONES EXTENDIDAS:\n");
                 verificar_ebr(path,inicio_particion_extendida(path));
             }
         }
         fclose(fp);
   }else{
        err(path,"no existe ruta");
   }
}

void verificar_ebr(char* dir,int posicion){
     EBR aux;
     FILE *fp;
     fp=fopen(dir,"rb+");

     if(fp!=NULL){
        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        if(aux.part_status == '1'){
         //printf("\nParticion Logica %s",mbr.mbr_partition_1.part_name);
             printf("\n        PART STATUS: %c",aux.part_status);
             printf("\n        PART FIT: %c",aux.part_fit);
             printf("\n        PART START: %d",aux.part_start);
             printf("\n        PART SIZE: %d",aux.part_size);
             printf("\n        PART NEXT: %d",aux.part_next);
             printf("\n        PART NAME: %s\n",aux.part_name);
             fclose(fp);

             verificar_ebr(dir,aux.part_next);
         }else{
            //inf("fin de EBR");
         }
     }else{
        err(dir, "error al abrir");
     }
}


int inicio_ultima_particion_creada(char* dir){

   int inicio = 0, inicio2 = 0, inicio3 = 0, inicio4 = 0,inicio_vacio = -1;
   int size1 = 0, size2 = 0, size3 = 0, size4 = 0;

   FILE *fp;
   MBR mbr;

   fp=fopen(dir,"rb+");

   if(fp!=NULL){

      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1'){
         inicio = mbr.mbr_partition_1.part_start;
         size1 = mbr.mbr_partition_1.part_size;
         inicio_vacio = sizeof(MBR);
      }if(mbr.mbr_partition_2.part_status == '1'){
          inicio2 = mbr.mbr_partition_2.part_start;
          size2 = mbr.mbr_partition_2.part_size;
          inicio_vacio = sizeof(MBR);
      }if(mbr.mbr_partition_3.part_status == '1'){
          inicio3 = mbr.mbr_partition_3.part_start;
          size3 = mbr.mbr_partition_3.part_size;
          inicio_vacio = sizeof(MBR);
      }if(mbr.mbr_partition_4.part_status == '1'){
          inicio4 = mbr.mbr_partition_4.part_start;
          size4 = mbr.mbr_partition_4.part_size;
          inicio_vacio = sizeof(MBR);
      }
      if(inicio_vacio == -1){
         return sizeof(MBR);
      }else if(inicio > inicio2 && inicio > inicio3 && inicio > inicio4){
         return inicio+size1;
      }else if(inicio2 > inicio && inicio2 > inicio3 && inicio2 > inicio4){
         return inicio2+size2;
      }else if(inicio3 > inicio && inicio3 > inicio2 && inicio3 > inicio4){
         return inicio3+size3;
      }else if(inicio4 > inicio && inicio4 > inicio2 && inicio4 > inicio3){
         return inicio+size4;
      }
      fclose(fp);

   }else{
    err("no existe la ruta para crear particion",dir);
   }
}

int numero_particiones_primarias(char* dir){
    MBR mbr;
   int contador = 0;

   FILE *fp;

   fp=fopen(dir,"rb+");

   if(fp!=NULL){

      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1' && mbr.mbr_partition_1.part_type == 'P'){
         contador++;
      }if(mbr.mbr_partition_2.part_status == '1' && mbr.mbr_partition_2.part_type == 'P'){
          contador++;
      }if(mbr.mbr_partition_3.part_status == '1' && mbr.mbr_partition_3.part_type == 'P'){
          contador++;
      }if(mbr.mbr_partition_4.part_status == '1' && mbr.mbr_partition_4.part_type == 'P'){
          contador++;
      }
      fclose(fp);
      return contador;
   }
}

int inicio_particion_extendida(char* dir){
    int inicio = 0;
    MBR mbr;
    FILE *fp;

    fp=fopen(dir,"rb+");

   if(fp!=NULL){
      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1' && mbr.mbr_partition_1.part_type == 'E'){
            inicio = mbr.mbr_partition_1.part_start;
      }else if(mbr.mbr_partition_2.part_status == '1' && mbr.mbr_partition_2.part_type == 'E'){
            inicio = mbr.mbr_partition_2.part_start;
      }else if(mbr.mbr_partition_3.part_status == '1' && mbr.mbr_partition_3.part_type == 'E'){
            inicio = mbr.mbr_partition_3.part_start;
      }else if(mbr.mbr_partition_4.part_status == '1' && mbr.mbr_partition_4.part_type == 'E'){
            inicio = mbr.mbr_partition_4.part_start;
      }
      fclose(fp);
      return inicio;

   }else{
        err("no existe ruta del archivo para crear particion",dir);
   }
}

int tamanio_particion_extendida(char* dir){

    int size_part = 0;
    FILE *fp;
    MBR mbr;
    fp = fopen(dir,"rb+");

   if(fp!=NULL){
      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1' && mbr.mbr_partition_1.part_type == 'E'){
         size_part = mbr.mbr_partition_1.part_size;
      }else if(mbr.mbr_partition_2.part_status == '1' && mbr.mbr_partition_2.part_type == 'E'){
         size_part = mbr.mbr_partition_2.part_size;
      }else if(mbr.mbr_partition_3.part_status == '1' && mbr.mbr_partition_3.part_type == 'E'){
         size_part = mbr.mbr_partition_3.part_size;
      }else if(mbr.mbr_partition_4.part_status == '1' && mbr.mbr_partition_4.part_type == 'E'){
         size_part = mbr.mbr_partition_4.part_size;
      }
      fclose(fp);
      return size_part;

   }else{
     err("No existe la ruta del archivo para crear la Particion",dir);
   }
}

void crear_particion_logica(char *dir,int size,char part_fit, char unit,char name[]){

     EBR ebr,aux;
     FILE *fp;
     int ini_ext = inicio_particion_extendida(dir);
     int tam_ext = tamanio_particion_extendida(dir);

     fp=fopen(dir,"rb+");

     if(fp!=NULL){
        fseek(fp,ini_ext,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        if(aux.part_status != '1'){

            ebr.part_status = '1';
            ebr.part_start = ini_ext;

            if(unit == 'B'){
                ebr.part_size = size;
                ebr.part_next = ini_ext+size;
            }else if(unit == 'K'){
                ebr.part_size = size*1024;
                ebr.part_next = ini_ext+size*1024;
            }else if(unit == 'M'){
                ebr.part_size = size*1024*1024;
                ebr.part_next = ini_ext+size*1024*1024;
            }

            strcpy(ebr.part_name,name);
            ebr.part_fit = part_fit;

            //printf("DATTTTT: name (%s) PART_NEXT (%d) PART_START (%d)\n",ebr.part_name,ebr.part_next,ebr.part_start);
            if((ini_ext+tam_ext)>ebr.part_next){
                fseek(fp,ini_ext,SEEK_SET);
                fwrite(&ebr,sizeof(EBR),1,fp);
                fflush(fp);
                contador_Logicas++;
                exe(name,"particion logica creada exitosametne");
                part_mont++;
            }else{
                err("en particion logica", "no hay espacio");
            }
            fclose(fp);
         }else{
               crear_mas_particiones_logicas(dir,size,part_fit,unit,name,aux.part_next);
         }
       }else{
         err(name,"no existe ruta del disco");
       }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    CREAR PARTICIONES

void crear_mas_particiones_logicas(char *dir,int size,char part_fit,char unit, char *name,int posicion){

     EBR ebr,aux;
     FILE *fp;
     fp=fopen(dir,"rb+");

     if(fp!=NULL){

        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        int ini_ext = inicio_particion_extendida(dir);
        int tam_ext = tamanio_particion_extendida(dir);

        if(aux.part_status != '1'){

            ebr.part_status = '1';
            ebr.part_start = posicion;

            if(unit == 'K'){
               ebr.part_size = size*1024;
               ebr.part_next = posicion+size*1024;
            }else if(unit == 'B'){
                     ebr.part_size = size;
                     ebr.part_next = posicion+size;
            }else if(unit == 'M'){
                     ebr.part_size = size*1024*1024;
                     ebr.part_next = posicion+size*1024*1024;
            }

            strcpy(ebr.part_name,name);
            ebr.part_fit = part_fit;
            //printf("TAM iniEXT + tamEXT %d, PART NEXT %d\n",(ini_ext+tam_ext),ebr.part_next);

            if((ini_ext+tam_ext)>ebr.part_next){
               fseek(fp,posicion,SEEK_SET);
               fwrite(&ebr,sizeof(EBR),1,fp);
               contador_Logicas++;
            //printf("\nPARTICION LOGICA %d CREADA %d %d %d",contador_Logicas,ebr.part_start,ebr.part_size,ebr.part_next);
               exe(name,"particion logica creada");
               part_mont++;

            }else{
                err("en particion logica","no hay espacio");//inicio_particion_extendida(dir)+tamanio_particion_extendida(dir),ebr.part_next);
            }
            fclose(fp);
         }else{

               crear_mas_particiones_logicas(dir,size,part_fit,unit,name,aux.part_next);
         }
       }else{
             err("no existe la ruta","para crear la particion logica");
       }
}

void crear_particion(char *path,int size,char unit, char type, char fit, char* name){
    /*
     *  Iniciar comando para crear particion */
    exe("CREANDO PARTICION:",name);

    /* Verificar si existe archivo en 'ubicacion_archivo' */
    inf("buscando archivo...");
    //printf("Buscando: %s\n",ubicacion_archivo);
    if(existe_archivo(path) == 1){
        int part_ext = numero_particiones_extendidas(path);
        int part_pri = numero_particiones_primarias(path);
        int part_ini = inicio_ultima_particion_creada(path);

        MBR mbr;
        EBR ebr;
        FILE *fp;
        fp=fopen(path,"rb+");

        // Verificar si hay espacio disponible //
        if(1){
            //inf("comprobando espacio en disco...");
            if(type == 'L'){
                if(get_size(name) < 16){
                    if(part_ext == 1){
                      crear_particion_logica(path,size,fit,unit,name);
                    }else{
                       err("particion logica","no existe particion extendida");
                    }
                }else{
                    err(name,"nombre excede del tamaño permitido");
                    return;
                }
            }else if(type == 'P' || type == 'E'){
                if((part_ext == 0 && part_pri <= 3 && type != 'L') || (part_ext == 1 && part_pri <= 2 && type != 'L' && type != 'E')){
                    if(fp!=NULL){
                        fseek(fp,0,SEEK_SET);
                        fread(&mbr,sizeof(MBR),1,fp);

                        if(mbr.mbr_partition_1.part_status == '0'){
                             mbr.mbr_partition_1.part_status = '1';
                             mbr.mbr_partition_1.part_type = type;
                             mbr.mbr_partition_1.part_fit = fit;
                             mbr.mbr_partition_1.part_start = part_ini;

                             // Calcular tamaño del disco a particionar //
                             if(unit == 'B'){
                                 mbr.mbr_partition_1.part_size = size;
                             }else if(unit == 'K'){
                                 mbr.mbr_partition_1.part_size = (size)*1024;
                             }else if(unit == 'M'){
                                 mbr.mbr_partition_1.part_size = (size)*1024*1024;
                             }

                             strcpy(mbr.mbr_partition_1.part_name,name);
                             exe(name, "particion creada exitosamente");
                             part_mont++;

                             /*
                              * Proceder a escribidr cambios en disco */
                             fseek(fp,0,SEEK_SET);
                             fwrite(&mbr,sizeof(MBR),1,fp);

                        }else if(mbr.mbr_partition_2.part_status == '0'){
                            mbr.mbr_partition_2.part_status = '1';
                            mbr.mbr_partition_2.part_type = type;
                            mbr.mbr_partition_2.part_fit = fit;
                            mbr.mbr_partition_2.part_start = part_ini;

                            // Calcular tamaño del disco a particionar //
                            if(unit == 'B'){
                                mbr.mbr_partition_2.part_size = size;
                            }else if(unit == 'K'){
                                mbr.mbr_partition_2.part_size = (size)*1024;
                            }else if(unit == 'M'){
                                mbr.mbr_partition_2.part_size = (size)*1024*1024;
                            }

                            strcpy(mbr.mbr_partition_2.part_name,name);
                            exe(name, "particion creada exitosamente");
                            part_mont++;

                            /*
                             * Proceder a escribidr cambios en disco */
                            fseek(fp,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1,fp);
                        }else if(mbr.mbr_partition_3.part_status == '0'){
                            mbr.mbr_partition_3.part_status = '1';
                            mbr.mbr_partition_3.part_type = type;
                            mbr.mbr_partition_3.part_fit = fit;
                            mbr.mbr_partition_3.part_start = part_ini;

                            // Calcular tamaño del disco a particionar //
                            if(unit == 'B'){
                                mbr.mbr_partition_3.part_size = size;
                            }else if(unit == 'K'){
                                mbr.mbr_partition_3.part_size = (size)*1024;
                            }else if(unit == 'M'){
                                mbr.mbr_partition_3.part_size = (size)*1024*1024;
                            }

                            strcpy(mbr.mbr_partition_3.part_name,name);
                            exe(name, "particion creada exitosamente");
                            part_mont++;

                            /*
                             * Proceder a escribidr cambios en disco */
                            fseek(fp,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1,fp);
                        }else if(mbr.mbr_partition_4.part_status == '0'){
                            mbr.mbr_partition_4.part_status = '1';
                            mbr.mbr_partition_4.part_type = type;
                            mbr.mbr_partition_4.part_fit = fit;
                            mbr.mbr_partition_4.part_start = part_ini;

                            // Calcular tamaño del disco a particionar //
                            if(unit == 'B'){
                                mbr.mbr_partition_4.part_size = size;
                            }else if(unit == 'K'){
                                mbr.mbr_partition_4.part_size = (size)*1024;
                            }else if(unit == 'M'){
                                mbr.mbr_partition_4.part_size = (size)*1024*1024;
                            }

                            strcpy(mbr.mbr_partition_4.part_name,name);
                            exe(name, "particion creada exitosamente");
                            part_mont++;
                            /*
                             * Proceder a escribidr cambios en disco */
                            fseek(fp,0,SEEK_SET);
                            fwrite(&mbr,sizeof(MBR),1,fp);
                        }
                        else{
                            err("crear particiones", "limite maximo de particiones (4)");
                        }
                        fclose(fp);

                    }else{
                        err(path,"no existe la ruta del archivo para crear particion");
                    }
                }else{
                    if(contador_extendidas == 1){
                        err("creando particion extendida","ya existe partición");
                    }else{
                        err("crear particiones","se ha llegado al límite de particiones");
                    }
                }
            }
        }else{
            err("en particion","espacio insuficiente");
        }
    }else{
        err("en archivo","archivo no existente");
    }
    leerMBR(path);
    //verificar_ebr(path,inicio_particion_extendida(path));
}

void eliminar_particion(char* path,char* name, char* r_delete){
    MBR mbr;
    EBR ebr;
    buffer[0] = '\0';
    FILE *fp;
    fp=fopen(path,"rb+");

    if(fp!=NULL){
        fseek(fp,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,fp);

        if(mbr.mbr_partition_1.part_status == '1' && strcasecmp(mbr.mbr_partition_1.part_name,name) == 0){
            mbr.mbr_partition_1.part_status = '0';
            if(strcasecmp(r_delete,"FAST") == 0){
                inf("FASTT");
                fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
            }else{//FULL
                inf("FULLL");
                int inis = mbr.mbr_partition_1.part_start;
                int size = mbr.mbr_partition_1.part_size;
                rewind(fp);//fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
                fseek(fp,inis,SEEK_SET);
                for(inde=0;inde<size;inde++){
                    fwrite (buffer, sizeof(buffer), 1, fp);
                }
            }
            exe(name,"particion eliminada");
            fflush(fp);
            fclose(fp);
            leerMBR(path);
            return;
        }else if(mbr.mbr_partition_2.part_status == '1' && strcasecmp(mbr.mbr_partition_2.part_name,name) == 0){
            mbr.mbr_partition_2.part_status = '0';
            if(strcasecmp(r_delete,"FAST") == 0){
                inf("FASTT");
                fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
            }else{//FULL
                inf("FULLL");
                int inis = mbr.mbr_partition_2.part_start;
                int size = mbr.mbr_partition_2.part_size;
                rewind(fp);//fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
                fseek(fp,inis,SEEK_SET);
                for(inde=0;inde<size;inde++){
                    fwrite (buffer, sizeof(buffer), 1, fp);
                }
            }
            exe(name,"particion eliminada");
            fflush(fp);
            fclose(fp);
            leerMBR(path);
            return;
        }else if(mbr.mbr_partition_3.part_status == '1' && strcasecmp(mbr.mbr_partition_3.part_name,name) == 0){
            mbr.mbr_partition_3.part_status = '0';
            if(strcasecmp(r_delete,"FAST") == 0){
                inf("FASTT");
                fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
            }else{//FULL
                inf("FULLL");
                int inis = mbr.mbr_partition_3.part_start;
                int size = mbr.mbr_partition_3.part_size;
                rewind(fp);//fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
                fseek(fp,inis,SEEK_SET);
                for(inde=0;inde<size;inde++){
                    fwrite (buffer, sizeof(buffer), 1, fp);
                }
            }
            exe(name,"particion eliminada");
            fflush(fp);
            fclose(fp);
            leerMBR(path);
            return;
        }else if(mbr.mbr_partition_4.part_status == '1' && strcasecmp(mbr.mbr_partition_4.part_name,name) == 0){
            mbr.mbr_partition_4.part_status = '0';
            if(strcasecmp(r_delete,"FAST") == 0){
                inf("FASTT");
                fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
            }else{//FULL
                inf("FULLL");
                int inis = mbr.mbr_partition_4.part_start;
                int size = mbr.mbr_partition_4.part_size;
                rewind(fp);//fseek(fp,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,fp);
                fseek(fp,inis,SEEK_SET);
                for(inde=0;inde<size;inde++){
                    fwrite (buffer, sizeof(buffer), 1, fp);
                }
            }
            exe(name,"particion eliminada");
            fflush(fp);
            fclose(fp);
            leerMBR(path);
            return;
        }else{
            war(name,"buscando particon en logicas...");
            inf("buscando en logicas...");
            eliminar_particion_logica(path,name,inicio_particion_extendida(path));
        }


    }else{
        printf("No existe la ruta del archivo para eliminar la Particion");
    }
   //verificar_ebr(path,inicio_particion_extendida(path));
    leerMBR(path);
}

void eliminar_particion_logica(char* path, char* name, int posicion){

     EBR ebr,aux;
     FILE *fp;
     fp=fopen(path,"rb+");

     if(fp!=NULL){
        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        if(aux.part_status = '1' &&  strcasecmp(aux.part_name,name) == 0){
           ebr.part_status = '1';
           ebr.part_next = aux.part_next;
           strcpy(ebr.part_name,"eliminada");
           fseek(fp,posicion,SEEK_SET);
           fwrite(&ebr,sizeof(EBR),1,fp);

           printf("\nParticion Logica Eliminada %s %d",aux.part_name,posicion);
           fclose(fp);

         }else{
               fclose(fp);
               eliminar_particion_logica(path,name,aux.part_next);
         }
       }else{
            err("no existe ruta","para eliminar particion logica");
       }
}

void eliminar_espacio_particion(char* path,char* name,char unit, int add){

   MBR mbr;
   FILE *fp;
   int eliminar = 0;

   if(unit == 'K'){
      eliminar = add*1024;
   }else if(unit == 'B'){
       eliminar = add;
   }else if(unit == 'M'){
       eliminar = add*1024*1024;
   }

   fp=fopen(path,"rb+");

   if(fp!=NULL){
      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1' && strcasecmp(mbr.mbr_partition_1.part_name,name) == 0){

         if(mbr.mbr_partition_1.part_size > eliminar){
             mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size - eliminar;
             fseek(fp,0,SEEK_SET);
             fwrite(&mbr,sizeof(MBR),1,fp);
             exe(name,"espacio reducido en partición");
          }else{
             err(name,"espacio insuficiente al intentar reducir");
          }
      }else if(mbr.mbr_partition_2.part_status == '1' && strcasecmp(mbr.mbr_partition_2.part_name,name) == 0){
              if(mbr.mbr_partition_2.part_size > eliminar){
                 mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size - eliminar;
                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio reducido en partición");
              }else{
                 err(name,"espacio insuficiente al intentar reducir");
              }
      }else if(mbr.mbr_partition_3.part_status == '1' && strcasecmp(mbr.mbr_partition_3.part_name,name) == 0){
            if(mbr.mbr_partition_3.part_size > eliminar){
                 mbr.mbr_partition_3.part_size = mbr.mbr_partition_3.part_size - eliminar;
                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio reducido en partición");
              }else{
                 err(name,"espacio insuficiente al intentar reducir");
              }
      }else if(mbr.mbr_partition_4.part_status == '1' && strcasecmp(mbr.mbr_partition_4.part_name,name) == 0){
             if(mbr.mbr_partition_4.part_size > eliminar){
                 mbr.mbr_partition_4.part_size = mbr.mbr_partition_4.part_size - eliminar;
                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio reducido en partición");
              }else{
                 err(name,"espacio insuficiente al intentar reducir");
              }
      }else{
        war(name,"no se pudo realizr operación, no existe partición");
      }

      fclose(fp);
   }else{
        err("no existe la ruta del archivo",  "en particion");
   }
}

void agregar_espacio_particion(char* path,char* name,char unit, int add){
   MBR mbr;
   FILE *fp;
   int agregar = 0;

   if(unit == 'K'){
      agregar = add*1024;
   }else if(unit == 'B'){
       agregar = add;
   }else if(unit == 'M'){
       agregar = add*1024*1024;
   }

   fp=fopen(path,"rb+");

   if(fp!=NULL){
      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(mbr.mbr_partition_1.part_status == '1' && strcasecmp(mbr.mbr_partition_1.part_name,name) == 0){

         if(mbr.mbr_tamanio > agregar){
             mbr.mbr_partition_1.part_size = mbr.mbr_partition_1.part_size + agregar;

             mbr.mbr_partition_2.part_start = mbr.mbr_partition_2.part_start+mbr.mbr_partition_2.part_size;
             mbr.mbr_partition_3.part_start = mbr.mbr_partition_3.part_start+mbr.mbr_partition_3.part_size;
             mbr.mbr_partition_4.part_start = mbr.mbr_partition_4.part_start+mbr.mbr_partition_4.part_size;

             fseek(fp,0,SEEK_SET);
             fwrite(&mbr,sizeof(MBR),1,fp);
             exe(name,"espacio agregado en partición");
          }else{
             err(name,"espacio insuficiente");
          }

      }else if(mbr.mbr_partition_2.part_status == '1' && strcasecmp(mbr.mbr_partition_2.part_name,name) == 0){
              if(mbr.mbr_tamanio > agregar){
                 mbr.mbr_partition_2.part_size = mbr.mbr_partition_2.part_size + agregar;
                 mbr.mbr_partition_3.part_start = mbr.mbr_partition_2.part_start+mbr.mbr_partition_2.part_size;
                 mbr.mbr_partition_4.part_start = mbr.mbr_partition_3.part_start+mbr.mbr_partition_3.part_size;

                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio agregado en partición");
              }else{
                 err(name,"espacio insuficiente");
              }
      }else if(mbr.mbr_partition_3.part_status == '1' && strcasecmp(mbr.mbr_partition_3.part_name,name) == 0){
              if(mbr.mbr_tamanio > agregar){
                 mbr.mbr_partition_3.part_size = mbr.mbr_partition_3.part_size + agregar;
                 mbr.mbr_partition_4.part_start = mbr.mbr_partition_3.part_start+mbr.mbr_partition_3.part_size;

                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio agregado en partición");
              }else{
                 err(name,"espacio insuficiente");
              }
      }else if(mbr.mbr_partition_4.part_status == '1' && strcasecmp(mbr.mbr_partition_4.part_name,name) == 0){
               if(mbr.mbr_tamanio > agregar){
                 mbr.mbr_partition_4.part_size = mbr.mbr_partition_4.part_size + agregar;
                 fseek(fp,0,SEEK_SET);
                 fwrite(&mbr,sizeof(MBR),1,fp);
                 exe(name,"espacio agregado en partición");
              }else{
                 err(name,"espacio insuficiente");
              }
      }else{
        war(name,"no se pudo realizr operación, no existe particion");
      }

      fclose(fp);
   }else{
        err("no existe la ruta del archivo",  "en particion");
   }
}

int existe_particion_primaria(char* dir,char name[]){

   MBR mbr;
   int existe = 0;
   FILE *fp;

   fp=fopen(dir,"rb+");

   if(fp!=NULL){

      fseek(fp,0,SEEK_SET);
      fread(&mbr,sizeof(MBR),1,fp);

      if(strcasecmp(mbr.mbr_partition_1.part_name,name) == 0 && mbr.mbr_partition_1.part_status == '1'){
          existe = 1;
      }else if(strcasecmp(mbr.mbr_partition_2.part_name,name) == 0 && mbr.mbr_partition_2.part_status == '1'){
          existe = 1;
      }else if(strcasecmp(mbr.mbr_partition_3.part_name,name) == 0 && mbr.mbr_partition_3.part_status == '1'){
          existe = 1;
      }else if(strcasecmp(mbr.mbr_partition_4.part_name,name) == 0 && mbr.mbr_partition_4.part_status == '1'){
          existe = 1;
      }

      fclose(fp);
      return existe;
   }else{
        err("no existe la ruta del archivo para crear particion",dir);
         existe = -1;
         return existe;
   }
}

void existencia_particion_logica(char* dir,char name[],int posicion){
     EBR aux;
     int existe = 0;
     FILE *fp;

     fp=fopen(dir,"rb+");

     if(fp!=NULL){

        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        if(aux.part_status == '1' && strcasecmp(aux.part_name,name) == 0){
        // printf("\nParticion Logica %s",mbr.mbr_partition_1.part_name);
          // printf("\nPArt %s",aux.part_name);
           existen_logicas = 1;
         }else{
               existencia_particion_logica(dir,name,aux.part_next);
         }
         fclose(fp);
      }else{
            err("no existe la ruta del archivo para crear particion",dir);
      }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    MONTAR Y DESMONTAR PARTICIONES

void montar_particion(char path[], char name[]){
    int existe_primaria = 0;

    //printf("\ndirecc existe %d",existe_particion_primaria(path,name));

    if(existe_particion_primaria(path,name) == 1){
       printf("\nParticion montada %s %s",path,name);
       montar_particion_Memoria(path,name);
       existe_primaria = 1;
    }else if(existe_particion_primaria(path,name) == -1){
       err(path,"no existe la path");
    }else{
       montar_particion_Logica(path,name);
    }

}

void montar_particion_Logica(char* dir,char name[]){
     existencia_particion_logica(dir,name,inicio_particion_extendida(dir));
    // printf("\ndirecc %d",existen_logicas);
     if(existen_logicas == 1){
       // printf("\n Particion Logica Encontrada %d",cont);
        montar_particion_Memoria(dir,name) ;
     //   printf("\nParticion montada %s %s",dir,name);
     }else{
         err(name,"no se puede montar la partición, no existe en disco");
     }
     existen_logicas = 0;
}

void montar_particion_Memoria(char* dir, char name[]){

      MONTAJE in;
      int i = 0;
      char letra[1];
      int numero_id;
      int existe = 0;
      for(i = 0; i<100; i++){
          if(strcasecmp(particiones_montadas[i].path,dir) == 0){
               letra[0] = particiones_montadas[i].letra[0];
               ultima_letra = letra[0];
               numero_id = atoi(particiones_montadas[i].numero)+1;
               existe = 1;
            }
     }

     if(contador_montadas == 0){
           letra[0] = 'a';
           ultima_letra = 'a';
           contador_letra++;
           numero_id = 1;
        }
         if(existe == 0 && contador_montadas!=0){
           if(contador_letra == 1){letra[0] = 'b';contador_letra++;numero_id = 1;
           }else if(contador_letra == 2){letra[0] = 'c';contador_letra++;numero_id = 1;
           }else if(contador_letra == 3){letra[0] = 'd';contador_letra++;numero_id = 1;
           }else if(contador_letra == 4){letra[0] = 'e';contador_letra++;numero_id = 1;
           }else if(contador_letra == 5){letra[0] = 'f';contador_letra++;numero_id = 1;
           }else if(contador_letra == 6){letra[0] = 'g';contador_letra++;numero_id = 1;
           }else if(contador_letra == 7){letra[0] = 'h';contador_letra++;numero_id = 1;
           }else if(contador_letra == 8){letra[0] = 'i';contador_letra++;numero_id = 1;
           }else if(contador_letra == 9){letra[0] = 'j';contador_letra++;numero_id = 1;
           }else if(contador_letra == 10){letra[0] = 'k';contador_letra++;numero_id = 1;
           }else if(contador_letra == 11){letra[0] = 'l';contador_letra++;numero_id = 1;
           }else if(contador_letra == 12){letra[0] = 'm';contador_letra++;numero_id = 1;
           }else if(contador_letra == 13){letra[0] = 'n';contador_letra++;numero_id = 1;
           }else if(contador_letra == 14){letra[0] = 'o';contador_letra++;numero_id = 1;
           }else if(contador_letra == 15){letra[0] = 'p';contador_letra++;numero_id = 1;
           }else if(contador_letra == 16){letra[0] = 'q';contador_letra++;numero_id = 1;
           }else if(contador_letra == 17){letra[0] = 'r';contador_letra++;numero_id = 1;
           }else if(contador_letra == 19){letra[0] = 's';contador_letra++;numero_id = 1;
           }else if(contador_letra == 20){letra[0] = 't';contador_letra++;numero_id = 1;
           }else if(contador_letra == 21){letra[0] = 'u';contador_letra++;numero_id = 1;
           }else if(contador_letra == 22){letra[0] = 'v';contador_letra++;numero_id = 1;
           }else if(contador_letra == 23){letra[0] = 'w';contador_letra++;numero_id = 1;
           }else if(contador_letra == 24){letra[0] = 'x';contador_letra++;numero_id = 1;
           }else if(contador_letra == 25){letra[0] = 'y';contador_letra++;numero_id = 1;
           }else if(contador_letra == 26){letra[0] = 'z';contador_letra++;numero_id = 1;
           }
        }
        strcpy(in.name,name);
        strcpy(in.path,dir);
        in.letra[0] = letra[0];
        sprintf(in.numero, "%d",(numero_id));
        sprintf(in.id, "vd%c%d",letra[0],(numero_id));

        particiones_montadas[contador_montadas] = in;

        contador_montadas++;
        exe(name,"particion monatada");//dir,name,in.id);
     for(i = 0; i<part_mont; i++){

         printf("\n     MONTADO: %s %s %s",particiones_montadas[i].path,particiones_montadas[i].id,particiones_montadas[i].name);

     }
}

void desmontar_particion(char id[]){
    int existe = 0,i;
    MONTAJE limpiar;
    strcpy(limpiar.id,"");
    strcpy(limpiar.name,"");
    strcpy(limpiar.path,"");
    limpiar.numero[0] = ' ';
    limpiar.letra[0] = ' ';

    for(i = 0;i<100;i++){
        if(strcasecmp(particiones_montadas[i].id,id) == 0){
            exe(particiones_montadas[i].name, "particion desmonatada");
           //printf("\nParticion Desmontada %s %s con ID %s",particiones_montadas[i].name,particiones_montadas[i].path,id);
           particiones_montadas[i] = limpiar;
           existe = 1;
        }else{
            //printf("\n SSSSSS: %s  %d",id, strlen(id));
        }
    }

    if(existe == 0){
      err(id,"no existe la particion a desmontar");
    }
    int l = 0;
    for(l = 0; l<part_mont; l++){
         printf("\n     DESMONTADO: %s %s %s",particiones_montadas[l].path,particiones_montadas[l].id,particiones_montadas[l].name);
     }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    GENERAR REPORTE DE DISCO

void Reporte_MBR_EBR(char id[],char path_img[],char name[]){

    int existe = 0;
    int i=0;
    for(i = 0;i<100;i++){
        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           existe = 1;
           Generar_MBR(particiones_montadas[i].path,path_img);
        }
    }

    if(existe == 0){
      err(id,"no se ha montado el disco");
    }else if(existe == 1){
       inf("reporte generado exitosamente");

    }

}

void Generar_MBR(char* path,char img_path[]){
   MBR in;
   EBR ebr;

   FILE *fp;

   fp=fopen(path,"rb+");


   if(fp!=NULL)
   {
         fseek(fp,0,SEEK_SET);
         fread(&in,sizeof(MBR),1,fp);

         set_time(in.mbr_fecha_creacion);
         sprintf(reporte_mbr1, "\n%s%s%s %s %s%s%d%s%s%s%d\n%s%s%s%s%s%s","\"","node0","\"","[label = ","\"","<f0>MBR|<f1>mbr_Tamaño ",in.mbr_tamanio,"|<f1>mbr_Fecha ",fecha,"|<f1>mbr_disk_signature ",in.mbr_disk_signature,"\"","shape = ","\"","record","\"","];");
         if(in.mbr_partition_1.part_status == '1'){
            sprintf(reporte_mbr2, "\n%s%s%s %s %s%s%c%s%c%s%c%s%d%s%d%s%s\n%s%s%s%s%s%s","\"","node1","\"","[label = ","\"","<f0>Particion1|<f1>part_status ",'1',"|<f1>part_type ",in.mbr_partition_1.part_type,"|<f1>part_fit ",in.mbr_partition_1.part_fit,"|<f1>part_start ",in.mbr_partition_1.part_start,"|<f1>part_size ",in.mbr_partition_1.part_size,"|<f1>part_name ",in.mbr_partition_1.part_name,"\"","shape = ","\"","record","\"","];");
            strcat(reporte_mbr1,reporte_mbr2);
         }
         if(in.mbr_partition_2.part_status == '1'){
            sprintf(reporte_mbr3, "\n%s%s%s %s %s%s%c%s%c%s%c%s%d%s%d%s%s\n%s%s%s%s%s%s","\"","node2","\"","[label = ","\"","<f0>Particion2|<f1>part_status ",'1',"|<f1>part_type ",in.mbr_partition_2.part_type,"|<f1>part_fit ",in.mbr_partition_2.part_fit,"|<f1>part_start ",in.mbr_partition_2.part_start,"|<f1>part_size ",in.mbr_partition_2.part_size,"|<f1>part_name ",in.mbr_partition_2.part_name,"\"","shape = ","\"","record","\"","];");
            strcat(reporte_mbr1,reporte_mbr3);
         }
         if(in.mbr_partition_3.part_status == '1'){
            sprintf(reporte_mbr4, "\n%s%s%s %s %s%s%c%s%c%s%c%s%d%s%d%s%s\n%s%s%s%s%s%s","\"","node3","\"","[label = ","\"","<f0>Particion3|<f1>part_status ",'1',"|<f1>part_type ",in.mbr_partition_3.part_type,"|<f1>part_fit ",in.mbr_partition_3.part_fit,"|<f1>part_start ",in.mbr_partition_3.part_start,"|<f1>part_size ",in.mbr_partition_3.part_size,"|<f1>part_name ",in.mbr_partition_3.part_name,"\"","shape = ","\"","record","\"","];");
            strcat(reporte_mbr1,reporte_mbr4);
         }
         if(in.mbr_partition_4.part_status == '1'){
            sprintf(reporte_mbr5, "\n%s%s%s %s %s%s%c%s%c%s%c%s%d%s%d%s%s\n%s%s%s%s%s%s","\"","node4","\"","[label = ","\"","<f0>Particion3|<f1>part_status ",'1',"|<f1>part_type ",in.mbr_partition_4.part_type,"|<f1>part_fit ",in.mbr_partition_4.part_fit,"|<f1>part_start ",in.mbr_partition_4.part_start,"|<f1>part_size ",in.mbr_partition_4.part_size,"|<f1>part_name ",in.mbr_partition_4.part_name,"\"","shape = ","\"","record","\"","];");
            strcat(reporte_mbr1,reporte_mbr5);
         }
        Generar_EBR(path,inicio_particion_extendida(path));

        strcat(reporte_mbr1,reporte_ebr3);
        Reporte_MBR_DOT(reporte_mbr1,img_path);
       //  fclose(fp);
        cont_log = 5;
        contar_ebr = 0;
   }else{
        err(path,"no existe ruta del archivo");
   }
}

void Generar_EBR(char* dir,int posicion){

     EBR ebr,aux;
     FILE *fp;

     fp=fopen(dir,"rb+");

     if(fp!=NULL){

        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);

        if(aux.part_status == '1'){

            if(strcasecmp(aux.part_name,"eliminada") != 0){
                contar_ebr++;

                sprintf(reporte_ebr2, "\n%s%s%d%s %s %s%s%d%s%c%s%c%s%d%s%d%s%s%s%d\n%s%s%s%s%s%s","\"","node",cont_log,"\"","[label = ","\"","<f0>EBR ",contar_ebr,"|<f1>part_status ",'1',"|<f1>part_fit ",aux.part_fit,"|<f1>part_start ",aux.part_start,"|<f1>part_size ",aux.part_size,"|<f1>part_name ",aux.part_name,"|<f1>part_next ",aux.part_next,"\"","shape = ","\"","record","\"","];");
                strcat(reporte_ebr,reporte_ebr2);
                strcpy(reporte_ebr3,reporte_ebr);
                cont_log++;
            }
            //fclose(fp);
            Generar_EBR(dir,aux.part_next);

         }else{

         }
       }else{

       }
}

void Reporte_MBR_DOT(char reporte[],char img_path[]){

     FILE* fp = fopen ( "/home/lex/grafo.dot", "w" );
     char concatenar[1000];
     char aux[1000];
     strcpy(aux,img_path);
     strcpy(concatenar,"dot -Tpng /home/lex/grafo.dot -o ");
     strcat(concatenar,aux);

     fprintf(fp,"digraph hola{");
     fprintf(fp,"rankdir=LR;");
     fprintf(fp,reporte);

     fprintf(fp,"}");
     fclose(fp);
     system(concatenar);

     memset(reporte_mbr1,'\0',1000);
     memset(reporte_mbr2,'\0',1000);
     memset(reporte_mbr3,'\0',1000);
     memset(reporte_mbr4,'\0',1000);
     memset(reporte_mbr5,'\0',1000);
     memset(reporte_ebr,'\0',1000);
     memset(reporte_ebr2,'\0',1000);
     memset(reporte_ebr3,'\0',1000);
}

void Reporte_Disco(char id[],char path_img[],char name[]){

    int existe = 0;
    int i=0;
    for(i = 0;i<100;i++){
        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           existe = 1;
           Generar_Imagen_MBR(particiones_montadas[i].path,path_img);
        }
    }
    if(existe == 0){
        err(id,"no se ha montado disco");
    }else if(existe == 1){
        inf("reporte generado exitosamente");
    }
}

void Generar_Imagen_MBR(char* path, char path_img[]){

   contar_log = 0;
   char logicas[1000];

   MBR in;
   EBR ebr;
   FILE *fp;

   fp=fopen(path,"rb+");

   if(fp!=NULL){
         fseek(fp,0,SEEK_SET);
         fread(&in,sizeof(MBR),1,fp);

         sprintf(reporte_disco,"\n%s%s%s%s%s","strcut0 [label=","\"","MBR","\"","];");
         strcat(reporte_disco5,reporte_disco);

         if(in.mbr_partition_1.part_status == '1'){

            if(in.mbr_partition_1.part_type == 'p' || in.mbr_partition_1.part_type == 'P'){
               sprintf(reporte_disco,"\n%s%s%s%s%s","strcut1 [label=","\"","Primaria","\"","];");
               strcat(reporte_disco5,reporte_disco);
            }else if(in.mbr_partition_1.part_type == 'e' || in.mbr_partition_1.part_type == 'E'){
                     contar_logicas(path,inicio_particion_extendida(path));

                     int i = 0;
                     for(i = 0;i<contar_log;i++){
                         if(i == 0){
                            sprintf(logicas,"EBR|Logica");
                         }else{
                             strcat(logicas,"|EBR|Logica");
                         }

                      }
                     sprintf(reporte_disco,"\n%s%s%s%s%s%s%s","strcut1 [label=","\"","{Extendida|{",logicas,"}}","\"","];");
                     strcat(reporte_disco5,reporte_disco);
            }
         }else{
               sprintf(reporte_disco,"\n%s%s%s%s%s","strcut1 [label=","\"","Libre","\"","];");
               strcat(reporte_disco5,reporte_disco);
         }
         if(in.mbr_partition_2.part_status == '1'){

            if(in.mbr_partition_2.part_type == 'p' || in.mbr_partition_2.part_type == 'P'){
               sprintf(reporte_disco2,"\n%s%s%s%s%s","strcut2 [label=","\"","Primaria","\"","];");
               strcat(reporte_disco5,reporte_disco2);
            }else if(in.mbr_partition_2.part_type == 'e' || in.mbr_partition_2.part_type == 'E'){
                     contar_logicas(path,inicio_particion_extendida(path));

                     int i = 0;
                     for(i = 0;i<contar_log;i++){
                         if(i == 0){
                            sprintf(logicas,"EBR|Logica");
                         }else{
                             strcat(logicas,"|EBR|Logica");
                         }

                      }
                     sprintf(reporte_disco2,"\n%s%s%s%s%s%s%s","strcut2 [label=","\"","{Extendida|{",logicas,"}}","\"","];");
                     strcat(reporte_disco5,reporte_disco2);
            }
         }else{
               sprintf(reporte_disco2,"\n%s%s%s%s%s","strcut2 [label=","\"","Libre","\"","];");
               strcat(reporte_disco5,reporte_disco2);
         }

         if(in.mbr_partition_3.part_status == '1'){
            if(in.mbr_partition_3.part_type == 'p' || in.mbr_partition_3.part_type == 'P'){
               sprintf(reporte_disco3,"\n%s%s%s%s%s","strcut3 [label=","\"","Primaria","\"","];");
               strcat(reporte_disco5,reporte_disco3);
            }else if(in.mbr_partition_3.part_type == 'e' || in.mbr_partition_3.part_type == 'E'){
                    contar_logicas(path,inicio_particion_extendida(path));

                     int i = 0;
                     for(i = 0;i<contar_log;i++){
                         if(i == 0){
                            sprintf(logicas,"EBR|Logica");
                         }else{
                             strcat(logicas,"|EBR|Logica");
                         }

                      }

                     sprintf(reporte_disco3,"\n%s%s%s%s%s%s%s","strcut3 [label=","\"","{Extendida|{",logicas,"}}","\"","];");
                     strcat(reporte_disco5,reporte_disco3);
                     //memset(logicas,'\0',1000);
            }
         }else{
               sprintf(reporte_disco3,"\n%s%s%s%s%s","strcut3 [label=","\"","Libre","\"","];");
               strcat(reporte_disco5,reporte_disco3);
         }

         if(in.mbr_partition_4.part_status == '1'){
            if(in.mbr_partition_4.part_type == 'p' || in.mbr_partition_4.part_type == 'P'){
               sprintf(reporte_disco4,"\n%s%s%s%s%s","strcut4 [label=","\"","Primaria","\"","];");
               strcat(reporte_disco5,reporte_disco4);
            }else if(in.mbr_partition_4.part_type == 'e' || in.mbr_partition_4.part_type == 'E'){
                     contar_logicas(path,inicio_particion_extendida(path));

                     int i = 0;
                     for(i = 0;i<contar_log;i++){
                         if(i == 0){
                            sprintf(logicas,"EBR|Logica");
                         }else{
                             strcat(logicas,"|EBR|Logica");
                         }

                      }
                     sprintf(reporte_disco4,"\n%s%s%s%s%s%s%s","strcut4 [label=","\"","{Extendida|{",logicas,"}}","\"","];");
                     strcat(reporte_disco5,reporte_disco4);
            }
         }else{
               sprintf(reporte_disco4,"\n%s%s%s%s%s","strcut4 [label=","\"","Libre","\"","];");
               strcat(reporte_disco5,reporte_disco4);
         }
         Reporte_Disco_DOT(reporte_disco5,path_img);
         //printf("\n%s",reporte_disco5);

       //  fclose(fp);

   }else{
        printf("No existe ruta del archivo\n\n");
   }
}

void Reporte_Disco_DOT(char reporte[],char img_path[]){

     FILE* fp = fopen ( "/home/lex/grafo2.dot", "w" );
     char concatenar[1000];
     char aux[1000];

     strcpy(aux,img_path);
     strcpy(concatenar,"dot -Tpng /home/lex/grafo2.dot -o ");
     strcat(concatenar,aux);

     fprintf(fp,"digraph structs{");
     fprintf(fp,"node [shape=record];");
     fprintf(fp,reporte);

     fprintf(fp,"}");
     fclose(fp);
     system(concatenar);

     memset(reporte_disco,'\0',1000);
     memset(reporte_disco2,'\0',1000);
     memset(reporte_disco3,'\0',1000);
     memset(reporte_disco4,'\0',1000);
     memset(reporte_disco5,'\0',5000);

     contar_log = 0;
}

void contar_logicas(char* dir,int posicion){
     EBR ebr,aux;
     FILE *fp;

     fp=fopen(dir,"rb+");

     if(fp!=NULL){
        fseek(fp,posicion,SEEK_SET);
        fread(&aux,sizeof(EBR),1,fp);
        if(aux.part_status == '1'){
           if(strcasecmp(aux.part_name,"eliminada") != 0){
             contar_log++;
         }
         contar_logicas(dir,aux.part_next);
      }else{
      }
     }else{
     }
}










