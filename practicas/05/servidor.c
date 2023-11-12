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
  *apt_request = 0;

  //getMemoryShared(CLAVE);
  /**/

  printf("\nServidor escuchando..\n");
  printf("\nEl valor en mc es: %d\n",*apt_request);
  printf("\nMi PID de servidor es:%d\n",getpid());
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
  };
  printf("\nDesactivando servicios del servidor\n");
  return 0;
};


/******** Crea el servicio (hilo) *********/


void service(pid_t *pid_client){
  //creo variable que voy a usar
  struct Request request;
  request.ID_client =  *pid_client;
  //Pedimos memoria compartida
  getMemoryShared(request.ID_client,&request);
  //Creamos hilos
  pthread_t id_thread;
  int threadCorrect;
  pthread_attr_t atributos;
  pthread_attr_init(&atributos);
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);
  threadCorrect = pthread_create(&id_thread,&atributos,serviceThread,(void *) &request);
  //printf("\nhilo creado\n");
  if(threadCorrect !=0){
    perror("\nError al crear hilo\n");
    exit(-1);
  };
  return;
};

/*********** Servicio del Hilo **************/
void *serviceThread(void *arg){
  struct Request *request = (struct Request *) arg;
  /* Crea semaforo para leer y escribir la mc*/
  key_t llave_hilo;
  int semaforo_shm_hc;
  llave_hilo = ftok("archivo",request->ID_client);
  semaforo_shm_hc = Crea_semaforo(llave_hilo,1);
  //se crea otro semaforo para escribir en un archivo
  key_t llave_hilo_io;
  int semaforo_hilo_io;
  llave_hilo_io = ftok("archivo",CLAVE_HILO_IO);
  semaforo_hilo_io = Crea_semaforo(llave_hilo_io,1);
  /**/
  request->ID_thread = pthread_self();
  request->ID_server = getppid();
  //request->apt_shm->ID_server= getpid();
  //request->apt_shm->ID_thread= pthread_self();
  //printf("\nID_hilo es :%d\n", pthread_self());
  shmWrite(request);
  while(1){ //lee frase del cliente
    down(semaforo_shm_hc);
    while(request->apt_shm->new_request ==1){
      printf("%s",request->apt_shm->message);
      down(semaforo_hilo_io);
      fileWrite(request);
      up(semaforo_hilo_io);
      request->apt_shm->new_request=0;
      request->apt_shm->message[0]= '\0';
    };
    up(semaforo_shm_hc);
    sleep(1);
  };
  pthread_exit(NULL);
};

void fileReader(){
  FILE* file;
  file = fopen("frases.txt","r");
  if(file ==NULL){
    perror("Error con el archivo");
    return;
  };
  
  // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long tamaño = ftell(file);

    if(tamaño == -1){
      perror("Error al obtener el tamaño del archivo");
      fclose(file);
      return;
    };

    // Retroceder hasta el inicio de la última línea
    fseek(file, -2, SEEK_END);  // Retrocedemos dos caracteres para evitar el salto de línea

    // Leer la última línea
    char buffer[200];  // Ajusta el tamaño del buffer según tus necesidades
    if(fgets(buffer, sizeof(buffer), file) != NULL){
      printf("La última línea del archivo es: %s", buffer);
    }else {
      perror("Error al leer la última línea");
    };
  fclose(file);
  return;
}

void fileWrite(struct Request *request){
  FILE* file;
  file = fopen("frases.txt","at");
  if(file ==NULL){
    perror("Error con el archivo");
    return;
  };
  fputs(request->apt_shm->message,file);
  fclose(file);
  return;
};

//escribir en la mc del hilo y cliente
void shmWrite(struct Request *request){
  request->apt_shm->connected =1;
  request->apt_shm->ID_server = request->ID_server;
  request->apt_shm->ID_thread = request->ID_thread;
  request->apt_shm->new_request = 1;
  return;
};

