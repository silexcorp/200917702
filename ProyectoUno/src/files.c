#include "files.h"
#include "disk.h"
#include "files.h"


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    CREAR DISCO
void buscar_para_formatear_particion(char* id, char* type){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){
        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           Particion_a_Formatear(particiones_montadas[i].path,particiones_montadas[i].name,type);
           existe = 1;
           break;
        }
    }
    if(existe == 0){
       printf("\nNo existe el ID");
    }
}

void Particion_a_Formatear(char* direccion,char* name,char* type){

   MBR in;
   int existe = 0;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,0,SEEK_SET);
      fread(&in,sizeof(MBR),1,fp);

      if(strcasecmp(in.mbr_partition_1.part_name,name) == 0 && in.mbr_partition_1.part_status == '1'){
         existe = 1;
         Crear_SuperBloque(direccion,in.mbr_partition_1.part_start,in.mbr_partition_1.part_size);
      }else if(strcasecmp(in.mbr_partition_2.part_name,name) == 0 && in.mbr_partition_2.part_status == '1'){
               existe = 1;
               Crear_SuperBloque(direccion,in.mbr_partition_2.part_start,in.mbr_partition_2.part_size);
      }else if(strcasecmp(in.mbr_partition_3.part_name,name) == 0 && in.mbr_partition_3.part_status == '1'){
               existe = 1;
               Crear_SuperBloque(direccion,in.mbr_partition_3.part_start,in.mbr_partition_3.part_size);
      }else if(strcasecmp(in.mbr_partition_4.part_name,name) == 0 && in.mbr_partition_4.part_status == '1'){
               existe = 1;
               Crear_SuperBloque(direccion,in.mbr_partition_4.part_start,in.mbr_partition_4.part_size);
      }

      if(existe == 1){
         //printf("\nParticion Formateada");
      }else{
             printf("\nNo existe la particion %s %s",name,direccion);
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo para Formatear la Particion",direccion);
   }

}
void Crear_SuperBloque(char *direccion,int posicion,int tamanio_particion){

   Super_Bloque in;
   int existe = 0;
   FILE *fp;

   int numero_inodos = (tamanio_particion-sizeof(Super_Bloque))/(4+sizeof(Tabla_Inodos)+3*sizeof(Bloques_Carpeta));
   int numero_bloques = 3*numero_inodos;

//   printf("\n%d %d %d %d %d",numero_inodos,numero_bloques,sizeof(Bloques_Carpeta),tamanio_particion,posicion);
   in.s_inodes_count = numero_inodos;
   in.s_blocks_count = numero_bloques;
   in.s_free_blocks_count = numero_bloques;
   in.s_free_inodes_count = numero_inodos;

   time_t t;
   struct tm *tm;
   char fechayhora[100];
   t=time(NULL);
   tm=localtime(&t);
   strftime(fechayhora, 100, "%d/%m/%Y %H:%M:%S", tm);

   strcpy(in.s_mtime,fechayhora);
   strcpy(in.s_umtime,fechayhora);

   strcpy(in.s_magic,"0xEF53");

   in.s_inode_size = sizeof(Tabla_Inodos);
   in.s_block_size = sizeof(Bloques_Carpeta);
   in.s_first_ino = 1;
   in.s_first_blo = 1;
   in.s_inode_start = posicion+sizeof(Super_Bloque) + numero_inodos + numero_bloques;
   in.s_block_start = posicion+sizeof(Super_Bloque) + numero_inodos + numero_bloques + numero_inodos*sizeof(Tabla_Inodos);
   in.s_bm_inode_star = posicion + sizeof(Super_Bloque);
   in.s_bm_block_star = posicion + sizeof(Super_Bloque) + numero_inodos;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,posicion,SEEK_SET);
      fwrite(&in,sizeof(Super_Bloque),1,fp);

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo para Formatear la Particion",direccion);
   }

   Crear_BM_Inodos(direccion,in.s_bm_inode_star,numero_inodos);
   Crear_BM_Bloque(direccion,in.s_bm_block_star,numero_bloques);
   Crear_Inodo_Raiz(direccion,in.s_inode_start);
   Crear_Bloque_Raiz(direccion,in.s_block_start);

  // Crear_Carpetas(direccion,"users.txt",in.s_inode_start,in.s_block_start,posicion);

 //  Modificar_BM_Bloques(direccion,in.s_bm_block_star,1,'1',posicion);
 //  Modificar_BM_Inodos(direccion,in.s_bm_inode_star,1,'1',posicion);

   //Archivo_User(direccion,"users.txt",in.s_inode_start,in.s_block_start,posicion);

}
int Obtener_SuperBloque(char* direccion,char* name){

   MBR in;
   int existe = 0;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,0,SEEK_SET);
      fread(&in,sizeof(MBR),1,fp);

      if(strcasecmp(in.mbr_partition_1.part_name,name) == 0 && in.mbr_partition_1.part_status == '1'){
         existe = 1;
         return in.mbr_partition_1.part_start;
      }else if(strcasecmp(in.mbr_partition_2.part_name,name) == 0 && in.mbr_partition_2.part_status == '1'){
               existe = 1;
               return in.mbr_partition_2.part_start;
      }else if(strcasecmp(in.mbr_partition_3.part_name,name) == 0 && in.mbr_partition_3.part_status == '1'){
               existe = 1;
               return in.mbr_partition_3.part_start;
      }else if(strcasecmp(in.mbr_partition_4.part_name,name) == 0 && in.mbr_partition_4.part_status == '1'){
               existe = 1;
               return in.mbr_partition_4.part_start;
      }

      if(existe == 1){
         //printf("\nParticion Formateada");
      }else{
             printf("\nNo existe la particion %s %s",name,direccion);
             return 0;
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }

}

void Leer_SuperBloque(char *direccion,int posicion){

   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);
      printf("\n%d",in.s_inodes_count);
      printf("\n%d",in.s_blocks_count);
      printf("\n%d",in.s_free_blocks_count);
      printf("\n%d",in.s_free_inodes_count);
      printf("\n%s",in.s_mtime);
      printf("\n%s",in.s_umtime);
      printf("\n%d",in.s_mnt_count);
      printf("\n%s",in.s_magic);
      printf("\n%d",in.s_inode_size);
      printf("\n%d",in.s_block_size);
      printf("\n%d",in.s_first_ino);
      printf("\n%d",in.s_first_blo);
      printf("\n%d",in.s_bm_inode_star);
      printf("\n%d",in.s_bm_block_star);
      printf("\n%d",in.s_inode_start);
      printf("\n%d",in.s_block_start);
      fclose(fp);
   }else{
        printf("No existe la ruta del archivo para Formatear la Particion",direccion);
   }

}

void Buscando_SuperBloque(char* id, char* path){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           Reporte_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name,path);
           existe = 1;
           break;
        }
    }

    if(existe == 0){
       printf("\nNo existe el ID");
    }
}
void Reporte_SuperBloque(char* direccion,char* name,char* path){

   MBR in;
   int existe = 0;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,0,SEEK_SET);
      fread(&in,sizeof(MBR),1,fp);

      if(strcasecmp(in.mbr_partition_1.part_name,name) == 0 && in.mbr_partition_1.part_status == '1'){
         existe = 1;
         Generar_SuperBloque(direccion,in.mbr_partition_1.part_start,path);
      }else if(strcasecmp(in.mbr_partition_2.part_name,name) == 0 && in.mbr_partition_2.part_status == '1'){
               existe = 1;
               Generar_SuperBloque(direccion,in.mbr_partition_2.part_start,path);
      }else if(strcasecmp(in.mbr_partition_3.part_name,name) == 0 && in.mbr_partition_3.part_status == '1'){
               existe = 1;
               Generar_SuperBloque(direccion,in.mbr_partition_3.part_start,path);
      }else if(strcasecmp(in.mbr_partition_4.part_name,name) == 0 && in.mbr_partition_4.part_status == '1'){
               existe = 1;
               Generar_SuperBloque(direccion,in.mbr_partition_4.part_start,path);
      }

      if(existe == 1){
         //printf("\nParticion Formateada");
      }else{
             printf("\nNo existe la particion %s %s",name,direccion);
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }

}

