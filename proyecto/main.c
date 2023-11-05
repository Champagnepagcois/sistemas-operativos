#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "Controller.h"
#include "Model.h"
#include "View.h"



int main() {
    struct Usuario user;

    Auth(user); 
    
    return 0;
}