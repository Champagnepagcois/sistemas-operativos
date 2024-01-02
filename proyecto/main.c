#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "Model.h"
#include "View.h"
#include "Controller.h"



int main() {
  struct Usuario user;
  while(user.logged!=1){
  Auth(&user);
  };
  while(user.logged ==1){
    MainMenu(&user);
  };
  return 0;
};