#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "Controller.h"
#include "Model.h"
#include "View.h"



int main() {
  struct Usuario user;
    
  struct Query consulta;
  struct QueryKeyValue kv;
  struct QueryKeyValue* apt_kv = &kv;
  struct Query* apt_consulta = &consulta;
  apt_kv->next= NULL;
  apt_consulta->keyValue = &kv;
  apt_consulta->keyValue->next= NULL;
  strcpy(apt_consulta->keyValue->field,"Nombre");
  strcpy(apt_consulta->keyValue->value,"Marlon");
  apt_consulta->table = USUARIO;
  apt_consulta->typeQuery= INSERT;
  apt_consulta->fileName[0] = '\0';
  apt_consulta->chmod[0] = '\0';
  strcpy(apt_consulta->fileName,"prueba.txt");
  strcpy(apt_consulta->chmod,"a");
  Auth(user);
  if(user.logged){
    printf("\n\nLogeado\n\n");
  };
  newQuery(apt_consulta);
  //printf("%s",apt_consulta->fileName);
    
  return 0;
};