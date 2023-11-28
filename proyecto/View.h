#ifndef VIEW_H
#define VIEW_H

#include "Model.h"
#include "Controller.h"
#include <menu.h>
#include <curses.h>


/******* CONSTANTES *******/
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD   4

/********* Definicion de estructuras *********/
struct Usuario;
struct Categoria;
struct Producto;
struct Venta;
struct DetalleVenta;
struct Proveedor;
struct Adquisicion;
//Muestra todos los datos al cliente

void showAuthPage(struct Usuario *);
void ShowMainMenu(struct Usuario *);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);


#endif