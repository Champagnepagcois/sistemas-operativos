#include "Server.h"

int main(){
  initAllSemaphores();//?
  
  return 0;
};

void initAllSemaphores(){ //?
  key_t llave_servidor;
  int semaforo_request;
  llave_servidor = ftok("archivo",'a');
  semaforo_request = Crea_semaforo(llave_servidor,1);
};

  
void initSemaphore(key_t *llave_serv_IO,int *semaforoIO,int *clave){

}

void *H_login(struct Usuario *usuario){
  printf("Activando servicio de login\n");
  key_t llave_serv_IO [7];
  int semaforo_IO [7];
  int clave = 12;
  int *apt_clave = &clave;
  initSemaphore(llave_serv_IO,semaforo_IO,clave);
  getShmLogin(usuario,'q');
  printf("Servicios activados, servicio listo.....\n");
  while(1){
    down(semaforo_IO);
    //verificar si hay alguna peticion
    //Atender peticion
    up(semaforo_IO);
  };
};
void *H_getItem(){};
void *H_addItem(){};
void *H_updateItem(){};
void *H_deleteItem(){};
void *H_writeFile(){};
void *H_readFile(){};




void openFile(struct FileManager *fileManager){
  fileManager->file = fopen(fileManager->fileName,"a+");
  if(fileManager->file == NULL) ErrorMessage("No se pudo abrir el archivo");
};
void closeFile(struct FileManager *fileManager){
  fclose(fileManager->file);
};


void getShmLogin(struct Usuario *usuario,int clave){
  int memoria;
  int *dato_memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);
  memoria = shmget(llave_memoria,sizeof(struct Usuario), IPC_CREAT | PERMISOS);
  if (memoria == -1) ErrorMessage("Error al pedir region en la memoria compartida de login");
  usuario->apt_mc_usuario = (struct Usuario *)shmat(memoria,0,0);
  if(usuario->apt_mc_usuario ==(struct Usuario *)(-1))  ErrorMessage("\nError al signar la memoria compartida al apuntador de login");
};

void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
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
/****************************************************************/