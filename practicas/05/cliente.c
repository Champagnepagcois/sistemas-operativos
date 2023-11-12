#include <stdio.h>
#include "Modelo.h"

int main(){
  printf("Conectando con el servidor...\n");

  /* Crea semaforo para las request*/
  key_t llave_servidor;
  int semaforo_request;
  llave_servidor = ftok("archivo",CLAVE_SERVIDOR_SEM);
  semaforo_request = Crea_semaforo(llave_servidor,1);
  printf("\nConexion establecida\n");
  /**/

  /* pide memoria compartida para almacenar  */
  int memoria;
  //key_t llave_servidor;
  llave_servidor = ftok("archivo",CLAVE_SERVIDOR_MC); 
  memoria = shmget(llave_servidor,sizeof(pid_t), IPC_CREAT | PERMISOS); //ID MC
  if(memoria ==-1){
    perror("Error al crear MC del servidor");
    exit(-1);
  };
  pid_t *apt_request = (pid_t *)shmat(memoria,NULL,0); //Asigna la direccion a mi apuntador
  if(apt_request == (void*) -1){
    perror("Error al asignar DM al apuntador");
    exit(-1);
  };

  //le decimos al servidor que hay un una request
  down(semaforo_request);
  //printf("\nEl valor en mc antes: %d\n",*apt_request);
  *apt_request = getpid();
  //printf("\nEl PID es: %d\n",getpid());
  //printf("\nEl valor en mc despues: %d\n",*apt_request);
  up(semaforo_request);

  printf("\nListo para la comunicacion\n");

  char frase[200];

  //Creamos Request
  struct Request request;
  request.ID_client = getpid();
  //Pedimos memoria compartida
  getMemoryShared(request.ID_client,&request);
  request.apt_shm->message[0] = '\0';
  //printf("\nEl ID_SERVER es %d\n",request.apt_shm->ID_server);
  //printf("\nEl ID_HILO es %d\n",request.apt_shm->ID_thread);

  while(1){
    if(kill(request.apt_shm->ID_server,0)== -1){
      perror("El servidor se ha cerrado");
      exit(-1);
    };
    printf("\nEscribe tu frase: \n");
    fgets(frase,sizeof(frase),stdin);

    strcat(request.apt_shm->message,frase);
    request.apt_shm->new_request =1;
  };



  return 0;
};