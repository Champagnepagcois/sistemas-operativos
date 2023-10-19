#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>

#define PERMISOS 0644
#define NUM_COLUMNAS 9
#define NUM_FILAS 3
#define CLAVE_MAT_A 'A'
#define CLAVE_MAT_B 'B'


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
}
/****************************************************************/

/********Pide memoria compartida y asigna la direccion a un apuntador****/

void getMemoryShared(int clave, int tam_memo, void  **apt_mat){
  int memoria;
  int *dato_memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);//Crea un ID
  memoria = shmget(llave_memoria,tam_memo*sizeof(int), IPC_CREAT | PERMISOS);//ID memoria compartida
  if (memoria == -1) {
    perror("\nError al pedir region en la memoria compartida\n");
    exit(-1);
  };
  *apt_mat = (int *)shmat(memoria,0,0); //direccion de la MC
};
/*************************************************************************/

/***********Libera la memoria compartida******************/

void freeMemoryShared(int clave){
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);
  shmctl(llave_memoria, IPC_RMID,0);
};
/*********************************************************/


/****************Imprimir matriz completa*********************/
void ImprimirMatrizCompleta(int *matriz, int columnas, int filas){
  for(int i=0; i< columnas*filas;i++){
    if(i%columnas == 0){
      printf("\n");
    };
    printf("\t%d",matriz[i]);
  }
}
/**************************************************************/


/***************Se presenta el hijo**************/
void presentarse(int numero_hijo){
  printf("\nHijo %d: Hola, soy el proceso hijo con PID: %d\n",numero_hijo,getpid());
  printf("Hijo %d: Mi proceso padre tiene el pid:%d\n",numero_hijo,getppid());
};
/**************************************************/
/***************manda mensaje de error y termina los procesos**************/
void crearError(){
  perror("Error al crear el proceso");
  exit(-1);
};
/***************************************************************************/
/********************Imprime una unica fila de la matriz********************/
void imprimirFila(int *matriz, int tam_columna, int pos_fila){
  for(int i=0;i<tam_columna;i++){
    printf("  %d",matriz[pos_fila * tam_columna +i]);
    if(i!=tam_columna-1){
      printf(",");
    };
  }
};
/****************************************************************************/

/*********************Suma una fila de la matriz*****************************/
int sumaFila( int *matriz, int tam_columna,int num_fila){
  int resultado =0;
  for(int i=0;i<tam_columna;i++){
    resultado += matriz[num_fila* tam_columna + i];
  };
  return resultado;
}

/****************************************************************************/

/****************Region critica lectura*********************/
void leerRegionCritica(int *apt_mat_a, int num_col, int fila,int hijo){
  printf("\n----ENTRA A REGION CRITICA DE LECTURA---\n");
  printf("Hijo %d: La fila a operar es:\t", hijo);
  imprimirFila(apt_mat_a,num_col,fila);
  printf("\n----SALE DE REGION CRITICA DE LECTURA---\n\n");
};

/***********************************************************/

/***************Region critica escritura********************/
void escribirRegionCritica(int *apt_mat_a,int *apt_mat_b, int num_col, int fila,int hijo){
  printf("\n----ENTRA A REGION CRITICA DE ESCRITURA---\n");
  printf("\nHijo %d: El resultado de la suma es:%d\n", hijo ,sumaFila(apt_mat_a,NUM_COLUMNAS,fila));
  *apt_mat_b= sumaFila(apt_mat_a,NUM_COLUMNAS,fila);
  printf("\n----SALE DE REGION CRITICA DE ESCRITURA---\n");
};
/**********************************************************/

