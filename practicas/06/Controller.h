#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <curses.h>
#include <string.h>
#include <openssl/aes.h>

/************** Contantes ***************/
#define PERMISOS 0644
#define KEY 109282

/*************** Estructuras ******************/
struct Usuario {
  char username [21];
  char password [21];
  int logged;
};

/**************** Definicion de funciones ***************/
//visual
void showAuthPage(struct Usuario *user);
void showRegisterPage(struct Usuario *user);
//herlpers
void cleanText(char *cadena);
void signin(struct Usuario *usuario);
void setUser(struct Usuario *usuario);
void getUser(struct Usuario *usuario,char *passwordFile);
void signup(struct Usuario *usuario);
void cifrar(char *password);
void descifrar(char *password);
void structToString(char *apt_salida,struct Usuario *usuario);
//archivo
void openFile(const char *fileName, FILE **file);
void closeFile(FILE **file);
//semaforo
int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
void initSemaphore(key_t *llave,int *semaforo,int *clave);
int getValueSemaphore(int *id);
//Errores
void ErrorMessage(char *message);

#endif