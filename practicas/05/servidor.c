#include "Modelo.h"

int main(){
  printf("Activando servicios de servidor..\n");
  /* Crea semaforo para las request*/
  key_t llave_servidor;
  int semaforo_request;
  llave_servidor = ftok("archivo",CLAVE_SERVIDOR_SEM);
  semaforo_request = Crea_semaforo(llave_servidor,1);
  printf("Servicios listos\n");
  /**/

  /* pide memoria compartida para almacenar  */
  int memoria;
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
  *apt_request = 0;
  //semaforo para ver si hay mensajes nuevos en el archivo
  key_t llave_hilo_mc;
  int semaforo_hilo_mc;
  llave_hilo_mc = ftok("archivo",CLAVE_HILO_IO);
  semaforo_hilo_mc = Crea_semaforo(llave_hilo_mc,1);
  //pedimos memoria compartida
    int memoria_h_c;
    key_t llave_hilo_mc_m = ftok("archivo",CLAVE_HILO_MC);
    memoria_h_c = shmget(llave_hilo_mc_m,sizeof(int),IPC_CREAT | PERMISOS);
    int *apt_hilo_mc = (int *)shmat(memoria_h_c,0,0);
    *apt_hilo_mc =0;


  printf("\nServidor escuchando..\n");
  pid_t aux = *apt_request;
  while(1){
    down(semaforo_request);
    while(aux != *apt_request){ //hay una request
      printf("\nConexion con un cliente nuevo\n");
      service(apt_request);
      aux=*apt_request;
    };
    up(semaforo_request);
    sleep(1);
    down(semaforo_hilo_mc);
    verifyNewMessage(apt_hilo_mc);
    up(semaforo_hilo_mc);
  };
  printf("\nDesactivando servicios del servidor\n");
  return 0;
};

