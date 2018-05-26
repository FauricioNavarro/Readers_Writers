/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   espia.h
 * Author: stickman
 *
 * Created on May 25, 2018, 1:24 AM
 */

#ifndef ESPIA_H
#define ESPIA_H

void mostrar_shm(Mem_comp *mem);
int espionaje_solic(string tipo);
void espiar(string tipo);
int is_num(char *buff);
void limpiar_pids(string *);
string get_pids_estado(string *pids, string estado);

#endif /* ESPIA_H */

