#include "Controller.h"

void Auth(struct Usuario *user){
  showAuthPage(user);
  return;
};
void MainMenu(struct Usuario *user){
  struct Producto *nodo = NULL;
  showPrincipalMenu(user,(void**)&nodo);
  return;
};

//inicia sesion
void c_login(struct Usuario *user){
  struct Request request;
  struct Usuario *nodo = NULL;
  enQueue((void**)&nodo,user,USUARIO);
  QuerySelect(&request,(void**)&nodo,USUARIO);
  deQueue((void*)user,(void**)&nodo,USUARIO);
  return;
};

//Agregar datos
void c_addNewProduct(struct Usuario *user,void **nodo){
  struct Request request;
  QueryInsert(&request,nodo,PRODUCTO);
  return;
};

//Traer productos
void c_getAllProducts(struct Usuario *user, char *productList){
  struct Request request;
  struct Producto *nodo = NULL;
  QuerySelect(&request,(void**)&nodo,PRODUCTO);
  //pasamos info de los nodos a string


};

//Editar productos
