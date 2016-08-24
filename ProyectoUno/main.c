#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


/****************************************++-
 *                                          *--------------+-
 *      FACULTAD DE  INGENIERAIA USAC       *---------+-
 *      Primera proyecto - MIA              *------+-
 *      Nombre: Marco Aledander Mateo      *-------------+-
 *      Carnet: 200917702 Sección: A      *-------------------+-
 *      Fase: PRIMERA FASE PROYECTO       *----------------------------+-
 *                                         *-------+-
 * ****************************************/

//void inicializar_sistema();
void mensaje_inicio();
void analizar_comandos();


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::    MENU PRINCIPAL
/*
 * Menu principal de la aplicaicón */
void menu_principal(){

    /* Inicializar sistema */
    inicializar_sistema();
    /* Mensaje de bienvenida */
    mensaje_inicio();
    /* Iniciar analisis de entradas */
    analizar_comandos();
}


//Entra: 100100011111010101110001
int posicion_bloque(int start, int size, char *fit, char bitmap[]){
    int res = 0, pos = 0, temp = 0, temp2 = 0;
    for(inde=0; inde < sizeof(bitmap); inde++){
        if(bitmap[inde] != '1'){
            pos++;
        }
    }

    if(strcasecmp(fit,BF)==0){      // Primer ajuste

    }else if(strcasecmp(fit,FF)==0){// Mejor ajuste

    }else if(strcasecmp(fit,WF)==0){// Peor ajuste

    }
    pos = (sizeof(char) - temp )/8;
    return pos;
}


int main(void){

    srand(time(NULL));
    printf("\033[%dmAlexander\n",30);printf("\033[0m");
    /*
     * Ejecutar programa */
    menu_principal();

    //system("\"dot -Tpng /tmp/proyecto/grafo_disc.dot -o /home/archivos/reporte.png\"");

    printf("char: %c, int: %d\n",'0','0');

    return 0;
}




