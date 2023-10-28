#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <string.h>

//Se encarga ded todo lo que deberia se DB
//CRUD, conexion a DB, etc

struct Usuario {
  int ID_usuario;
  char nombre [15];
  char usuario [25];
  char password [120];
  int typeUser;
  int logged;
  struct Usuario* siguiente;
};

struct Categoria{
  int ID_categoria;
  char descripcion [25];
  struct Categoria* siguiente;
};


struct Producto {
  int ID_producto;
  char nombre [20];
  int cantidad;
  char descripcion [100]; 
  long double precio;
  int ID_categoria;
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

void signin(char usuario,char password);
void signout();



/**********Control de datos***********/
struct Producto* newProducto(int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria);
void insertarProducto(struct Producto** InicioLista, int ID_producto,const char* nombre,int cantidad,const char* descripcion, long double precio,int ID_categoria);
void freeProducto(); //por hacer
/***********************************/




/*************Queries***************/
void selectUsers(struct Usuario usuario);
void selectProducts();
/***********************************/


#endif