void Generar_SuperBloque(char *direccion,int posicion,char* path){

   Super_Bloque in;
   FILE *fp;

   char reporte_sb[2000];

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      sprintf(reporte_sb, "\n%s%s%d%s%s%s%s%s%d%s%d%s%d%s%d%s%s%s%s%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%s%s%s","\"","node",1,"\"","[label = ","\"","<f0>SUPER BLOQUE","|<f1>s_inodes_count ",in.s_inodes_count,"|<f1>s_blocks_count ",in.s_blocks_count,"|<f1>s_free_blocks_count ",in.s_free_blocks_count,"|<f1>s_free_inodes_count ",in.s_free_inodes_count,"|<f1>s_mtime ",in.s_mtime,"|<f1>s_umtime ",in.s_umtime,"|<f1>s_mnt_count ",in.s_mnt_count,"|<f1>s_magic ",in.s_magic,"|<f1>s_inode_size ",in.s_inode_size,"|<f1>s_block_size ",in.s_block_size,"|<f1>s_first_ino ",in.s_first_ino,"|<f1>s_first_blo ",in.s_first_blo,"|<f1>s_bm_inode_star ",in.s_bm_inode_star,"|<f1>s_bm_block_star ",in.s_bm_block_star,"|<f1>s_inode_star ",in.s_inode_start,"|<f1>s_block_star ",in.s_block_start,"\"","shape = ","\"","record","\"","];");

      SuperBloque_Dot(reporte_sb,path);

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }

}
void SuperBloque_Dot(char reporte_sb[],char img_path[]){

     FILE* fp = fopen ( "/home/freddy/Escritorio/Reportes/SuperBloque.dot", "w" );
     char concatenar[1000];
     char aux[1000];

     strcpy(aux,img_path);
     strcpy(concatenar,"dot -Tpng /home/freddy/Escritorio/Reportes/SuperBloque.dot -o ");
     strcat(concatenar,aux);

     if(fp!=NULL){
         fprintf(fp,"digraph structs{");
         fprintf(fp,"rankdir=LR;");
         fprintf(fp,reporte_sb);

         fprintf(fp,"}");
         fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     system(concatenar);


}

void Crear_BM_Inodos(char* direccion,int inicio_bm,int numero_inodos){

   Bmp_Inodos in;
   FILE *fp;

   int inicio = inicio_bm;
   int fin = inicio + numero_inodos;
   int contador = 0;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      while(inicio<fin){

         if(contador == 0){
            in.inodo[0] = '1';
         }else{
               in.inodo[0] = '0';
         }

         fseek(fp,inicio,SEEK_SET);
         fwrite(&in,sizeof(Bmp_Inodos),1,fp);

         inicio+=sizeof(Bmp_Inodos);
         contador++;
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }

 //  printf("\nParticion Formateada con Sistema EXT2\n");
   //Leer_BM_Inodos(direccion,inicio_bm,numero_inodos);
}

void Modificar_BM_Inodos(char* direccion,int inicio_bm,int numero_inodo_libre,char estado,int inicio_superbloque){

   Bmp_Inodos in;
   FILE *fp;
   int inicio = inicio_bm;
   int fin = inicio + numero_inodo_libre;

   fp=fopen(direccion,"rb+");
   if(fp!=NULL)
   {
      in.inodo[0] = estado;
      fseek(fp,fin,SEEK_SET);
      fwrite(&in,sizeof(Bmp_Inodos),1,fp);
      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
   Modificar_Primer_InodoLibre(direccion,inicio_superbloque,numero_inodo_libre+1);
}
void Modificar_BM_Bloques(char* direccion,int inicio_bm,int numero_bloque_libre,char estado,int inicio_superbloque){

   Bmp_Bloques in;
   FILE *fp;
   int inicio = inicio_bm;
   int fin = inicio + numero_bloque_libre;

   fp=fopen(direccion,"rb+");
   if(fp!=NULL)
   {
      in.bloque[0] = estado;
      fseek(fp,fin,SEEK_SET);
      fwrite(&in,sizeof(Bmp_Bloques),1,fp);
      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
//   Modificar_Primer_InodoLibre(direccion,inicio_superbloque,numero_inodo_libre+1);
}
void Modificar_Primer_InodoLibre(char* direccion,int inicio_superbloque,int numero_inodo_libre){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,inicio_superbloque,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);
      in.s_first_ino = numero_inodo_libre;
      fseek(fp,inicio_superbloque,SEEK_SET);
      fwrite(&in,sizeof(Super_Bloque),1,fp);


      fclose(fp);
   }else{
        printf("No existe la ruta del archivo para Formatear la Particion",direccion);
   }

   //printf("\nUltimo %d %d",Ultimo_Inodo_Creado(direccion,Obtener_Inicio_Inodos(direccion,inicio_superbloque),Obtener_Numero_Inodos(direccion,inicio_superbloque)),sizeof(Tabla_Inodos));
   //printf("\nUltimo %d %d",Ultimo_Bloque_Creado(direccion,Obtener_Inicio_Bloques(direccion,inicio_superbloque),Obtener_Numero_Bloques(direccion,inicio_superbloque)),sizeof(Bloques_Carpeta));
}
int  Ultimo_Inodo_Creado(char* direccion,int inicio_inodo,int numero_inodos){
     Tabla_Inodos in;
     int verdadero = 0;
     int inicio = inicio_inodo;
     int fin = inicio+ sizeof(Tabla_Inodos)*numero_inodos;
     int pos = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
             fseek(fp,inicio,SEEK_SET);
             fread(&in,sizeof(Tabla_Inodos),1,fp);

             if(in.id >= 0 && in.i_perm>0){
                pos = inicio+sizeof(Tabla_Inodos);
                //break;
             }
             inicio += sizeof(Tabla_Inodos);
        }
        fclose(fp);
        return pos;
     }else{
           printf("\nNo existe la ruta");
     }
}
int Ultimo_Bloque_Creado(char* direccion,int inicio_bloque,int numero_bloque){
     Bloques_Carpeta in;
     int inicio = inicio_bloque;
     int fin = inicio+ sizeof(Bloques_Carpeta)*numero_bloque;
     int cons = 0;
     int pos = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
             fseek(fp,inicio,SEEK_SET);
             fread(&in,sizeof(Bloques_Carpeta),1,fp);
             if(cons == 0 && in.carpetas[0].b_inodo == 0){
                pos = inicio + sizeof(Bloques_Carpeta);
             }else{

                 if(in.carpetas[0].b_inodo > 0){
                    pos = inicio + sizeof(Bloques_Carpeta);
                 }
             }
             cons++;
             inicio += sizeof(Bloques_Carpeta);
        }
        fclose(fp);
        return pos;
     }else{
           printf("\nNo existe la ruta");
     }
}
void Crear_BM_Bloque(char* direccion,int inicio_bm,int numero_bloques){

   Bmp_Bloques in;
   FILE *fp;

   int inicio = inicio_bm;
   int fin = inicio + numero_bloques;
   int contador = 0;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      while(inicio<fin){

         if(contador == 0){
            in.bloque[0] = '1';
         }else{
               in.bloque[0] = '0';
         }

         fseek(fp,inicio,SEEK_SET);
         fwrite(&in,sizeof(Bmp_Bloques),1,fp);

         inicio+=sizeof(Bmp_Bloques);
         contador++;
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }

   printf("\nParticion Formateada con Sistema EXT2\n");
   //Leer_BM_Inodos(direccion,inicio_bm,numero_inodos);
}
void Buscar_BM_Inodos(char* id, char* path){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           Reporte_BM_Inodos(particiones_montadas[i].path,particiones_montadas[i].name,path);
           existe = 1;
           break;
        }
    }

    if(existe == 0){
       printf("\nNo existe el ID");
    }
}

void Reporte_BM_Inodos(char* direccion,char* name,char* path){

   MBR in;
   int existe = 0;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,0,SEEK_SET);
      fread(&in,sizeof(MBR),1,fp);

      if(strcasecmp(in.mbr_partition_1.part_name,name) == 0 && in.mbr_partition_1.part_status == '1'){
         existe = 1;
         Concatenar_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,in.mbr_partition_1.part_start),Obtener_Numero_Inodos(direccion,in.mbr_partition_1.part_start),path);
      }else if(strcasecmp(in.mbr_partition_2.part_name,name) == 0 && in.mbr_partition_2.part_status == '1'){
               existe = 1;
               Concatenar_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,in.mbr_partition_2.part_start),Obtener_Numero_Inodos(direccion,in.mbr_partition_2.part_start),path);
      }else if(strcasecmp(in.mbr_partition_3.part_name,name) == 0 && in.mbr_partition_3.part_status == '1'){
               existe = 1;
               Concatenar_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,in.mbr_partition_3.part_start),Obtener_Numero_Inodos(direccion,in.mbr_partition_3.part_start),path);
      }else if(strcasecmp(in.mbr_partition_4.part_name,name) == 0 && in.mbr_partition_4.part_status == '1'){
               existe = 1;
               Concatenar_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,in.mbr_partition_4.part_start),Obtener_Numero_Inodos(direccion,in.mbr_partition_4.part_start),path);
      }

      if(existe == 1){
         //printf("\nParticion Formateada");
      }else{
             printf("\nNo existe la particion %s %s",name,direccion);
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_Inicio_BMInodos(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_bm_inode_star;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_Numero_Inodos(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_inodes_count;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
void Concatenar_Inodos(char* direccion, int inicio_bm,int numero_inodos,char* path){
//   printf("\n %s %d %s",direccion,inicio_inodo,path);
   Bmp_Inodos in;
   FILE *fp;
   char caracter[1];
   char repor[5000];
   char repor2[2500];
   char* reporte_bm;
   reporte_bm = (char*)malloc(4*numero_inodos*sizeof(char));
   char* reporte_bm2;
   reporte_bm2 = (char*)malloc(16*numero_inodos*sizeof(char));

   int inicio = inicio_bm;
   int fin = inicio + numero_inodos;
   int contador = 0;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      sprintf(reporte_bm,"%s,%s\n","ID","Estado");
      strcat(reporte_bm2,reporte_bm);
      while(inicio<fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Bmp_Inodos),1,fp);

            if(contador == 0){
               sprintf(reporte_bm,"%d,%c\n",contador,in.inodo[0]);
               strcat(reporte_bm2,reporte_bm);
            }else{
            sprintf(reporte_bm,"%d,%c\n",contador,in.inodo[0]);
            strcat(reporte_bm2,reporte_bm);
            }
            inicio+=sizeof(Bmp_Inodos);
            contador++;
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
   //printf("\ntama: %s %d",reporte_bm2,strlen(reporte_bm2));
   Generar_Reporte_BMInodo(reporte_bm2,path);
}
void Generar_Reporte_BMInodo(char reporte[],char direc[]){

     FILE* fp = fopen(direc,"w");

     if(fp!=NULL){
     fprintf(fp,"%s",reporte);
     fclose(fp);
     }else{
          printf("\nNo existe la ruta");
     }
}

void Buscar_BM_Bloques(char* id, char* path){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           Reporte_BM_Bloques(particiones_montadas[i].path,particiones_montadas[i].name,path);
           existe = 1;
           break;
        }
    }

    if(existe == 0){
       printf("\nNo existe el ID");
    }
}

