#include "Server.h"


int main(){
  if(system("ipcrm -a")== -1) ErrorMessage("No se pudo limpiar la MC y semaforos");
  /************** Creamos hilos Endpoints **************/
  pthread_t thread_add,thread_get, thread_update, thread_delete;

  createThreadPublic(&thread_get,H_getItem,NULL);
  createThreadPublic(&thread_add,H_addItem,NULL);
  //createThreadPublic(&thread_update,H_updateItem,NULL);
  //createThreadPublic(&thread_delete,H_deleteItem,NULL);
  sleep(100);
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
void deleteSemPublic(int *semaforo){
  if(semctl(*semaforo,0,IPC_RMID)==-1){
    ErrorMessage("Error al eliminar el semaforo");
  };
};
/****************************************************************/


/******************* Endpoints Request ***********************/
//GET ENDPOINT
void *H_getItem(){
  struct Request request,requestCopy;
  char clave_getItem = CLAVE_SER_H_GETITEM;
  getShmPublic(&request,clave_getItem,TYPEDATA_REQUEST);
  int semaforo_create_dispatcher, semaforo_request, semaforo_done;
  createSemPublic(&semaforo_create_dispatcher,CLAVE_SER_H_GETITEM,0);
  createSemPublic(&semaforo_request,CLAVE_SER_H_GETITEM,0);
  //createSemPublic(&semaforo_done,CLAVE_SER_H_GETITEM,0);
  pthread_t hilo_dispatcher;
  printf("Servicio getItem, \x1b[32mescuchando....\x1b[0m\n");
  while(1){
    down(semaforo_request);
    requestCopy.dataType = request.apt_mc_request->dataType;
    requestCopy.ID_usuario = request.apt_mc_request->ID_usuario;
    createThreadPublic(&hilo_dispatcher,(void*)Dispatch_H_getItem, (void*) &requestCopy);
    //up(semaforo_request);
  }
};
void *Dispatch_H_getItem(struct Request *request){
  printf("\x1b[32mNueva conexion ");
  printf("\x1b[0mcon el cliente: ");
  printf("\x1b[34m%d\n",request->ID_usuario);
  int dataType = request->dataType;
  pid_t ID_cliente = request->ID_usuario;
  struct FileManager fileManager;
  char filename[20];
  int semaforo_newData,semaforo_RW_file, semaforo_done;
  int num_data_in_response;

  switch (dataType){
  case USUARIO:
    struct Usuario usuario;
    struct Usuario *nodo =NULL;
    strcpy(filename,"usuario.txt");
    fileManager.fileName = filename;
    getShmPublic(&usuario,ID_cliente,USUARIO);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_USUARIO,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    //Le avisa al cliente que se ha terminado de configurar el servidor
    up(semaforo_done);
    //Recibimos Request
    down(semaforo_done);
    //Leemos y copiamos datos
    strcpy(usuario.usuario, usuario.apt_mc_usuario->usuario);
    strcpy(usuario.password, usuario.apt_mc_usuario->password);
    /*
    printf("\n\n%s\n",usuario.apt_mc_usuario->usuario);
    printf("%s\n\n",usuario.apt_mc_usuario->password);
    */
    //Buscamos en archivo y agregamos resultados en la cola
    down(semaforo_RW_file);
    searchInDoc(&fileManager,(void**)&nodo,&usuario,USUARIO); //Buscamos en el archivo
    up(semaforo_RW_file);
    //Obtenemos el numero de nodos en la cola.
    int count =getNumberItemQueue(nodo,USUARIO);
    //printf("\nNumero de resultados:%d\n",count);
    //Escribimos en mc count
    usuario.apt_mc_usuario->count = count;
    //Validamos que haya almenos 1 valor
    if(count ==0){
      up(semaforo_newData);
    };
    int firstData =1;
    while(count>0){
      printf("\x1b[32mnew Request/Get_user\x1b[0m\n");
      if(firstData ==0){//No es primera vez entonces se queda aqui
        down(semaforo_done);//hasta que acabe el cliente
      };
      //Escribimos informacion del nodo en la cola en mc
      deQueue(usuario.apt_mc_usuario,(void**)&nodo,USUARIO);
      /*
      char salida [200];
      structToString(salida,usuario.apt_mc_usuario,USUARIO);
      printf("Dato de cola en MC:%s\n",salida);
      */

      firstData =0;//Ya no es la primera vez
      up(semaforo_newData);
      count-=1;
    };
    //Terminamos la conexion eliminando este hilo   
    printf("\x1b[0mServicio al cliente ");
    printf("\x1b[34m%d ",ID_cliente);
    printf("\x1b[31mfinalizado\x1b[0m\n");
    freeShmPublic(&usuario,ID_cliente,USUARIO);
    pthread_exit(NULL);

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
  printf("Servicio AddItem, \x1b[32mescuchando....\x1b[0m\n");
  
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
    createSemPublic(&semaforo_done,(ID_cliente)*-1,0);
    num_data_in_request = usuario.apt_mc_usuario->count;
    while (num_data_in_request>0){
      down(semaforo_newData);
      printf("\x1b[32mnew Request/Add_user\x1b[0m\n");
      //Ciframos contrasena y creamos ID
      generateID(usuario.apt_mc_usuario->ID_usuario);
      cifrar(usuario.apt_mc_usuario->password);
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
      printf("\x1b[32mnewnew Request/Add_categoria\x1b[0m\n");
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
    strcpy(filename,"producto.txt\n");
    fileManager.fileName = filename;
    getShmPublic(&producto,ID_cliente,PRODUCTO);
    createSemPublic(&semaforo_RW_file,CLAVE_SER_H_D_IO_PRODUCTO,1);
    createSemPublic(&semaforo_newData,ID_cliente,0);
    createSemPublic(&semaforo_done,ID_cliente*-1,0);
    num_data_in_request = producto.apt_mc_producto->count;
    //printf("#elementos a guardar:%d\n",num_data_in_request);
    up(semaforo_done);
    while(num_data_in_request>0){
      down(semaforo_newData);
      printf("\x1b[32mnewnew Request/Add_producto\x1b[0m\n");
      //Generamos ID
      generateID(producto.apt_mc_producto->ID_producto);
      /*structToString(salida,producto.apt_mc_producto,PRODUCTO);
      printf("%s\n",salida);*/
      down(semaforo_RW_file);
      openFile(&fileManager);
      fprintf(fileManager.file,"%s",salida);
      closeFile(&fileManager);
      up(semaforo_RW_file);
      num_data_in_request-=1;
      up(semaforo_done);
    };
    up(semaforo_done);
    //Terminamos la conexion eliminando este hilo   
    printf("\nServicio al cliente %d finalizado\n", ID_cliente);
    freeShmPublic(&producto,ID_cliente,PRODUCTO);
    pthread_exit(NULL);
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
void searchInDoc(struct FileManager *filemanager,void **nodo,void *data,int dataType){
  //printf("Ingreso al serachInDoc\n");
  openFile(filemanager);
  char salida[200];
  char linea[200];
  
  switch(dataType){
    case USUARIO:
    char pass_des[20];
      int TAM_LINEA_USUARIO=84;
      struct Usuario tmpUsuario;
      struct Usuario *usuario = (struct Usuario*)data;
      //Va leyendo de linea en linea
      while(fgets(linea,TAM_LINEA_USUARIO,filemanager->file) !=NULL){
        sscanf(linea,"%s\t%s\t%s\t%s\t%d",tmpUsuario.ID_usuario, tmpUsuario.nombre,tmpUsuario.usuario,tmpUsuario.password,&(tmpUsuario.typeUser));
        //Verificamos el match para que retorne los datos
        if(strcmp(tmpUsuario.usuario,usuario->usuario)==0){
          //printf("PASSWORD: %s\n",tmpUsuario.password);
          //Comparamos passwords para dar acceso o no
          if(passwordCmp(usuario->password,tmpUsuario.password)==0){
            tmpUsuario.logged =1;
            //printf("Match de contrase;as\n");
          }else{
            tmpUsuario.logged=0;
          }
          enQueue(nodo,&tmpUsuario,USUARIO);
        };
      };
      closeFile(filemanager);
      //Imprimimos contenido de la cola
      //imprimirCola(*nodo,USUARIO);
      break;
    default:
      break;
  };
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
void stringToStruct(const char *string,void *data, int dataType){ //Por terminar
  switch (dataType){
  case USUARIO:
    struct Usuario usuario = *(struct Usuario*)data;
    sscanf(string,"%16s\t%15s\t%25s\t%20s\t%d",usuario.ID_usuario,usuario.nombre,usuario.usuario,usuario.password,&(usuario.typeUser));
    //descifrar(usuario.password);
    break;
  case CATEGORIA:
    struct Categoria categoria = *(struct Categoria*)data;
    sscanf(string,"%16s\t%25s",categoria.ID_categoria,categoria.descripcion);
    break;
  case PRODUCTO:
    struct Producto producto = *(struct Producto*)data;
    sscanf(string,"%16s\t%20s\t%d\t%50s\t%Lf\t%d\t%d",
    producto.ID_producto,producto.nombre,producto.cantidad,producto.descripcion,producto.precio,producto.ID_categoria,producto.existencia);
    break;
  case VENTA:
    struct Venta venta = *(struct Venta*)data;
    sscanf(string,"%d\t%19s\t%12s\t%Lf\t%Lf\t%d",
    venta.ID_venta,venta.fecha,venta.metodo,venta.isr,venta.monto_total,venta.ID_usuario);
    break;
  //FALTA ACABAR!!!
  default:
    break;
  }
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

int passwordCmp(const char *pwdCli,const char *pwdFile){
  char tmp [20];
  strcpy(tmp,pwdFile);
  descifrar(tmp);
  return strcmp(pwdCli,tmp);
};
void structToStruct(void *dest,void *src, int dataType){
  switch (dataType){
  case USUARIO:{
    struct Usuario *destino = (struct Usuario*)dest;
    struct Usuario *fuente = (struct Usuario*)src;
    strcpy(destino->ID_usuario,fuente->ID_usuario);
    strcpy(destino->nombre,fuente->nombre);
    strcpy(destino->usuario,fuente->usuario);
    strcpy(destino->password,fuente->password);
    destino->typeUser = fuente->typeUser;
    destino->logged = fuente->logged;
    break;
  }
  case PRODUCTO:{
    struct Producto *destino = (struct Producto*)dest;
    struct Producto *fuente = (struct Producto*)src;
    strcpy(destino->ID_producto,fuente->ID_producto);
    strcpy(destino->nombre,fuente->nombre);
    destino->cantidad = fuente->cantidad;
    strcpy(destino->descripcion,fuente->descripcion);
    destino->precio = fuente->precio;
    strcpy(destino->ID_categoria,fuente->ID_categoria);
    destino->existencia = fuente->existencia;
    break;}
  //Falta terminar
  default:
    break;
  }
  return;
};
void imprimirCola(void *nodo, int dataType) {
  char salida[200];
  switch (dataType) {
    case USUARIO: {
      struct Usuario *currentNode = (struct Usuario *)nodo;
      printf("Contenido de la cola:\n");
      while (currentNode != NULL) {
        // Imprimir los datos del nodo actual
        printf("\n\nData:\n");
        structToString(salida,currentNode,USUARIO);
        printf("\n%s\n",salida);
        // Agrega más campos según la estructura de tu Usuario
        // Avanzar al siguiente nodo
        currentNode = currentNode->siguiente;
      };
      break;
    }
    case PRODUCTO:{
      struct Producto *currentNode = (struct Producto *)nodo;
      printf("Contenido de la cola:\n");
      while (currentNode != NULL) {
        // Imprimir los datos del nodo actual
        printf("\n\nData:\n");
        structToString(salida,currentNode,PRODUCTO);
        printf("\n%s\n",salida);
        // Avanzar al siguiente nodo
        currentNode = currentNode->siguiente;
      };
      break;
    }
    default:
      break;
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
/****************** Estructura de datos **************/
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
  case USUARIO:
    struct Usuario *primerNodo = (struct Usuario*)*nodo;
    if(primerNodo !=NULL){
      structToStruct(destino,primerNodo,USUARIO);
      *nodo = primerNodo->siguiente;
      free(primerNodo);
    };
    break;
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


/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/
