#include "View.h"

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


char *choices[] = {
  "Agregar",
  "Ver elementos",
  "Cerrar sesion",
  (char *)NULL,
};

int choices_enabled[ARRAY_SIZE(choices)] = {1}; // Inicializado a todos habilitados

void toggle_item(int index) {
    choices_enabled[index] = !choices_enabled[index];
};
void ShowMainMenu(struct Usuario *user){
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;

  /* Inicializa curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);

  /* Crea los elementos */
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(10, 40, 4, 4);
  keypad(my_menu_win, TRUE);

  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_format(my_menu, 5, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, 40, "Mi Menu", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  mvprintw(LINES -3,0, "Use las fechas arriba y abajo para desplazarse por el menu");
  mvprintw(LINES -2,0, "Use Enter para seleccionar una opcion");
  //mvprintw(LINES - 2, 0, "Use PageUp y PageDown para desplazar una pagina de elementos abajo o arriba");
  //mvprintw(LINES - 1, 0, "Las flechas para navegar (F1 Para Salir)");
  attroff(COLOR_PAIR(2));
  refresh();

  while((c = wgetch(my_menu_win)) != KEY_F(1)){
    switch(c){
    case KEY_DOWN:
      menu_driver(my_menu, REQ_DOWN_ITEM);
      break;
    case KEY_UP:
      menu_driver(my_menu, REQ_UP_ITEM);
      break;
    case KEY_NPAGE:
      menu_driver(my_menu, REQ_SCR_DPAGE);
      break;
    case KEY_PPAGE:
      menu_driver(my_menu, REQ_SCR_UPAGE);
      break;
    case 10:
      ITEM *cur_item = current_item(my_menu);
      int cur_index = item_index(cur_item);
      if (choices_enabled[cur_index]) {
        // If the choice is enabled, toggle its state and process the action
        toggle_item(cur_index);

        // Lógica específica para cada opción
        switch (cur_index) {
          case 0:
            mvprintw(LINES -5,2, "Seleccionaste Agregar");
            break;
          case 1:
            mvprintw(LINES -5,2, "Seleccionaste ver elementos");
            break;
          case 2:
            mvprintw(LINES -5,2, "Seleccionaste cerrar sesion");
            sleep(3);
            exit(-1);
            break;
          // Agrega más casos según sea necesario para otras opciones
          // ...
          case 10: // Salir
            // Lógica para salir
            endwin(); // Cierra curses
            exit(EXIT_SUCCESS);
            break;
        }
      }
      break;
    };
    wrefresh(my_menu_win);
  };

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
};

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color){
  int length, x, y;
  float temp;

  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
};