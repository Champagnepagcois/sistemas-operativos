#include "Server.h"

int main(){
  ///initAllSemaphores();//?
  //Creamos hilos
  pthread_t id_thread;
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(&id_thread,&atributos,H_login,NULL/*(void *) 0*/);
  if(threadCorrect !=0){
    perror("\nError al crear hilo\n");
    exit(-1);
  };
  while (1)
  {
    /* code */
  }
  
  return 0;
};


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
void initSemaphore(key_t *llave,int *semaforo,int *clave){
  *llave = ftok("archivo",*clave);
  *semaforo = Crea_semaforo(*llave,1);
};
/****************************************************************/


/******************* Endpoints Request ***********************/
void *H_login(){
  printf("Activando servicio de login\n");
  struct Usuario usuario;
  struct Usuario *apt_usuario_mc= &usuario;
  key_t llave_serv_H_IO,llave_serv_H_Req;
  int semaforo__h__login_IO, semaforo_h_login_Req;
  int clave = 12;
  int count =0;
  int *apt_cout =&count;
  initSemaphore(&llave_serv_H_IO,&semaforo__h__login_IO,&clave);
  initSemaphore(&llave_serv_H_Req,&semaforo_h_login_Req,&clave);
  getShmLogin(apt_usuario_mc,CLAVE_SER_H_LOGIN_MC);
  getShmRequest(apt_cout,CLAVE_SER_H_LOGIN_MC_REQ);
  printf("Servicios activados, login escuchando...\n");
  while(1){
    down(semaforo_h_login_Req);
    //Hay peticion nueva?
    //->Atiende peticion.
    //Libera semaforo MCUsuario
    up(semaforo_h_login_Req);
  };
};
void *H_getItem(){};
void *H_addItem(){};
void *H_updateItem(){};
void *H_deleteItem(){};
void *H_writeFile(){};
void *H_readFile(){};
/****************************************************************/
/******************** Memoria Compartida ************************/
void getShmRequest(int *count, char clave){
  key_t llave_memoria = ftok("archivo", clave);
  int memoria  = shmget(llave_memoria,sizeof(int), IPC_CREAT | PERMISOS);
  if(memoria ==-1) ErrorMessage("Error al pedir region en la memoria compartida de X/Request");
  count = (int *)shmat(memoria,0,0);
  if(count == (int *)(-1)) ErrorMessage("Error al signar la memoria compartida al apuntador de X/Request");
};
void getShmLogin(struct Usuario *usuario,char clave){
  int memoria;
  int *dato_memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);
  memoria = shmget(llave_memoria,sizeof(struct Usuario), IPC_CREAT | PERMISOS);
  if (memoria == -1) ErrorMessage("Error al pedir region en la memoria compartida de login");
  usuario->apt_mc_usuario = (struct Usuario *)shmat(memoria,0,0);
  if(usuario->apt_mc_usuario ==(struct Usuario *)(-1))  ErrorMessage("\nError al signar la memoria compartida al apuntador de login");
  return;
};
/****************************************************************/
/************************* Archivos *****************************/
void openFile(struct FileManager *fileManager){
  fileManager->file = fopen(fileManager->fileName,"a+");
  if(fileManager->file == NULL) ErrorMessage("No se pudo abrir el archivo");
};

void closeFile(struct FileManager *fileManager){
  fclose(fileManager->file);
};
/****************************************************************/

/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/
