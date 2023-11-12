#include "Modelo.h"

void verifyNewMessage(int *apt_hilo_mc){
  if(*apt_hilo_mc > 0){
    fileReader();
    *apt_hilo_mc -=1;
  };
  return;
};

/********Pide memoria compartida y asigna la direccion a un apuntador****/

void getMemoryShared(int clave, struct Request *request){
  int memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);//Crea un ID
  memoria = shmget(llave_memoria,sizeof(struct Request), IPC_CREAT | PERMISOS);//ID memoria compartida
  if (memoria == -1) {
    perror("\nError al pedir region en la memoria compartida\n");
    exit(-1);
  };
  request->apt_shm = (struct Request *)shmat(memoria,0,0);
  if(request->apt_shm == (struct Request *)(-1)){
    perror("\nError al asignar la memoria compartida a la Request\n");
    exit(-1);
  }
};
/*************************************************************************/


/******************Codigo del semaforo*************************/
int Crea_semaforo(key_t llave,int valor_inicial){
  int semid=semget(llave,1,IPC_CREAT|PERMISOS);
  if(semid==-1){
    return -1;
  };
  semctl(semid,0,SETVAL,valor_inicial);
  return semid;
};
void down(int semid){
  struct sembuf op_p[]={0,-1,0};
  semop(semid,op_p,1);
};
void up(int semid){
  struct sembuf op_v[]={0,+1,0};
  semop(semid,op_v,1);
};
/****************************************************************/

void fileReader(){
  FILE *archivo;
    char linea[256]; // Puedes ajustar el tamaño según tus necesidades
    linea[0]='\0';
    // Abre el archivo en modo lectura
    archivo = fopen("frases.txt", "r");
    if(archivo == NULL){
      perror("Error al abrir el archivo");
      return;
    };
    // Lee el archivo línea por línea hasta llegar al final
    while (fgets(linea, sizeof(linea), archivo) != NULL) {};
    // Ahora 'linea' contiene la última línea del archivo
    printf("La frase leia desde el archivo es: %s\n", linea);
    // Cierra el archivo
    fclose(archivo);
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
  //semaforo para ver si hay mensajes nuevos en el archivo
  key_t llave_hilo_mc;
  int semaforo_hilo_mc;
  llave_hilo_mc = ftok("archivo",CLAVE_HILO_IO);
  semaforo_hilo_mc = Crea_semaforo(llave_hilo_mc,1);
    //pedimos memoria compartida
      int memoria;
      key_t llave_hilo_mc_m = ftok("archivo",CLAVE_HILO_MC);
      memoria = shmget(llave_hilo_mc_m,sizeof(int),IPC_CREAT | PERMISOS);
      int *apt_hilo_mc = (int *)shmat(memoria,0,0);
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
      //Verificar si hay un texto nuevo en el archivo
      down(semaforo_hilo_mc);
      *apt_hilo_mc +=1;
      up(semaforo_hilo_mc);
      request->apt_shm->new_request=0;
      request->apt_shm->message[0]= '\0';
    };
    up(semaforo_shm_hc);
    sleep(1);
  };
  pthread_exit(NULL);
};


