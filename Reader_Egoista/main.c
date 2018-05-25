/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: fauricio
 *
 * Created on 24 de mayo de 2018, 04:51 PM
 */

#include "reader_egoista.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char *linea = "\n---------------------------------------------------\n";
char *setshm = "0000000000000000000000000";
Mem_comp *mem;
sem_t sem_controlador;
int band[];
int n_procesos;
char *ruta_proceso = "/home/fauricio/NetBeansProjects/Readers – Writers/Data/procesos.txt";
char *ruta_bitacora ="/home/fauricio/NetBeansProjects/Readers – Writers/bitacora.txt";
/*
 * READER EGOISTA
 */
int main(int argc, char** argv) {
    //Parametros temporales
    n_procesos = 2;
    int t_sleep = 4;
    int t_read = 2;    
    int i = 0;       
    int p_id = getpid(); 
    pthread_t readr_array[n_procesos];    
    
    band[n_procesos];
    escribir_proc("Reader egoista\npid",p_id);        
    get_shm();                   
        
    sem_init(&sem_controlador,1,1);
    sem_wait(&sem_controlador);
    
    while(i<n_procesos){            
        Reader_eg *re = malloc(sizeof(Reader_eg));         
        re->id = i;        
        re->tiempo_sleep = t_sleep;
        re->tiempo_read = t_read;  
        mem->r_e_wants_shm = 1;
        pthread_create(&readr_array[i], NULL, reader_function, (void*) re);
        i=i+1;
    }
    
    while(1){
        if(flags_on()){
            mem->r_e_wants_shm = 1;
            sem_wait(&mem->sem_shm_r_e);        
            sem_wait(&mem->sem_fin_r_e);
            sem_post(&sem_controlador);
            sem_wait(&sem_controlador);
            if(not_flags_on()){
                sem_post(&mem->sem_shm_r_e);        
                sem_post(&mem->sem_fin_r_e); 
            }            
        }else{
            mem->r_e_wants_shm = 0;
        }        
    }
    pthread_join(readr_array[0], NULL);     
    
    return (EXIT_SUCCESS);
}


void *reader_function(void *vargp){
    Reader_eg *re = (Reader_eg*) vargp;
    pthread_t thId = pthread_self();    
    pid_t tid = (pid_t) syscall (SYS_gettid);
    int max = mem->num_lineas;
    int i;
    while(1){ 
        time_t t;
        srand((unsigned) time(&t));
        i = rand() % (max+1)-1; 
        band[re->id]=0;
        pthread_mutex_lock(&mutex);
        sem_wait(&sem_controlador);
        band[re->id]=1;
        if(strcmp(&mem->lineas[i].Mensaje,setshm)==0){
            printf("Casilla vacia \n");
        }else{
            char *time = timestamp(re->id);
            escribir_bitacora(timestamp);
            strcpy(mem->lineas->Mensaje[i],setshm);
            printf("Linea leida: %s",mem->lineas->Mensaje[i]);
            sleep(re->tiempo_read);
        }
        band[re->id]=-1;
        sleep(re->tiempo_sleep);              
        sem_wait(&sem_controlador);
        pthread_mutex_unlock(&mutex);
        sleep(0.1);        
    }
    return NULL;
}


char* timestamp(int id){
    char *timestamp = (char *)malloc(sizeof(char) * 16);      
    time_t ltime = time(NULL);    
    struct tm *tm;      
    tm=localtime(&ltime); 
    sprintf(timestamp,"%s%d-%04d/%02d/%02d-%02d:%02d:%02d","r",id, tm->tm_year+1900, tm->tm_mon, 
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    return timestamp;     
}


void get_shm(){
    key_t key = ftok("shmfile",21);        
    int shmid = shmget(key,sizeof(Mem_comp),0666|IPC_CREAT);           
    mem = (Mem_comp*) shmat(shmid, NULL, 0);
}


void escribir_bitacora(char *msj){
    FILE *bitacora;
    bitacora = fopen (ruta_bitacora, "a+");  
    fprintf(bitacora,"Reader egoista -> %s\n",msj);
    fclose(bitacora);
}


void escribir_proc(char *msj,int proceso){
    FILE *bitacora;    
    bitacora = fopen (ruta_proceso, "a+");  
    fprintf(bitacora,"%s:%d\n",msj,proceso);
    fclose(bitacora);
}


int flags_on(){
    int i=0;
    while(i<n_procesos){
        if(band[i]==1){
            return 1;
        }            
    }
    return 0;
}

int not_flags_on(){
    int i=0;
    while(i<n_procesos){
        if(band[i]==1){
            return 0;
        }            
    }
    return 1;
}

