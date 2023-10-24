#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define NUM_COLUMNAS 9
#define NUM_FILAS 3

/****Definimos la estructura que mandaremos como parametro a la funcion****/
typedef struct Parametro { 
  int* filaMAtriz; //Definir la direccion de memoria donde empieza la fila de la matriz
  int* resultadoMAtriz; //Definir la direccion de memoria del resultado
  int tam_col;
  int num_hilo;
  pthread_t id_hilo;
};

/****************************************************************************/
/***************manda mensaje de error y termina los procesos**************/
void crearError(){
  perror("Ocurrio un error al crear el hilo");
  exit(-1);
};
/***************************************************************************/

/*******************************Operacion hilo*******************************/
void *operacionHilo(void *arg){
  struct Parametro *param  =(struct Parametro*)arg;
  *(param->resultadoMAtriz) =1;
  printf("\nHola soy el hilo %d y mi identificador es: %d\n", param->num_hilo, param->id_hilo);
  printf("\nHilo %d:\tLa fila que voy a operar es:\n",param->num_hilo);
  printf("Hilo %d:",param->num_hilo);
  for(int i=0; i<param->tam_col;i++){
    printf("\t%d",param->filaMAtriz[i]);
    *(param->resultadoMAtriz) *= param->filaMAtriz[i];
  };
  printf("\nHilo %d: El resultado de multiplicar toda la fila es: %d\n",param->num_hilo,*(param->resultadoMAtriz));
};
/****************************************************************************/

int main(){
  pthread_t id_hilo[3];

  int matriz [] = {
    1,2,3,4,5,6,7,8,9,
    1,3,5,7,9,11,13,15,17,
    2,4,6,8,10,12,14,16,18
  };
  
  int resultadosMatriz [3];
  int hiloCorrecto[3];

  struct Parametro param[3];
  //Inicializamos las estructuras
  for(int i=0;i<NUM_FILAS;i++){
  int posicion = i==0?0:i==1?9:18;
  param[i].filaMAtriz = &matriz[posicion];
  param[i].resultadoMAtriz = &resultadosMatriz[i];
  param[i].tam_col = NUM_COLUMNAS;  
  param[i].num_hilo =i+1;
  param[i].id_hilo =id_hilo[i];
  };


  //Creamos hilos
  printf("\n\n*****************Creamos hilos*****************\n\n");

  for(int i=0;i<NUM_FILAS;i++){
    hiloCorrecto[i] = pthread_create(&(id_hilo[i]),NULL, operacionHilo,(void*) &(param[i]));
    param[i].id_hilo = id_hilo[i];
    if(hiloCorrecto[i]!= 0) crearError();
  };

  //Esperamos a que los hilos acaben
  for(int i =0;i<NUM_FILAS;i++){
    pthread_join(id_hilo[i],NULL);
  };
  printf("\n\n**************Los hilos han terminado*********************\n\n");

  //Mostramos resultados desde el hilo padre
  printf("\n\n**************Proceso padre muestra los resultados*********************\n\n");
  for(int i=0;i<NUM_FILAS;i++){
    printf("Resultados de la operacion en la fila %d es : %d\n",i+1,resultadosMatriz[i]);
  };

  return 0;
}