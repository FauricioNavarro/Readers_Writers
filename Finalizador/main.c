/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: fauricio
 *
 * Created on 24 de mayo de 2018, 04:54 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 * Finalizador temporal
 */
int main(int argc, char** argv) {
    int shm_id; 
    int * mem;
    int err,err1;
    
    key_t key = ftok("shmfile",21);    
    shm_id = shmget(key, 1 , IPC_CREAT | 0666);    
    mem = ( int *) shmat ( shm_id , ( void *) 0 , 0) ;
    
    
    printf("Id shared memory: %d \n",shm_id);
        
    //-----------------------------------------------
    err = shmdt (( void *) mem ) ;
        
        
    err1 = shmctl ( shm_id , IPC_RMID , NULL) ;               

    //shmdt(shm_id);

    return 0;
}
