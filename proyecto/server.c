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

void *H_getItem(){
  struct Request request,requestCopy;
  char clave_getItem = CLAVE_SER_H_GETITEM;
  getShmPublic(&request,clave_getItem,TYPEDATA_REQUEST);
  int semaforo_create_dispatcher, semaforo_request, semaforo_done;
  createSemPublic(&semaforo_create_dispatcher,CLAVE_SER_H_GETITEM,0);
  createSemPublic(&semaforo_request,CLAVE_SER_H_GETITEM,0);
  createSemPublic(&semaforo_done,CLAVE_SER_H_GETITEM,0);
  pthread_t hilo_dispatcher;
  printf("Servicio getItem, escuchando....\n");
  while(1){
    down(semaforo_request);
    requestCopy.dataType = request.apt_mc_request->dataType;
    requestCopy.ID_usuario = request.apt_mc_request->ID_usuario;
    createThreadPublic(&hilo_dispatcher,(void*)Dispatch_H_getItem, (void*) &requestCopy);
    up(semaforo_done);
  }
};
void *Dispatch_H_getItem(struct Request *request){
  printf("Nueva conexion con el cliente:%d\n",request->ID_usuario);
  int dataType = request->dataType;
  pid_t ID_cliente = request->ID_usuario;
  struct FileManager fileManager;
  char filename[20];
  int semaforo_newData,semaforo_RW_file, semaforo_done;
  int num_data_in_response;

  switch (dataType){
  case USUARIO:
    struct Usuario usuario;
    strcpy(filename,"usuario.txt");
    fileManager.fileName = filename;
    getShmPublic(&usuario,ID_cliente,USUARIO);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_USUARIO,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    //Como le decimos alcliente que ya? count ==0?
    down(semaforo_RW_file);
    //Extrae el dato
    
    up(semaforo_RW_file);
    //Pega el dato en mc
    //le avisa al cliente que ya acabo

    break;
  default:
    break;
  }
};

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

