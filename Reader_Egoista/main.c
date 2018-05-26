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
Mem_comp *mem;
sem_t pflag;
int band[];
int n_procesos;
/*
 * READER EGOISTA
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
    escribir_proc("Reader Egoista\n",p_id);        
    get_shm();                   
        
    sem_init(&pflag,1,1);
    sem_wait(&pflag);
    
    while(i<n_procesos){            
        Reader_eg *re = malloc(sizeof(Reader_eg));         
        re->id = i;        
        re->tiempo_sleep = t_sleep;
        re->tiempo_read = t_read;
        pthread_create(&readr_array[i], NULL, reader_function, (void*) re);
        i=i+1;
    }      
    
    while(1){
        if(flags_on()){
            mem->r_e_wants_shm = 1;            
            sem_wait(&mem->sem_shm_r_e);        
            sem_wait(&mem->sem_fin_r_e);                  
            mem->r_e_wants_shm = 0;
            
            sem_post(&pflag);            
            sleep(1);            
            sem_wait(&pflag);            
            
            sem_post(&mem->sem_shm_r_e);        
            sem_post(&mem->sem_fin_r_e);             

            sleep(1);
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
    escribir_thread(tid);
    int max = mem->num_lineas;
    int i;
    while(1){ 
        time_t t;
        srand((unsigned) time(&t));
        i = (rand() % (max+1)-1)-1;
        
        band[re->id]=0;
        pthread_mutex_lock(&mutex);
        band[re->id]=1;
        sem_wait(&pflag);
        
        if(strcmp(&mem->lineas[i].Mensaje,LINEA_VACIA)!=0){            
            char *time = timestamp(re->id);            
            mem->lineas[i].ID = re->id;
            mem->lineas[i].linea = i;      
            strcpy(mem->lineas[i].Mensaje,LINEA_VACIA);
            escribir_bitacora(time);
            //printf("Linea leida: %s",mem->lineas->Mensaje[i]);
            sleep(re->tiempo_read);
        }        
        sem_post(&pflag);
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sleep(re->tiempo_sleep);
    }
    return NULL;
}


char* timestamp(int id){
    char *timestamp = (char *)malloc(sizeof(char) * 16);      
    time_t ltime = time(NULL);    
    struct tm *tm;      
    tm=localtime(&ltime); 
    sprintf(timestamp,"%s%d-%04d/%02d/%02d-%02d:%02d:%02d","re",id, tm->tm_year+1900, tm->tm_mon, 
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    return timestamp;     
}


void get_shm(){
    key_t key = ftok(KEY_FILE,21);        
    int shmid = shmget(key,sizeof(Mem_comp),0666|IPC_CREAT);           
    mem = (Mem_comp*) shmat(shmid, NULL, 0);
}


void escribir_bitacora(char *msj){
    sem_wait(&mem->sem_bitacora);
    FILE *bitacora;
    bitacora = fopen (BITACORA, "a+");  
    fprintf(bitacora,"Reader egoista -> %s\n",msj);
    fclose(bitacora);
    sem_post(&mem->sem_bitacora);
}


void escribir_proc(char *msj,int proceso){
    sem_wait(&mem->sem_proceso);
    FILE *procs;    
    procs = fopen (PROCS, "a+");  
    fprintf(procs,"%s:%d\n",msj,proceso);
    fclose(procs);
    sem_post(&mem->sem_proceso);
}

void escribir_thread(int proceso){
    sem_wait(&mem->sem_proceso);
    FILE *procs;    
    procs = fopen (PROCS, "a+");  
    fprintf(procs,"%d\n",proceso);
    fclose(procs);
    sem_post(&mem->sem_proceso);
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