void Reporte_BM_Bloques(char* direccion,char* name,char* path){

   MBR in;
   int existe = 0;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {

      fseek(fp,0,SEEK_SET);
      fread(&in,sizeof(MBR),1,fp);

      if(strcasecmp(in.mbr_partition_1.part_name,name) == 0 && in.mbr_partition_1.part_status == '1'){
         existe = 1;
         Concatenar_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,in.mbr_partition_1.part_start),Obtener_Numero_Bloques(direccion,in.mbr_partition_1.part_start),path);
      }else if(strcasecmp(in.mbr_partition_2.part_name,name) == 0 && in.mbr_partition_2.part_status == '1'){
               existe = 1;
               Concatenar_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,in.mbr_partition_2.part_start),Obtener_Numero_Bloques(direccion,in.mbr_partition_2.part_start),path);
      }else if(strcasecmp(in.mbr_partition_3.part_name,name) == 0 && in.mbr_partition_3.part_status == '1'){
               existe = 1;
               Concatenar_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,in.mbr_partition_3.part_start),Obtener_Numero_Bloques(direccion,in.mbr_partition_3.part_start),path);
      }else if(strcasecmp(in.mbr_partition_4.part_name,name) == 0 && in.mbr_partition_4.part_status == '1'){
               existe = 1;
               Concatenar_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,in.mbr_partition_4.part_start),Obtener_Numero_Bloques(direccion,in.mbr_partition_4.part_start),path);
      }

      if(existe == 1){
         //printf("\nParticion Formateada");
      }else{
             printf("\nNo existe la particion %s %s",name,direccion);
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_Inicio_BMBloques(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_bm_block_star;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_Numero_Bloques(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_blocks_count;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
void Concatenar_Bloques(char* direccion, int inicio_bm,int numero_inodos,char* path){
//   printf("\n %s %d %s",direccion,inicio_inodo,path);
   printf("\nTamana %d %d",numero_inodos,sizeof(Tabla_Inodos));
   Bmp_Bloques in;
   FILE *fp;
   char caracter[1];
   char repor[5000];
   char repor2[2500];
   char* reporte_bm;
   reporte_bm = (char*)malloc(4*numero_inodos*sizeof(char));
   char* reporte_bm2;
   reporte_bm2 = (char*)malloc(16*numero_inodos*sizeof(char));

   int inicio = inicio_bm;
   int fin = inicio + numero_inodos;
   int contador = 0;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      sprintf(reporte_bm,"%s,%s\n","ID","Estado");
      strcat(reporte_bm2,reporte_bm);
      while(inicio<fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Bmp_Bloques),1,fp);

            if(contador == 0){
               sprintf(reporte_bm,"%d,%c\n",contador,in.bloque[0]);
               strcat(reporte_bm2,reporte_bm);
            }else{
                 sprintf(reporte_bm,"%d,%c\n",contador,in.bloque[0]);
                 strcat(reporte_bm2,reporte_bm);
            }
            inicio+=sizeof(Bmp_Bloques);
            contador++;
      }

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
   //printf("\ntama: %s %d",reporte_bm2,strlen(reporte_bm2));
   Generar_Reporte_BMBloque(reporte_bm2,path);
}
void Generar_Reporte_BMBloque(char reporte[],char direc[]){

     FILE* fp = fopen(direc,"w");

     if(fp!=NULL){
     fprintf(fp,"%s",reporte);
     fclose(fp);
     }else{
          printf("\nNo existe la ruta");
     }
}
int Obtener_Inicio_Inodos(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_inode_start;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_Inicio_Bloques(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_block_start;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_InodoLibre(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_first_ino;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
int Obtener_BoqueLibre(char* direccion, int posicion){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,posicion,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      return in.s_first_blo;

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}
void Crear_Inodo_Raiz(char* direccion,int inicio){
     Tabla_Inodos in;
     in.id = 0;
     in.i_uid = 1;
     in.i_gid = 1;
     in.i_size = 0;
     time_t t;
     struct tm *tm;
     char fechayhora[100];
     t=time(NULL);
     tm=localtime(&t);
     strftime(fechayhora, 100, "%d/%m/%Y %H:%M:%S", tm);
     strcpy(in.i_atime,fechayhora);
     strcpy(in.i_ctime,fechayhora);
     strcpy(in.i_mtime,fechayhora);

     int i = 0;
     for(i = 0;i<15;i++){
         if(i == 0){
           in.i_block[i] = 0;
         }else{
               in.i_block[i] = -1;
         }
     }
     in.i_type[0] = '0';
     in.i_perm = 777;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,inicio,SEEK_SET);
        fwrite(&in,sizeof(Tabla_Inodos),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

  //   Leer_Inodo(direccion,inicio);
}
void Crear_Bloque_Raiz(char* direccion,int inicio){

     Bloques_Carpeta in;

     in.carpetas[0].b_inodo = 0;
     strcpy(in.carpetas[0].b_name,".");
     in.carpetas[1].b_inodo = 0;
     strcpy(in.carpetas[1].b_name,"..");
     in.carpetas[2].b_inodo = -1;
     strcpy(in.carpetas[2].b_name,"-1");
     in.carpetas[3].b_inodo = -1;
     strcpy(in.carpetas[3].b_name,"-1");

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,inicio,SEEK_SET);
        fwrite(&in,sizeof(Bloques_Carpeta),1,fp);
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
//     LeerBloque(direccion,inicio);
}
void Leer_Inodo(char* direccion,int posicion,int numero_inodos){

     Tabla_Inodos in;

     int inicio = posicion;
     int fin = inicio + sizeof(Tabla_Inodos)*numero_inodos;
     int con = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Tabla_Inodos),1,fp);
            if(con < 7){
                if(in.id>=0 && in.i_perm > 0){
                    printf("\n%d",in.id);
                    printf("\n%d",in.i_gid);
                    printf("\n%d",in.i_uid);
                    printf("\n%s",in.i_mtime);
                    printf("\n%s",in.i_ctime);
                    printf("\n%s",in.i_atime);
                    printf("\n%d",in.i_perm);
                    printf("\n%d",in.i_size);
                    int i=0;
                    for(i=0;i<15;i++){
                        printf("\n%d",in.i_block[i]);
                    }
                }
             }
             con++;
            inicio += sizeof(Tabla_Inodos);
        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void LeerBloque(char* direccion, int posicion, int numero_bloques){
     Bloques_Carpeta in;
     int inicio = posicion;
     int fin = inicio + numero_bloques*sizeof(Bloques_Carpeta);
     int contador = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        //printf("\nENTRASSS %d %d %d %d",inicio,fin,numero_bloques,sizeof(Bloques_Carpeta));
        while(inicio < fin){

            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Bloques_Carpeta),1,fp);
            if(contador<14){
                printf("\n");
                int i=0;
                for(i=0;i<4;i++){
                    printf("\nInodo: %d",in.carpetas[i].b_inodo);
                    printf("\nNombre: %s",in.carpetas[i].b_name);
                }
            }
            contador++;
            inicio+=sizeof(Bloques_Carpeta);
        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
}

void Crear_Carpetas(char* direccion,char* name_dir,int pos_inodo_raiz,int pos_bloque_raiz,int inicio_superbloque){

     Tabla_Inodos in;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,pos_inodo_raiz,SEEK_SET);
        fread(&in,sizeof(Tabla_Inodos),1,fp);
        int espacio;
        int i = 0;
        for(i = 0; i<15; i++){

            if(in.i_block[i] !=-1){
               int pos_raiz_bloque = Obtener_Inicio_Bloques(direccion,inicio_superbloque);
               int numero_bloq = Obtener_Numero_Bloques(direccion,inicio_superbloque);
               int pos_bloques = Obtener_PosBloqueID(direccion,pos_raiz_bloque,numero_bloq,in.i_block[i]);
               espacio = Agregar_Carpeta_BloqueExisente(direccion,name_dir,pos_bloques,inicio_superbloque);
               //printf("\nEspacio %d %d %d",espacio, pos_bloques, in.i_block[i]);
               if(espacio == 0){//si hay espacio
                  int pos_raiz_inodo = Obtener_Inicio_Inodos(direccion,inicio_superbloque);
                  int numero_ino = Obtener_Numero_Inodos(direccion,inicio_superbloque);
                  int ultimo_inodo = Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino);
                  int ultimo_bloque = Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq);

                  if(id_inodo_mv == 0){
                     Crear_Nuevo_Inodo(direccion,ultimo_inodo,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
                     Crear_Bloque_Nuevo_Inodo(direccion,ultimo_bloque,Obtener_InodoLibre(direccion,inicio_superbloque),0,inicio_superbloque);
                  }

                  break;
               }
               //break;
            }
             if(in.i_block[i] == -1 && espacio == 1){
                //printf("\nName %s",name_dir);
                in.i_block[i] = Obtener_BoqueLibre(direccion,inicio_superbloque);
                fseek(fp,pos_inodo_raiz,SEEK_SET);
                fwrite(&in,sizeof(Tabla_Inodos),1,fp);

                int pos_raiz_bloque = Obtener_Inicio_Bloques(direccion,inicio_superbloque);
                int numero_bloq = Obtener_Numero_Bloques(direccion,inicio_superbloque);
                int ultimo_bloque = Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq);
                Crear_Bloque_Nuevo(direccion,ultimo_bloque,name_dir,inicio_superbloque);

                int pos_raiz_inodo = Obtener_Inicio_Inodos(direccion,inicio_superbloque);
                int numero_ino = Obtener_Numero_Inodos(direccion,inicio_superbloque);
                int ultimo_inodo = Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino);
                int ultimo_bloque2 = Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq);

                if(id_inodo_mv == 0){
                    Crear_Nuevo_Inodo(direccion,ultimo_inodo,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
                    Crear_Bloque_Nuevo_Inodo(direccion,ultimo_bloque2,Obtener_InodoLibre(direccion,inicio_superbloque),0,inicio_superbloque);
                }

                break;
            }
        }
       //LeerBloque(direccion,pos_bloque_raiz,Obtener_Numero_Bloques(direccion,inicio_superbloque));
       //Leer_Inodo(direccion,pos_inodo_raiz,Obtener_Numero_Inodos(direccion,inicio_superbloque));
        //printf("\nUlitmo %d",Ultimo_Bloque_Creado(direccion,pos_bloque_raiz,Obtener_Numero_Bloques(direccion,inicio_superbloque)));
       // Leer_Inodo(direccion,pos_inodo_raiz);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
int Agregar_Carpeta_BloqueExisente(char* direccion,char* name,int pos_bloque_raiz,int inicio_superbloque){
     Bloques_Carpeta in;
     //printf("\nPrintf %s",name);

     FILE* fp = fopen(direccion,"rb+");

     int no_hay_espacio = 0;

     if(fp!=NULL){
        fseek(fp,pos_bloque_raiz,SEEK_SET);
        fread(&in,sizeof(Bloques_Carpeta),1,fp);
        int i=0;
        for(i=0;i<4;i++){
            if(in.carpetas[i].b_inodo == -1){
             //  printf("\nENTRA");
               strcpy(in.carpetas[i].b_name,name);

               if(id_inodo_mv == 1){
                  in.carpetas[i].b_inodo = inodo_ids2;
               }else{
                    in.carpetas[i].b_inodo = Obtener_InodoLibre(direccion,inicio_superbloque);
               }
               fseek(fp,pos_bloque_raiz,SEEK_SET);
               fwrite(&in,sizeof(Bloques_Carpeta),1,fp);
               no_hay_espacio = 0;
               break;
            }else{
                  no_hay_espacio = 1;
            }
        }
        fclose(fp);
        //LeerBloque(direccion,pos_bloque_raiz,);
        return no_hay_espacio;
     }else{
           printf("\nNo existe la ruta");
     }

}
void Crear_Bloque_Nuevo(char*direccion,int pos_Bloque_nuevo,char* name,int inicio_superbloque){
     Bloques_Carpeta in;

     if(id_inodo_mv == 1){
        in.carpetas[0].b_inodo = inodo_ids2;
     }else{
          in.carpetas[0].b_inodo = Obtener_InodoLibre(direccion,inicio_superbloque);
     }

     strcpy(in.carpetas[0].b_name,name);
     in.carpetas[1].b_inodo = -1;
     strcpy(in.carpetas[1].b_name,"-1");
     in.carpetas[2].b_inodo = -1;
     strcpy(in.carpetas[2].b_name,"-1");
     in.carpetas[3].b_inodo = -1;
     strcpy(in.carpetas[3].b_name,"-1");

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        fseek(fp,pos_Bloque_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Bloques_Carpeta),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_BloqueLibre_SB(direccion,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
     Modificar_BM_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,inicio_superbloque),Obtener_BoqueLibre(direccion,inicio_superbloque)-1,'1',inicio_superbloque);
}
void Crear_Nuevo_Inodo(char*direccion,int pos_inodo_nuevo,int inicio_superbloque,int apuntador_bloque){

     //printf("\nPosINODOS %d %d\n",pos_inodo_nuevo,sizeof(Tabla_Inodos));

     Tabla_Inodos in;
     in.id = Obtener_InodoLibre(direccion,inicio_superbloque);
     in.i_uid = 1;
     in.i_gid = 1;
     in.i_size = 0;
     time_t t;
     struct tm *tm;
     char fechayhora[100];
     t=time(NULL);
     tm=localtime(&t);
     strftime(fechayhora, 100, "%d/%m/%Y %H:%M:%S", tm);
     strcpy(in.i_atime,fechayhora);
     strcpy(in.i_ctime,fechayhora);
     strcpy(in.i_mtime,fechayhora);

     int i = 0;
     for(i = 0;i<15;i++){
         if(i == 0){
           in.i_block[i] = apuntador_bloque;
         }else{
               in.i_block[i] = -1;
         }
     }
     in.i_type[0] = '0';
     in.i_perm = 777;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,pos_inodo_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Tabla_Inodos),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_InodoLibre_SB(direccion,inicio_superbloque,in.id);
     Modificar_BM_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,inicio_superbloque),in.id,'1',inicio_superbloque);
}
void Crear_Bloque_Nuevo_Inodo(char*direccion,int pos_Bloque_nuevo,int inodo_actual,int inodo_padre,int inicio_superbloque){
     Bloques_Carpeta in;

     in.carpetas[0].b_inodo = inodo_actual-1;
     strcpy(in.carpetas[0].b_name,".");
     in.carpetas[1].b_inodo = inodo_padre;
     strcpy(in.carpetas[1].b_name,"..");
     in.carpetas[2].b_inodo = -1;
     strcpy(in.carpetas[2].b_name,"-1");
     in.carpetas[3].b_inodo = -1;
     strcpy(in.carpetas[3].b_name,"-1");

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        fseek(fp,pos_Bloque_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Bloques_Carpeta),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_BloqueLibre_SB(direccion,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
     Modificar_BM_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,inicio_superbloque),Obtener_BoqueLibre(direccion,inicio_superbloque)-1,'1',inicio_superbloque);

}
void Modificar_InodoLibre_SB(char* direccion, int inicio_superbloque,int numero_inodo_libre){
   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,inicio_superbloque,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      in.s_first_ino = numero_inodo_libre+1;
      fseek(fp,inicio_superbloque,SEEK_SET);
      fwrite(&in,sizeof(Super_Bloque),1,fp);

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}

