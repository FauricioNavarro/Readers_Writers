/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Memoria_compartida.h
 * Author: fauricio
 *
 * Created on 18 de mayo de 2018, 11:03 PM
 */

#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include <semaphore.h>

typedef char * string;

typedef struct{
    int ID;
    char Mensaje[25];
    int linea;
}Linea;

/*
typedef struct{
    //Linea *lineas; // Si es para hacer una lista, creo que no funcionaría en shm :S
}Lineas;
*/

typedef struct{
    sem_t sem_shm_writer;
    sem_t sem_shm_reader;
    sem_t sem_shm_r_e;
    sem_t sem_shm_espia;
    sem_t sem_fin_writer;
    sem_t sem_fin_reader;
    sem_t sem_fin_r_e;
    sem_t sem_fin_espia;
    char writer_wants_shm;
    char reader_wants_shm;
    char r_e_wants_shm;
    char espia_wants_shm;
    Linea lineas[]; // Se inicializa en el main.c de Inicializador
}Mem_comp;

#endif /* MEMORIA_COMPARTIDA_H */