/**Dispatch_H_addItem
 - Es la funcion que se encarga de atendedr una peticion de tipo PUT.
 * 
 * request (struct Request)  Tiene los datos necesarios para crear conexion {PID del cliente, tabla en la que se va a operar}
 * dataType (int) variable para guardar en que tabla vamos a operar
 * ID_cliente (pid_t) PID del cliente
 * fileManager (struct FileManager) Tiene los datos para abrir un archivo (nombre de archivo, apuntador a archivo)
 * filename (char)  guarda el nombre del archivo de la tabla que vamos a operar
 * salida (char)  Es la cadena que almacenara los datos del insert en un formato especifico para guardar en archivo de texto
 * semaforo_RW_file (semaforo)  Semaforo para asegurar zona critica en escritura del archivo
 * semaforo_newData (semaforo)  Semaforo que se utiliza para saber si el cliente ya coloco la informacion en la memoria compartida
 * semaforo_done (semaforo) Semaforo para avisarle al cliente que ya copiamos los datos de memoria compartida
 * num_data_in_request (int)  Variable que nos indica cuantos datos nos pasara el cliente
*/
void *Dispatch_H_addItem(struct Request *request){
  printf("Nueva conexion con cliente:%d\n", request->ID_usuario);
  int dataType = request->dataType;
  pid_t ID_cliente = request->ID_usuario;
  struct FileManager fileManager;
  char filename [20];
  char salida [200];
  int semaforo_newData, semaforo_RW_file, semaforo_done;
  int num_data_in_request;

  switch (dataType){
  case USUARIO:
    struct Usuario usuario;
    strcpy(filename,"usuario.txt");
    fileManager.fileName = filename;
    getShmPublic(&usuario,ID_cliente,USUARIO);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_USUARIO,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = usuario.apt_mc_usuario->count;
    while (num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_user\n");
      structToString(salida,usuario.apt_mc_usuario,USUARIO);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
  case CATEGORIA:
    struct Categoria categoria;
    strcpy(filename,"categoria.txt");
    fileManager.fileName = filename;
    getShmPublic(&categoria, ID_cliente,CATEGORIA);
    createSemPublic(&semaforo_RW_file, CLAVE_SER_H_D_IO_CATEGORIA,1);
    createSemPublic(&semaforo_newData, ID_cliente,0);
    createSemPublic(&semaforo_done, ID_cliente*-1,0);
    num_data_in_request = categoria.apt_mc_categoria->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_categoria\n");
      structToString(salida,categoria.apt_mc_categoria,CATEGORIA);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
  case PRODUCTO:
    struct Producto producto;
    strcpy(filename,"producto.txt");
    fileManager.fileName = filename;
    getShmPublic(&producto,ID_cliente,PRODUCTO);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_PRODUCTO,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = producto.apt_mc_producto->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_producto\n");
      structToString(salida,producto.apt_mc_producto,PRODUCTO);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
  case VENTA:
    struct Venta venta;
    strcpy(filename,"venta.txt");
    fileManager.fileName = filename;
    getShmPublic(&venta,ID_cliente,VENTA);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_VENTA,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = venta.apt_mc_venta->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_venta\n");
      structToString(salida,venta.apt_mc_venta,VENTA);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
    case DETALLEVENTA:
    struct DetalleVenta detalleventa;
    strcpy(filename,"detalleventa.txt");
    fileManager.fileName = filename;
    getShmPublic(&detalleventa,ID_cliente,DETALLEVENTA);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_DETALLEVENTA,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = detalleventa.apt_mc_detalleventa->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_detalleventa\n");
      structToString(salida,detalleventa.apt_mc_detalleventa,DETALLEVENTA);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
  case PROVEEDOR:
    struct Proveedor proveedor;
    strcpy(filename,"proveedor.txt");
    fileManager.fileName = filename;
    getShmPublic(&proveedor,ID_cliente,PROVEEDOR);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_PROVEEDOR,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = proveedor.apt_mc_proveedor->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_proveedor\n");
      structToString(salida,proveedor.apt_mc_proveedor,PROVEEDOR);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;
  case ADQUISICION:
    struct Adquisicion adquisicion;
    strcpy(filename,"adquisicion.txt");
    fileManager.fileName = filename;
    getShmPublic(&adquisicion,ID_cliente,ADQUISICION);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_ADQUISICION,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = adquisicion.apt_mc_adquisicion->count;
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("new Request/Add_adquisicion\n");
      structToString(salida,adquisicion.apt_mc_adquisicion,ADQUISICION);
      printf("%s\n",salida);
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    break;  
  default:
    break;
  };
  pthread_exit(0);
};

//Busca en el archivo y rellena la estructura en caso de que si exista alguna coincidencia
void searchInDoc(struct FileManager *filemanager,const char *palabra,int longitudLinea,struct Usuario *usuario){
  //printf("Bug0\n");
  openFile(filemanager);
  char salida[200];
  char linea[longitudLinea];
  struct Usuario tmpUsuario;
  char lineaa[6];
  
  while(fgets(linea,sizeof(linea),filemanager->file) != NULL){
    sscanf(linea,"%d\t%s\t%s\t%s\t%d",
    &(tmpUsuario.ID_usuario),tmpUsuario.nombre,tmpUsuario.usuario,tmpUsuario.password,&(tmpUsuario.typeUser));
    //if(strcmp(tmpUsuario.usuario,palabra) == 0){
      printf("|->%s",linea);
    //}
  };
  closeFile(filemanager);  
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
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)data;
    memoria = shmget(llave_memoria,sizeof(struct Producto), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/PRODUCTO");
    producto->apt_mc_producto = (struct Producto*)shmat(memoria,0,0);
    if(producto->apt_mc_producto == (struct Producto*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)data;
    memoria = shmget(llave_memoria,sizeof(struct Venta), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/VENTA");
    venta->apt_mc_venta = (struct Venta*)shmat(memoria,0,0);
    if(venta->apt_mc_venta == (struct Venta*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleventa = (struct DetalleVenta*)data;
    memoria = shmget(llave_memoria,sizeof(struct DetalleVenta), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/DETALLEVENTA");
    detalleventa->apt_mc_detalleventa = (struct DetalleVenta*)shmat(memoria,0,0);
    if(detalleventa->apt_mc_detalleventa == (struct DetalleVenta*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  case PROVEEDOR:
    struct Proveedor *proveedor = (struct Proveedor*)data;
    memoria = shmget(llave_memoria,sizeof(struct Proveedor), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/PROVEEDOR");
    proveedor->apt_mc_proveedor = (struct Proveedor*)shmat(memoria,0,0);
    if(proveedor->apt_mc_proveedor == (struct Proveedor*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)data;
    memoria = shmget(llave_memoria,sizeof(struct Adquisicion), IPC_CREAT | PERMISOS);
    if(memoria == -1) ErrorMessage("\nError al pedir region en la memoria compartida de getShmPublic/ADQUISICION");
    adquisicion->apt_mc_adquisicion = (struct Adquisicion*)shmat(memoria,0,0);
    if(adquisicion->apt_mc_adquisicion == (struct Adquisicion*)(-1)) ErrorMessage("\nError al signar la memoria compartida al apuntador de getShmPublic/TYPEDATAREQUEST");
    break;
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
    typeDataToString(apt_salida,&(usuario->ID_usuario),DATATYPE_ID,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->nombre,DATATYPE_CHAR,15);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,usuario->usuario,DATATYPE_CHAR,25);
    strcat(apt_salida,"\t");
    cifrar(usuario->password);
    typeDataToString(apt_salida,usuario->password,DATATYPE_CHAR,20);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(usuario->typeUser),DATATYPE_INT,1);
    strcat(apt_salida,"\n");
    break;
  case CATEGORIA:
    struct Categoria *categoria = (struct Categoria*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,&(categoria->ID_categoria),DATATYPE_ID,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,categoria->descripcion,DATATYPE_CHAR,25);
    strcat(apt_salida,"\n");
    break;  
  case PRODUCTO:
    struct Producto *producto = (struct Producto*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,&(producto->ID_producto),DATATYPE_ID,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->nombre,DATATYPE_CHAR,20);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(producto->cantidad),DATATYPE_INT,5);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->descripcion,DATATYPE_CHAR,100);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,producto->descripcion,DATATYPE_CHAR,100);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(producto->ID_categoria),DATATYPE_CHAR,16);
    strcat(apt_salida,"\n");
    break;
  case VENTA:
    struct Venta *venta = (struct Venta*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,&(venta->ID_venta),DATATYPE_ID,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,venta->fecha,DATATYPE_CHAR,19);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,venta->metodo,DATATYPE_CHAR,12);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(venta->isr),DATATYPE_LD,0);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(venta->monto_total),DATATYPE_LD,0);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(venta->ID_usuario),DATATYPE_CHAR,16);
    strcat(apt_salida,"\n");
    break;
  case DETALLEVENTA:
    struct DetalleVenta *detalleventa = (struct DetalleVenta*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,&(detalleventa->ID_detalleventa),DATATYPE_ID,16);
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
    typeDataToString(apt_salida,&(proveedor->ID_proveedor),DATATYPE_ID,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,proveedor->descripcion,DATATYPE_CHAR,150);
    strcat(apt_salida,"\n");
    break;
  case ADQUISICION:
    struct Adquisicion *adquisicion = (struct Adquisicion*)data;
    apt_salida[0]='\0';
    typeDataToString(apt_salida,&(adquisicion->ID_provedor),DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(adquisicion->ID_producto),DATATYPE_CHAR,16);
    strcat(apt_salida,"\t");
    typeDataToString(apt_salida,&(adquisicion->ID_usuario),DATATYPE_CHAR,16);
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
  case DATATYPE_ID:
    generateID(salida);
    break;
  default:
    break;
  };
  return;
};