void Modificar_BloqueLibre_SB(char* direccion, int inicio_superbloque, int numero_bloque_libre){

   Super_Bloque in;
   FILE *fp;

   fp=fopen(direccion,"rb+");

   if(fp!=NULL)
   {
      fseek(fp,inicio_superbloque,SEEK_SET);
      fread(&in,sizeof(Super_Bloque),1,fp);

      in.s_first_blo = numero_bloque_libre+1;
      fseek(fp,inicio_superbloque,SEEK_SET);
      fwrite(&in,sizeof(Super_Bloque),1,fp);

      fclose(fp);
   }else{
        printf("No existe la ruta del archivo ",direccion);
   }
}

void Buscar_ID_CrearCarpetas(char* id,char* name_path,int sin_par){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
      // Crear_Carpetas(path_name,"home",pos_inodo,pos_bloque,inicio_sb);
       //if(sin_par == 0){
          Crear_Varias_Carpetas(path_name,name_path,pos_inodo,pos_bloque,inicio_sb,sin_par);
       //}
    }else{
          printf("\nNo existe el ID");
    }
}
int existe_carpeta = 0;
int inodo_ids = 0;
int bloque_ids = 0;
int pos_in = 0;
int pos_in_existente = 0;
int pos_blo = 0;
int bloque_verificar = 0;

int bloque_eliminar = 0;
void Crear_Varias_Carpetas(char* direccion, char* path_crear, int pos_inodo, int pos_bloque, int inicio_sb,int sin_par){

     int co = 0,co2;
     char cadena[500];

     if(sin_par == 0){
        strcpy(cadena,substr(path_crear,1,strlen(path_crear)-2));
     }else{
          strcpy(cadena,substr(path_crear,1,strlen(path_crear)));
     }

     char** tokens;
     tokens = str_split(cadena, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co == 1){
        Crear_Carpetas(direccion,*(tokens),pos_inodo,pos_bloque,inicio_sb);
     }else if(co > 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {
                      if(i == 0){
                         existe_carpeta = 0;
                      }

                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);
                //         printf("\nDireccion %s %d %d",*(tokens+i),existe_carpeta,inodo_ids);
                       //  existe_carpeta = 0;
                      }
                      else if(existe_carpeta == 1 && i>0){

                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                  //        printf("\n\nCarpeta a crear %s %d\n\n",*(tokens + i),existe_carpeta);
                         if(sin_par == 1){

                            if(existe_carpeta == 0){
                    //           printf("\nINODOS %d",inodo_ids);
                               Crear_Carpetas(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n|Carpeta Creada %s Creada en la ruta %s",*(tokens+i),path_crear);
                               printf("\n'------------------------------------------------------------------------'");
                               Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);

                            }
                         }

                      }else if(existe_carpeta == 0 && i>0 && i<co){

                               if(sin_par == 0){
                                   printf("\n.------------------------------------------------------------------------.");
                                   printf("\n No existe el Directorio que quiere Crear %s",*(tokens+i));
                                   printf("\n'------------------------------------------------------------------------'");
                                   break;
                               }else{
                                     //Crear_Carpetas(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);
                      //               printf("\n\nCarpeta a crear %s %d\n\n",*(tokens + i),existe_carpeta);
                               }
                      }
                      //printf("\nINODO %d",inodo_ids);
                      if(existe_carpeta == 0 && (i+1) == co){
                        //  printf("\nBlqoue %d",bloque_ids);
                        //  printf("\nEntra a crear Subdirectorio %d %d %d %d",existe_carpeta,inodo_ids,bloque_ids,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids));
                            //printf("\nPree %d %d %d",bloque_ids,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),pos_in);

                          Crear_Carpetas(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);
                          printf("\n.------------------------------------------------------------------------.");
                          printf("\n|Carpeta Creada Creada %s en la ruta %s",*(tokens+i),path_crear);
                          printf("\n'------------------------------------------------------------------------'");

                      }

                 }

              }

     }
   //LeerBloque(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb));
   //  Leer_Inodo(direccion,pos_inodo,Obtener_Numero_Inodos(direccion,inicio_sb));
}

int Obtener_PosBloqueID(char* direccion, int posicion,int numero_bloques,int id_bloque){

     Bloques_Carpeta in;
     int contador = 0;
     int inicio = posicion;
     int fin = inicio + sizeof(Bloques_Carpeta)*numero_bloques;
     int posicion_bloque = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
              fseek(fp,inicio,SEEK_SET);
              fread(&in,sizeof(Bloques_Carpeta),1,fp);

              if(contador == id_bloque){
                 posicion_bloque = inicio;
                 break;
              }
              contador++;
              inicio += sizeof(Bloques_Carpeta);
        }
        fclose(fp);
        return posicion_bloque;
     }else{
           printf("\nNo existe la ruta");
     }
}

void Crear_Subdirectorio(char* direccion,char* name_path,int pos_inodo, int pos_bloque,int inicio_sb,int posicion_apuntador,int id_inodo){
     Tabla_Inodos in;

     FILE* fp = fopen(direccion,"rb+");
     int inicio = pos_inodo;
     int fin = inicio + sizeof(Tabla_Inodos)*Obtener_Numero_Inodos(direccion,inicio_sb);
     if(fp!=NULL){
        while(inicio < fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Tabla_Inodos),1,fp);
            if(in.id == id_inodo){
               pos_in = inicio;
               int i = posicion_apuntador;
               for(i = posicion_apuntador;i<15;i++){
                   if(in.i_block[i] != -1){
                      //printf("\npath %s bloque %d",name_path,in.i_block[i]);
                      bloque_eliminar = in.i_block[i];
                      Verificar_Bloques(direccion,name_path,pos_bloque,in.i_block[i],Obtener_Numero_Bloques(direccion,inicio_sb),posicion_apuntador,pos_inodo,inicio_sb);
                      break;
                   }
               }
               break;
            }
            inicio += sizeof(Tabla_Inodos);
        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
}
void Verificar_Bloques(char* direccion,char* name_path,int pos_bloque_inicio,int apuntador_bloque,int numero_bloques,int posicion_apuntador,int pos_inodo,int inicio_sb){

     Bloques_Carpeta in;
     int existencia = 0;
     int contador = 0;
     int inicio = pos_bloque_inicio;
     int fin = inicio + sizeof(Bloques_Carpeta)*numero_bloques;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
              fseek(fp,inicio,SEEK_SET);
              fread(&in,sizeof(Bloques_Carpeta),1,fp);
              int i = 0;
              for(i = 0;i<4;i++){
                  /*if(strcasecmp(in.carpetas[0].b_name,".") == 0 && strcasecmp(in.carpetas[1].b_name,"..") == 0 && strcasecmp(in.carpetas[2].b_name,"-1") == 0){
                     bloque_ids = contador;
                  }*/
                  if(contador == apuntador_bloque){

                     if(strcasecmp(in.carpetas[i].b_name,name_path) == 0){
                         //printf("\npath %s inodo %d bloque %d",name_path,in.carpetas[i].b_inodo,apuntador_bloque);
                         existencia = 1;
                         inodo_ids = in.carpetas[i].b_inodo;
                       //  Crear_Subdirectorio(direccion,name_path,pos_inodo,pos_bloque_inicio,inicio_sb,posicion_apuntador+1,in.carpetas[i].b_inodo);
                         break;
                      }
                  }else{

                  }

              }

              contador++;
              inicio += sizeof(Bloques_Carpeta);
        }
        if(existencia == 1){
           existe_carpeta = existencia;
           //printf("\nExiste Carpeta %d",inodo_ids);
        }else{
              existencia = 0;
              existe_carpeta = existencia;
             // printf("\nNo Existe Carpeta");
              Crear_Subdirectorio(direccion,name_path,pos_inodo,pos_bloque_inicio,inicio_sb,posicion_apuntador+1,inodo_ids);
        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
}

void Buscar_TablaInodos(char* id, char* paths){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           int inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           int pos_i = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           int num_i = Obtener_Numero_Inodos(particiones_montadas[i].path,inicio_sb);
           Reporte_Inodos(particiones_montadas[i].path,pos_i,num_i,paths);

           existe = 1;
           break;
        }
    }

    if(existe == 0){
       printf("\nNo existe el ID");
    }
}
void Reporte_Inodos(char* direccion,int posicion,int numero_inodos,char* paths){

     Tabla_Inodos in;

     char bloques[1000];
     char nodos[100000],nodos2[50000],nodos3[50000];

     int inicio = posicion;
     int fin = inicio + sizeof(Tabla_Inodos)*numero_inodos;
     int con = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        //sprintf(nodos2,"%s%d","node",con);
        //strcat(nodos,nodos2);
        while(inicio < fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Tabla_Inodos),1,fp);
           // if(con < 7){
                if(in.id>=0 && in.i_perm > 0){

                    sprintf(bloques,"%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d","|<f1>i_block_1 ",in.i_block[0],"|<f1>i_block_2 ",in.i_block[1],"|<f1>i_block_3 ",in.i_block[2],"|<f1>i_block_4 ",in.i_block[3],"|<f1>i_block_5 ",in.i_block[4],"|<f1>i_block_6 ",in.i_block[5],"|<f1>i_block_7 ",in.i_block[6],"|<f1>i_block_8 ",in.i_block[7],"|<f1>i_block_9 ",in.i_block[8],"|<f1>i_block_10 ",in.i_block[9],"|<f1>i_block_11 ",in.i_block[10],"|<f1>i_block_12 ",in.i_block[11],"|<f1>i_block_13 ",in.i_block[12],"|<f1>i_block_14 ",in.i_block[13],"|<f1>i_block_15 ",in.i_block[14]);
                    sprintf(reporte_inodo2, "\n%s%s%d%s%s%s%s%d%s%d%s%d%s%s%s%s%s%s%s%d%s%d%s%c%s%s%s%s%s%s%s","\"","node",con,"\"","[label = ","\"","<f0>Inodo ",in.id,"|<f1>i_gid ",in.i_gid,"|<f1>i_uid ",in.i_uid,"|<f1>i_mtime ",in.i_mtime,"|<f1>i_ctime ",in.i_ctime,"|<f1>i_atime ",in.i_atime,"|<f1>i_perm ",in.i_perm,"|<f1>i_size ",in.i_size,"|<f1>i_type ",in.i_type[0],bloques,"\"","shape = ","\"","record","\"","];");
                    strcat(reporte_inodo,reporte_inodo2);
                    strcpy(reporte_inodo3,reporte_inodo);

                    if(con == 0){
                       sprintf(nodos2,"%s%d","node",con);
                       strcat(nodos,nodos2);
                    }else{
                          sprintf(nodos3,"->%s%d","node",con);
                          strcat(nodos,nodos3);
                    }
                    con++;
                }
           //  }
             //con++;
            inicio += sizeof(Tabla_Inodos);
        }
        Reporte_Inodo_DOT(reporte_inodo3,nodos,paths);
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Reporte_Inodo_DOT(char reporte[],char reporte2[],char img_path[]){

     FILE* fp = fopen ( "/home/freddy/Escritorio/Reportes/inodo.dot", "w" );
     char concatenar[1000];
     char aux[1000];

     strcpy(aux,img_path);
     strcpy(concatenar,"dot -Tpng /home/freddy/Escritorio/Reportes/inodo.dot -o ");
     strcat(concatenar,aux);

     fprintf(fp,"digraph structs{");
     fprintf(fp,"rankdir=LR;");
     fprintf(fp,reporte);
     fprintf(fp,reporte2);
    // fprintf(fp,"<f1>-><f1>");

     fprintf(fp,"}");
     fclose(fp);
     system(concatenar);

     memset(reporte_inodo,'\0',25000);
     memset(reporte_inodo2,'\0',25000);
     memset(reporte_inodo3,'\0',50000);

}
void Buscar_TablaBloques(char* id, char* paths){

    int i = 0;
    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           int inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           int pos_i = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           int num_i = Obtener_Numero_Bloques(particiones_montadas[i].path,inicio_sb);
           Reporte_Bloques(particiones_montadas[i].path,pos_i,num_i,paths);

           existe = 1;
           break;
        }
    }

    if(existe == 0){
       printf("\nNo existe el ID");
    }
}

