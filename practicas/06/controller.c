#include "Controller.h"


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
  char usuario[20];
  char contrasena[200];
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
  //prueba
  //endprueba
  strcpy(user->username,usuario);
  strcpy(user->password,contrasena);
  if(strcmp(usuario, "") == 0 && strcmp(contrasena, "") == 0) {
    //user->logged = 1;
    mvwprintw(win, 5, 2, "Inicio de sesión exitoso!");
  };
  mvwprintw(win, 5, 2, "Inicio de sesión fallido.");
  mvwprintw(win, 6, 2, "Intente de nuevo.");
  wrefresh(win);
  wgetch(win);
  delwin(win);
  endwin(); // Finalizar ncurses
  return;
};

void showRegisterPage(struct Usuario *usuario){
  return;
};
void signin(struct Usuario *usuario){
  char passwordFile [20];
  passwordFile[0] = '\0';
  getUser(usuario,passwordFile);
  descifrar(passwordFile);
  if(strcmp(passwordFile,usuario->password)==0){
    usuario->logged=1;
  };
  return;
};
void signup(struct Usuario *usuario){
  cifrar(usuario->password);
  setUser(usuario);
};
//Helpers
void getUser(struct Usuario *usuario,char *passwordFile){
  struct Usuario tmpUser;
  char linea[44];
  FILE *file;
  //semaforo
  openFile("usuario.txt",&file);
  //semaforo
  //Encuentra y retorna la linea en la que se encuetre la coincidencia
  while (fgets(linea, sizeof(linea),file) != NULL){
    if(sscanf(linea,"%s\t%s\n",tmpUser.username,tmpUser.password) ==2){
      if(strcmp(tmpUser.username,usuario->username)==0){
        strcpy(passwordFile,tmpUser.password);
        return;
      };
    };
  };
  //semaforo
  closeFile(&file);
  //semaforo
  return;
};
void setUser(struct Usuario *usuario){
  FILE *file;
  openFile("usuario.txt",&file);
  fprintf(file,"%s\t%s\n",usuario->username,usuario->password);
  closeFile(&file);
};

void cifrar(char *password){
  int clave = KEY;
  char caracter;
  for(int i =0;password[i] != '\0'; ++i) {
    caracter = password[i];
    // Cifrar solo letras mayúsculas
    if(caracter >= 'A' && caracter <= 'Z'){
      caracter = (caracter + clave - 'A' + 26) % 26 + 'A';
    };
    // Cifrar solo letras minúsculas
    if(caracter >= 'a' && caracter <= 'z'){
      caracter = (caracter + clave - 'a' + 26) % 26 + 'a';
    };
    if(caracter >= '0' && caracter <= '9'){
      caracter = (caracter + clave - '0' + 10) % 10 + '0';
    }
    password[i] = caracter;
  };
};

void descifrar(char *password){
  int clave = KEY;
  char caracter;
  for(int i =0;password[i] != '\0';i++) {
    caracter = password[i];
    // Descifrar solo letras mayúsculas
    if(caracter >= 'A' && caracter <= 'Z'){
      caracter = (caracter - clave - 'A' + 26) % 26 + 'A';
    };
    // Descifrar solo letras minúsculas
    if(caracter >= 'a' && caracter <= 'z'){
      caracter = (caracter - clave - 'a' + 26) % 26 + 'a';
    };
    //Numeros
    if(caracter >= '0' && caracter <= '9'){
      caracter = (caracter - clave - '0' + 10) % 10 + '0';
    }
    password[i] = caracter;
  }
};

void structToString(char *apt_salida,struct Usuario *usuario){
  apt_salida[0]='\0';
  sprintf(apt_salida,"%s\t%02x\n",usuario->username,usuario->password);
  return;
};

/************************* Errores *****************************/
void ErrorMessage(char *message){
  perror(message);
  exit(EXIT_FAILURE);
};
/****************************************************************/

/************************* Archivos *****************************/
void openFile(const char *fileName, FILE **file){
  *file = fopen(fileName,"a+");
  if(*file == NULL) ErrorMessage("No se pudo abrir el archivo");
};

void closeFile(FILE **file){
  fclose(*file);
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
