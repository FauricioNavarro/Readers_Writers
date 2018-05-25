/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: stickman
 *
 * Created on May 25, 2018, 1:16 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "definiciones.h"
#include "espia.h"

#define MAX_LINEA_BIT 45

/*
 * 
 */
int main(int argc, char** argv) {
    key_t key = ftok("shmfile", 21);
    int shmid = shmget(key, 1, 0666 | IPC_CREAT);
    Mem_comp *mem = shmat(shmid, NULL, 0);

    string param = argv[1];
    if (argc == 2) {
        if (strcmp(param, "mem") == 0)
            mostrar_shm(mem);
        else if (espionaje_solic(param))
            espiar(param);
    }
    else
        perror("Uso: espia [mem | Writer | Reader | \"Reader egoista\"]");

    return (EXIT_SUCCESS);
}

void mostrar_shm(Mem_comp *mem) {
    printf("=====================================\n");
    printf(" Contenido de la memoria compartida:\n");
    printf("=====================================\n");
    for (int i = 0; i < mem->num_lineas; i++) {
        printf("[%d][Thread %d] \"%s\"\n",
                mem->lineas[i].linea,
                mem->lineas[i].ID,
                mem->lineas[i].Mensaje);
        printf("-------------------------------------\n");
    }
}

int espionaje_solic(string tipo) {
    if (strcmp(tipo, "Writer") == 0 || strcmp(tipo, "Reader egoísta") == 0 ||
            strcmp(tipo, "Reader") == 0)
        return 1;
    else
        return 0;
}

void espiar(string tipo) {
    FILE *file = fopen(PROCS, "r");
    //printf("Error: %s", strerror(errno));
    char buff[MAX_LINEA_BIT];

    strcat(tipo, "\n"); // Para que el strcmp no falle
    // Encuentra la sección en el archivo correspondiente a la info. del proceso
    do {
        fgets(buff, MAX_LINEA_BIT, file);
        //printf("%s\n", buff);
    } while (strcmp(buff, tipo) != 0);
    
    // Construcción de la ruta base para consultar archivo status de los hilos
    char ruta[MAX_LINEA_BIT] = "/proc/";
    fgets(buff, MAX_LINEA_BIT, file);
    strcat(ruta, buff); // "/proc/#"
    strcat(ruta, "/task/"); // "/proc/#/task/
    
    
    /* Consulta por PID's del status de los hilos del proceso
    do {
        fgets(buff, MAX_LINEA_BIT, file);
        //printf("%s\n", buff);
        if (is_num(buff)) {
            strcat(ruta, buff); // "/proc/#/task/#
            
        }
    } while ();
     */
}

int is_num(char *buff) {
    for (int i = 0; i < MAX_LINEA_BIT; i++)
        if (isdigit(buff[i]) == 0)
            return 0;
    
    return 1;
}
