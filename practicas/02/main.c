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

void GetMemoryShared(int clave, int tam_memo, void  **apt_mat){
  int memoria;
  int *dato_memoria;
  key_t llave_memoria;
  llave_memoria = ftok("archivo",clave);//Crea un ID
  memoria = shmget(llave_memoria,tam_memo*sizeof(int), IPC_CREAT | PERMISOS);//ID memoria compartida
  if (memoria == -1) {
    perror("\nError al crear la memoria compartida\n");
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
  for(int i=0;i<filas;i++){
    for(int j=0;j<columnas;j++){
      printf("\t%d",matriz[j+i]);
    };
    printf("\n");
  };
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


int main(){
  int semafoto_memoria;
  key_t llave;
  llave = ftok("archivo",'m');
  semafoto_memoria = Crea_semaforo(llave,1);

  int matriz [] = {
    1,2,3,4,5,6,7,8,9,
    1,3,5,7,9,11,13,15,17,
    2,4,6,8,10,12,14,16,18
  };
  
  //Pide MC
  printf("\n\n*****************Solicitamos una region de memoria compartida*****************\n\n");

  int *apt_mat_a, *apt_mat_b;
  GetMemoryShared(CLAVE_MAT_A, NUM_FILAS*NUM_COLUMNAS,(void **)&apt_mat_a);
  GetMemoryShared(CLAVE_MAT_B, NUM_FILAS,(void **) &apt_mat_b);

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
    //Accedemos a la MC
    down(semafoto_memoria);
    printf("Hijo 1: La fila a operar es:\t");
    imprimirFila(apt_mat_a,NUM_COLUMNAS,0);
    printf("\nHijo 1: El resultado de la suma es:%d\n", sumaFila(apt_mat_a,NUM_COLUMNAS,0));
    apt_mat_b[0]= sumaFila(apt_mat_a,NUM_COLUMNAS,0);
    //sleep(2);
    //Dejamos la MC
    up(semafoto_memoria);
    exit(0);
  };

  pid2 = fork();
  if(pid2 ==-1) crearError();
  if(pid2==0){
    presentarse(2);
    //Accedemos a la MC
    down(semafoto_memoria);
    printf("Hijo 2:La fila a operar es:\t");
    imprimirFila(apt_mat_a,NUM_COLUMNAS,1);
    printf("\nHijo 2: El resultado de la suma es:%d\n", sumaFila(apt_mat_a,NUM_COLUMNAS,1));
    apt_mat_b[1]= sumaFila(apt_mat_a,NUM_COLUMNAS,1);
    //sleep(3);
    //Dejamos la MC
    up(semafoto_memoria);
    exit(0);
  };

  pid3 = fork();
  if(pid3 ==-1) crearError();
  if(pid3==0){
    presentarse(3);
    //Accedemos a la MC
    down(semafoto_memoria);
    printf("Hijo 3: La fila a operar es:\t");
    imprimirFila(apt_mat_a,NUM_COLUMNAS,2);
    printf("\nHijo 3: El resultado de la suma es:%d\n", sumaFila(apt_mat_a,NUM_COLUMNAS,2));
    apt_mat_b[2]= sumaFila(apt_mat_a,NUM_COLUMNAS,2);
    //sleep(4);
    //Dejamos la MC
    up(semafoto_memoria);
    exit(0);
  };


  //El padre espera que los procesos hijos acaben
  int numeroProcesos=3;
  for(int i=0; i<numeroProcesos;i++){
    wait(NULL);
  };

  //Imprime la matriz de resultados
  printf("\n\nLos resultados de la suma, mostrados en el proceso padre son:\n");
  ImprimirMatrizCompleta(apt_mat_b,1,3);

  freeMemoryShared(CLAVE_MAT_A);
  freeMemoryShared(CLAVE_MAT_B);

  //sleep(5);
  return 0;
}