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
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "definiciones.h"

typedef struct{
    int id;     
    int tiempo_sleep;
    int tiempo_write;
} Writer;

void *writer_function(void *vargp);
void escribir_bitacora(char *msj);
void escribir_proc(char *msj,int proceso);
void crea_threads();
void get_shm();
char* timestamp(int id);
int flags_on();
int not_flags_on();

#endif /* WRITER_H */