void Reporte_Bloques(char* direccion,int posicion,int numero_bloques,char* paths){

     Bloques_Carpeta in;
     Bloques_Archivo in2;

     char bloques[1000];
     char nods[100002],nods2[50000],nods3[50000];

     memset(nods,'\0',100002);
     memset(nods2,'\0',50000);
     memset(nods3,'\0',50000);

     int inicio = posicion;
     int fin = inicio + sizeof(Bloques_Carpeta)*numero_bloques;
     int con = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        while(inicio < fin){
            fseek(fp,inicio,SEEK_SET);
            fread(&in,sizeof(Bloques_Carpeta),1,fp);

                if(con == 0 && in.carpetas[0].b_inodo == 0){
                   sprintf(reporte_bloque2, "\n%s%s%d%s%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%s%s%s","\"","node",con,"\"","[label = ","\"","<f0>Bloque ",con,"|<f1>",in.carpetas[0].b_name," ",in.carpetas[0].b_inodo,"|<f1>",in.carpetas[1].b_name," ",in.carpetas[1].b_inodo,"|<f1>",in.carpetas[2].b_name," ",in.carpetas[2].b_inodo,"|<f1>",in.carpetas[3].b_name," ",in.carpetas[3].b_inodo,"\"","shape = ","\"","record","\"","];");
                   strcat(reporte_bloque,reporte_bloque2);
                   strcpy(reporte_bloque3,reporte_bloque);

                   sprintf(nods2,"%s%d","node",con);
                   strcat(nods,nods2);



                 }else{
                     if(in.carpetas[0].b_inodo > 0 && in.carpetas[0].b_inodo != 5175 && in.carpetas[0].b_inodo!=892613426 && in.carpetas[0].b_inodo!=858927408 && in.carpetas[0].b_inodo!=1869574700
                        || in.carpetas[0].b_inodo == -1 && in.carpetas[0].b_inodo != 5175 && in.carpetas[0].b_inodo!=892613426 && in.carpetas[0].b_inodo!=858927408 && in.carpetas[0].b_inodo!=1869574700 ){
                        sprintf(reporte_bloque2, "\n%s%s%d%s%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%d%s%s%s%s%s%s","\"","node",con,"\"","[label = ","\"","<f0>Bloque ",con,"|<f1>",in.carpetas[0].b_name," ",in.carpetas[0].b_inodo,"|<f1>",in.carpetas[1].b_name," ",in.carpetas[1].b_inodo,"|<f1>",in.carpetas[2].b_name," ",in.carpetas[2].b_inodo,"|<f1>",in.carpetas[3].b_name," ",in.carpetas[3].b_inodo,"\"","shape = ","\"","record","\"","];");
                        strcat(reporte_bloque,reporte_bloque2);
                        strcpy(reporte_bloque3,reporte_bloque);

                        //printf("\nPrin2 %d %d",in.carpetas[0].b_inodo,con);
                        sprintf(nods3,"->%s%d","node",con);
                        strcat(nods,nods3);
                     }else if(in.carpetas[0].b_inodo == 5175 || in.carpetas[0].b_inodo == 892613426 ||in.carpetas[0].b_inodo == 858927408 || in.carpetas[0].b_inodo == 1869574700){
                              fseek(fp,inicio,SEEK_SET);
                              fread(&in2,sizeof(Bloques_Archivo),1,fp);

                              char caracter[strlen(in2.b_content)];
                              memset(caracter,'\0',strlen(in2.b_content));
                             // printf("\nTamanio %d",strlen(in2.b_content));
                            //  char concatenar[2*strlen(in2.b_content)];
                              int s = 0;
                              int cos=0;

                              if(in2.b_content[1] == ','){
                                //printf("\n%s",in2.b_content);
                                strcpy(caracter,in2.b_content);

                              }else{
                                  for(s = 0; s < strlen(in2.b_content);s++){

                                      if(in2.b_content[s] == '0' || in2.b_content[s] == '1' || in2.b_content[s] == '2'|| in2.b_content[s] == '3' || in2.b_content[s] == '4' || in2.b_content[s] == '5' || in2.b_content[s] == '6' || in2.b_content[s] == '7' || in2.b_content[s] == '8' || in2.b_content[s] == '9'){
                                     //   caracter[s] = in2.b_content[s];
                                        cos++;

                                      }
                                  }
                                  int u = 0;
                                  for(u = 0; u < cos;u++){

                                      if(in2.b_content[u] == '0' || in2.b_content[u] == '1' || in2.b_content[u] == '2'|| in2.b_content[u] == '3' || in2.b_content[u] == '4' || in2.b_content[u] == '5' || in2.b_content[u] == '6' || in2.b_content[u] == '7' || in2.b_content[u] == '8' || in2.b_content[u] == '9'){
                                         caracter[u] = in2.b_content[u];
                               //     cos++;
                                       }
                                  }
                              }

                              //printf("\nPrintf %d",cos);
                              sprintf(reporte_bloque2, "\n%s%s%d%s%s%s%s%d%s%s%s%s%s%s%s%s","\"","node",con,"\"","[label = ","\"","<f0>Bloque ",con,"|<f1>",caracter,"\"","shape = ","\"","record","\"","];");
                              strcat(reporte_bloque,reporte_bloque2);
                              strcpy(reporte_bloque3,reporte_bloque);

                              sprintf(nods3,"->%s%d","node",con);
                              strcat(nods,nods3);

                     }
                 }

                    //sprintf(reporte_inodo2, "\n%s%s%d%s%s%s%s%d%s%d%s%d%s%s%s%s%s%s%s%d%s%d%s%c%s%s%s%s%s%s%s","\"","node",con,"\"","[label = ","\"","<f0>Inodo ",in.id,"|<f1>i_gid ",in.i_gid,"|<f1>i_uid ",in.i_uid,"|<f1>i_mtime ",in.i_mtime,"|<f1>i_ctime ",in.i_ctime,"|<f1>i_atime ",in.i_atime,"|<f1>i_perm ",in.i_perm,"|<f1>i_size ",in.i_size,"|<f1>i_type ",in.i_type[0],bloques,"\"","shape = ","\"","record","\"","];");
                   // strcat(reporte_inodo,reporte_inodo2);
                   // strcpy(reporte_inodo3,reporte_inodo);

                    con++;
             //}
            inicio += sizeof(Bloques_Carpeta);
        }
        Reporte_Bloques_DOT(reporte_bloque3,nods,paths);
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Reporte_Bloques_DOT(char reporte[],char repor2[],char img_path[]){

     FILE* fp = fopen ( "/home/freddy/Escritorio/Reportes/bloque.dot", "w" );
     char concatenar[1000];
     char aux[1000];

     strcpy(aux,img_path);
     strcpy(concatenar,"dot -Tpng /home/freddy/Escritorio/Reportes/bloque.dot -o ");
     strcat(concatenar,aux);

     fprintf(fp,"digraph structs{");
     fprintf(fp,"rankdir=LR;");
     fprintf(fp,reporte);
     fprintf(fp,repor2);
    // fprintf(fp,"node1->node2");

     fprintf(fp,"}");
     fclose(fp);
     system(concatenar);

     memset(reporte_bloque,'\0',25000);
     memset(reporte_bloque2,'\0',25000);
     memset(reporte_bloque3,'\0',50000);

}

void Buscar_ID_CrearArchivos(char* id,char* name_path,int sin_par,int size_file){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
      // Crear_Carpetas(path_name,"home",pos_inodo,pos_bloque,inicio_sb);
       //if(sin_par == 0){
        Crear_Varias_Carpetas2(path_name,name_path,pos_inodo,pos_bloque,inicio_sb,sin_par,size_file);
        // printf("\nExiste el ID");
       //}
    }else{
          printf("\nNo existe el ID");
    }
}

void Crear_Varias_Carpetas2(char* direccion, char* path_crear, int pos_inodo, int pos_bloque, int inicio_sb,int sin_par,int size_file){

     int co = 0,co2;
     char cadena[500];

     if(sin_par == 0){
        strcpy(cadena,substr(path_crear,1,strlen(path_crear)-2));
     }else{
          strcpy(cadena,substr(path_crear,1,strlen(path_crear)));
     }

     char** tokens;
     tokens = str_split(cadena, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co == 1){
        Crear_Carpetas2(direccion,*(tokens),pos_inodo,pos_bloque,inicio_sb,size_file);
     }else if(co > 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {
                      if(i == 0){
                         existe_carpeta = 0;
                      }

                      //printf("\nfds %s",*(tokens+i));
                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);
                         //printf("\nDireccion %s %d %d",*(tokens+i),existe_carpeta,inodo_ids);
                       //  existe_carpeta = 0;
                      }
                      else if(existe_carpeta == 1 && i>0){

                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                        // printf("\n\nDirectorio %s %d %d %d\n\n",*(tokens + i),existe_carpeta,i,co);
                         if(sin_par == 1){

                            if(existe_carpeta == 0 && (i+1)!=co){
                    //           printf("\nINODOS %d",inodo_ids);
                               Crear_Carpetas(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n|Carpeta %s Creada en la ruta %s %d",*(tokens+i),path_crear,i);
                               printf("\n'------------------------------------------------------------------------'");
                               Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);

                            }
                         }

                      }else if(existe_carpeta == 0 && i>0 && i<co){

                               if(sin_par == 0){
                                   printf("\n.------------------------------------------------------------------------.");
                                   printf("\n No existe el Directorio para Crear el archivo %s %d",*(tokens+i),strlen(*(tokens+i)));
                                   printf("\n'------------------------------------------------------------------------'");
                                   break;
                               }else{
                                     //Crear_Carpetas(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);
                      //               printf("\n\nCarpeta a crear %s %d\n\n",*(tokens + i),existe_carpeta);
                               }
                      }
                      //printf("\nINODO %d",inodo_ids);
                      if(existe_carpeta == 0 && (i+1) == co){

                          if(strcmp(*(tokens+i),"users.txt") == 0){
                         //    printf("\nCIERTO");
                             Archivo_User(direccion,"users.txt",pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb);

                          }else{
                              // printf("\nFALSO");
                          //   Archivo_User(direccion,"users.txt",pos_inodo,pos_bloque,inicio_sb);
                         // }else{
                              Crear_Carpetas2(direccion,*(tokens+i),pos_in,Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_ids),inicio_sb,size_file);
                              printf("\n.------------------------------------------------------------------------.");
                              printf("\n|Archivo Creado %s en la ruta %s",*(tokens+i),path_crear);
                              printf("\n'------------------------------------------------------------------------'");
                          //printf("\n\nCarpeta a crear %s %d %d %d\n\n",*(tokens + i),existe_carpeta,i,co);
                          }
                          break;

                      }

                 }

              }

     }
   //LeerBloque(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb));
   //  Leer_Inodo(direccion,pos_inodo,Obtener_Numero_Inodos(direccion,inicio_sb));
}

