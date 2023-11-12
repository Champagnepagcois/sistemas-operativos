#include "Modelo.h"

/********Pide memoria compartida y asigna la direccion a un apuntador****/

void getMemoryShared(int clave, struct Request *request){
  int memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);//Crea un ID
  memoria = shmget(llave_memoria,sizeof(struct Request), IPC_CREAT | PERMISOS);//ID memoria compartida
  if (memoria == -1) {
    perror("\nError al pedir region en la memoria compartida\n");
    exit(-1);
  };
  request->apt_shm = (struct Request *)shmat(memoria,0,0);
  if(request->apt_shm == (struct Request *)(-1)){
    perror("\nError al asignar la memoria compartida a la Request\n");
    exit(-1);
  }
};
/*************************************************************************/


/******************Codigo del semaforo*************************/
int Crea_semaforo(key_t llave,int valor_inicial){
  int semid=semget(llave,1,IPC_CREAT|PERMISOS);
  if(semid==-1){
    return -1;
  };
  semctl(semid,0,SETVAL,valor_inicial);
  return semid;
};
void down(int semid){
  struct sembuf op_p[]={0,-1,0};
  semop(semid,op_p,1);
};
void up(int semid){
  struct sembuf op_v[]={0,+1,0};
  semop(semid,op_v,1);
};
/****************************************************************/