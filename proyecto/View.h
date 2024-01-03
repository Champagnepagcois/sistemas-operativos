#ifndef VIEW_H
#define VIEW_H

#include "Model.h"
#include "Controller.h"
#include <menu.h>
#include <curses.h>
#include <form.h>
#include <ctype.h>


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
void showPrincipalMenu(struct Usuario*);

void showNewSale(struct Usuario* user);
void showEditInventory(struct Usuario* user);
void showInfoSales(struct Usuario* user);
void showPageshoppingCart(struct Usuario *user);
void showPageInventory(struct Usuario *user);
void showPageTest(struct Usuario *user);
void showPageProducts(struct Usuario *user);
void showPageAdd(struct Usuario *user);
void showPageSells(struct Usuario *user);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
//Helpers
void print_menu(WINDOW *menu_win, int highlight, int n_choices,char **choices);
int isAllWhitespaceString(const char *str);

#endif