void Crear_Carpetas2(char* direccion,char* name_dir,int pos_inodo_raiz,int pos_bloque_raiz,int inicio_superbloque,int size_file){

     Tabla_Inodos in;
     FILE* fp = fopen(direccion,"rb+");
     char archivo[10];
     strcpy(archivo,"0123456789");

     int com = 0;

     if(fp!=NULL){
        fseek(fp,pos_inodo_raiz,SEEK_SET);
        fread(&in,sizeof(Tabla_Inodos),1,fp);
        int espacio;
        int i = 0;
        for(i = 0; i<15; i++){

            if(in.i_block[i] !=-1){
               int pos_raiz_bloque = Obtener_Inicio_Bloques(direccion,inicio_superbloque);
               int numero_bloq = Obtener_Numero_Bloques(direccion,inicio_superbloque);
               int pos_bloques = Obtener_PosBloqueID(direccion,pos_raiz_bloque,numero_bloq,in.i_block[i]);
               espacio = Agregar_Carpeta_BloqueExisente(direccion,name_dir,pos_bloques,inicio_superbloque);
               //printf("\nEspacio %d %d %d",espacio, pos_bloques, in.i_block[i]);
               if(espacio == 0){//si hay espacio
                  int pos_raiz_inodo = Obtener_Inicio_Inodos(direccion,inicio_superbloque);
                  int numero_ino = Obtener_Numero_Inodos(direccion,inicio_superbloque);
                  int ultimo_inodo = Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino);
                  int ultimo_bloque = Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq);


                  float division = 0;
                  int bloques = 0;

                  division = size_file/64;
                   //printf("\nContenido: %s",conte);
                  if(division<=0){
                     bloques = 1;
                     //printf("\nCierto %f",division);
                  }else if(division == 1){
                           bloques = 2;
                  }else if(division>1){
                           bloques = division;
                           //printf("\nCierto2 %d",bloques);
                  }else{
                        //printf("\nCierto3 %d",bloques);
                  }
                  int p = 0;
                  int mayor = 0;

                  int par = 0;
                  if(size_file <= 64){
                     par = size_file;
                  }else{
                        par = 64;
                  }
                  int par2 = size_file,par3 = 0;


                  for(p = 0;p <bloques;p++){

                           char conte[par];
                           int k = 0;
                           int indice = -1;

                           if(par>0 && par<=64){
                               for(k = 0;k < par; k++){

                                       indice++;
                                       if(indice>=0 && indice<=9){
                                          conte[k] = archivo[indice];

                                       }
                                       if(indice == 9){
                                          indice = -1;
                                       }


                               }
                              // printf("\nConte %s %d",conte,par);
                               Crear_Bloque_Archivos(direccion,Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq),name_dir,inicio_superbloque,conte);
                               Modificar_Inodo_Archivo(direccion,Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino)-sizeof(Tabla_Inodos),inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));

                               if(par>0 && par<=64){
                                  par2= par2-64;
                                  par = par2;
                                  if(mayor == 1){
                                     par = par3 -64;
                                    // printf("\nss %d",par);
                                     p = p-1;
                                     mayor  = 0;
                                  }
                               }
                           }else if(par>64){
                                // printf("\nFds %d %d",p,par);
                                 par3 = par;
                                 par = 64;
                                 mayor = 1;
                                 p = 0;
                           }

                      if(p == 0 && com == 0){
                         //Crear_Bloque_Archivos(direccion,Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq),name_dir,inicio_superbloque,conte);
                         Crear_Nuevo_Inodo2(direccion,ultimo_inodo,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque)-1,size_file);
                      }else{
                           // Modificar_Inodo_Archivo(direccion,ultimo_inodo,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
                            //Crear_Bloque_Archivos(direccion,Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq),name_dir,inicio_superbloque,conte);
                      }
                      com++;
                  }

                  break;
               }
               //break;
            }

        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Crear_Nuevo_Inodo2(char*direccion,int pos_inodo_nuevo,int inicio_superbloque,int apuntador_bloque,int size_file){

     Tabla_Inodos in;
     in.id = Obtener_InodoLibre(direccion,inicio_superbloque);
     in.i_uid = 1;
     in.i_gid = 1;
     in.i_size = size_file;
     time_t t;
     struct tm *tm;
     char fechayhora[100];
     t=time(NULL);
     tm=localtime(&t);
     strftime(fechayhora, 100, "%d/%m/%Y %H:%M:%S", tm);
     strcpy(in.i_atime,fechayhora);
     strcpy(in.i_ctime,fechayhora);
     strcpy(in.i_mtime,fechayhora);

     int i = 0;
     for(i = 0;i<15;i++){
         if(i == 0){
           in.i_block[i] = apuntador_bloque;
         }else{
               in.i_block[i] = -1;
         }
     }
     in.i_type[0] = '1';
     in.i_perm = 777;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,pos_inodo_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Tabla_Inodos),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_InodoLibre_SB(direccion,inicio_superbloque,in.id);
     Modificar_BM_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,inicio_superbloque),in.id,'1',inicio_superbloque);
}
void Modificar_Inodo_Archivo(char*direccion,int pos_inodo_nuevo,int inicio_superbloque,int apuntador_bloque){

     Tabla_Inodos in;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        fseek(fp,pos_inodo_nuevo,SEEK_SET);
        fread(&in,sizeof(Tabla_Inodos),1,fp);

        int i = 0;
        for(i = 0;i<15;i++){

            if(in.i_block[i] == -1){
               in.i_block[i] = apuntador_bloque;
               break;
            }
        }
        fseek(fp,pos_inodo_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Tabla_Inodos),1,fp);


        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_InodoLibre_SB(direccion,inicio_superbloque,in.id);
     Modificar_BM_Inodos(direccion,Obtener_Inicio_BMInodos(direccion,inicio_superbloque),in.id,'1',inicio_superbloque);
}
void Crear_Bloque_Archivos(char*direccion,int pos_Bloque_nuevo,char* name,int inicio_superbloque,char archivo[]){
     Bloques_Archivo in;

     strcpy(in.b_content,archivo);

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        fseek(fp,pos_Bloque_nuevo,SEEK_SET);
        fwrite(&in,sizeof(Bloques_Archivo),1,fp);

        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
     Modificar_BloqueLibre_SB(direccion,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
     Modificar_BM_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,inicio_superbloque),Obtener_BoqueLibre(direccion,inicio_superbloque)-1,'1',inicio_superbloque);
}

void Buscar_ID_Eliminar(char* id,char* name_path){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
       Eliminar_Carpeta_Archivos(path_name,name_path,pos_inodo,pos_bloque,inicio_sb);
    }else{
          printf("\nNo existe el ID");
    }
}


void Eliminar_Carpeta_Archivos(char* direccion, char* path_eliminar, int pos_inodo, int pos_bloque, int inicio_sb){

     int co = 0,co2;
     char cadena[500];

     strcpy(cadena,substr(path_eliminar,1,strlen(path_eliminar)-2));

     char** tokens;
     tokens = str_split(cadena, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co == 1){
        //Crear_Carpetas(direccion,*(tokens),pos_inodo,pos_bloque,inicio_sb);
     }if(co >= 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {
                      if(i == 0){
                         existe_carpeta = 0;
                      }

                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);

                      }
                      else if(existe_carpeta == 1 && i>0){

                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                         //printf("\nDireccion2 %s %d %d %d",*(tokens + i),existe_carpeta,bloque_eliminar,inodo_ids);

                      }else if(existe_carpeta == 0 && i>0 && i<co){
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n No existe el Directorio %s",*(tokens+i));
                               printf("\n'------------------------------------------------------------------------'");
                               break;
                      }
                      if(existe_carpeta == 1 && (i+1) == co){
                         Eliminar_CarpetaArchivo(direccion,*(tokens+i),Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_eliminar),inicio_sb,bloque_eliminar,0);
                      }

                 }

              }
     }
   //LeerBloque(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb));
   //  Leer_Inodo(direccion,pos_inodo,Obtener_Numero_Inodos(direccion,inicio_sb));
}
void Eliminar_CarpetaArchivo(char* direccion,char* name_path,int pos_bloque_raiz,int inicio_superbloque,int numero_bloque, int tipo){
     Bloques_Carpeta in;

     FILE* fp = fopen(direccion,"rb+");

     int existe = 0;
     int contadors = 0;
     if(fp!=NULL){
        fseek(fp,pos_bloque_raiz,SEEK_SET);
        fread(&in,sizeof(Bloques_Carpeta),1,fp);
        int i=0;
        for(i=0;i<4;i++){
            if(in.carpetas[i].b_inodo != -1){
             //  printf("\nENTRA");
               if(strcmp(in.carpetas[i].b_name,name_path) == 0){
                  strcpy(in.carpetas[i].b_name,"-1");
                  in.carpetas[i].b_inodo = -1;
                  fseek(fp,pos_bloque_raiz,SEEK_SET);
                  fwrite(&in,sizeof(Bloques_Carpeta),1,fp);
                  existe = 1;
                          //printf("\nEntra %s %d",name_path,existe);

                  break;
               }
               if(in.carpetas[i].b_inodo == -1){
                  contadors++;
               }

            }
        }

        if(existe == 0){
           printf("\n.---------------------------------------.\n");
           printf("\nNo Existe %s",name_path);
           printf("\n'---------------------------------------'\n");
        }else{

              if(tipo == 0){
                  printf("\n.---------------------------------------.\n");
                  printf("\nDirectorio Eliminado %s  %d",name_path,contadors);
                  printf("\n'---------------------------------------'\n");
              }

              if(contadors == 2){
                // Modificar_BloqueLibre_SB(direccion,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
                 //Modificar_BM_Bloques(direccion,Obtener_Inicio_BMBloques(direccion,inicio_superbloque),numero_bloque,'0',inicio_superbloque);
              }
        }

        fclose(fp);
        //LeerBloque(direccion,pos_bloque_raiz,);
     }else{
           printf("\nNo existe la ruta");
     }

}

void Buscar_ID_Renombrar(char* id,char* name_path,char* path_renombrar){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
       Renombrar_Carpeta_Archivos(path_name,name_path,pos_inodo,pos_bloque,inicio_sb,path_renombrar);
    }else{
          printf("\nNo existe el ID");
    }
}

