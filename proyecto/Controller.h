#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "Model.h"
#include "View.h"

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/********* Definicion de estructuras *********/
struct Usuario;
struct Categoria;
struct Producto;
struct Venta;
struct DetalleVenta;
struct Proveedor;
struct Adquisicion;

//Se encarga de controlar todo

void Auth(struct Usuario *);
void MainMenu(struct Usuario *);
void c_login(struct Usuario *user);
void c_addNewProduct(struct Usuario *usuario,void **nodo);
void AddItems();
void GetItems();
void UpdateItems();
void DeleteItems();






#endif
