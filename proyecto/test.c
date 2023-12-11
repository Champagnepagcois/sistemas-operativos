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


#define DATATYPE_INT    840
#define DATATYPE_CHAR    841

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
  char password [20];
  int typeUser; // lon 2
  int logged;   // 1on 1
  struct Usuario* siguiente;
  struct Usuario* apt_mc_usuario;
  int count;
};
struct FileManager{
  char *fileName;
  FILE *file;
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
int getShmPublic(void *data,char clave,int typeData){ //Falta terminar
  key_t llave_memoria = ftok("archivo",clave);
  int memoria;
  switch (typeData){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    memoria = shmget(llave_memoria, sizeof(struct Usuario), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/USUARIO");
    usuario->apt_mc_usuario = (struct Usuario*)shmat(memoria,0,0);
    if(usuario->apt_mc_usuario == (struct Usuario*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/USUARIO");
    return memoria;
    break;
  case TYPEDATA_REQUEST:
    struct Request *request = (struct Request*)data;
    memoria = shmget(llave_memoria, sizeof(struct Request), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/REQUEST");
    request->apt_mc_request = (struct Request*)shmat(memoria,0,0);
    if(request->apt_mc_request == (struct Request*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/REQUEST");
    return memoria;
    break;
    default:
    break;
  };
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
/************************* Archivos *****************************/
void openFile(struct FileManager *fileManager){
  fileManager->file = fopen(fileManager->fileName,"a+");
  if(fileManager->file == NULL) ErrorMessage("No se pudo abrir el archivo");
};

void closeFile(struct FileManager *fileManager){
  fclose(fileManager->file);
};
/****************************************************************/
//Helpers
void typeDataToString(char *salida, void *data, int dataType, int lonMax){
  char bufferTmp [lonMax+1];
  switch (dataType){
  case DATATYPE_INT:
    //printf("%d\n",*((int*)data));
    sprintf(bufferTmp, "%0*d",lonMax+1,*((int*)data));
    strcat(salida,bufferTmp);
    break;
  case DATATYPE_CHAR:
  //printf("%s\n",data);
    snprintf(bufferTmp,sizeof(bufferTmp), "%-*s*",lonMax+1,data);
    strcat(salida,bufferTmp);
    break;
  default:
    break;
  };
  return;
};

void structToString(char* apt_salida,void *data,int dataType){
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    apt_salida[0]='\0';
    //sprintf(apt_salida,"%s\t%s\t%d\n",usuario->nombre,usuario->password,usuario->typeUser);
    typeDataToString(apt_salida,&(usuario->ID_usuario),DATATYPE_INT,6);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->nombre,DATATYPE_CHAR,15);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->usuario,DATATYPE_CHAR,25);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->password,DATATYPE_CHAR,20);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(usuario->typeUser),DATATYPE_INT,2);
    strcat(apt_salida,"\n");
    break;
  default:
    printf("Salu2");
    break;
  }
  return;
};

/****************************************************************/

int getID_Cliente(pid_t id_cliente){
  return id_cliente%2==0?(id_cliente>32767?getID_Cliente(id_cliente/2):id_cliente):id_cliente>32767?getID_Cliente(id_cliente/3):id_cliente;
};

void searchInDoc(struct FileManager *filemanager,const char *palabra,int longitudLinea,struct Usuario *usuario){
  printf("Bug0\n");
  openFile(filemanager);
  char salida[200];
  char linea[longitudLinea];
  struct Usuario tmpUsuario;
  char lineaa[6];
  
  while(fgets(linea,sizeof(linea),filemanager->file) != NULL){
    sscanf(linea,"%d\t%s\t%s\t%s\t%d",
    &(tmpUsuario.ID_usuario),tmpUsuario.nombre,tmpUsuario.usuario,tmpUsuario.password,&(tmpUsuario.typeUser));
    if(strcmp(tmpUsuario.usuario,palabra) == 0){
      printf("|->%s",linea);
    }
  };
  closeFile(filemanager);  
  return;
};

int main(){
  struct FileManager filemanager;
  char filename []= {"usuario.txt"};

  filemanager.fileName =filename;

  struct Usuario usuario2;
  strcat(usuario2.usuario,"2");
  searchInDoc(&filemanager,"champa",75,&usuario2);

  int ID_cliente = getID_Cliente(getpid());
  /********* Pide canal de comunicacion para pasar datos **********/
  int ID_Shm [2];

  struct Request request;
  char clave_addItem = CLAVE_SER_H_ADDITEM;
  //getMemoria compartida
  ID_Shm[0] = getShmPublic(&request,clave_addItem,TYPEDATA_REQUEST); //MC de request
  //semaforo
  int semaforo_new_request;
  createSemPublic(&semaforo_new_request,CLAVE_SER_H_ADDITEM,0);

  request.apt_mc_request->dataType = USUARIO;
  request.apt_mc_request->ID_usuario = ID_cliente;
  //Hacemos una nueva request
  up(semaforo_new_request);



  /****************************************************************/
  /*************** Pasa los datos de la request ******************/

  /****************************************************************/
  struct Usuario usuario, usuarioTmp;
  //pide MC
  ID_Shm[1]=getShmPublic(&usuario,ID_cliente,USUARIO);
  //Escribo la usuario

  usuario.apt_mc_usuario->count =2;

  //crea SEMAFORO
  int semaforo_newData,semaforo_WR_mc, semaforo_done;
  createSemPublic(&semaforo_WR_mc,CLAVE_SER_H_ADDITEM+1,1); //?
  createSemPublic(&semaforo_newData,ID_cliente,0); //Nueva request
  createSemPublic(&semaforo_done,ID_cliente*-1,0);

  //definir variable de hilo dispatcher
  //pthread_t hilo_dispatcher;

  int cc=usuario.apt_mc_usuario->count;

  while (cc>0){
    //usuarioTmp.nombre[0] = '\0';
    printf("Ingresa tu nombre:");
    scanf("%15s",usuarioTmp.nombre);
    printf("Ingresa el usuario:");
    scanf("%15s",usuarioTmp.usuario);
    printf("Ingresa el password:");
    scanf("%s",usuarioTmp.password);
    printf("%d-\n",0);
    printf("Nombre antes MC: %s\n",usuario.apt_mc_usuario->nombre);
    down(semaforo_WR_mc);//escribe en MC
    usuario.apt_mc_usuario->ID_usuario= cc;
    usuario.apt_mc_usuario->typeUser =1;
    strcpy(usuario.apt_mc_usuario->nombre,usuarioTmp.nombre);
    strcpy(usuario.apt_mc_usuario->usuario,usuarioTmp.usuario);
    strcpy(usuario.apt_mc_usuario->password,usuarioTmp.password);
    up(semaforo_WR_mc);//escribe en MC
    printf("Nombre despues MC: %s\n",usuario.apt_mc_usuario->nombre);
    printf("Termino de escribir..\n");
    up(semaforo_newData);
    cc-=1;
    down(semaforo_done);
  };
  shmctl(ID_Shm[0], IPC_RMID, NULL);//Eliminar MC
  shmctl(ID_Shm[1], IPC_RMID, NULL);
  semctl(semaforo_done,0,IPC_RMID);//Elimina los semaforos
  //semctl(semaforo_new_request,0,IPC_RMID);
  semctl(semaforo_newData,0,IPC_RMID);
  semctl(semaforo_WR_mc,0,IPC_RMID);
};
