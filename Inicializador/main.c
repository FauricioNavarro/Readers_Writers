/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: fauricio
 *
 * Created on 10 de mayo de 2018, 11:52 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "Memoria_compartida.h"
/*
 * INICIALIZADOR
 */
int main(int argc, char** argv) {
    
    Mem_comp *memoria_comp = (Mem_comp*)malloc(sizeof(Mem_comp));    
    FILE * fp;
    int total_lineas = size(memoria_comp);
        
    key_t key = ftok("shmfile",21);    
    int shmid = shmget(key,total_lineas,0666|IPC_CREAT);     
    char *str = (char*) shmat(shmid,(void*)0,0);
    fp = fopen ("/home/fauricio/NetBeansProjects/Readers – Writers/bitacora.txt", "w+");    
    int i=0;
    
    //Limpia la memoria
    while(i<total_lineas){
        str[i] = '0';
        //sprintf(&str[i], "%d", 0);        
        i=i+1;
    }
               
    return (EXIT_SUCCESS);
}

