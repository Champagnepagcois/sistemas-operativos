#include "Controller.h"

void Auth(struct Usuario *user){
  showAuthPage(user);
};
void MainMenu(struct Usuario *user){
  //get all the data
  //ShowMainMenu(user);
  showPrincipalMenu(user);
  return;
};

//Pide mc y hace conexion, hace request
void c_login(struct Usuario *user){
  struct Request request;
  request.dataType = USUARIO;
  QuerySelect(&request,user,USUARIO);
  return;
};
void NuevaVenta(){
};
void getUser();

