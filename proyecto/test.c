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

/************ Constantes *************/
#define PERMISOS 0644

#define INT 50

#define USUARIO        100 
#define CATEGORIA      101
#define PRODUCTO       102
#define VENTA          103
#define DETALLEVENTA   104
#define PROVEEDOR      105
#define ADQUISICION    106
#define TYPEDATA_REQUEST  110

#define CLAVE_SER_H_ADDITEM 'a'
#define CLAVE_SER_H_LOGIN_MC 'z'
#define CLAVE_SER_H_LOGIN_MC_REQ 'z'
#define CLAVE_SER_H_D_IO_USUARIO 'u'
/*********************** Estructuras ***********************/
struct Request{
  pid_t ID_usuario;
  int dataType;
  struct Request* apt_mc_request;
};
struct Usuario {
  int ID_usuario; //lon 6
  char nombre [15]; 
  char usuario [25];
  char password [120];
  int typeUser; // lon 2
  int logged;   // 1on 1
  struct Usuario* siguiente;
  struct Usuario* apt_mc_usuario;
  int count;
};
/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/
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
void createSemPublic(int *semaforo,int clave, int valor){
  key_t llave = ftok("archivo",clave);
  *semaforo = Crea_semaforo(llave,valor);
};
int getValueSemaphore(int *id){
  return semctl(*id,0,GETVAL,0);
};

/******************** Memoria Compartida ************************/
void getShmPublic(void *data,char clave,int typeData){ //Falta terminar
  key_t llave_memoria = ftok("archivo",clave);
  int memoria;
  switch (typeData){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    memoria = shmget(llave_memoria, sizeof(struct Usuario), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/USUARIO");
    usuario->apt_mc_usuario = (struct Usuario*)shmat(memoria,0,0);
    if(usuario->apt_mc_usuario == (struct Usuario*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/USUARIO");
    break;
  case TYPEDATA_REQUEST:
    struct Request *request = (struct Request*)data;
    memoria = shmget(llave_memoria, sizeof(struct Request), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/REQUEST");
    request->apt_mc_request = (struct Request*)shmat(memoria,0,0);
    if(request->apt_mc_request == (struct Request*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/REQUEST");
    break;
    default:
    break;
  };
  return;
};

/****************************************************************/

/******************** Crea los Hiloss ************************/

void createThreadPublic(pthread_t *ID_thread,void*(*__start_routine)(void *),void *__restrict__ __arg){
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(ID_thread,&atributos,__start_routine, __arg);
  if(threadCorrect != 0) ErrorMessage("Error en la creación del hilo");
  return;
};
/****************************************************************/

int main(){
  /********* Pide canal de comunicacion para pasar datos **********/
  struct Request request;
  char clave_addItem = CLAVE_SER_H_ADDITEM;
  //getMemoria compartida
  getShmPublic(&request,clave_addItem,TYPEDATA_REQUEST); //MC de request
  //semaforo
  int semaforo_new_request;
  createSemPublic(&semaforo_new_request,CLAVE_SER_H_ADDITEM,0);

  request.apt_mc_request->dataType = USUARIO;
  request.apt_mc_request->ID_usuario = getpid();
  //Hacemos una nueva request
  up(semaforo_new_request);



  /****************************************************************/
  /*************** Pasa los datos de la request ******************/

  /****************************************************************/
  struct Usuario usuario, usuarioTmp;
  //pide MC
  getShmPublic(&usuario,getpid(),USUARIO);
  //Escribo la usuario

  usuario.apt_mc_usuario->count =2;

  //crea SEMAFORO
  int semaforo_newData,semaforo_WR_mc, semaforo_done;
  createSemPublic(&semaforo_WR_mc,CLAVE_SER_H_ADDITEM+1,1); //?
  createSemPublic(&semaforo_newData,0,0); //Nueva request
  createSemPublic(&semaforo_done,2,0);

  //definir variable de hilo dispatcher
  //pthread_t hilo_dispatcher;

  int cc=usuario.apt_mc_usuario->count;
  //printf("\n%d\n",usuario.apt_mc_usuario->count);
  printf("Mi pid es:%d\n",getpid());
  while (cc>0){
    printf("Ingresa el usuario:");
    scanf("%15s",usuarioTmp.nombre);
    printf("Ingresa el password:");
    scanf("%s",usuarioTmp.password);
    printf("%d-\n",0);
    down(semaforo_WR_mc);//escribe en MC
    //request.apt_mc_request->dataType = USUARIO;
    //request.apt_mc_request->ID_usuario = getpid();
    strcpy(usuario.apt_mc_usuario->nombre,usuarioTmp.nombre);
    strcpy(usuario.apt_mc_usuario->usuario,usuarioTmp.nombre);
    strcpy(usuario.apt_mc_usuario->password,usuarioTmp.password);
    //strcpy(usuario.apt_mc_usuario->nombre,"USER");
    //strcpy(usuario.apt_mc_usuario->password,"PASSWORD");
    up(semaforo_WR_mc);//escribe en MC
    printf("Termino de escribr");
    up(semaforo_newData);
    cc-=1;
    down(semaforo_done);
  };
};
