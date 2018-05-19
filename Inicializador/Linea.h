/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Linea.h
 * Author: fauricio
 *
 * Created on 18 de mayo de 2018, 10:55 PM
 */

#ifndef LINEA_H
#define LINEA_H

typedef struct{
    int ID;
    char Mensaje[25];
    int linea;
}Linea;

typedef struct{
    Linea *lineas;
}Lineas;

#endif /* LINEA_H */

