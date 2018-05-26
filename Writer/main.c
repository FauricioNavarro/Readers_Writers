/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: fauricio
 *
 * Created on 10 de mayo de 2018, 05:23 PM
 */

#include <iso646.h>

#include "writer.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char *linea = "\n---------------------------------------------------\n";
Mem_comp *mem;
sem_t pflag;
int band[]; //Bandera para ver el estado de cada thread
int n_procesos;

/*
 * WRITER
 */
int main(int argc, char** argv) {
    n_procesos = 1;
    int t_sleep = 4;
    int t_write = 2;    
    int i = 0;       
    int p_id = getpid(); 
    pthread_t writer_array[n_procesos];    
    
    //band[n_procesos];
    escribir_proc("Writer\npid",p_id);        
    get_shm();                   
    
    sem_init(&pflag,1,1);
    sem_wait(&pflag);
    printf("1\n");
    while(i<n_procesos){    
        Writer *w = malloc(sizeof(Writer));        
        w->id = i;        
        w->tiempo_sleep = t_sleep;
        w->tiempo_write = t_write;        
        pthread_create(&writer_array[i], NULL, writer_function, (void*) w);                                
        i=i+1;
    }
    
    while(1){
        if(flags_on()){
            mem->writer_wants_shm = 1;
            printf("5\n");
            sem_wait(&mem->sem_shm_writer);        
            sem_wait(&mem->sem_fin_writer);             
            mem->writer_wants_shm = 0;
            sem_post(&pflag);
            printf("6\n");
            //sleep(0.1);
            printf("7(proc)\n");
            sem_wait(&pflag);            
            printf("8(proc)\n");
            //if(not_flags_on()){
            sem_post(&mem->sem_shm_writer);        
            sem_post(&mem->sem_fin_writer);             
            //}            
            sleep(0.1);
        }else{
            mem->writer_wants_shm = 0;
        }        
    }
    pthread_join(writer_array[0], NULL); 
                   
    
             
    return (EXIT_SUCCESS);
}


void *writer_function(void *vargp)
{     
    Writer *writer = (Writer*) vargp;
    pthread_t thId = pthread_self();    
    pid_t tid = (pid_t) syscall (SYS_gettid);
    int lim= mem->num_lineas-1;    
    int i = 0;
    printf("2\n");
    while(1){        
        band[writer->id]=0;        
        pthread_mutex_lock(&mutex);
        printf("3\n");
        band[writer->id]=1;
        printf("4\n");
        sem_wait(&pflag);   
        printf("7(thread)\n");
        if(strcmp(&mem->lineas[i].Mensaje,LINEA_VACIA)==0){            
            char *time = timestamp(writer->id);
            mem->lineas->ID = writer->id;
            mem->lineas->linea = i;            
            strcpy(mem->lineas[i].Mensaje,time);            
            escribir_bitacora(time);            
            //char *msj = mem->lineas[i].Mensaje;
            //printf("Linea escrita: %s",msj);
            sleep(writer->tiempo_write);
            band[writer->id]=-1;                          
        }
              
        if(i==lim){            
            i=0;            
        }else{                
            i=i+1;
        }         
        sem_post(&pflag);
        printf("8(thread)\n");
        sleep(0.1);      
        pthread_mutex_unlock(&mutex);        
        printf("9\n");
        sleep(writer->tiempo_sleep);
    }  
    return NULL;
}

char* timestamp(int id){
    char *timestamp = (char *)malloc(sizeof(char) * 45);      
    time_t ltime = time(NULL);    
    struct tm *tm;      
    tm=localtime(&ltime); 
    sprintf(timestamp,"%s%d-%04d/%02d/%02d-%02d:%02d:%02d","w",id, tm->tm_year+1900, tm->tm_mon, 
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    return timestamp;     
}


void get_shm(){
    key_t key = ftok(KEY_FILE,21);
    int shmid = shmget(key,sizeof(Mem_comp),0666|IPC_CREAT);           
    mem = (Mem_comp*) shmat(shmid, NULL, 0);
}


void escribir_bitacora(char *msj){    
    FILE *bitacora;    
    bitacora = fopen (BITACORA, "a+");
    fprintf(bitacora,"Writer->%s\n",msj);    
    fclose(bitacora);    
}


void escribir_proc(char *msj,int proceso){
    FILE *procs;    
    procs = fopen (PROCS, "a+");  
    fprintf(procs,"%s:%d\n",msj,proceso);
    fclose(procs);
}


int flags_on(){
    int i=0;
    while(i<n_procesos){
        if(band[i]==1){
            return 1;
        } 
        i++;
    }
    return 0;
}


int not_flags_on(){
    int i=0;
    while(i<n_procesos){
        if(band[i]==1){
            return 0;
        }            
        i++;
    }
    return 1;
}