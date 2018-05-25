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

#include "definiciones.h"
#include "espia.h"

/*
 * 
 */
int main(int argc, char** argv) {
    key_t key = ftok("shmfile", 21);
    int shmid = shmget(key, 1, 0666 | IPC_CREAT);
    Mem_comp *mem = shmat(shmid, NULL, 0);

    string param = argv[1];
    if (argc == 2)
        if (strcmp(param, "mem") == 0)
            mostrar_shm(mem);
        else if (espionaje_solic(param))
            espiar(param);

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

}