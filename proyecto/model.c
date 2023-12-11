#include "Model.h"

void new_Ini_User(struct Usuario *user){
  user->ID_usuario = 0;
  user->logged = 0;
  user->nombre[0] = '\0';
  user->password[0]= '\0';
  user->siguiente = NULL;
  user->typeUser = 0;
  user->usuario[0] = '\0';
  return;
};




/****************************************/



void structToStr(struct Query* consulta){
  char *string;
  while (consulta->keyValue->next != NULL){

  };
  
};
/*************Logic query***********/
int QueryInsert(struct Query* consulta){
  char output [500] = {'\0'};
  FILE* file;
  switch (consulta->table){

  case USUARIO:
  break;
  case CATEGORIA:
  /* code */
  break;
  case PRODUCTO:
  /* code */
  break;
  case VENTA:
  /* code */
  break;
  case DETALLEVENTA:
  /* code */
  break;
  case PROVEEDOR:
  /* code */
  break;
  case ADQUISICION:
  /* code */
  break;
  default:
    break;
  }
  return 0;
};

void QuerySelect(struct Request* request,void* data,int dataType){
  request->ID_usuario = getID_Cliente(getpid());
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    int semaforo_new_request,semaforo_newData,semaforo_WR_mc,semaforo_done;
    //pide mc
    getShmPublic(usuario,request->ID_usuario,USUARIO);
    usuario->apt_mc_usuario->count =1;
    getShmPublic(request,CLAVE_SER_H_GETITEM,TYPEDATA_REQUEST);
    //copia datos a la mc
    strcpy(usuario->apt_mc_usuario->usuario,usuario->usuario);
    strcpy(usuario->apt_mc_usuario->password,usuario->password);
    //crea semaforos
    createSemPublic(&semaforo_WR_mc,CLAVE_SER_H_ADDITEM+1,1); //?
    createSemPublic(&semaforo_newData,usuario->ID_usuario,0); //Nueva request
    createSemPublic(&semaforo_done,(usuario->ID_usuario)*-1,0);



    break;
  
  default:
    break;
  };
};
int QueryUpdate(struct Query* consulta){
  return 0;
};
int QueryDelete(struct Query* consulta){
  return 0;
};
/***********************************/

/******************** variado ************************/

void createThreadPublic(pthread_t *ID_thread,void*(*__start_routine)(void *),void *__restrict__ __arg){
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(ID_thread,&atributos,__start_routine, __arg);
  if(threadCorrect != 0) ErrorMessage("Error en la creación del hilo");
  return;
};

int getID_Cliente(pid_t id_cliente){
  return id_cliente%2==0?(id_cliente>32767?getID_Cliente(id_cliente/2):id_cliente):id_cliente>32767?getID_Cliente(id_cliente/3):id_cliente;
};

/*************************/

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
  case CATEGORIA:
    struct Categoria *categoria = (struct Categoria*)data;
    memoria = shmget(llave_memoria, sizeof(struct Categoria), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/CATEGORIA");
    categoria->apt_mc_categoria = (struct Categoria*)shmat(memoria,0,0);
    if(categoria->apt_mc_categoria == (struct Categoria*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/CATEGORIA");
    break;
  //Falta terminar todos las tablas
    default:
    break;
  };
  return;
};

/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/

