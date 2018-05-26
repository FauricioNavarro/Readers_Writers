/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   reader_egoista.h
 * Author: fauricio
 *
 * Created on 24 de mayo de 2018, 05:50 PM
 */

#ifndef READER_EGOISTA_H
#define READER_EGOISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "definiciones.h"

typedef struct{
    int id;    
    int tiempo_sleep;
    int tiempo_read;    
} Reader_eg;

void *reader_function(void *vargp);
void escribir_bitacora(char *msj);
void escribir_proc(char *msj,int proceso);
void get_shm();
char* timestamp(int id);
int flags_on();
int not_flags_on();

#endif /* READER_EGOISTA_H */