void generateID(char *salida){
  srand((unsigned int)time(NULL));
  // Tiempo actual
  time_t t = time(NULL);
  struct tm tiempoLocal = *localtime(&t);
  // El lugar en donde se pondrá la fecha y hora formateadas y 2 valores random
  char string_ID[70],buffer[2];
  char *formato = "%Y%m%d%H%M%S";
  int bytesEscritos =
  strftime(string_ID, sizeof string_ID, formato, &tiempoLocal);
  int random_value1 = rand() % 1000;
  int random_value2 = rand() % 1000;
  snprintf(buffer,sizeof(buffer),"%d",random_value1);
  strcat(string_ID,buffer);
  snprintf(buffer,sizeof(buffer),"%d",random_value2);
  strcat(string_ID,buffer);
  strcat(salida,string_ID);
};

void cifrar(char *password){
  int clave = KEY_SECRET_PASSWORD;
  char caracter;
  for(int i =0;password[i] != '\0'; ++i) {
    caracter = password[i];
    // Cifrar solo letras mayúsculas
    if(caracter >= 'A' && caracter <= 'Z'){
      caracter = (caracter + clave - 'A' + 26) % 26 + 'A';
    };
    // Cifrar solo letras minúsculas
    if(caracter >= 'a' && caracter <= 'z'){
      caracter = (caracter + clave - 'a' + 26) % 26 + 'a';
    };
    if(caracter >= '0' && caracter <= '9'){
      caracter = (caracter + clave - '0' + 10) % 10 + '0';
    }
    password[i] = caracter;
  };
};

void descifrar(char *password){
  int clave = KEY_SECRET_PASSWORD;
  char caracter;
  for(int i =0;password[i] != '\0';i++) {
    caracter = password[i];
    // Descifrar solo letras mayúsculas
    if(caracter >= 'A' && caracter <= 'Z'){
      caracter = (caracter - clave - 'A' + 26) % 26 + 'A';
    };
    // Descifrar solo letras minúsculas
    if(caracter >= 'a' && caracter <= 'z'){
      caracter = (caracter - clave - 'a' + 26) % 26 + 'a';
    };
    //Numeros
    if(caracter >= '0' && caracter <= '9'){
      caracter = (caracter - clave - '0' + 10) % 10 + '0';
    }
    password[i] = caracter;
  };
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
