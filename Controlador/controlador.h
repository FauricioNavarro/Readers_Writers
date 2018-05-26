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
 * File:   controlador.h
 * Author: stickman
 *
 * Created on May 19, 2018, 11:45 PM
 */

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

typedef enum {
    writer, // 0
    reader, // 1
    r_e,    // 2
    espia   // 3
} tipo_proc;

typedef struct {
    tipo_proc tipo;
    Mem_comp *mem;
} thread_info;

void pedir_sem_procs(Mem_comp *mem);
tipo_proc selec_segun_jerar(Mem_comp *mem, char *contador_r_e);
tipo_proc selec_segun_3_r_e(Mem_comp *mem, char *contador_r_e);
void spawn_fin_chck_thread(Mem_comp *mem, tipo_proc tipo_selec);
void *relock_sem_shm(void *tmp);
void print_want_flags(Mem_comp *mem);

#endif /* CONTROLADOR_H */

