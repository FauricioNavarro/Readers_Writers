/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   writer.h
 * Author: fauricio
 *
 * Created on 15 de mayo de 2018, 10:20 AM
 */

#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    int id;
    int shmid;    
    int tiempo_sleep;
    int tiempo_write;
} Writer;

void *writer_function(void *vargp);
void escribir_bitacora(char *msj);
#endif /* WRITER_H */

