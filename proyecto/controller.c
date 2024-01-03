#include "Controller.h"

void Auth(struct Usuario *user){
  showAuthPage(user);
  return;
};
void MainMenu(struct Usuario *user){
  //get all the data
  //ShowMainMenu(user);
  showPrincipalMenu(user);
  return;
};

//inicia sesion
void c_login(struct Usuario *user){
  struct Request request;
  struct Usuario *nodo = NULL;
  QuerySelect(&request,user,(void**)&nodo,USUARIO);
  return;
};

//Agregar datos
void c_addNewProduct(struct Usuario *user,void **nodo){
  struct Request request;
  QueryInsert(&request,nodo,PRODUCTO);
  return;
};


//Traer productos

//Editar productos