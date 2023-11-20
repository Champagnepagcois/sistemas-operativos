#include "Controller.h"

int main(){
  struct Usuario usuario;
  while (1){
    showAuthPage(&usuario);
    if(usuario.logged== 0){
      showRegisterPage(&usuario);
    };
    signup(&usuario);
  };
  return 0;
};