void Renombrar_Carpeta_Archivos(char* direccion, char* path_renombrar, int pos_inodo, int pos_bloque, int inicio_sb,char* path_new){

     int co = 0,co2;
     char cadena[500];

     strcpy(cadena,substr(path_renombrar,1,strlen(path_renombrar)-2));

     char** tokens;
     tokens = str_split(cadena, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co == 1){
        //Crear_Carpetas(direccion,*(tokens),pos_inodo,pos_bloque,inicio_sb);
     }if(co >= 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {
                      if(i == 0){
                         existe_carpeta = 0;
                      }

                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);

                      }
                      else if(existe_carpeta == 1 && i>0){

                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                         //printf("\nDireccion2 %s %d %d %d",*(tokens + i),existe_carpeta,bloque_eliminar,inodo_ids);

                      }else if(existe_carpeta == 0 && i>0 && i<co){
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n No existe el Directorio %s",*(tokens+i));
                               printf("\n'------------------------------------------------------------------------'");
                               break;
                      }
                      if(existe_carpeta == 1 && (i+1) == co){
                         Renombrar_CarpetaArchivo(direccion,*(tokens+i),Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_eliminar),inicio_sb,path_new);
                      }

                 }

              }
     }

}
void Renombrar_CarpetaArchivo(char* direccion,char* name_path,int pos_bloque_raiz,int inicio_superbloque,char* new_path){
     Bloques_Carpeta in;

     FILE* fp = fopen(direccion,"rb+");

     int existe = 0;

     if(fp!=NULL){
        fseek(fp,pos_bloque_raiz,SEEK_SET);
        fread(&in,sizeof(Bloques_Carpeta),1,fp);
        int i=0;
        for(i=0;i<4;i++){
            if(in.carpetas[i].b_inodo != -1){
             //  printf("\nENTRA");
               if(strcmp(in.carpetas[i].b_name,name_path) == 0){
                  strcpy(in.carpetas[i].b_name,new_path);
                  //in.carpetas[i].b_inodo = -1;
                  fseek(fp,pos_bloque_raiz,SEEK_SET);
                  fwrite(&in,sizeof(Bloques_Carpeta),1,fp);
                  existe = 1;
                          //printf("\nEntra %s %d",name_path,existe);

                  break;
               }

            }
        }

        if(existe == 0){
           printf("\n.---------------------------------------.\n");
           printf("\nNo Existe %s",name_path);
           printf("\n'---------------------------------------'\n");
        }else{
              printf("\n.---------------------------------------.\n");
              printf("\nDirectorio %s Renombrado como %s",name_path,new_path);
              printf("\n'---------------------------------------'\n");
        }

        fclose(fp);
        //LeerBloque(direccion,pos_bloque_raiz,);
     }else{
           printf("\nNo existe la ruta");
     }

}

void Buscar_ID_Mover(char* id,char* path_mover,char* path_destino){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
       Mover_Carpeta_Archivos(path_name,path_mover,pos_inodo,pos_bloque,inicio_sb,path_destino);
    }else{
          printf("\nNo existe el ID");
    }
}

void Mover_Carpeta_Archivos(char* direccion, char* path_mover, int pos_inodo, int pos_bloque, int inicio_sb,char* path_new){

     int co = 0,co2 = 0;
     char cadena[500];
     char cadena2[500];

     memset(cadena2,'\0',500);
     strcpy(cadena,substr(path_mover,1,strlen(path_mover)));
     strcpy(cadena2,substr(path_new,1,strlen(path_new)-2));

     char** tokens;
     char** tokens2;

     tokens = str_split(cadena, '/');
     tokens2 = str_split(cadena2, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co >= 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {
                      if(i == 0){
                         existe_carpeta = 0;
                      }

                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);
                      }
                      else if(existe_carpeta == 1 && i>0){
                             Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                             //printf("\nExiste %s %d %d",*(tokens+i),existe_carpeta,inodo_ids);

                      }else if(existe_carpeta == 0 && i>0 && i<co){
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n No existe el Directorio %s",*(tokens+i));
                               printf("\n'------------------------------------------------------------------------'");
                               break;
                      }
                      if(existe_carpeta == 1 && (i+1) == co){
                       //  printf("\nExiste2 %s %d %d %d",*(tokens+i),existe_carpeta,inodo_ids,bloque_eliminar);
                         inodo_ids2 = inodo_ids;
                         Eliminar_CarpetaArchivo(direccion,*(tokens+i),Obtener_PosBloqueID(direccion,pos_bloque,Obtener_Numero_Bloques(direccion,inicio_sb),bloque_eliminar),inicio_sb,bloque_eliminar,1);
                         id_inodo_mv = 1;
                         char destino[1500];
                         strcpy(cadena2,substr(path_new,1,strlen(path_new)-2));

                         sprintf(destino,"/%s/%s ",cadena2,*(tokens+i));

                   //      printf("\nPrueba %s %s",destino,cadena2);
                         Crear_Varias_Carpetas(direccion,destino,pos_inodo,pos_bloque,inicio_sb,0);
                         id_inodo_mv = 0;
                      }
                 }
              }
     }
}

void Buscar_ID_find(char* id,char* name_path,char* name_dir){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){
           //printf("\nParticion existente %s %s",id,type);
           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);
           //Crear_Carpetas(particiones_montadas[i].path,"home",pos_inodo,pos_bloque,inicio_sb);
           //printf("\nID existente %s %d",particiones_montadas[i].path,inicio_sb);
           existe = 1;
           break;
        }
    }

    if(existe == 1){
       find_Carpeta_Archivos(path_name,name_path,pos_inodo,pos_bloque,inicio_sb);
    }else{
          printf("\nNo existe el ID");
    }
}

void find_Carpeta_Archivos(char* direccion, char* path_find, int pos_inodo, int pos_bloque, int inicio_sb){
     Tabla_Inodos in;
     int co = 0,co2 = 0;
     char cadena[500];

     FILE* fp = fopen(direccion,"rb+");

     if(path_find[1] == '/'){
        printf("\nARBOL ACTUAL");
        Buscar_Subdirectorios(direccion,pos_bloque,pos_inodo,0,inicio_sb,0);

     }else{

     strcpy(cadena,substr(path_find,1,strlen(path_find)));

     char** tokens;

     tokens = str_split(cadena, '/');

     if (tokens)
     {
         int i;
         for (i = 0; *(tokens + i); i++)
         {
              co++;
         }
     }
     if(co >= 1){
              if(tokens)
              {
                 int i;
                 for (i = 0; *(tokens + i); i++)
                 {

                      if(i == 0){
                         existe_carpeta = 0;
                      }
                      if(existe_carpeta == 0 && i== 0){
                         Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,0);
                         //printf("\nExistem %s %d %d %d",*(tokens+i),existe_carpeta,inodo_ids,strlen(*(tokens+i)));
                      }
                      else if(existe_carpeta == 1 && i>0){

                             Crear_Subdirectorio(direccion,*(tokens+i),pos_inodo,pos_bloque,inicio_sb,0,inodo_ids);
                            // printf("\nExistem2 %s %d %d %d",*(tokens+i),existe_carpeta,inodo_ids,strlen(*(tokens+i)));


                      }if(existe_carpeta == 0 && i>0 && i<co){
                               printf("\n.------------------------------------------------------------------------.");
                               printf("\n No existe el Directorio %s",*(tokens+i));
                               printf("\n'------------------------------------------------------------------------'");
                               break;
                      }
                      if(existe_carpeta == 1 && (i+1) == co){

                        // printf("\nExiste2 %s %d %d %d",*(tokens+i),existe_carpeta,inodo_ids,bloque_eliminar);
                         inodo_ids2 = inodo_ids;
                         printf("\nARBOL ACTUAL");
                         Buscar_Subdirectorios(direccion,pos_bloque,pos_inodo,inodo_ids2,inicio_sb,0);

                      }

              }
          }
    }
   }
}
char dirTabuladas[500];
int contador_inodo0 = 0;
void Buscar_Subdirectorios(char* direccion,int pos_bloque,int pos_inodo,int id_inodos,int inicio_sb,int posicion_dir){

     Tabla_Inodos in;

     int inicio = pos_inodo;
     int fin = inicio + sizeof(Tabla_Inodos)*Obtener_Numero_Inodos(direccion,inicio_sb);

     FILE* fp;

     fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        while(inicio < fin){

             fseek(fp,inicio,SEEK_SET);
             fread(&in,sizeof(Tabla_Inodos),1,fp);

             //printf("\nIdinodos %d ",in.id);
             if(in.id == id_inodos){

                if(in.id == 0){
                   contador_inodo0++;
                }


                int i = 0;
                for(i = 0;i<15;i++){

                    if(in.i_block[i]!=-1 && in.i_type[0] !='1'){
                       //printf(" Bloque %d ",in.i_block[i]);
                       Buscar_Bloques_Subdir(direccion,pos_bloque,pos_inodo,in.i_block[i],inicio_sb,posicion_dir);
                     //  memset(dirTabuladas,'\0',500);
                  //    break;
                    }else{

                          memset(dirTabuladas,'\0',500);
                          break;
                    }

                    if(i == 14){
                       break;
                    }
                }

             }
             if(contador_inodo0>1){
                break;
             }

             inicio += sizeof(Tabla_Inodos);
        }

        fclose(fp);
     }else{

           printf("\nNo existe ruta Indos");
     }
}
void Buscar_Bloques_Subdir(char* direccion,int pos_bloque,int pos_inodo,int id_bloque,int inicio_sb,int posicion_dir){

     Bloques_Carpeta in;

     int inicio = pos_bloque;
     int fin = inicio + sizeof(Bloques_Carpeta)*Obtener_Numero_Bloques(direccion,inicio_sb);
     int contador = 0;
     int existen = 0;
     char names[100],names2[100],names3[100],names4[100];
     char dir[100],dir2[100],dir3[100],dir4[100];
     int ino = 0,ino2 = 0,ino3 = 0,ino4 = 0;

     strcpy(names,"false");
     strcpy(names2,"false");
     strcpy(names3,"false");
     strcpy(names4,"false");

     FILE* fp;

     fp = fopen(direccion,"rb+");

     if(fp!=NULL){

        while(inicio < fin){

             fseek(fp,inicio,SEEK_SET);
             fread(&in,sizeof(Bloques_Carpeta),1,fp);

             if(contador == id_bloque){

               if(in.carpetas[0].b_inodo >= 0 && in.carpetas[0].b_inodo != 5175 && in.carpetas[0].b_inodo!=892613426 && in.carpetas[0].b_inodo!=858927408 && in.carpetas[0].b_inodo != 1869574700
                || in.carpetas[0].b_inodo == -1 && in.carpetas[0].b_inodo != 5175 && in.carpetas[0].b_inodo!=892613426 && in.carpetas[0].b_inodo!=858927408 && in.carpetas[0].b_inodo != 1869574700){

                int i=0;
                for(i=0;i<4;i++){

                    if(in.carpetas[i].b_name[0] != '.' && in.carpetas[i].b_inodo != -1){

                        if(contador_inodo0>1){
                           break;
                        }
                       //strcat(dirTabuladas,in.carpetas[i].b_name);
                            strcat(dirTabuladas,"|   ");
                            char pr[500];
                            sprintf(pr,"%s%s",dirTabuladas,in.carpetas[i].b_name);
    //                       printf("\n%s",in.carpetas[i].b_name);
                            printf("\n%s",pr);
                            Buscar_Subdirectorios(direccion,pos_bloque,pos_inodo,in.carpetas[i].b_inodo,inicio_sb,posicion_dir);
                            memset(dirTabuladas,'\0',500);

                       //break;
                    }

                }
                }
                break;
             }
             inicio += sizeof(Bloques_Carpeta);
             contador++;
          }
        fclose(fp);
     }else{

           printf("\nNo existe ruta bloq");
     }


}