int main(){
  int semafoto_memoria_leer,semaforo_memoria_escribir;
  key_t llave_acceder, llave_ingresar;
  llave_acceder = ftok("archivo",'m');
  llave_ingresar = ftok("archivo3",'k');
  semafoto_memoria_leer = Crea_semaforo(llave_acceder,1);
  semaforo_memoria_escribir = Crea_semaforo(llave_ingresar,1);

  int matriz [] = {
    1,2,3,4,5,6,7,8,9,
    1,3,5,7,9,11,13,15,17,
    2,4,6,8,10,12,14,16,18
  };
  
  //Pide MC
  printf("\n\n*****************Solicitamos una region de memoria compartida*****************\n\n");

  int *apt_mat_a, *apt_mat_b;
  getMemoryShared(CLAVE_MAT_A, NUM_FILAS*NUM_COLUMNAS,(void **)&apt_mat_a);
  getMemoryShared(CLAVE_MAT_B, NUM_FILAS,(void **) &apt_mat_b);

  //Asignamos valores en la MC
  printf("\nPadre: Se asignan los valores de la matriz en la region de memoria compartida.....\n");
  for(int i=0;i<NUM_COLUMNAS*NUM_FILAS;i++){
    apt_mat_a[i] = matriz[i];
  };

  printf("\nPadre: La matriz colocada en la region de memoria compartida es:\n\n\n");
  ImprimirMatrizCompleta(apt_mat_a,NUM_COLUMNAS,NUM_FILAS);


  /*************Codigo para crear procesos hjos*******************/
  printf("\n\n********************Empezamos a crear los procesos hijos********************\n\n");
  pid_t pid1, pid2, pid3;

  pid1 = fork();
  if(pid1 ==-1) crearError();
  if(pid1==0){
    presentarse(1);
    down(semafoto_memoria_leer); //Accedemos a region critica lectura
    leerRegionCritica(apt_mat_a,NUM_COLUMNAS,0,1);
    up(semafoto_memoria_leer); //Salimos de region critica lectura
    down(semaforo_memoria_escribir); //Accedemos a region critica escritura
    escribirRegionCritica(apt_mat_a,&apt_mat_b[0],NUM_COLUMNAS,0,1);
    up(semaforo_memoria_escribir); //Salimos de region critica escritura
    //sleep(2);
    exit(0);
  };

  pid2 = fork();
  if(pid2 ==-1) crearError();
  if(pid2==0){
    presentarse(2);
    down(semafoto_memoria_leer); //Accedemos a region critica lectura
    leerRegionCritica(apt_mat_a,NUM_COLUMNAS,1,2);
    up(semafoto_memoria_leer); //Salimos de region critica lectura
    down(semaforo_memoria_escribir); //Accedemos a region critica escritura
    escribirRegionCritica(apt_mat_a,&apt_mat_b[1],NUM_COLUMNAS,1,2);
    up(semaforo_memoria_escribir); //Salimos de region critica escritura
    //sleep(2);
    exit(0);
  };

  pid3 = fork();
  if(pid3 ==-1) crearError();
  if(pid3==0){
    presentarse(3);
    down(semafoto_memoria_leer); //Accedemos a region critica lectura
    leerRegionCritica(apt_mat_a,NUM_COLUMNAS,2,3);
    up(semafoto_memoria_leer); //Salimos de region critica lectura
    down(semaforo_memoria_escribir); //Accedemos a region critica escritura
    escribirRegionCritica(apt_mat_a,&apt_mat_b[2],NUM_COLUMNAS,2,3);
    up(semaforo_memoria_escribir); //Salimos de region critica escritura
    //sleep(2);
    exit(0);
  };


  //El padre espera que los procesos hijos acaben
  int numeroProcesos=3;
  for(int i=0; i<numeroProcesos;i++){
    wait(NULL);
  };

  printf("\n\n**************Los procesos hijos han terminado*********************\n\n");

  //Imprime la matriz de resultados
  printf("\n\nLos resultados de la suma guardados en la region de memoria compartida, mostrados desde el proceso padre son:\n");
  ImprimirMatrizCompleta(apt_mat_b,1,3);

  freeMemoryShared(CLAVE_MAT_A);
  freeMemoryShared(CLAVE_MAT_B);

  //sleep(4);
  return 0;
}