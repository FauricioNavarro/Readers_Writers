/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Memoria_compartida.h
 * Author: fauricio
 *
 * Created on 18 de mayo de 2018, 11:03 PM
 */

#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

typedef char * string;

typedef struct{
    int ID;
    char Mensaje[25];
    int linea;
}Linea;

typedef struct{
    Linea *lineas;
}Lineas;

typedef struct{
    Lineas lineas;
}Mem_comp;

#endif /* MEMORIA_COMPARTIDA_H */

