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
    //inicializar_sistema();
    /* Mensaje de bienvenida */
    mensaje_inicio();
    /* Iniciar analisis de entradas */
    analizar_comandos();
}

int main(void){

    srand(time(NULL));
    printf("\033[%dm",CYAN);printf("\033[0m");
    /*
     * Ejecutar programa */
    menu_principal();


    return 0;
}


