#ifndef SERVER_H
#define SERVER_H

/************ Bibliotecas ************/
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

#define USUARIO        100 
#define CATEGORIA      101
#define PRODUCTO       102
#define VENTA          103
#define DETALLEVENTA   104
#define PROVEEDOR      105
#define ADQUISICION    106

#define CLAVE_SER_H_LOGIN_MC 'z'
#define CLAVE_SER_H_LOGIN_MC_REQ 'z'


/************ Estructuras ************/
//Queries


//Tablas
struct Usuario {
  int ID_usuario; //lon 6
  char nombre [15]; 
  char usuario [25];
  char password [120];
  int typeUser; // lon 2
  int logged;   // 1on 1
  struct Usuario* siguiente;
  struct Usuario* apt_mc_usuario;
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

//Programa
struct FileManager{
  char *fileName;
  FILE *file;
};


/******* Definicion de funciones *********/

//Semaforos
int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
void initSemaphore(key_t *llave,int *semaforo,int *clave);
int getValueSemaphore(int *id);

//Endpoints Request
void *H_login();
void *H_getItem();
void H_addItem(void *data,int dataType);
void *H_updateItem();
void *H_deleteItem();
void *H_writeFile();
void *H_readFile();

//controller Endpoints Request
void  dispatch_H_login(struct Usuario *usuario);
void searchInDoc(struct FileManager *file,struct Usuario *usuario);

//Memoria compartida
void getShmLogin(struct Usuario *usuario,char clave);
void getShmRequest(int *count, char clave);

//archivos
void openFile(struct FileManager *fileManager);
void closeFile(struct FileManager *fileManager);
void AddData();
void GetData();
void UpdateData();
void DeleteData();

//Helpers
void structToString(char* apt_salida,void *data,int dataType);
//Errores
void ErrorMessage(char *message);

#endif