#include "Controller.h"

int main(){

  return 0;
};

void showAuthPage(struct Usuario *user){

  initscr(); // Inicializar ncurses
  cbreak(); // Habilitar la entrada de caracteres sin necesidad de presionar "Enter"
  noecho(); // No mostrar caracteres en la pantalla mientras se escriben
  curs_set(1); // Mostrar el cursor
  keypad(stdscr, TRUE); // Habilitar el uso de teclas especiales
  int height = 10;
  int width = 40;
  int y = (LINES - height) / 2;
  int x = (COLS - width) / 2;
  // Crear una ventana para el formulario de inicio de sesión
  WINDOW *win = newwin(height, width, y, x);
  box(win, 0, 0);
  mvwprintw(win, 2, 2, "Usuario:");
  mvwprintw(win, 3, 2, "Contraseña:");
  wrefresh(win);
  char usuario[50];
  char contrasena[50];
  int ch;
  int contrasena_index = 0;
  // Capturar el usuario
  while (1) {
    ch = mvwgetch(win, 2, 15 + contrasena_index);
    if(ch == '\n' || ch == KEY_ENTER) break;
    if(contrasena_index < sizeof(usuario) - 1){
      usuario[contrasena_index++] = ch;
      waddch(win, ch);
      wrefresh(win);
    };
  };
  usuario[contrasena_index] = '\0';
  contrasena_index =0;
  // Capturar la contraseña y mostrar asteriscos
  while(1){
    ch = mvwgetch(win, 3, 15 + contrasena_index);
    if (ch == '\n' || ch == KEY_ENTER) break;
    if (contrasena_index < sizeof(contrasena) - 1) {
      contrasena[contrasena_index++] = ch;
      waddch(win, '*');
      wrefresh(win);
    };
  };
  contrasena[contrasena_index] = '\0';
  // Simular la autenticación
  if(strcmp(usuario, "") == 0 && strcmp(contrasena, "") == 0) {
    user->logged = 1;
    mvwprintw(win, 5, 2, "Inicio de sesión exitoso!");
  }else{
    mvwprintw(win, 5, 2, "Inicio de sesión fallido.");
    mvwprintw(win, 6, 2, "Intente de nuevo.");
  };
  wrefresh(win);
  wgetch(win);
  delwin(win);
  endwin(); // Finalizar ncurses
  return;
};
//Helpers

void structToString(char* apt_salida,void *data,int dataType){
  switch (dataType){
  case USUARIO:
    struct Usuario *usuario = (struct Usuario*)data;
    apt_salida[0]='\0';
    sprintf(apt_salida,"%s\t%s\t%d\n",usuario->nombre,usuario->password,usuario->typeUser);
    /*strcpy(*apt_salida,usuario->nombre);
    strcpy(*apt_salida,"\t");
    strcpy(*apt_salida,usuario->password);//Cambiar por modulo que cifra
    strcpy(*apt_salida,"\t");
    sprintf(*apt_salida,"%d",usuario->typeUser);*/
    break;
  default:
    break;
  }
  return;
};

/************************* Archivos *****************************/
void openFile(struct FileManager *fileManager){
  fileManager->file = fopen(fileManager->fileName,"a+");
  if(fileManager->file == NULL) ErrorMessage("No se pudo abrir el archivo");
};

void closeFile(struct FileManager *fileManager){
  fclose(fileManager->file);
};
/****************************************************************/

/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/

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
void initSemaphore(key_t *llave,int *semaforo,int *clave){
  *llave = ftok("archivo",*clave);
  *semaforo = Crea_semaforo(*llave,1);
};
int getValueSemaphore(int *id){
  return semctl(*id,0,GETVAL,0);
};
/****************************************************************/
