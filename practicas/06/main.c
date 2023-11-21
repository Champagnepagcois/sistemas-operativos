#include "Controller.h"

int main(){
  struct Usuario usuario;
  usuario.logged=0;
  while (1){
    showAuthPage(&usuario);
    if(usuario.logged== 0){
      //showRegisterPage(&usuario);
      signup(&usuario);
    };
  };
  return 0;
};