void Archivo_User(char* direccion,char* name_dir,int pos_inodo_raiz,int pos_bloque_raiz,int inicio_superbloque){

     Tabla_Inodos in;
     FILE* fp = fopen(direccion,"rb+");

     char usuarios[64];
     strcpy(usuarios,"1,G,root\n1,U,root,root,123\n");

     if(fp!=NULL){
        fseek(fp,pos_inodo_raiz,SEEK_SET);
        fread(&in,sizeof(Tabla_Inodos),1,fp);
        int espacio;
        int i = 0;
        for(i = 0; i<15; i++){

            if(in.i_block[i] !=-1){
               int pos_raiz_bloque = Obtener_Inicio_Bloques(direccion,inicio_superbloque);
               int numero_bloq = Obtener_Numero_Bloques(direccion,inicio_superbloque);
               int pos_bloques = Obtener_PosBloqueID(direccion,pos_raiz_bloque,numero_bloq,in.i_block[i]);
               espacio = Agregar_Carpeta_BloqueExisente(direccion,name_dir,pos_bloques,inicio_superbloque);
               //printf("\nEspacio %d %d %d",espacio, pos_bloques, in.i_block[i]);
               if(espacio == 0){//si hay espacio
                  int pos_raiz_inodo = Obtener_Inicio_Inodos(direccion,inicio_superbloque);
                  int numero_ino = Obtener_Numero_Inodos(direccion,inicio_superbloque);
                  int ultimo_inodo = Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino);
                  int ultimo_bloque = Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq);


                  Crear_Bloque_Archivos(direccion,Ultimo_Bloque_Creado(direccion,pos_raiz_bloque,numero_bloq),name_dir,inicio_superbloque,usuarios);
                 // Modificar_Inodo_Archivo(direccion,Ultimo_Inodo_Creado(direccion,pos_raiz_inodo,numero_ino)-sizeof(Tabla_Inodos),inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque));
                  Crear_Nuevo_Inodo2(direccion,ultimo_inodo,inicio_superbloque,Obtener_BoqueLibre(direccion,inicio_superbloque)-1,strlen(usuarios));

                  Crear_Archivo_UserFisico(id);
                  Mod_Archivo_UserFisico(id,1,'U',"root","root","123");

                 // Leer_Usuarios(id);

                 // printf("\n%d",ultimo_usuarios(id));

                //  Agregar_Usuario(direccion,"2,root,freddy,2345",ultimo_inodo,ultimo_bloque,inicio_superbloque);
                //  Agregar_Usuario(direccion,"2,root,Alejandro,2345",ultimo_inodo,ultimo_bloque,inicio_superbloque);


                }
            }

        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Crear_Archivo_UserFisico(char* id){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int pos = 0;
     int fin = 100*sizeof(Users);

     FILE* fp = fopen(direccion,"w");

     if(fp!=NULL){
       while(pos<fin){

           if(pos == 0){
              us.gid_uid = 1;
              us.tipo[0] = 'G';
              strcpy(us.grupo,"root");
              strcpy(us.usuario,"-1");
              strcpy(us.contrasenia,"-1");

              fseek(fp,pos,SEEK_SET);
              fwrite(&us,sizeof(Users),1,fp);
           }else{
                 us.gid_uid = -1;
                 us.tipo[0] = '-1';
                 strcpy(us.grupo,"-1");
                 strcpy(us.usuario,"-1");
                 strcpy(us.contrasenia,"-1");

                 fseek(fp,pos,SEEK_SET);
                 fwrite(&us,sizeof(Users),1,fp);
           }
           pos+=sizeof(Users);
       }
       fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Mod_Archivo_UserFisico(char* id,int gid,char* tipo,char* grupo,char* usuario,char* contrasenia){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     us.gid_uid = gid;
     us.tipo[0] = tipo;
     strcpy(us.grupo,grupo);
     strcpy(us.usuario,usuario);
     strcpy(us.contrasenia,contrasenia);

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){

       fseek(fp,ultimo_usuarios(id)*sizeof(Users),SEEK_SET);
       fwrite(&us,sizeof(Users),1,fp);

       fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
int ultimo_id_Grupo(char* id){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(us.tipo[0] == 'G'){
               ids = us.gid_uid+1;
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
       return ids;
     }else{
           printf("\nNo existe la ruta");
     }
}

int Existe_Grupo(char* id,char* name_grp){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;
     int existe = 0;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(us.tipo[0] == 'G' && strcasecmp(us.grupo,name_grp) == 0){
               existe = 1;
               break;
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
       return existe;
     }else{
           printf("\nNo existe la ruta");
     }
}

int Existe_Usuario(char* id,char* name_usr){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;
     int existe = 0;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(us.tipo[0] == 'U' && strcasecmp(us.usuario,name_usr) == 0){
               existe = 1;
               break;
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
       return existe;
     }else{
           printf("\nNo existe la ruta");
     }
}

void Leer_Usuarios(char* id){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

               printf("\n%c %d %s %s %s",us.tipo[0],us.gid_uid,us.grupo,us.usuario,us.contrasenia);

            if(us.gid_uid>0){
              // printf("\n%c",us.tipo[0]);
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
}


int ultimo_id_user(char* id){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(us.tipo[0] == 'U'){
               ids = us.gid_uid+1;
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
       return ids;
     }else{
           printf("\nNo existe la ruta");
     }
}

int ultimo_usuarios(char* id){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,id);
     strcat(direccion,".dsk");

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int pos = -1;
     int contador = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(us.tipo[0] == 'G' || us.tipo[0] == 'U'){

              // pos = posicion+sizeof(Users);
               //printf("\n%c",us.tipo[0]);
               contador++;

            }

            posicion+=sizeof(Users);
       }
       return contador;

       fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }
}


void Buscar_ID_AgregarGrupo(char* id,char* name_group){

      int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){

           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);

           existe = 1;
           break;
        }
    }

    if(existe == 1){
       if(Existe_Grupo(id,name_group) == 0){
           char ca[64];
           sprintf(ca,"%d,G,%s\n",ultimo_id_Grupo(id),name_group);
           Agregar_Usuario(path_name,ca,Obtener_PosInodoID(path_name,Obtener_Inicio_Inodos(path_name,inicio_sb),Obtener_Numero_Inodos(path_name,inicio_sb),3),pos_bloque,inicio_sb);
           Mod_Archivo_UserFisico(id,ultimo_id_Grupo(id),'G',name_group,"-1","-1");
           Leer_Usuarios(id);
        }else{
              printf("\nYa existe el grupo");
        }


    }else{
          printf("\nNo existe el ID");
    }
}

void Buscar_ID_AgregarUsuario(char* id,char* usr,char* pwd,char* grp){

    int i = 0;
    int inicio_sb = 0;
    int pos_inodo = 0;
    int pos_bloque = 0;
    char path_name[50];

    int existe = 0;
    for(i = 0; i < 100; i++){

        if(strcasecmp(particiones_montadas[i].id,id) == 0){

           inicio_sb = Obtener_SuperBloque(particiones_montadas[i].path,particiones_montadas[i].name);
           pos_inodo = Obtener_Inicio_Inodos(particiones_montadas[i].path,inicio_sb);
           pos_bloque = Obtener_Inicio_Bloques(particiones_montadas[i].path,inicio_sb);
           strcpy(path_name,particiones_montadas[i].path);

           existe = 1;
           break;
        }
    }

    if(existe == 1){

       if(Existe_Grupo(id,grp) == 1){

           if(Existe_Usuario(id,usr) == 0){
               char ca[64];
               sprintf(ca,"%d,U,%s,%s,%s\n",ultimo_id_user(id),grp,usr,pwd);
               Agregar_Usuario(path_name,ca,Obtener_PosInodoID(path_name,Obtener_Inicio_Inodos(path_name,inicio_sb),Obtener_Numero_Inodos(path_name,inicio_sb),3),pos_bloque,inicio_sb);
               Mod_Archivo_UserFisico(id,ultimo_id_user(id),'U',grp,usr,pwd);
               Leer_Usuarios(id);
           }else{
                 printf("\nYa existe este usuario %s",usr);
           }
       }else{
             printf("\nNo existe el grupo %s",grp);
       }

    }else{
          printf("\nNo existe el ID");
    }
}

int Obtener_PosInodoID(char* direccion, int posicion,int numero_inodos,int id_inodo){

     Tabla_Inodos in;
     int contador = 0;
     int inicio = posicion;
     int fin = inicio + sizeof(Tabla_Inodos)*numero_inodos;
     int posicion_bloque = 0;
     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
        while(inicio < fin){
              fseek(fp,inicio,SEEK_SET);
              fread(&in,sizeof(Tabla_Inodos),1,fp);

              if(contador == id_inodo){
                 posicion_bloque = inicio;
                 break;
              }
              contador++;
              inicio += sizeof(Bloques_Carpeta);
        }
        fclose(fp);
        return posicion_bloque;
     }else{
           printf("\nNo existe la ruta");
     }
}




void Agregar_Usuario(char* direccion,char* contenido,int pos_inodo_raiz,int pos_bloque_raiz,int inicio_superbloque){

     Tabla_Inodos in;
     FILE* fp = fopen(direccion,"rb+");

     char usuarios[64];
     strcpy(usuarios,contenido);

     if(fp!=NULL){
        fseek(fp,pos_inodo_raiz,SEEK_SET);
        fread(&in,sizeof(Tabla_Inodos),1,fp);
        int espacio;
        int i = 0;
        for(i = 0; i<15; i++){

            if(in.i_block[i] == -1){

               in.i_block[i] = Obtener_BoqueLibre(direccion,inicio_superbloque);
               fseek(fp,pos_inodo_raiz,SEEK_SET);
               fwrite(&in,sizeof(Tabla_Inodos),1,fp);

               Crear_Bloque_Archivos(direccion,Ultimo_Bloque_Creado(direccion,Obtener_Inicio_Bloques(direccion,inicio_superbloque),Obtener_Numero_Bloques(direccion,inicio_superbloque)),"users.txt",inicio_superbloque,usuarios);
               break;
            }

        }
        fclose(fp);
     }else{
           printf("\nNo existe la ruta");
     }

}
void Loguear_usuario(char* usrs,char* pwds){

  DIR *dir;
  struct dirent *ent;
  int existe = 0;
  dir = opendir ("/home/freddy/Personal");

  if (dir == NULL)
     error("No puedo abrir el directorio");

  while((ent = readdir (dir)) != NULL)
  {
      if((strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) )
      {
         // printf("\nArchivos %s",ent->d_name);

          if(Buscar_Usuario(ent->d_name,usrs,pwds) == 1){
             printf("\nUSUARIO LOGUEADO");
             existe = 1;
             break;
          }

      }
  }

 // printf("\n %d %c %s %s %s",login.gid_uid,login.tipo[0],login.grupo,login.usuario,login.contrasenia);
  if(existe == 0){
     printf("\nNo existe usuario");
  }
  closedir (dir);

}

int Buscar_Usuario(char* name_file,char* usrs,char* pwds){

     Users us;

     char direccion[500];
     strcpy(direccion,"/home/freddy/Personal/");
     strcat(direccion,name_file);

     int posicion = 0;
     int fin = 100*sizeof(Users);
     int ids = -1;
     int existe = 0;

     FILE* fp = fopen(direccion,"rb+");

     if(fp!=NULL){
       while(posicion<fin){
            fseek(fp,posicion,SEEK_SET);
            fread(&us,sizeof(Users),1,fp);

            if(strcasecmp(us.usuario,usrs) == 0 && strcmp(us.contrasenia,pwds) == 0){
               existe = 1;
               login.tipo[0] = us.tipo[0];
               login.gid_uid = us.gid_uid;
               strcpy(login.grupo,us.grupo);
               strcpy(login.usuario,us.usuario);
               strcpy(login.contrasenia,us.contrasenia);
               break;
            }

            posicion+=sizeof(Users);
       }

       fclose(fp);
       return existe;
     }else{
           printf("\nNo existe la ruta");
     }
}
