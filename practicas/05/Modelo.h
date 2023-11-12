#ifndef MODELO_H
#define MODELO_H


/************* Librerias **************/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>


/************ Constantes ***************/

#define PERMISOS 0644
#define CLAVE_SERVIDOR_SEM 's'
#define CLAVE_SERVIDOR_MC 'm'
#define CLAVE_HILO_SEM 'h'
#define CLAVE_HILO_IO 'i'
#define CLAVE_HILO_MC 'c'

/**************** Estructuras *************/
struct Request{
  pid_t ID_server;
  pid_t ID_client;
  pthread_t ID_thread;
  key_t key_shm_c;
  key_t key_shm_s;
  struct Request *apt_shm;
  int  connected;
  int new_request;
  char message[200];
};


struct Cliente {
  key_t ID_cliente;
  key_t ID_hilo;
  char* frase;
  int lon_frase;
  int con_activa;
  struct Cliente* siguiente;
};




/************* Servidor ****************/

void getMemoryShared(int clave, struct Request *request);
void service(pid_t *pid_client);
void *serviceThread(void *arg);
void shmWrite(struct Request *request);
void fileWrite(struct Request *request);
void fileReader();
void verifyNewMessage(int *apt_hilo_mc);




/************* Cliente ****************/



/************* Semaforo ***************/
int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);




/**************** IO ******************/

int openFile();
void readFile();
void writeFile();

#endif