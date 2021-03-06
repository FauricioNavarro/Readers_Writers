/* Tecnológico de Costa Rica
 * Escuela de Computación
 *
 * Proyecto 2
 *
 * Estudiantes:
 *  - Fauricio Navarro
 *  - David Montero
 *
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
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "definiciones.h"
#include "controlador.h"

struct timespec time_audit_value;
struct timespec *time_audit = &time_audit_value;

/*
 *  Implementa las reglas especiales de coordinación entre procesos.
 *  Prioridades entre procesos:
 *      1) Writer / Reader egoísta
 *      2) Reader
 *  Monitorea situación 3 Readers egoístas seguidos:
 *      Tienen prioridad sobre los Reader normales, pero misma que los Writer.
 *          No pueden acceder 3 de ellos consecutivamente, entonces se le
 *      entrega a un proceso de otro tipo. En este escenario, los Writer tienen
 *      prioridad sobre los Reader. Si no hay nadie más, pueden continuar hasta
 *      que llegue alguien más.
 */
int main(int argc, char** argv) {
    key_t key = ftok(KEY_FILE, 21);
    if (key == -1) {
        printf("[Error con llave] %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int shmid = shmget(key, 1, 0666 | IPC_CREAT);
    if (shmid == -1) {
        printf("[Error con llave] %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    Mem_comp *mem = shmat(shmid, NULL, 0);
    if ((int) mem == -1) {
        printf("[Error al hacer attach] %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    printf("key %x\n", key);
    printf("shmid %d\n", shmid);

    pedir_sem_procs(mem);
    
    srand(time(NULL));
    char contador_r_e = 0;
    tipo_proc tipo_selec;
    while (1) {
        //printf("Probe {\n");
        print_want_flags(mem);
        
        if (mem->r_e_wants_shm && contador_r_e == 2)
            tipo_selec = selec_segun_3_r_e(mem, &contador_r_e);
        else
            tipo_selec = selec_segun_jerar(mem, &contador_r_e);
        
        if (tipo_selec != -1) {
            printf("\n[Proc of type %d entered]\n\n", tipo_selec);
            //clock_gettime(CLOCK_REALTIME, time_audit);
            //printf("Timestamp: %lu : %lu\n", time_audit->tv_sec, time_audit->tv_nsec);
            spawn_fin_chck_thread(mem, tipo_selec);
        }
        
        sleep(2);
        //printf("} End cycle probe\n");
    }

    return (EXIT_SUCCESS);
}

/*
 * Bloquea todos los procesos para que dependan de la coordinación 
 * del Controlador.
 */
void pedir_sem_procs(Mem_comp *mem) {
    sem_wait(&mem->sem_shm_writer);
    sem_wait(&mem->sem_shm_reader);
    sem_wait(&mem->sem_shm_r_e);
    sem_wait(&mem->sem_shm_espia);
}

void print_want_flags(Mem_comp *mem) {
    if (mem->writer_wants_shm || mem->reader_wants_shm || mem->r_e_wants_shm ||
            mem->espia_wants_shm) {
        printf("===========\n");
        printf(" Flags:\n");
        printf("===========\n");
        printf(" Writer %d\n", mem->writer_wants_shm);
        printf(" Reader %d\n", mem->reader_wants_shm);
        printf(" RE     %d\n", mem->r_e_wants_shm);
        printf(" Espia  %d\n", mem->espia_wants_shm);
    } else {
        printf("Flags apagadas\n");
    }
    
    //clock_gettime(CLOCK_REALTIME, time_audit);
    //printf("Timestamp: %lu : %lu\n", time_audit->tv_sec, time_audit->tv_nsec);
}

/*
 * Implementa la vigilancia de la situación de excepción que prohíbe el acceso
 * seguido de 3 Readers egoístas.
 */
tipo_proc selec_segun_3_r_e(Mem_comp* mem, char *contador_r_e) {
    int random;
    tipo_proc tipo = -1;
    if (mem->writer_wants_shm) {
        sem_post(&(mem->sem_shm_writer));
        *contador_r_e = 0;
        return writer;
    } else { // (EDITAR si cambia la de abajo, son lo mismo en lógica)
        if (mem->reader_wants_shm || mem->espia_wants_shm) {
            if (mem->reader_wants_shm && mem->espia_wants_shm) {
                random = rand();
                random = random > 0 ? 1 : 0;
                if (random) {
                    sem_post(&(mem->sem_shm_reader));
                    tipo = reader;
                } else {
                    sem_post(&(mem->sem_shm_espia));
                    tipo = espia;
                }
            } else {
                if (mem->reader_wants_shm) {
                    sem_post(&(mem->sem_shm_reader));
                    tipo = reader;
                }
                else {
                    sem_post(&(mem->sem_shm_espia));
                    tipo = espia;
                }
            }

            *contador_r_e = 0;
        }
    }

    if (mem->r_e_wants_shm) {
        sem_post(&(mem->sem_shm_r_e));
        return r_e;
    }
    
    return tipo;
}

/*
 * Implementa la jerarquía entre procesos que determina cuál entra a la región
 * crítica.
 */
tipo_proc selec_segun_jerar(Mem_comp *mem, char *contador_r_e) {
    int random;
    tipo_proc tipo = -1;
    if (mem->writer_wants_shm || mem->r_e_wants_shm) {
        if (mem->writer_wants_shm && mem->r_e_wants_shm) { // Ambos pidieron
            random = rand();
            random = random > 0 ? 1 : 0;
            if (random) {
                sem_post(&(mem->sem_shm_writer));
                *contador_r_e = 0;
                tipo = writer;
            } else {
                sem_post(&(mem->sem_shm_r_e));
                *contador_r_e++;
                tipo = r_e;
            }
        } else {
            if (mem->writer_wants_shm) { // Para averiguar cuál fue
                sem_post(&(mem->sem_shm_writer));
                *contador_r_e = 0;
                tipo = writer;
            } else {
                sem_post(&(mem->sem_shm_r_e));
                *contador_r_e++;
                tipo = r_e;
            }
        }
    } else {
        if (mem->reader_wants_shm || mem->espia_wants_shm) {
            if (mem->reader_wants_shm && mem->espia_wants_shm) {
                random = rand();
                random = random > 0 ? 1 : 0;
                if (random) {
                    sem_post(&(mem->sem_shm_reader));
                    tipo = reader;
                } else {
                    sem_post(&(mem->sem_shm_espia));
                    tipo = espia;
                }
            } else {
                if (mem->reader_wants_shm) {
                    sem_post(&(mem->sem_shm_reader));
                    tipo = reader;
                }
                else {
                    sem_post(&(mem->sem_shm_espia));
                    tipo = espia;
                }
            }

            *contador_r_e = 0;
        }
    }
    
    return tipo;
}

/*
 * Levanta un hilo que espera a que el proceso entrante a la región crítica
 * finalice para volver a capturar su semáforo para bloquerlo nuevamente.
 */
void spawn_fin_chck_thread(Mem_comp *mem, tipo_proc tipo_selec) {
    pthread_t fin_chck_thread;
    thread_info *data = malloc(sizeof(thread_info));
    data->mem = mem;
    data->tipo = tipo_selec;
    pthread_create(&fin_chck_thread, NULL, relock_sem_shm, (void *) data);
}

/*
 * Selección como tal del semáforo que corresponde tanto para notificar
 * finalización por parte de un proceso como del que lo bloquea para entrar
 * a la región crítica.
 */
void *relock_sem_shm(void *tmp) {
    thread_info *data = (thread_info *) tmp;
    switch (data->tipo) {
        case writer:
            //clock_gettime(CLOCK_REALTIME, time_audit);
            //printf("Timestamp: %lu : %lu\n", time_audit->tv_sec, time_audit->tv_nsec);
            //printf("Esperando finalización\n");
            sem_wait(&(data->mem->sem_fin_writer));
            
            //clock_gettime(CLOCK_REALTIME, time_audit);
            //printf("Timestamp: %lu : %lu\n", time_audit->tv_sec, time_audit->tv_nsec);
            //printf("Bloqueando proceso...\n");
            sem_wait(&(data->mem->sem_shm_writer));
            
            //clock_gettime(CLOCK_REALTIME, time_audit);
            //printf("Timestamp: %lu : %lu\n", time_audit->tv_sec, time_audit->tv_nsec);
            //printf("Proceso bloqueado\n");
            
            sem_post(&(data->mem->sem_fin_writer));
            break;

        case reader:
            sem_wait(&(data->mem->sem_fin_reader));
            sem_wait(&(data->mem->sem_shm_reader));
            sem_post(&(data->mem->sem_fin_reader));
            break;

        case r_e:
            sem_wait(&(data->mem->sem_fin_r_e));
            sem_wait(&(data->mem->sem_shm_r_e));
            sem_post(&(data->mem->sem_fin_r_e));
            break;

        case espia:
            sem_wait(&(data->mem->sem_fin_espia));
            sem_wait(&(data->mem->sem_shm_espia));
            sem_post(&(data->mem->sem_fin_espia));
            break;
    }
}
