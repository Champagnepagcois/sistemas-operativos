#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#include "Model.h"

//Muestra todos los datos al cliente

void showAuthPage(struct Usuario user);
void showProductsPage();


#endif