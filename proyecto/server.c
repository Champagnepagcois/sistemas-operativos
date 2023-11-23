#include "Server.h"

int main(){
  const char *comando = "ipcrm -a";
  int resultado  = system(comando);
  if(resultado== -1) ErrorMessage("No se pudo limpiar la MC y semaforos");
  //Creamos hilos
  pthread_t id_thread1,id_thread2;
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(&id_thread1,&atributos,H_login,NULL/*(void *) 0*/); //se crea hilo
  if(threadCorrect !=0) ErrorMessage("\nError al crear hilo\n");
  //otro hilo
  threadCorrect = pthread_create(&id_thread2,&atributos,H_addItem,NULL/*(void *) 0*/); //se crea hilo
  if(threadCorrect !=0) ErrorMessage("\nError al crear hilo\n");
  while (1){
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
void createSemPublic(int *semaforo,int clave, int valor){
  key_t llave = ftok("archivo",clave);
  *semaforo = Crea_semaforo(llave,valor);
};
int getValueSemaphore(int *id){
  return semctl(*id,0,GETVAL,0);
};
/****************************************************************/


/******************* Endpoints Request ***********************/
void *H_login(){
  printf("Activando servicio de login\n");
  struct Usuario usuario;
  key_t llave_serv_H_login,llave_serv_H_login_Req;
  int semaforo__h__login, semaforo_h_login_Req;
  int clave = 12; //Cambiar clave
  int count =0;
  //initSemaphore(&llave_serv_H_login,&semaforo__h__login,&clave);
  //initSemaphore(&llave_serv_H_login_Req,&semaforo_h_login_Req,&clave);
  getShmLogin(&usuario,CLAVE_SER_H_LOGIN_MC);
  getShmRequest(&count,CLAVE_SER_H_LOGIN_MC_REQ);
  printf("Servicios activados, servicio login escuchando...\n");
  strcat(usuario.nombre,"\0");
  strcat(usuario.password,"\0");
  strcat(usuario.nombre,"Marlon");
  strcat(usuario.password,"1234");
  H_addItem(&usuario,USUARIO); // de prueba
  //Lo que realmente va a ejecutar
  /*while(1){
    if(getValueSemaphore(&semaforo_h_login_Req)==1){
      dispatch_H_login(&usuario);
      down(semaforo_h_login_Req);
    };
  };*/
};

void *H_getItem(int dataType){
  //Creamos hilos
  pthread_t id_thread;
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(&id_thread,&atributos,H_login,NULL/*(void *) 0*/);
  if(threadCorrect !=0) ErrorMessage("\nError al crear hilo\n");
  switch (dataType){
  case USUARIO:
    break;
  
  default:
    break;
  }
};
void *Dispatch_H_getItem(){};

void *H_addItem(void *data,int dataType){

  /**
   * No recibe argumentos
   * crea hilos que van a atender
   * pide memoria compartida donde observa si llego algun request
   * Crea el semaforo para avisar si esta ocupando un hiloDispatcher
   * este hilo esta pendiente de algun request de tipo addItem
   * si llega lgun request cre al hilo para atenderlo 
  */
  struct FileManager fileManager;
  char salida [200];
  fileManager.fileName[0]='\0';
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    strcat(fileManager.fileName,"usuario.txt");
    structToString(salida,usuario,USUARIO);
    //Semaforo;
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //Semaforo;
    break;
  case CATEGORIA:
    struct Categoria *categoria = (struct Categoria*)data;
    strcat(fileManager.fileName,"categoria.txt");
    structToString(salida,categoria,CATEGORIA);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break;
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)data;
    strcat(fileManager.fileName,"producto.txt");
    structToString(salida,producto,PRODUCTO);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)data;
    strcat(fileManager.fileName,"venta.txt");
    structToString(salida,venta,VENTA);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleVenta = (struct DetalleVenta*)data;
    strcat(fileManager.fileName,"detalleVenta.txt");
    structToString(salida,detalleVenta,DETALLEVENTA);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break; 
  case PROVEEDOR:
    struct Proveedor *proveedor = (struct Proveedor*)data;
    strcat(fileManager.fileName,"proveedor.txt");
    structToString(salida,proveedor,PROVEEDOR);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break; 
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)data;
    strcat(fileManager.fileName,"adquisicion.txt");
    structToString(salida,adquisicion,ADQUISICION);
    //semaforo
    openFile(&fileManager);
    fprintf(fileManager.file,"%s",salida);
    closeFile(&fileManager);
    //semaforo
    break;  
  default:
    break;
  };
};
void *H_updateItem(){};
void *H_deleteItem(){};
void *H_writeFile(){};
void *H_readFile(){};
/****************************************************************/
/*************** Controller Endpoints Request *******************/
void *dispatch_H_addItem(pid_t ID_client, int dataType){
  int semaforo_mutex,semaforo_LE;
  switch (dataType){
  case USUARIO:
    struct Usuario usuario;
    //Pedimos la memoria compartida para usuario
    getShmPublic(&usuario,ID_client,USUARIO);
    //hacemos semaforo
    createSemPublic(&semaforo_mutex,ID_client,0);
    createSemPublic(&semaforo_LE,ID_client,1);
    //mientras las queries sean mayor de 0
    while (usuario.apt_mc_usuario->count > 0){
      if(getValueSemaphore(&semaforo_mutex) == 0){ // Termino de escribir el cliente
        //semafo_LE down
        down(semaforo_LE);
        //Copia datos de la estructura
        /*********/
        strcpy(usuario.nombre,usuario.apt_mc_usuario->nombre);
        strcpy(usuario.password,usuario.apt_mc_usuario->password);
        usuario.typeUser =usuario.apt_mc_usuario->typeUser;
        /*********/
        //semaforo_LE up
        up(semaforo_LE);
        //Le hace saber al clienteque ya copiamos los datos
        up(semaforo_mutex);
      };      
    };
    
    break;
  
  default:
    break;
  };
  //iniciar semaforos del cliente para saber si ya terminode escribir y avisarle que ya terminaste de escribir
  //semaforo para leer o escribir en un archivo especifico
};

