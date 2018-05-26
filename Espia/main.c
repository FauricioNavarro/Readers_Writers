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

char ruta[MAX_LINEA_BIT] = "/proc/";

/*
 * 
 */
int main(int argc, char** argv) {
    key_t key = ftok(KEY_FILE, 21);
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
    
    shmdt((void *) mem);

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
    char buff[MAX_LINEA_BIT];
    string pids[100]; // TO DO: Hacer dinámico
    limpiar_pids(pids);

    strcat(tipo, "\n"); // Para que el strcmp no falle
    // Encuentra la sección en el archivo correspondiente a la info. del proceso
    do {
        fgets(buff, MAX_LINEA_BIT, file);
        //printf("%s\n", buff);
    } while (strcmp(buff, tipo) != 0);
    
    // Construcción de la ruta base para consultar archivo status de los hilos
    fgets(buff, MAX_LINEA_BIT, file);
    strcat(ruta, buff); // "/proc/#"
    strcat(ruta, "/task/"); // "/proc/#/task/
    
    // Almacena PID's de los hilos
    for (int i = 0; i < 100; i++) {// TO DO: Ajustar con respecto a def pids
        fgets(pids[i], MAX_LINEA_BIT, file);
        if (!is_num(pids[i])) {
            strcpy(pids[i], "");
            break;
        }
    }
    
    fclose(file);
    
    printf("============================================\n");
    printf("Estado de los %s\n", tipo);
    printf("============================================\n");
    printf("--------------------------------------------\n");
    printf("\tHilos con acceso al archivo:\n");
    printf("--------------------------------------------\n");
    printf("%s", get_pids_estado(pids, 'R'));
    printf("--------------------------------------------\n");
    printf("\tHilos dormidos:\n");
    printf("--------------------------------------------\n");
    printf("%s", get_pids_estado(pids, 'D'));
    printf("--------------------------------------------\n");
    printf("\tHilos bloqueados:\n");
    printf("--------------------------------------------\n");
    printf("%s", get_pids_estado(pids, 'S'));
}

int is_num(char *buff) {
    for (int i = 0; i < MAX_LINEA_BIT; i++)
        if (isdigit(buff[i]) == 0)
            return 0;
    
    return 1;
}

void limpiar_pids(string *pids) {
    for (int i = 0; i < 100; i++) { // TO DO: Ajustar con def pids
        strcpy(pids[i], "");
    }
}

string get_pids_estado(string *pids, char estado) {
    char *res[1000];
    strcpy(res, "");
    char *ruta2[MAX_LINEA_BIT];
    FILE *file;
    char buff[MAX_LINEA_BIT];
    
    for (int i = 0; i < 100; i++) { // TO DO: Ajustar con def pids
        // Construcción de la ruta
        strcpy(ruta2, ruta); // "/proc/#/task/"
        strcat(ruta2, pids[i]); // "/proc/#/task/#"
        strcat(ruta2, "/status"); // "/proc/#/task/#/status"
        file = fopen(ruta2, "r");
        
        // Obtención de la línea de interés en status
        fgets(buff, MAX_LINEA_BIT, file); // Salta primera línea
        fgets(buff, MAX_LINEA_BIT, file);
        
        if (str_in_str(estado, buff)) {
            strcat(res, pids[i]);
            strcat(res, "\n");
        }
        
        fclose(file);
    }
    
    if (strcmp(res, "") == 0)
        strcpy(res, "\n");
    
    return res;
}

int str_in_str(char estado, string buff) {
    for (int i = 0; i < MAX_LINEA_BIT; i++)
        if (buff[i] == estado)
            return 1;
    
    return 0;
}
