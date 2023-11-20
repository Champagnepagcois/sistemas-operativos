/********* Bibliotecas ********/
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

/*************** Estructuras ******************/
struct Usuario {
  char usuario [20];
  char password [200];
};

/**************** Definicion de funciones ***************/
//visual
void showAuthPage(struct Usuario *user);
//herlpers
void structToString(char* apt_salida,void *data,int dataType);
//archivo
void openFile(struct FileManager *fileManager);
void closeFile(struct FileManager *fileManager);
//semaforo
int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
void initSemaphore(key_t *llave,int *semaforo,int *clave);
int getValueSemaphore(int *id);
//Errores
void ErrorMessage(char *message);
