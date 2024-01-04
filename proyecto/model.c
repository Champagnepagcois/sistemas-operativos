#include "Model.h"

void new_Ini_User(struct Usuario *user){
  user->ID_usuario[0] = '\0';
  user->logged = 0;
  user->nombre[0] = '\0';
  user->password[0]= '\0';
  user->siguiente = NULL;
  user->typeUser = 0;
  user->usuario[0] = '\0';
  return;
};
/*************** QUEUE LOGIC ***************/


/****************************************/

void structToStr(struct Query* consulta){
  char *string;
  while (consulta->keyValue->next != NULL){

  };
  
};
/*************Logic query***********/
//int QueryInsert(struct Request* request,void* data,void **nodo,int dataType){
int QueryInsert(struct Request* request,void **nodo,int dataType){
  //obtener id del cliente
  request->ID_usuario = getID_Cliente(getpid());
  switch(dataType){
  case USUARIO:
  break;
  case CATEGORIA:
  /* code */
  break;
  case PRODUCTO:
  struct Producto producto;// = (struct Producto*)data;
  /************ Preparamos Request *************/
  //Pedimos mc para request
  getShmPublic(request,CLAVE_SER_H_ADDITEM,TYPEDATA_REQUEST);
  int semaforo_new_request;
  createSemPublic(&semaforo_new_request,CLAVE_SER_H_ADDITEM,0);
  //Asignamos valores a la request
  request->dataType = PRODUCTO;
  request->apt_mc_request->dataType = request->dataType;
  request->apt_mc_request->ID_usuario = request->ID_usuario;
  //Hacemos peticion.
  up(semaforo_new_request);
  /********************************************/
  /************* Preparamos datos *************/
  //Pide mc
  getShmPublic(&producto,request->ID_usuario,PRODUCTO);
  int count = getNumberItemQueue(*nodo,PRODUCTO);
  producto.apt_mc_producto->count =count;
  //Creamos los semafotos 
  int semaforo_newData,semaforo_WR_mc,semaforo_done;
  createSemPublic(&semaforo_newData,request->ID_usuario,0);
  createSemPublic(&semaforo_done,(request->ID_usuario)*-1,0);
  down(semaforo_done);
  while(count>0){
    //Agregar a mc los datos de la cola
    deQueue((void*)producto.apt_mc_producto,nodo,PRODUCTO);
    up(semaforo_newData);
    count-=1;
    down(semaforo_done);
  };
  freeShmPublic(&producto,request->ID_usuario,PRODUCTO);
  deleteSemPublic(&semaforo_newData);
  deleteSemPublic(&semaforo_done);
  /********************************************/
 
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

void QuerySelect(struct Request* request,void **nodo,int dataType){
  //obtener id del cliente
  request->ID_usuario = getID_Cliente(getpid());
  switch (dataType){
  case USUARIO:
    /******************* Prepara Request **********************/
    //pedir mc para Request
    getShmPublic(request,CLAVE_SER_H_GETITEM,TYPEDATA_REQUEST);
    //Creamos semaforo para avisarle al servidor que hay una nueva Request
    int semaforo_new_request;
    createSemPublic(&semaforo_new_request,CLAVE_SER_H_GETITEM,0);
    //Asignamos valores en la request y  mc
    request->dataType = USUARIO;
    request->ID_usuario = getID_Cliente(getpid());
    request->apt_mc_request->dataType = request->dataType;
    request->apt_mc_request->ID_usuario = request->ID_usuario;
    //Lanzamos la Request
    up(semaforo_new_request);
    /**********************************************************/
    /******************* Prepara Data **********************/
    struct Usuario usuario;
    //Pedimos mc
    getShmPublic(&usuario,request->ID_usuario,USUARIO);
    //Creamos semaforos
    int semaforo_newData,semaforo_done;
    createSemPublic(&semaforo_newData,request->ID_usuario,0);
    createSemPublic(&semaforo_done,(request->ID_usuario)*-1,0);

    //Escribimos los datos 
    deQueue(usuario.apt_mc_usuario,nodo,USUARIO);
    //Espera a que se termine de configurar el servidor
    down(semaforo_done);
    sleep(0.09);
    //Le decimos al servidor que ya escribimos
    up(semaforo_done);
    sleep(0.1);
    down(semaforo_newData);
    //Logica de pasar datos
    int firstData =1;
    int count = usuario.apt_mc_usuario->count;
    //Validamos caso en el que no hay ninguna coincidencia
    struct Usuario *userNodo = (struct Usuario*)*nodo;
    if(count==0) userNodo->siguiente->logged=0; //usuario->logged=0;
    while(count>0){
      if(firstData==0){
        down(semaforo_newData);
      };
      //Agregamos dato a una cola
      enQueue(nodo,usuario.apt_mc_usuario,USUARIO);
      //Actualizamos valor de count si ya se acabo no entramos al down!!
      count-=1;
      //Le decimos al servidor que hemos terminado de leer
      up(semaforo_done);
    };
    //deQueue(data,nodo,USUARIO);
    freeShmPublic(&usuario,request->ID_usuario,USUARIO);
    deleteSemPublic(&semaforo_newData);
    deleteSemPublic(&semaforo_done);
    return;
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
void deleteSemPublic(int *semaforo){
  if(semctl(*semaforo,0,IPC_RMID)==-1){
    ErrorMessage("Error al eliminar el semaforo");
  };
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
  case TYPEDATA_REQUEST:
    struct Request *request = (struct Request*)data;
    memoria = shmget(llave_memoria,sizeof(struct Request), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/TYPEDATAREQUEST");
    request->apt_mc_request = (struct Request*)shmat(memoria,0,0);
    if(request->apt_mc_request == (struct Request*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)data;
    memoria = shmget(llave_memoria,sizeof(struct Producto), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/PRODUCTO");
    producto->apt_mc_producto = (struct Producto*)shmat(memoria,0,0);
    if(producto->apt_mc_producto == (struct Producto*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/PRODUCTO");
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)data;
    memoria = shmget(llave_memoria,sizeof(struct Venta), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/VENTA");
    venta->apt_mc_venta = (struct Venta*)shmat(memoria,0,0);
    if(venta->apt_mc_venta == (struct Venta*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/VENTA");
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleventa = (struct DetalleVenta*)data;
    memoria = shmget(llave_memoria,sizeof(struct DetalleVenta), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/DETALLEVENTA");
    detalleventa->apt_mc_detalleventa = (struct DetalleVenta*)shmat(memoria,0,0);
    if(detalleventa->apt_mc_detalleventa == (struct DetalleVenta*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/DETALLEVENTA");
    break;
  case PROVEEDOR:
    struct Proveedor *proveedor = (struct Proveedor*)data;
    memoria = shmget(llave_memoria,sizeof(struct Proveedor), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/PROVEEDOR");
    proveedor->apt_mc_proveedor = (struct Proveedor*)shmat(memoria,0,0);
    if(proveedor->apt_mc_proveedor == (struct Proveedor*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/PROVEEDOR");
    break;
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)data;
    memoria = shmget(llave_memoria,sizeof(struct Adquisicion), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/ADQUISICION");
    adquisicion->apt_mc_adquisicion = (struct Adquisicion*)shmat(memoria,0,0);
    if(adquisicion->apt_mc_adquisicion == (struct Adquisicion*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/ADQUISICION");
    break;
    default:
    break;
  };
  return;
};

void freeShmPublic(void *apt,char clave,int dataType){
  key_t llave_memoria = ftok("archivo",clave);
  int memoria;
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)apt;
    memoria = shmget(llave_memoria, sizeof(struct Usuario), IPC_CREAT | PERMISOS);
    shmdt(usuario);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case CATEGORIA:
    struct Categoria *categoria = (struct Categoria*)apt;
    memoria = shmget(llave_memoria, sizeof(struct Categoria), IPC_CREAT | PERMISOS);
    shmdt(categoria);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case TYPEDATA_REQUEST:
    struct Request *request = (struct Request*)apt;
    memoria = shmget(llave_memoria,sizeof(struct Request), IPC_CREAT | PERMISOS);
    shmdt(request);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)apt;
    memoria = shmget(llave_memoria,sizeof(struct Producto), IPC_CREAT | PERMISOS);
    shmdt(producto);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)apt;
    memoria = shmget(llave_memoria,sizeof(struct Venta), IPC_CREAT | PERMISOS);
    shmdt(venta);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleventa = (struct DetalleVenta*)apt;
    memoria = shmget(llave_memoria,sizeof(struct DetalleVenta), IPC_CREAT | PERMISOS);
    shmdt(detalleventa);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case PROVEEDOR:
    struct Proveedor *proveedor = (struct Proveedor*)apt;
    memoria = shmget(llave_memoria,sizeof(struct Proveedor), IPC_CREAT | PERMISOS);
    shmdt(proveedor);
    shmctl(memoria,IPC_RMID,NULL);
    break;
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)apt;
    memoria = shmget(llave_memoria,sizeof(struct Adquisicion), IPC_CREAT | PERMISOS);
    shmdt(adquisicion);
    shmctl(memoria,IPC_RMID,NULL);
    break;
    default:
    break;
  };
  return;
};
/********************** Estructura de datos ********************/
void enQueue(void **nodo, void *data,int dataType){
  switch (dataType){
  case USUARIO:{
    struct Usuario *newnodo = (struct Usuario*)*nodo;
    if((*nodo)==NULL){
      struct Usuario *usuario = (struct Usuario*)data;
      struct Usuario *nuevoNodo = malloc(sizeof(struct Usuario));
      structToStruct(nuevoNodo,usuario,USUARIO);
      nuevoNodo->siguiente = NULL;
      *nodo = nuevoNodo;
    }else{
      enQueue((void**)&newnodo->siguiente,data,dataType);
    };
    break;}
  case PRODUCTO:{
    struct Producto *newnodo = (struct Producto*)*nodo;
    if((*nodo)==NULL){
      struct Producto *producto = (struct Producto*)data;
      struct Producto *nuevoNodo = malloc(sizeof(struct Producto));
      structToStruct(nuevoNodo,producto,PRODUCTO);
      nuevoNodo->siguiente = NULL;
      *nodo = nuevoNodo;
    }else{
      enQueue((void**)&newnodo->siguiente,data,dataType);
    };
    break;}
  
  default:
    break;
  }
  return;
};

void deQueue(void *destino,void **nodo,int dataType){
  //printf("\nEntro a DeQueue\n");
  switch (dataType){
  case USUARIO:{
    struct Usuario *primerNodo = (struct Usuario*)*nodo;
    if(primerNodo !=NULL){
      structToStruct(destino,primerNodo,USUARIO);
      *nodo = primerNodo->siguiente;
      free(primerNodo);
    };
    break;}
  case PRODUCTO:{
    struct Producto *primerNodo = (struct Producto*)*nodo;
    if(primerNodo !=NULL){
      structToStruct(destino,primerNodo,PRODUCTO);
      *nodo = primerNodo->siguiente;
      *nodo = primerNodo->siguiente;
      free(primerNodo);
    };
    break;}
  default:
    break;
  };
};
int getNumberItemQueue(void *queue, int dataType){
  int contador=0;
  switch (dataType){
  case USUARIO:{
    struct Usuario *nodo = (struct Usuario*)queue;
    while(nodo !=NULL){
      contador++;
      nodo = nodo->siguiente;
    };
    break;}
  case PRODUCTO:{
    struct Producto *nodo = (struct Producto*)queue;
    while(nodo !=NULL){
      contador++;
      nodo = nodo->siguiente;
    };
    break;}
  
  default:
    break;
  };
  return contador;
};
void imprimirCola(void *nodo, int dataType) {
  char salida[200];
  switch (dataType) {
    case USUARIO: {
      struct Usuario *currentNode = (struct Usuario *)nodo;
      //printf("Contenido de la cola:\n");
      while (currentNode != NULL) {
        // Imprimir los datos del nodo actual
        //printf("\n\nData:\n");
        //structToString(salida,currentNode,USUARIO);
        printf("%s | %s | [%s] | %s",currentNode->nombre,currentNode->usuario,currentNode->ID_usuario,currentNode->password);
        //printf("\n%s\n",salida);
        // Agrega más campos según la estructura de tu Usuario
        // Avanzar al siguiente nodo
        currentNode = currentNode->siguiente;
      };
      break;
    }
    case PRODUCTO:{
      struct Producto *currentNode = (struct Producto *)nodo;
      //printf("Contenido de la cola:\n");
      while (currentNode != NULL) {
        // Imprimir los datos del nodo actual
        printf("Data:\n");
        //structToString(salida,currentNode,PRODUCTO);
        printf("%s | %s | %d",currentNode->nombre,currentNode->descripcion,currentNode->existencia);
        //printf("%s\n",salida);
        // Avanzar al siguiente nodo
        currentNode = currentNode->siguiente;
      };
      break;
    }
    default:
      break;
  };
};


void structToStruct(void *dest,void *src, int dataType){
  switch (dataType){
  case USUARIO:{
    struct Usuario *destino = (struct Usuario*)dest;
    struct Usuario *fuente = (struct Usuario*)src;
    //destino->ID_usuario = fuente->ID_usuario;
    strcpy(destino->ID_usuario, fuente->ID_usuario);
    strcpy(destino->nombre,fuente->nombre);
    strcpy(destino->usuario,fuente->usuario);
    strcpy(destino->password,fuente->password);
    destino->typeUser = fuente->typeUser;
    destino->logged = fuente->logged;
    break;}
  case PRODUCTO:
    struct Producto *destino = (struct Producto*)dest;
    struct Producto *fuente = (struct Producto*)src;
    strcpy(destino->ID_producto,fuente->ID_producto);
    strcpy(destino->nombre,fuente->nombre);
    destino->cantidad = fuente->cantidad;
    strcpy(destino->descripcion,fuente->descripcion);
    destino->precio = fuente->precio;
    strcpy(destino->ID_categoria,fuente->ID_categoria);
    destino->existencia = fuente->existencia;
    break;
  default:
    break;
  }
  return;
};

void structToString(char* apt_salida,void *data,int dataType){
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    apt_salida[0]='\0';
    //typeDataToString(apt_salida,usuario->ID_usuario,DATATYPE_ID,16);
    typeDataToString(apt_salida,usuario->ID_usuario,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->nombre,DATATYPE_CHAR,15);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->usuario,DATATYPE_CHAR,25);
    strcat(apt_salida,"\t");
    //cifrar(usuario->password);
    typeDataToString(apt_salida,usuario->password,DATATYPE_CHAR,20);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(usuario->typeUser),DATATYPE_INT,1);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(usuario->logged),DATATYPE_INT,1);
    strcat(apt_salida,"\n");
    break;
  case CATEGORIA:
    struct Categoria *categoria = (struct Categoria*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,categoria->ID_categoria,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,categoria->descripcion,DATATYPE_CHAR,25);
    strcat(apt_salida,"\n");
    break;  
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,producto->ID_producto,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->nombre,DATATYPE_CHAR,20);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->descripcion,DATATYPE_CHAR,50);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(producto->precio),DATATYPE_LD,10);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(producto->existencia),DATATYPE_INT,8);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->ID_categoria,DATATYPE_CHAR,16);
    strcat(apt_salida,"\n");
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,venta->ID_venta,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,venta->fecha,DATATYPE_CHAR,19);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,venta->metodo,DATATYPE_CHAR,12);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(venta->isr),DATATYPE_LD,0);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(venta->monto_total),DATATYPE_LD,0);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,venta->ID_usuario,DATATYPE_CHAR,16);
    strcat(apt_salida,"\n");
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleventa = (struct DetalleVenta*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,detalleventa->ID_detalleventa,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(detalleventa->precio_unitario),DATATYPE_LD,0);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(detalleventa->descuento),DATATYPE_INT,2);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(detalleventa->cantidad),DATATYPE_INT,6);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(detalleventa->ID_producto),DATATYPE_CHAR,0);
    strcat(apt_salida,"\n");
    break;
  case PROVEEDOR:
    struct Proveedor *proveedor = (struct Proveedor*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,proveedor->ID_proveedor,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,proveedor->descripcion,DATATYPE_CHAR,150);
    strcat(apt_salida,"\n");
    break;
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,adquisicion->ID_provedor,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,adquisicion->ID_producto,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,adquisicion->ID_usuario,DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,adquisicion->fecha,DATATYPE_CHAR,19);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(adquisicion->cantidad),DATATYPE_INT,0);
    strcat(apt_salida,"\n");
    break;
  default:
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
  case DATATYPE_LD://agregar constante
    char bufferld [10];
    snprintf(bufferld,sizeof(bufferld),"%09.2Lf",*((long double*)data));
    strcat(salida,bufferld);
    break;
  default:
    break;
  };
  return;
};

/***************************************************************/
/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/

