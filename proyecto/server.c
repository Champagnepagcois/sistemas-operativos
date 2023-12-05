#include "Server.h"


int main(){
  if(system("ipcrm -a")== -1) ErrorMessage("No se pudo limpiar la MC y semaforos");
  /************** Creamos hilos Endpoints **************/
  pthread_t thread_add,thread_get, thread_update, thread_delete;

  //createThreadPublic(&thread_get,H_getItem,NULL);
  createThreadPublic(&thread_add,H_addItem,NULL);
  //createThreadPublic(&thread_update,H_updateItem,NULL);
  //createThreadPublic(&thread_delete,H_deleteItem,NULL);

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

void *H_getItem(int dataType){
  //Creamos hilos
  pthread_t id_thread;
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  //threadCorrect = pthread_create(&id_thread,&atributos,H_login,NULL/*(void *) 0*/);
  if(threadCorrect !=0) ErrorMessage("\nError al crear hilo\n");
  switch (dataType){
  case USUARIO:
    break;
  
  default:
    break;
  }
};
void *Dispatch_H_getItem(){};

void *H_addItem(){
  struct Request request,requestCopy;
  char clave_addItem = CLAVE_SER_H_ADDITEM;
  //pide MC
  getShmPublic(&request,clave_addItem,TYPEDATA_REQUEST);
  //crea semaforo
  int semaforo_create_dispatcher,semaforo_request, semaforo_done;
  createSemPublic(&semaforo_create_dispatcher,CLAVE_SER_H_ADDITEM,0); //?
  createSemPublic(&semaforo_request,CLAVE_SER_H_ADDITEM,0); //Nueva request
  createSemPublic(&semaforo_done,23,0);
  //definir variable de hilo dispatcher
  pthread_t hilo_dispatcher;
  printf("Servicio AddItem, escuchando....\n");
  
  while (1){
    down(semaforo_request);
    requestCopy.dataType =request.apt_mc_request->dataType;
    requestCopy.ID_usuario = request.apt_mc_request->ID_usuario;
    //printf("dataType:%d\n",requestCopy.dataType);
    //printf("ID_usuario:%d\n\n",requestCopy.ID_usuario);
    createThreadPublic(&hilo_dispatcher,(void *)Dispatch_H_addItem , (void*)&requestCopy);
    //printf("Termino de despachar el hilo\n");
    up(semaforo_done);
  };
};
void *H_updateItem(){};
void *H_deleteItem(){};
void *H_writeFile(){};
void *H_readFile(){};
/****************************************************************/
/*************** Controller Endpoints Request *******************/
/**
 * Recibe parametro de tipo Request
 * Segun la tabla es como atiende
 * pide la memoria compartida
 * Crea semaforos
 * -Semaforo para datosCompartidos  semaforo_newData
 * -semaforo para saber si ya escribio o leyo semaforo_done
 * -semaforo region critica del archivo semaforo_rc_file
 * mienttras haya datos()
 * -copia datos
 * abre archivo
 * los escribe en archivo
 * cierra arcchivo
 * avisa que acabo
 * repite
 * 
*/
void *Dispatch_H_addItem(struct Request *request){
  printf("Nueva conexion con cliente:%d\n", request->ID_usuario);
  //Se copian los datos de la request
  int dataType = request->dataType;
  pid_t ID_cliente = request->ID_usuario;
  struct FileManager fileManager;
  char salida [200];
  switch (dataType){
  case USUARIO:
    char fileName []= {"PRUEBA.txt"};
    fileManager.fileName = fileName;
    //pide MC
    struct Usuario usuario;
    getShmPublic(&usuario,ID_cliente,USUARIO);
    //Crea semaforo
    int semaforo_newData, semaforo_RW_file, semaforo_done;
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_USUARIO,1);  //semaforo para los archivos
    createSemPublic(&semaforo_newData,ID_cliente,0); //Semaforo pasar datos
    createSemPublic(&semaforo_done,ID_cliente*-1,0);  //semaforo informar ya se copio
    //itera por cada dato que pase el cliente
    int count = usuario.apt_mc_usuario->count;
    while (count>0){
      down(semaforo_newData);
      structToString(salida,usuario.apt_mc_usuario,USUARIO);
      down(semaforo_RW_file);//para abrir y escribir en archivo
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);//deja que otro abra el archivo
      printf("new Request/AddItem\n");
      count-=1; //servidor mantiene un registro propia de cuantos inserts lleva
      up(semaforo_done);
    };
    break;
  /*case CATEGORIA:
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
  */default:
    break;
  };
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
  case TYPEDATA_REQUEST:
    struct Request *request = (struct Request*)data;
    memoria = shmget(llave_memoria,sizeof(struct Request), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/TYPEDATAREQUEST");
    request->apt_mc_request = (struct Request*)shmat(memoria,0,0);
    if(request->apt_mc_request == (struct Request*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  //Falta terminar todos las tablas
    default:
    break;
  };
  return;
};

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
//Hilos

void createThreadPublic(pthread_t *ID_thread,void*(*__start_routine)(void *),void *__restrict__ __arg){
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(ID_thread,&atributos,__start_routine, __arg);
  if(threadCorrect != 0) ErrorMessage("Error en la creación del hilo");
  return;
};
/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/
