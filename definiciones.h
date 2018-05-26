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

#define PROCS "/home/fauricio/NetBeansProjects/Readers – Writers/Data/procesos.txt"
//#define PROCS "/home/stickman/Documents/TEC/SO/Proyecto_2/Readers_Writers/Data/procesos.txt"

#define BITACORA "/home/fauricio/NetBeansProjects/Readers – Writers/bitacora.txt"
//#define BITACORA "/home/stickman/Documents/TEC/SO/Proyecto_2/Readers_Writers/bitacora.txt"

#define KEY_FILE "/home/fauricio/NetBeansProjects/Readers – Writers/key_file.txt"
//#define KEY_FILE "/home/stickman/Documents/TEC/SO/Proyecto_2/Readers_Writers/key_file.txt"

#define LINEA_VACIA "000000000000000000000000000000000000000000000"

typedef char * string;

typedef struct{
    int ID;
    char Mensaje[45];
    int linea;
}Linea;

typedef struct{
    // Semáforo para controlar acceso a la bitácora
    sem_t sem_bitacora;
    sem_t sem_proceso;
    
    // Semáforos para controlar acceso a memoria compartida
    sem_t sem_shm_writer;
    sem_t sem_shm_reader;
    sem_t sem_shm_r_e;
    sem_t sem_shm_espia;

    // Semáforos para interactura con flag finalización
    sem_t sem_fin_writer;
    sem_t sem_fin_reader;
    sem_t sem_fin_r_e;
    sem_t sem_fin_espia;
    
    // Banderas para señalar intención de acceso a memoria compartida
    char writer_wants_shm;
    char reader_wants_shm;
    char r_e_wants_shm;
    char espia_wants_shm;
    
    // Lineas
    int num_lineas;
    Linea lineas[];  // Se inicializa en el main.c de Inicializador    	
}Mem_comp;

#endif /* MEMORIA_COMPARTIDA_H */

