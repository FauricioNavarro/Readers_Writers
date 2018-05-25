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

/*
 * INICIALIZADOR
 */

#include "inicializador.h"

Mem_comp *memoria_comp;
char *setshm = "0000000000000000000000000"; 

int main(int argc, char** argv) {           
    get_shm();     
    init_sem();
    init_flags();
    clean_shm();
    create_bitacora();   
    return (EXIT_SUCCESS);
}

void get_shm(){
    key_t key = ftok("shmfile",21);    
    int shmid = shmget(key,sizeof(Mem_comp),0666|IPC_CREAT);  
    memoria_comp = (Mem_comp*) shmat(shmid,(void*)0,0);
    memoria_comp->num_lineas = 20;
    memoria_comp->lineas[memoria_comp->num_lineas]; 
}


void init_sem(){
    sem_init(&memoria_comp->sem_shm_reader,1,1);
    sem_init(&memoria_comp->sem_shm_writer,1,1);
    sem_init(&memoria_comp->sem_shm_r_e,1,1); 
    sem_init(&memoria_comp->sem_shm_espia,1,1);   
    
    sem_init(&memoria_comp->sem_fin_reader,1,1);
    sem_init(&memoria_comp->sem_fin_writer,1,1);
    sem_init(&memoria_comp->sem_fin_r_e,1,1); 
    sem_init(&memoria_comp->sem_fin_espia,1,1);       
}


void init_flags(){
    memoria_comp->reader_wants_shm = 0;
    memoria_comp->writer_wants_shm = 0; 
    memoria_comp->r_e_wants_shm = 0;
    memoria_comp->espia_wants_shm = 0;
}


void clean_shm(){
    int i=0;
    while(i<memoria_comp->num_lineas){
        strcpy(&memoria_comp->lineas[i].Mensaje,setshm);       
        printf("msj:%s\n",memoria_comp->lineas[i].Mensaje);
        i=i+1;
    } 
}


void create_bitacora(){
    FILE *fb;
    fb = fopen ("/home/fauricio/NetBeansProjects/Readers – Writers/bitacora.txt", "w+");        
}