void  dispatch_H_login(struct Usuario *usuario){
  struct FileManager fileManager;
  char *username = usuario->apt_mc_usuario->usuario;
  char *password = usuario->apt_mc_usuario->password;
  openFile(&fileManager);
  //searchInDoc(&fileManager,usuario,usuario,usuario);
  closeFile(&fileManager);

  return;
};
//Busca en el archivo y rellena la estructura en caso de que si exista alguna coincidencia
void searchInDoc(struct FileManager *file){
  //Buscar dato en en 
  return;
};

/****************************************************************/

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
}

void getShmRequest(int *count, char clave){
  key_t llave_memoria = ftok("archivo", clave);
  int memoria  = shmget(llave_memoria,sizeof(int), IPC_CREAT | PERMISOS);
  if(memoria ==-1) ErrorMessage("Error al pedir region en la memoria compartida de X/Request");
  count = (int *)shmat(memoria,0,0);
  if(count == (int *)(-1)) ErrorMessage("Error al signar la memoria compartida al apuntador de X/Request");
};
void getShmLogin(struct Usuario *usuario,char clave){
  key_t llave_memoria = ftok("archivo",clave);
  int memoria = shmget(llave_memoria,sizeof(struct Usuario), IPC_CREAT | PERMISOS);
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


//Helpers

void structToString(char* apt_salida,void *data,int dataType){
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    apt_salida[0]='\0';
    sprintf(apt_salida,"%s\t%s\t%d\n",usuario->nombre,usuario->password,usuario->typeUser);
    /*strcpy(*apt_salida,usuario->nombre);
    strcpy(*apt_salida,"\t");
    strcpy(*apt_salida,usuario->password);//Cambiar por modulo que cifra
    strcpy(*apt_salida,"\t");
    sprintf(*apt_salida,"%d",usuario->typeUser);*/
    break;
  default:
    break;
  }
  return;
};

/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/
