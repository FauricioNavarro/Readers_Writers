/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */ 

/* 
 * File:   main.c
 * Author: fauricio
 *
 * Created on 10 de mayo de 2018, 11:54 AM
 */

#include "reader.h"

char *linea = "\n---------------------------------------------------\n";
/*
 * READER
 */
int main(int argc, char** argv) {
    //Parametros temporales
    int n_procesos = 2;
    int t_sleep = 4;
    int t_read = 2;    
    
    int p_id,p_pid; 
    p_id=getpid();  /*process id*/
    p_pid=getpid(); /*parent process id*/
    
    escribir_proc("Process id",p_id);
    escribir_proc("Parent process id",p_pid);
    
    key_t key = ftok("shmfile",21);        
    int shmid = shmget(key,1,0666|IPC_CREAT);             
    
    pthread_t readr_array[n_procesos];    
    int i = 0;        
    while(i<n_procesos){    
        Reader *reader = malloc(sizeof(Reader));         
        reader->id = i;
        reader->shmid = shmid;
        reader->tiempo_sleep = t_sleep;
        reader->tiempo_read = t_read;        
        int thread_id = pthread_create(&readr_array[i], NULL, reader_function, (void*) reader);                                
        //escribir_proc("Thread id",thread_id);        
        i=i+1;
    }
    pthread_join(readr_array[0], NULL); 
                   
    
             
    return (EXIT_SUCCESS);
}

void *reader_function(void *vargp)
{     
    Reader *reader = (Reader*) vargp;
    pthread_t thId = pthread_self();
    //int id = gettid();
    pid_t tid = (pid_t) syscall (SYS_gettid);
    escribir_proc("Thread id:",tid);
    time_t ltime;
    struct tm *tm;         
    char *shm = (char*) shmat(reader->shmid,(void*)0,0);       
    int pages = strlen(shm)/25;
    int len = strlen(shm);
    int regBase = 0;
    int cont = 0;
    int i = 0;
    while(1){
        reader->mem = shmat(reader->shmid, NULL, 0);
        sem_wait(&reader->mem->sem_shm_reader);
        regBase = cont*25;                                                         
        
        if(shm[regBase]=='0'){
            printf("Casilla vacia \n"); 
            printf("%s",linea);
        }else{            
            sleep(reader->tiempo_read);            
            char *timestamp = (char *)malloc(sizeof(char) * 16);  
            ltime=time(NULL);    
            tm=localtime(&ltime); 
            sprintf(timestamp,"%s%d-%04d/%02d/%02d-%02d:%02d:%02d","r",reader->id, tm->tm_year+1900, tm->tm_mon, 
                tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);                            
            escribir_bitacora(timestamp);
            printf("\nSe lee en el registro Base %d:",regBase);                                       
            int j;
            for(j=0;j<25;j++){
                printf("%c",shm[regBase+j]);
            }            
            printf("%s",linea);
            sleep(reader->tiempo_sleep);
        }
        
        cont++;
        i=i+1;
        if(cont == pages){
            cont = 0;
        }
        sleep(60);
    }      
            
    return NULL;
}

void escribir_bitacora(char *msj){
    FILE *bitacora;
    bitacora = fopen ("/home/fauricio/NetBeansProjects/Readers – Writers/bitacora.txt", "a+");  
    fprintf(bitacora,"Reader-> %s\n",msj);
    fclose(bitacora);
}

void escribir_proc(char *msj,int proceso){
    FILE *bitacora;    
    bitacora = fopen ("/home/fauricio/NetBeansProjects/Readers – Writers/Data/procesos.txt", "a+");  
    fprintf(bitacora,"%s:%d\n",msj,proceso);
    fclose(bitacora);
}
