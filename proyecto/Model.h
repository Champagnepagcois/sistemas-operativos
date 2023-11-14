#ifndef MODEL_H
#define MODEL_H

#include "Controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*************Constantes****************/
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

#define QUERYSUCCESS   200
#define ERRORFILEOPEN  404

#define VENTANA_MAIN  1000
#define VENTANA_VENTA 1001
#define VENTANA_EDITAR  1002

/**************Modelos******************/

//Navegacion
struct Navegacion{
  int path_ubicacion;
  
};

struct Usuario {
  int ID_usuario; //lon 6
  char nombre [15]; 
  char usuario [25];
  char password [120];
  int typeUser; // lon 2
  int logged;   // 1on 1
  struct Usuario* siguiente;
};

struct Categoria{
  int ID_categoria;
  char descripcion [25];
  struct Categoria* siguiente;
};

struct Producto {
  int ID_producto; //lon 6
  char nombre [20];
  int cantidad;   // lon 1000
  char descripcion [100]; 
  long double precio; //lon 10
  int ID_categoria;     //6
  struct Producto* siguiente;
};

struct Venta {
  int ID_ventas;
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  char metodo [12];
  long double isr;
  long double monto_total;
  int ID_usuario;
  struct Venta* siguiente;
};

struct DetalleVenta {
  int ID_venta;
  int ID_producto;
  long double precio_unitario;
  int descuento;//por definir
  int cantidad;
  struct DetalleVenta* siguiente;
};

struct Proveedor{
 int ID_proveedor;
 char descripcion [150];
  struct Proveedor* siguiente;
};

struct Adquisicion{
  int ID_provedor;
  int ID_producto;
  int ID_usuario;
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  int cantidad;
  struct Adquisicion* siguiente;
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
/************Operaciones Query *****************/
void new_Ini_User(struct Usuario *);
/************Operaciones Query *****************/

void signin(char,char);
/***************Queries*****************/

void structToStr(struct Query*);
void usuarioTostr(struct Query*, char*);

int newQuery(struct Query*);
int QueryInsert (struct Query*);
int QuerySelect (struct Query*);
int QueryUpdate (struct Query*);
int QueryDelete (struct Query*);



/***********************Logic IO*************************/

int openFile(struct Query*);



/********************************************************/

/****Control de estructura de datos****/

struct Nodo* crearNodo(void* dataTabla);
void insertarNodoA(struct Nodo** InicioNodo, void* dataTabla);
void insertarNodoZ(struct Nodo** InicioNodo, void* dataTabla);
  
struct Producto* newProducto(int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria);
void insertarProducto(struct Producto** InicioLista, int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria);
void freeProducto(); //por hacer
#endif