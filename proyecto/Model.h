#ifndef MODEL_H
#define MODEL_H

#include "Controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

/*************Constantes****************/
#define PERMISOS 0644
#define USER_TYPE_ADMIN  777
#define USER_TYPE_EMPLOYEE 644

#define INSERT         300
#define SELECT         301
#define UPDATE         302
#define DELETE         304

#define USUARIO        100 
#define CATEGORIA      101
#define PRODUCTO       102
#define VENTA          103
#define DETALLEVENTA   104
#define PROVEEDOR      105
#define ADQUISICION    106
#define TYPEDATA_REQUEST  110

#define DATATYPE_INT    840
#define DATATYPE_CHAR   841
#define DATATYPE_LD     842
#define DATATYPE_ID     843

#define QUERYSUCCESS   200
#define ERRORFILEOPEN  404

#define VENTANA_MAIN  1000
#define VENTANA_VENTA 1001
#define VENTANA_EDITAR  1002

#define CLAVE_SER_H_ADDITEM 'a'
#define CLAVE_SER_H_GETITEM 'g'
#define CLAVE_SER_H_UPDATEITEM 'e'
#define CLAVE_SER_H_DELETEITEM 'f'

/**************Modelos******************/

//Navegacion
struct Navegacion{
  int path_ubicacion;
  
};

struct Usuario {
  char ID_usuario [16]; //lon 16
  char nombre [15]; 
  char usuario [25];
  char password [20];
  int typeUser; // lon 2
  int logged;   // 1on 1
  struct Usuario* siguiente;
  struct Usuario* apt_mc_usuario;
  int count;
};

struct Categoria{
  char ID_categoria [16];
  char descripcion [25];
  struct Categoria* siguiente;
  struct Categoria* apt_mc_categoria;
  int count;
};

struct Producto {
  char ID_producto [16]; //lon 16
  char nombre [20];
  int cantidad;   // lon 4char
  char descripcion [50]; 
  long double precio; //lon 10
  char ID_categoria [16];     //6
  int existencia; //8
  struct Producto* apt_mc_producto;
  struct Producto* siguiente;
  int count;
};

struct Venta {
  char ID_venta [16];
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  char metodo [12];
  long double isr;
  long double monto_total;
  char ID_usuario [16];
  struct Venta* siguiente;
  struct Venta* apt_mc_venta;
  int count;
};

struct DetalleVenta {
  char ID_detalleventa [16];
  int ID_producto;
  long double precio_unitario;
  int descuento;//por definir
  int cantidad;
  struct DetalleVenta* siguiente;
  struct DetalleVenta* apt_mc_detalleventa;
  int count;
};

struct Proveedor{
  char ID_proveedor [16];
  char descripcion [150];
  struct Proveedor* siguiente;
  struct Proveedor* apt_mc_proveedor;
  int count;
};

struct Adquisicion{
  char ID_provedor [16];
  char ID_producto [16];
  char ID_usuario [16];
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  int cantidad;
  struct Adquisicion* siguiente;
  struct Adquisicion* apt_mc_adquisicion;
  int count;
};

/*******Modelos funcionamiento**********/
struct Nodo {
  void* dataTabla;
  struct Nodo* siguiente;
};

struct Condicion {
  char* field;
  char* operador;
  char* valor;
  struct Condicion* siguiente;
};

struct QueryKeyValue {
  char field [20];
  char value [20];
  struct QueryKeyValue* next;
};

struct Query {
  int typeQuery;
  int table;
  struct QueryKeyValue *keyValue;
  struct Condicion *condition;
  char fileName [15];
  char chmod [3];
  FILE *file;
};


struct Request {
  pid_t ID_usuario;
  int dataType;
  struct Request* apt_mc_request;
};

/******* Modelo para carrito de compras*/

struct ShoppingCart {
  long double montoTotal;
  struct ShoppingCart* next;
  struct ShoppingCart* apt_mc_sp;
  int count;
};



/****************** Variado ****************/

void createThreadPublic(pthread_t *ID_thread,void*(*__start_routine)(void *),void *__restrict__ __arg);
int getID_Cliente(pid_t id_cliente);
/*************** Semaforos ********************/
int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
int getValueSemaphore(int *id);
void createSemPublic(int *semaforo,int clave, int valor);
void deleteSemPublic(int *semaforo);

/************** Memoria compartida **************/
void getShmPublic(void *data,char clave,int typeData);
void freeShmPublic(void *aptchar, char clave,int dataType);

/************Operaciones Query *****************/
void new_Ini_User(struct Usuario *);
/************Operaciones Query *****************/

void signin(char,char);
/***************Queries*****************/

void structToStr(struct Query*);
void usuarioTostr(struct Query*, char*);

int newQuery(struct Query*);
//int QueryInsert (struct Request* request,void* data,void **nodo,int dataType);
int QueryInsert (struct Request* request,void **nodo,int dataType);
void QuerySelect(struct Request* request,void* data,void **nodo,int dataType);

int QueryUpdate (struct Query*);
int QueryDelete (struct Query*);



/***********************Logic IO*************************/

int openFile(struct Query*);



/********************************************************/
//Helpers
void structToStruct(void *data1,void *data2, int dataType);
void structToString(char* apt_salida,void *data,int dataType);
void typeDataToString(char *salida, void *data, int dataType, int lonMax);
/****Control de estructura de datos****/

void enQueue(void **nodo, void *data,int dataType);
void deQueue(void *destino,void **nodo,int dataType);
int getNumberItemQueue(void *, int dataType);
void imprimirCola(void *nodo, int dataType);

//Errores
void ErrorMessage(char *message);

#endif
