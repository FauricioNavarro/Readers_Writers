Error reading included file file : ///home/fauricio/NetBeansProjects/Readers%20–%20Writers/License_header.txt
        /* 
         * File:   inicializador.h
         * Author: fauricio
         *
         * Created on 23 de mayo de 2018, 06:14 PM
         */

#ifndef INICIALIZADOR_H
#define INICIALIZADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "definiciones.h"

void get_shm();
void init_sem();
void init_flags();
void clean_shm();
void create_bitacora();

#endif /* INICIALIZADOR_H */

