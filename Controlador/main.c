/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: stickman
 *
 * Created on May 19, 2018, 7:44 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "definiciones.h"

/*
 * 
 */
int main(int argc, char** argv) {
    key_t key = ftok("shmfile",21);        
    int shmid = shmget(key,1,0666|IPC_CREAT);  
    
    return (EXIT_SUCCESS);
}

