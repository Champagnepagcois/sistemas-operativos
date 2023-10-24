#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define NUM_COLUMNAS 9
#define NUM_FILAS 3


/***************manda mensaje de error y termina los procesos**************/
void crearError(){
  perror("Ocurrio un error al crear el hilo");
  exit(-1);
};
/***************************************************************************/

/*******************************Operacion hilo*******************************/
void *operacionHilo(void *arg){
  int *numeros = (int*)arg;
  int resultado = 1;
  printf("\nLa fila a operar es:\n");
  for(int i=0; i<NUM_COLUMNAS;i++){
    printf("\t%d",numeros[i]);
    resultado *= numeros[i];
  };
  printf(" =  %d\n",resultado);
}
/****************************************************************************/

void *test(void *arg){
  struct Parametro *param  =(struct Parametro*)arg;
  int resultado = param->resultadoMAtriz;
  for(int i=0; i<param->tam_col;i++){
    printf("\t%d",param->filaMAtriz[i]);
    resultado *= param->filaMAtriz[i];
  };
  printf(" =  %d\n",resultado);

  
}
/****Definimos la estructura que mandaremos como parametro a la funcion****/
typedef struct Parametro{ 
  int* filaMAtriz; //Definir la direccion de memoria donde empieza la fila de la matriz
  int* resultadoMAtriz;
  int tam_col; 
};

/****************************************************************************/
int main(){
  pthread_t id_hilo1,id_hilo2,id_hilo3;

  int matriz [] = {
    1,2,3,4,5,6,7,8,9,
    1,3,5,7,9,11,13,15,17,
    2,4,6,8,10,12,14,16,18
  };
  int resultadosMatriz [3];
  int hiloCorrecto[3];

  struct Parametro param[3];
  param[0].filaMAtriz = &matriz[0];
  param[0].resultadoMAtriz = &resultadosMatriz[0];
  param[0].tam_col = NUM_COLUMNAS;  

  param[1].filaMAtriz = &matriz[9];
  param[1].resultadoMAtriz = &resultadosMatriz[1];
  param[1].tam_col = NUM_COLUMNAS;  

  param[2].filaMAtriz = &matriz[18];
  param[2].resultadoMAtriz = &resultadosMatriz[2];
  param[2].tam_col = NUM_COLUMNAS;  

  //Creamos hilos
  printf("\n\n*****************Creamos hilos*****************\n\n");

  hiloCorrecto[0] = pthread_create(&id_hilo1,NULL, operacionHilo,(void*) test);
  if(hiloCorrecto[0]!= 0) crearError();
  printf("El identificador del hilo 1 es : %d\n",id_hilo1);

  hiloCorrecto[1] = pthread_create(&id_hilo2,NULL, operacionHilo,(void*)&(matriz[9]));
  if(hiloCorrecto[1]!= 0) crearError();
  printf("El identificador del hilo 2 es : %d\n",id_hilo2);
  
  hiloCorrecto[2] = pthread_create(&id_hilo3,NULL, operacionHilo,(void*)&(matriz[18]));
  if(hiloCorrecto[2]!= 0) crearError();

  printf("El identificador del hilo 3 es : %d\n",id_hilo3);

  //Esperamos a que los hilos acaben

  pthread_join(id_hilo1,NULL);
  pthread_join(id_hilo2,NULL);
  pthread_join(id_hilo3,NULL);

  printf("\n\n**************Los hilos han terminado*********************\n\n");

  return 0;
}