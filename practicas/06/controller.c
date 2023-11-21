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
  char usuario[21];
  char contrasena[21];
  int ch;
  int contrasena_index = 0;
  // Capturar el usuario
  while (1){
    ch = mvwgetch(win, 2, 15 + contrasena_index);
    if ((ch == '\n' || ch == KEY_ENTER) && contrasena_index>0) break;
    if (ch == KEY_BACKSPACE || ch == 127) {
      if (contrasena_index > 0){
        usuario[--contrasena_index] = '\0';
        mvwaddch(win, 2, 15 + contrasena_index, ' ');
        wrefresh(win);
      }
    }else if (contrasena_index < sizeof(usuario) - 1){
      if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
        usuario[contrasena_index++] = ch;
        waddch(win, ch);
        wrefresh(win);
      };
    };
};
  usuario[contrasena_index] = '\0';
  contrasena_index =0;
  // Capturar la contraseña y mostrar asteriscos
  while(1){
    ch = mvwgetch(win, 3, 15 + contrasena_index);
    if ((ch == '\n' || ch == KEY_ENTER) && contrasena_index>0) break;
    if(ch == KEY_BACKSPACE || ch ==127){
      if (contrasena_index > 0){
        contrasena[--contrasena_index] = '\0';
        mvwaddch(win, 3, 16 + contrasena_index, ' ');
        wrefresh(win);
      }
    }else if (contrasena_index < sizeof(contrasena) - 1) {
      if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch >=48 && ch<=57){    
        contrasena[contrasena_index++] = ch;
        waddch(win, '*');
        wrefresh(win);
      };
    };
  };
  contrasena[contrasena_index] = '\0';
  // Simular la autenticación
  //prueba
  //endprueba
  user->username[0] = '\0';
  user->password[0] = '\0';
  strcpy(user->username,usuario);
  strcpy(user->password,contrasena);
  signin(user);
  //if(strcmp(usuario, "a") == 0 && strcmp(contrasena, "a") == 0) {
  if(user->logged ==1){  
    //user->logged = 1;
    mvwprintw(win, 5, 2, "Inicio de sesión exitoso!");
  }else{
    mvwprintw(win, 5, 2, "Inicio de sesión fallido.");
    mvwprintw(win, 6, 2, "Nuevo usuario creado");
  };
  
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
  char passwordFile [21];
  passwordFile[0] = '\0';
  getUser(usuario,passwordFile);
  cleanText(passwordFile);
  //descifrar(passwordFile);
  cifrar(usuario->password);
  printf("%s",passwordFile);
  if(strcmp(passwordFile,usuario->password) ==0){
    usuario->logged=1;
  };
  return;
};
void signup(struct Usuario *usuario){
  cifrar(usuario->password);
  setUser(usuario);
};
//Helpers
void cleanText(char *cadena){
  int i = 0, j = 0;
  // Encuentra la posición del primer carácter que no sea un espacio en blanco
  while (cadena[i] == ' ' || cadena[i] == '\t' || cadena[i] == '\n') {
    i++;
  }
  // Mueve los caracteres restantes al principio de la cadena
  while (cadena[i] != '\0') {
    cadena[j] = cadena[i];
    i++;
    j++;
  };
  // Agrega el terminador nulo al final de la cadena
  cadena[j] = '\0';
}
void getUser(struct Usuario *usuario,char *passwordFile){
  struct Usuario tmpUser;
  tmpUser.password[0] = '\0';
  passwordFile[0] = '\0';
  char linea[42];
  FILE *file;
  //semaforo
  openFile("usuario.txt",&file);
  //semaforo
  //Encuentra y retorna la linea en la que se encuetre la coincidencia
  while(fgets(linea, sizeof(linea),file) != NULL){
    if(sscanf(linea,"%20s\t%20s\n",tmpUser.username,tmpUser.password) ==2){
      //printf("\n%s  %s\n\t",tmpUser.username, tmpUser.password);
      if(strcmp(tmpUser.username,usuario->username)==0){
        strcat(passwordFile,tmpUser.password);
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
  fprintf(file,"%20s\t%20s\n",usuario->username,usuario->password);
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
  };
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
