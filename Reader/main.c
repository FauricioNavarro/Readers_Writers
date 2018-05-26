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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char *linea = "\n---------------------------------------------------\n";
Mem_comp *mem;
int band[];
int n_procesos;

/*
 * READER
 */
int main(int argc, char** argv) {
    //Parametros temporales
    /*
    n_procesos = atoi(argv[1]);
    int t_read = atoi(argv[2]);  
    int t_sleep = atoi(argv[3]);    
    printf("param 1:%d,2:%d,3:%d\n",n_procesos,t_read,t_read);
    */
    n_procesos = 2;
    int t_sleep = 4;
    int t_read = 2;    
    
    int i = 0;       
    int p_id = getpid(); 
    pthread_t readr_array[n_procesos];    
    
    band[n_procesos];
    escribir_proc("Reader\npid",p_id);        
    get_shm();                           
    
    while(i<n_procesos){            
        Reader *reader = malloc(sizeof(Reader));         
        reader->id = i;        
        reader->tiempo_sleep = t_sleep;
        reader->tiempo_read = t_read;  
        mem->reader_wants_shm = 1;
        pthread_create(&readr_array[i], NULL, reader_function, (void*) reader);                                        
        i=i+1;
    }
    
    while(1){
        if(flags_on()){
            mem->reader_wants_shm = 1;
            sem_wait(&mem->sem_shm_reader);        
            sem_wait(&mem->sem_fin_reader);             
            mem->reader_wants_shm = 0;           
            
            if(not_flags_on()==0){
                sem_post(&mem->sem_shm_reader);        
                sem_post(&mem->sem_fin_reader);  
            }
            sleep(1);
        }else{
            mem->reader_wants_shm = 0;
        }        
    }
    pthread_join(readr_array[0], NULL);     
    
    return (EXIT_SUCCESS);
}


void *reader_function(void *vargp){
    Reader *reader = (Reader*) vargp;
    pthread_t thId = pthread_self();    
    pid_t tid = (pid_t) syscall (SYS_gettid);
    int lim= mem->num_lineas;    
    int i = 0;
    while(1){        
        band[reader->id]=0;
        band[reader->id]=1;
        if(strcmp(&mem->lineas[i].Mensaje,LINEA_VACIA)!=0){
            char *time = timestamp(reader->id);            
            mem->lineas->ID = reader->id;
            mem->lineas->linea = i;            
            escribir_bitacora(time);
            sleep(reader->tiempo_read);
            band[reader->id]=-1;
        }        
        
        if(i==lim){            
            i=0;            
        }else{                
            i=i+1;
        }         
        sleep(reader->tiempo_sleep); 
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
    key_t key = ftok(KEY_FILE, 21);
    printf("key %d", key);
    int shmid = shmget(key,sizeof(Mem_comp),0666|IPC_CREAT);
    mem = (Mem_comp*) shmat(shmid, NULL, 0);
}


void escribir_bitacora(char *msj){
    sem_wait(&mem->sem_bitacora);
    FILE *bitacora;
    bitacora = fopen (BITACORA, "a+");  
    fprintf(bitacora,"Reader-> %s\n",msj);
    fclose(bitacora);
    sem_post(&mem->sem_bitacora);
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