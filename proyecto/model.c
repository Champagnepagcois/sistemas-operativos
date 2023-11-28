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

struct Nodo* crearNodo(void* dataTabla){
  struct Nodo* nuevoNodo = (struct Nodo*)malloc(sizeof(struct Nodo));
  nuevoNodo->dataTabla= dataTabla;
  nuevoNodo->siguiente = NULL;
  return nuevoNodo;
};

void insertarNodoA(struct Nodo** InicioNodo, void* dataTabla){
  struct Nodo* nuevoNodo = crearNodo(dataTabla);
  nuevoNodo->siguiente = *InicioNodo;
  *InicioNodo = nuevoNodo;
};

void insertarNodoZ(struct Nodo** InicioNodo, void* dataTabla){
  struct Nodo* nuevoNodo = crearNodo(dataTabla);
  nuevoNodo->siguiente = NULL;

  if(*InicioNodo == NULL){
    *InicioNodo = nuevoNodo;  
  }else{
    struct Nodo* actual = *InicioNodo;
    while (actual->siguiente != NULL){
      actual = actual->siguiente;
    };
    actual->siguiente =nuevoNodo;
  };
};

struct Producto* newProducto(int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria){
  struct Producto* nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
  nuevoProducto->ID_producto = ID_categoria;
  strcpy(nuevoProducto->nombre,nombre);
  strcpy(nuevoProducto->descripcion, descripcion);
  nuevoProducto->precio = precio;
  nuevoProducto->cantidad = cantidad;
  nuevoProducto->ID_categoria = ID_categoria;
  nuevoProducto->siguiente = NULL;
  return nuevoProducto;
};

void insertarProducto(struct Producto** InicioLista, int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria){
  struct Producto* nuevoProducto = newProducto( ID_producto, nombre, cantidad, descripcion,precio, ID_categoria);
  nuevoProducto->siguiente = *InicioLista;
  *InicioLista = nuevoProducto;
};

/*************Logica query***************/
int newQuery(struct Query* consulta){
  printf("\nAqui paso en newQuery()\n");
  printf("\ntypeQuery = %d\n", consulta->typeQuery);
  switch (consulta->typeQuery){
  case INSERT:
    printf("\nSi entra al insert\n");
    return QueryInsert(consulta);
    break;
  case SELECT:
    /* code */
    return QuerySelect(consulta);
    break;
  case UPDATE:
    /* code */
    return QueryUpdate(consulta);
    break;
  case DELETE:
    /* code */
    return QueryDelete(consulta);
    break;
  default:
    break;
  }    

  /*void (*Operaciones[])(void)={QueryInsert,QuerySelect,QueryUpdate,QueryDelete};
  abrirArchivo(consulta,Operaciones[consulta->typeQuery]());//Tengo que pasar la funcion a realizar
  */
};

/****************************************/



/*************FileIO***************/
int openFile(struct Query* consulta){
  printf("Aqui se abre el archivo");
  consulta->file = fopen(consulta->fileName, consulta->chmod);
  if(consulta->file == NULL){
    printf("\nNo se pudo abrir el archivo\n");
    return ERRORFILEOPEN;
  };
  return 0; 
};

void structToStr(struct Query* consulta){
  char *string;
  while (consulta->keyValue->next != NULL){

  };
  
};
/*************Logic query***********/
int QueryInsert(struct Query* consulta){
  printf("\nAqui paso en QueryInsert()\n");
  char output [500] = {'\0'};
  FILE* file;
  //consulta->file = file;


  switch (consulta->table){

  case USUARIO:
    printf("\nEntro al case de USUARIO\n");
    openFile(consulta);
    //START CODE
    usuarioTostr(consulta,output);
    printf("\n\n%s\n\n",output);
    fprintf(consulta->file,"%s",output);
    //END
    fclose(consulta->file);

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
int QuerySelect(struct Query* consulta){
  return 0;
};
int QueryUpdate(struct Query* consulta){
  return 0;
};
int QueryDelete(struct Query* consulta){
  return 0;
};
/***********************************/

void usuarioTostr(struct Query* consulta, char* output){
  struct QueryKeyValue* curent = consulta->keyValue;
  if(curent == NULL){
    printf("\ncurrent es  null\n");
    strcat(output,"No hay nada");
  }
  while (curent != NULL){
    strcat(output,curent->value);
    curent = curent->next;
    strcat(output,"\t"); //agregar tabulador
  };
  return;
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

