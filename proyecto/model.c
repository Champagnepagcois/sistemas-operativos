#include "Model.h"


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