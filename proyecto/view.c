#include "View.h"

void showAuthPage(struct Usuario *user){
  user->logged=0;//Formazos a cerrar sesion
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
  char usuario[25];
  char contrasena[20];
  int ch;
  int contrasena_index = 0;
  // Capturar el usuario
  while (1) {
  ch = mvwgetch(win, 2, 15 + contrasena_index);
  if(ch == '\n' || ch == KEY_ENTER) break;

  if (ch == 127 ||  ch ==8) {
    // Utilizar código ASCII para "delete" (127)
    if (contrasena_index > 0) {
      contrasena_index--;
      wmove(win, 2, 15 + contrasena_index);
      wdelch(win);
      wmove(win, 2, 15 + contrasena_index);
      waddch(win,32);
      wrefresh(win);
    }
  }else if(contrasena_index < sizeof(usuario) - 1){
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
    if(ch == 8){
      if(contrasena_index>0){
        contrasena_index--;
        wmove(win,3,15 + contrasena_index);
        wdelch(win);
        wrefresh(win);
      }
    }else if (contrasena_index < sizeof(contrasena) - 1) {
      contrasena[contrasena_index++] = ch;
      waddch(win, '*');
      wrefresh(win);
    };
  };
  contrasena[contrasena_index] = '\0';
  // Simular la autenticación
  strcpy(user->usuario, usuario);
  strcpy(user->password, contrasena);
  c_login(user);
  if(user->logged == 1) {
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
void showPrincipalMenu(struct Usuario *user){
  if(user->logged ==0){
    showAuthPage(user);
  };
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  char *choices [4] = {
    "Nueva venta",
    "Editar inventario",
    "Ventas",
    "Salir"
  };
  int n_choices = sizeof(choices) / sizeof(char *);
  int WIDTH=20;
  int HEIGHT =screen_height-2;
  WINDOW *menu_win;
  int highlight = 1;
  int choice = 0;
  int c;

  initscr();
  clear();
  noecho();
  cbreak();   /* Buffer de linea desactivado. Pasar de todo */
  int startx =0;
  int starty =1;
  char titleMenu [] ={"Menu principal"};
  mvprintw(0,((screen_width-(sizeof(titleMenu))/sizeof(char*))/2)-((sizeof(titleMenu)/sizeof(char*))/2),titleMenu);
  refresh();
  menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(menu_win, TRUE);
  mvprintw(screen_height-1, 0, "Utilice las flechas para ir arriba y abajo, Presione enter para elegir");
  refresh();
  print_menu(menu_win, highlight,n_choices,choices);
  while(1){ 
    c = wgetch(menu_win);
    switch(c) {
      case KEY_UP:
        if(highlight == 1){
          highlight = n_choices;
        };
        --highlight;
        break;
      case KEY_DOWN:
        if(highlight == n_choices){
          highlight = 1;
        };
        ++highlight;
        break;
      case 10:
        choice = highlight;
        break;
      default:
        mvprintw(24, 15, "Caracter presionado = %3d Ojala se imprima como '%c'", c, c);
        refresh();
        break;
      }
    print_menu(menu_win, highlight,n_choices,choices);
    if(choice != 0) break;/* El usuario eligio salir del bucle infinito */    
  }
  mvprintw(screen_height-1, 0, "Elegiste la opcion %d con la cadena %s\n", choice, choices[choice - 1]);
  switch (choice)
  {
  case 1:
    clear();
    refresh();
    showNewSale(user);
    break;
  case 2:
    //showEditInventory(user);
    clear();
    refresh();
    //showPageTest(user);
    showPageInventory(user);
    //showPageshoppingCart(user);
  break;
  case 3:
    clear();
    refresh();
    showInfoSales(user);
    break;
  case 4:
    clear();
    refresh();
    showAuthPage(user);
  default:
    break;
  } 
  clrtoeol(); //Borra todo el contenido desde la ubicacion del cursor hasta el final.
  refresh();
  getch();
  endwin();
  return;
};

void showNewSale(struct Usuario *user){
  int choice = 0;
  char *choices []= {
    //"leche lala 1 lt      26.00      1        Leche entera de vaca, Vitamina A y Vitamina D      cocina",
    "pan Bimbo 680 g      47.50      2        Pan blanco Bimbo grande 680 g                      cocina",
    NULL,
  };
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;

  //Agrega el total del carrito
  mvprintw(LINES - 4, 0, "Monto total: $%f", 95.0);
  refresh();

  /* Inicializa curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /* Crea los elementos */
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    //my_items[i] = new_item(choices[i], choices[i]);
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(screen_height-5 ,screen_width,0, 0);
  keypad(my_menu_win, TRUE);


  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  //set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_sub(my_menu, derwin(my_menu_win, screen_height-10, screen_width-2, 4, 1));
  set_menu_format(my_menu, screen_height-10, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, screen_width, "Nueva venta", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  //mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, screen_width-2);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvwaddch(my_menu_win, 2, screen_width-1, ACS_RTEE);

  /*Imprime la cabecera de los productos*/
  //print_in_middle(my_menu_win, 3, 0, screen_width/2, "Nombre              precio    cantidad  descripcion", COLOR_PAIR(3));
  //mvwprintw(my_menu_win, 3, 4, "%s", "Nombre              precio    cantidad  descripcion                                                                                           Categoria  \n");
  mvwprintw(my_menu_win, 3, 4, "%s", "Nombre               precio     cantidad descripcion                                        Categoria  \n");
  
  wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  //mvprintw(LINES - 2, 0, "Use PageUp y PageDown para desplazar una pagina de elementos abajo o arriba");
  mvprintw(LINES - 2, 0, "Use | F2 para agregar un producto| F3 para aumentar catidad| F4 para disminuir cantidad| ");
  mvprintw(LINES - 1, 0, "Las flechas para navegar| F1 Para salir | F10 para terminar venta | F11 para cancelar venta");
  attroff(COLOR_PAIR(2));
  refresh();

  wattron(my_menu_win, COLOR_PAIR(2));
  mvwprintw(my_menu_win,-3, screen_width/2, "VENTA FINALIZADA!!");
  wattroff(my_menu_win, COLOR_PAIR(2));
  wrefresh(my_menu_win);

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
    case '\n':  // Añade este caso para Enter
      ITEM *selected_item;
      selected_item= current_item(my_menu);
      choice = item_index(selected_item);
      move(LINES - 4, 0);
      clrtoeol();
      move(LINES - 3, 0);
      clrtoeol();
      mvprintw(LINES - 4, 0, "Seleccionaste la opción %d: %s", choice + 1, choices[choice]);
      refresh();
      //print_in_middle(my_menu_win,(screen_height/2)+5,0,screen_width,"Monto total:$",COLOR_PAIR(2));
      // mvwprintw(my_menu_win, 3,screen_width/2, "%s", "Monto total: $\n");
      wattroff(my_menu_win,COLOR_PAIR(3));
      refresh();
      break;
    }
    wrefresh(my_menu_win);
  };
  //mvwprintw(my_menu_win, 3,screen_width/2, "%s", "Monto total: $\n");
  //wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
};



void showPageInventory(struct Usuario* user){
  if(user->logged ==0){
    showAuthPage(user);
  };
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  char *choices [] = {
    "Agregar producto",
    "Actualizar producto",
    "Eliminar producto",
    "Ver productos",
    "Salir"
  };
  int n_choices = sizeof(choices) / sizeof(char *);
  int WIDTH=25;
  int HEIGHT =screen_height-2;
  WINDOW *menu_win;
  int highlight = 1;
  int choice = 0;
  int c;

  initscr();
  clear();
  noecho();
  cbreak();   /* Buffer de linea desactivado. Pasar de todo */
  int startx =0;
  int starty =1;
  char titleMenu [] ={"Editar productos"};
  mvprintw(0,((screen_width-(sizeof(titleMenu))/sizeof(char*))/2)-((sizeof(titleMenu)/sizeof(char*))/2),titleMenu);
  refresh();
  menu_win = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(menu_win, TRUE);
  mvprintw(screen_height-1, 0, "Utilice las flechas para ir arriba y abajo, Presione enter para elegir");
  refresh();
  print_menu(menu_win, highlight,n_choices,choices);
  while(1){ 
    c = wgetch(menu_win);
    switch(c) {
      case KEY_UP:
        if(highlight == 1){
          highlight = n_choices;
        };
        --highlight;
        break;
      case KEY_DOWN:
        if(highlight == n_choices){
          highlight = 1;
        };
        ++highlight;
        break;
      case 10:
        choice = highlight;
        break;
      default:
        mvprintw(24, 15, "Caracter presionado = %3d Ojala se imprima como '%c'", c, c);
        refresh();
        break;
      }
    print_menu(menu_win, highlight,n_choices,choices);
    if(choice != 0) break;/* El usuario eligio salir del bucle infinito */    
  }
  //mvprintw(screen_height-1, 0, "Elegiste la opcion %d con la cadena %s\n", choice, choices[choice - 1]);
  switch (choice){
  case 1:
    clear();
    refresh();
    showPageAdd(user);
    break;
  case 2:
    //showEditInventory(user);
    clear();
    refresh();
    //showPageTest(user);
    //showPageshoppingCart(user);
  break;
  case 3:
    clear();
    refresh();
    showInfoSales(user);
    break;
  case 4:
    clear();
    refresh();
    showPageProducts(user);
    break;
  case 5:
    clear();
    refresh();
    showPrincipalMenu(user);
    break;
  default:
    break;
  } 
  clrtoeol(); //Borra todo el contenido desde la ubicacion del cursor hasta el final.
  refresh();
  getch();
  endwin();
  return;
};
void showInfoSales(struct Usuario* user){
  int choice = 0;
  char *choices []= {
    "2023121014223292 14:22:32 10/12/2023 pan Bimbo 680 g      2          47.50      95.00      Marlon         ",
    NULL,
  };
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
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /* Crea los elementos */
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    //my_items[i] = new_item(choices[i], choices[i]);
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(screen_height-5 ,screen_width,0, 0);
  keypad(my_menu_win, TRUE);


  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  //set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_sub(my_menu, derwin(my_menu_win, screen_height-10, screen_width-2, 4, 1));
  set_menu_format(my_menu, screen_height-10, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, screen_width, "Informacion sobre mis ventas", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  //mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, screen_width-2);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvwaddch(my_menu_win, 2, screen_width-1, ACS_RTEE);

  /*Imprime la cabecera de los productos*/
  //print_in_middle(my_menu_win, 3, 0, screen_width/2, "Nombre              precio    cantidad  descripcion", COLOR_PAIR(3));
  mvwprintw(my_menu_win, 3, 4, "%s", "ID_venta         Hora y Fecha        Nombre Producto      Cantidad   $Producto  $Total     Personal       ");
  wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "Use Enter para agregar producto");
  mvprintw(LINES - 1, 0, "Las flechas para navegar | F1 Para Salir");
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
    case '\n':  // Añade este caso para Enter
      ITEM *selected_item;
      selected_item= current_item(my_menu);
      choice = item_index(selected_item);
      move(LINES - 4, 0);
      clrtoeol();
      move(LINES - 3, 0);
      clrtoeol();
      mvprintw(LINES - 4, 0, "Seleccionaste la opción %d: %s", choice + 1, choices[choice]);
      refresh();
      break;
    }
    wrefresh(my_menu_win);
  };

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();

};



void showPageAdd(struct Usuario *user){
  struct Producto producto;
  FIELD *field[4];
  FORM  *my_form;
  int ch;
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);

  /* Inicializar curses */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  refresh();
  /* Inicializar los campos */
  //                   altura,largo, empieza a escribir
  field[0] = new_field(1, 20, 4, 22, 0, 0);
  field[1] = new_field(1, 100, 6, 22, 0, 0);
  field[2] = new_field(1, 10, 8, 22, 0, 0);
  field[3] = new_field(1, 10, 10, 22, 0, 0);
  field[4] = NULL;
  /* Establecer las opciones de campo */
  for(int i = 0; i < 4; ++i) {
    set_field_back(field[i], A_UNDERLINE);
    field_opts_off(field[i], O_AUTOSKIP);
  };
  /* Crear el formulario y publicarlo */
  my_form = new_form(field);
  post_form(my_form);
  refresh();

  mvprintw(4, 10, "Nombre:");
  mvprintw(6, 10, "Descripcion:");
  mvprintw(8, 10, "Precio: $ ");
  mvprintw(10, 10, "Cantidad: # ");

  /******/

  char titleMenu [] ={"Agregar producto"};
  mvprintw(0,((screen_width-(sizeof(titleMenu))/sizeof(char*))/2)-((sizeof(titleMenu)/sizeof(char*))/2),titleMenu);
  attron(COLOR_PAIR(1));
  //mvprintw(LINES - 2, 0, "Use PageUp y PageDown para desplazar una pagina de elementos abajo o arriba");
  mvprintw(LINES - 2, 0, "Use  F2 para agregar el producto| F1 Para cancelar ");
  mvprintw(LINES - 1, 0, "Las flechas para navegar");
  attroff(COLOR_PAIR(1));
  move(4, 22);
  refresh();

  /* Ciclo para optener solicitudes del usuario */
  while((ch = getch()) != KEY_F(1)){
    switch(ch){
    case KEY_DOWN:
      /* Ir al siguiente campo */
      form_driver(my_form, REQ_NEXT_FIELD);
      /* Ir al final del buffer actual */
      /* Deja en el ultimo caracter    */
      form_driver(my_form, REQ_END_LINE);
      break;
    case KEY_UP:
      /* Ir al campo anterior */
      form_driver(my_form, REQ_PREV_FIELD);
      form_driver(my_form, REQ_END_LINE);
      break;
    case 8: // KEY DELETE
      form_driver(my_form, REQ_END_FIELD);
      /* Ir al final del buffer actual */
      /* Deja en el ultimo caracter    */
      form_driver(my_form, REQ_DEL_PREV);
      break;
    case 266: // KEY F2//Obtiene todos los datos
      /*Obtiene todos los datos*/
      form_driver(my_form,REQ_FIRST_FIELD);
      const char *nombre = field_buffer(field[0], 0);
      form_driver(my_form,REQ_NEXT_FIELD);
      const char *descripcion = field_buffer(field[1], 0);
      form_driver(my_form, REQ_NEXT_FIELD);
      const char *precio_str = field_buffer(field[2], 0);
      long double precio = atof(precio_str);
      form_driver(my_form, REQ_NEXT_FIELD);
      const char *existencia_str = field_buffer(field[3], 0);
      int existencia = atoi(existencia_str);
      //Validamos los campos
      if(isAllWhitespaceString(nombre)==1 | isAllWhitespaceString(descripcion)==1 || isAllWhitespaceString(precio_str)==1 || isAllWhitespaceString(existencia_str)==1){
        attron(COLOR_PAIR(2));
        mvprintw(15, 10, "Completa todos los campos");
        attroff(COLOR_PAIR(2));
        move(4, 22);
        refresh();
      }else{
        strcpy(producto.nombre,nombre);
        strcpy(producto.descripcion, descripcion);
        producto.precio = precio;
        producto.existencia = existencia;
        //Agregamos producto a la cola
        struct Producto *nodo=NULL;
        enQueue((void**)&nodo,&producto,PRODUCTO);
        c_addNewProduct(user,(void**)&nodo);
        //Limpiamos campos
        form_driver(my_form,REQ_FIRST_FIELD);
        for(int i=0;i<4;i++){
          form_driver(my_form,REQ_CLR_FIELD);
          form_driver(my_form,REQ_NEXT_FIELD);
        };
        //Imprimimos mensaje de exito
        attron(COLOR_PAIR(3));
        //mvprintw(LINES - 2, 0, "Use PageUp y PageDown para desplazar una pagina de elementos abajo o arriba");
        mvprintw(15, 10, "El producto %s fue agregado",producto.nombre);
        attroff(COLOR_PAIR(3));
        move(4, 22);
        refresh();
      }
      
      break;
    default:
      /* Si este es un caracter normal, se optiene */
      /* Imprime                                   */
      form_driver(my_form, ch);
      break;
    };
  };

  /* Remueve el formulario y libera la memoria */
  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]);

  endwin();
  return;
};

void showPageProducts(struct Usuario *user){
  int choice = 0;
  char *choices []= {
    "leche lala 1 lt      26.00      15       Leche entera de vaca, Vitamina A y Vitamina D      cocina",
    "papel rendiplus 12pz 86.00      10       Contiene 12 rollos con 300 hojas dobles            higiene",
    "refresco cocacola 2l 33.00      18       Refresco Coca Cola 2 l                             cocina",
    "jamon san rafael 1kg 320.00     10       Jamon de pavo San Rafael                           cocina",
    "aceite Nutriol 946ml 47.00      8        Aceite puro de soya Nutrioli 946 ml                cocina",
    "crema Lala 900 ml    69.00      5        Crema Lala entera 900 ml                           cocina",
    "pan Bimbo 680 g      47.50      6        Pan blanco Bimbo grande 680 g                      cocina",
    "huevo 1kg            70.00      10       Huevo blanco                                       cocina",
    "garrafon 20lt        57.00      9        Garrafon Bonafont 20lt                             cocina",
    "jabon Escudo         12         6        Jabon corporal Escudo antibacterial 110g           higiene",
    "arroz Verde Valle    39.50      13       Arroz Verde Valle super extra 1 kg                 cocina",
    "croqueta pedigree 1k 64.00      25       croqueta para perro pedigree adulto 1kg            general",
    NULL,
  };
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
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /* Crea los elementos */
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    //my_items[i] = new_item(choices[i], choices[i]);
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(screen_height-5 ,screen_width,0, 0);
  keypad(my_menu_win, TRUE);


  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  //set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_sub(my_menu, derwin(my_menu_win, screen_height-10, screen_width-2, 4, 1));
  set_menu_format(my_menu, screen_height-10, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, screen_width, "Mis productos", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  //mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, screen_width-2);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvwaddch(my_menu_win, 2, screen_width-1, ACS_RTEE);

  /*Imprime la cabecera de los productos*/
  //print_in_middle(my_menu_win, 3, 0, screen_width/2, "Nombre              precio    cantidad  descripcion", COLOR_PAIR(3));
  mvwprintw(my_menu_win, 3, 4, "%s", "Nombre               precio     cantidad descripcion                                        Categoria  \n");
  wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  //mvprintw(LINES - 2, 0, "Use Enter para agregar producto");
  mvprintw(LINES - 1, 0, "Las flechas para navegar | F1 Para Salir");
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
    case '\n':  // Añade este caso para Enter
      ITEM *selected_item;
      selected_item= current_item(my_menu);
      choice = item_index(selected_item);
      move(LINES - 4, 0);
      clrtoeol();
      move(LINES - 3, 0);
      clrtoeol();
      mvprintw(LINES - 4, 0, "Seleccionaste la opción %d: %s", choice + 1, choices[choice]);
      refresh();
      break;
    }
    wrefresh(my_menu_win);
  };

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();

}; 

void showPageTest(struct Usuario *user){
  int choice = 0;
  char *choices []= {
    "leche lala 1 lt      26.00      15       Leche entera de vaca, Vitamina A y Vitamina D      cocina",
    "papel rendiplus 12pz 86.00      10       Contiene 12 rollos con 300 hojas dobles            higiene",
    "refresco cocacola 2l 33.00      18       Refresco Coca Cola 2 l                             cocina",
    "jamon san rafael 1kg 320.00     10       Jamon de pavo San Rafael                           cocina",
    "aceite Nutriol 946ml 47.00      8        Aceite puro de soya Nutrioli 946 ml                cocina",
    "crema Lala 900 ml    69.00      5        Crema Lala entera 900 ml                           cocina",
    "pan Bimbo 680 g      47.50      6        Pan blanco Bimbo grande 680 g                      cocina",
    "huevo 1kg            70.00      10       Huevo blanco                                       cocina",
    "garrafon 20lt        57.00      9        Garrafon Bonafont 20lt                             cocina",
    "jabon Escudo         12         6        Jabon corporal Escudo antibacterial 110g           higiene",
    "arroz Verde Valle    39.50      13       Arroz Verde Valle super extra 1 kg                 cocina",
    NULL,
  };
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
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /* Crea los elementos */
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    //my_items[i] = new_item(choices[i], choices[i]);
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(screen_height-5 ,screen_width,0, 0);
  keypad(my_menu_win, TRUE);


  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  //set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_sub(my_menu, derwin(my_menu_win, screen_height-10, screen_width-2, 4, 1));
  set_menu_format(my_menu, screen_height-10, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, screen_width, "Mis productos", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  //mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, screen_width-2);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvwaddch(my_menu_win, 2, screen_width-1, ACS_RTEE);

  /*Imprime la cabecera de los productos*/
  //print_in_middle(my_menu_win, 3, 0, screen_width/2, "Nombre              precio    cantidad  descripcion", COLOR_PAIR(3));
  mvwprintw(my_menu_win, 3, 4, "%s", "Nombre               precio     cantidad descripcion                                        Categoria  \n");
  wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "Use Enter para agregar producto");
  mvprintw(LINES - 1, 0, "Las flechas para navegar | F1 Para Salir");
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
    case '\n':  // Añade este caso para Enter
      ITEM *selected_item;
      selected_item= current_item(my_menu);
      choice = item_index(selected_item);
      move(LINES - 4, 0);
      clrtoeol();
      move(LINES - 3, 0);
      clrtoeol();
      mvprintw(LINES - 4, 0, "Seleccionaste la opción %d: %s", choice + 1, choices[choice]);
      refresh();
      break;
    }
    wrefresh(my_menu_win);
  };

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
};

void showPageshoppingCart(struct Usuario *user){
  int choice = 0;
  char *choices []= {
    "Leche lala 1 lt",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6","opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6","opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6","opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6","opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6","opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1opcion1",
    "opcion2opcion2opcion2opcion2opcion2opcion2opcion2",
    "opcion3",
    "opcion4",
    "opcion5",
    "opcion6",
  };
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;

  //Agrega el total del carrito
  mvprintw(LINES - 4, 0, "Monto total: $%f", 45.322);
  refresh();

  /* Inicializa curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /* Crea los elementos */
  int screen_width,screen_height;
  getmaxyx(stdscr, screen_height, screen_width);
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    //my_items[i] = new_item(choices[i], choices[i]);
    my_items[i] = new_item(choices[i],NULL);

  /* Crea el menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Crea la ventana que sera asociada con el menu */
  my_menu_win = newwin(screen_height-5 ,screen_width,0, 0);
  keypad(my_menu_win, TRUE);


  /* Establece la ventana principal y la subventana */
  set_menu_win(my_menu, my_menu_win);
  //set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_sub(my_menu, derwin(my_menu_win, screen_height-10, screen_width-2, 4, 1));
  set_menu_format(my_menu, screen_height-10, 1);

  /* Establece la marca del menu con la cadena " * " */
  set_menu_mark(my_menu, " * ");

  /* Imprime un margen alrededor de la ventana pricipal e imprime el titulo */
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, screen_width, "Mis productos", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  //mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, screen_width-2);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvwaddch(my_menu_win, 2, screen_width-1, ACS_RTEE);

  /*Imprime la cabecera de los productos*/
  //print_in_middle(my_menu_win, 3, 0, screen_width/2, "Nombre              precio    cantidad  descripcion", COLOR_PAIR(3));
  mvwprintw(my_menu_win, 3, 4, "%s", "Nombre              precio    cantidad  descripcion                                                                                           Categoria  \n");
  wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Publica el menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  attron(COLOR_PAIR(2));
  mvprintw(LINES - 2, 0, "Use PageUp y PageDown para desplazar una pagina de elementos abajo o arriba");
  mvprintw(LINES - 1, 0, "Las flechas para navegar (F1 Para Salir)");
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
    case '\n':  // Añade este caso para Enter
      ITEM *selected_item;
      selected_item= current_item(my_menu);
      choice = item_index(selected_item);
      move(LINES - 4, 0);
      clrtoeol();
      move(LINES - 3, 0);
      clrtoeol();
      mvprintw(LINES - 4, 0, "Seleccionaste la opción %d: %s", choice + 1, choices[choice]);
      refresh();
      //print_in_middle(my_menu_win,(screen_height/2)+5,0,screen_width,"Monto total:$",COLOR_PAIR(2));
      // mvwprintw(my_menu_win, 3,screen_width/2, "%s", "Monto total: $\n");
      wattroff(my_menu_win,COLOR_PAIR(3));
      refresh();
      break;
    }
    wrefresh(my_menu_win);
  };
  //mvwprintw(my_menu_win, 3,screen_width/2, "%s", "Monto total: $\n");
  //wattroff(my_menu_win,COLOR_PAIR(3));
  refresh();

  /* Remueve el menu y libera toda la memoria tomada */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
};




void print_menu(WINDOW *menu_win, int highlight,int n_choices,char **choices){
  int x, y, i;

  x = 2;
  y = 2;
  box(menu_win, 0, 0);
  for(i = 0; i < n_choices; i++){   
    if(highlight == i + 1){ /* Resalta lo opcion actual */ 
        wattron(menu_win, A_REVERSE);
        mvwprintw(menu_win, y, x, "%s", choices[i]);
        wattroff(menu_win, A_REVERSE);
      }
      else
        mvwprintw(menu_win, y, x, "%s", choices[i]);
      y++;
    }
  wrefresh(menu_win);
  return;
};

char *choices[] = {
  "Nueva venta",
  "Editar productos",
  "Ver ventas",
  "Cerrar sesion",
  (char *)NULL,
};




/*
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

  // Inicializa curses 
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);

  // Crea los elementos 
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i],NULL);

  // Crea el menu 
  my_menu = new_menu((ITEM **)my_items);

  // Crea la ventana que sera asociada con el menu 
  my_menu_win = newwin(10, 40, 4, 4);
  keypad(my_menu_win, TRUE);

  // Establece la ventana principal y la subventana 
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
  set_menu_format(my_menu, 5, 1);

  // Establece la marca del menu con la cadena " * " 
  set_menu_mark(my_menu, " * ");

  // Imprime un margen alrededor de la ventana pricipal e imprime el titulo 
  box(my_menu_win, 0, 0);
  print_in_middle(my_menu_win, 1, 0, 40, "Mi Menu", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  // Publica el menu 
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
            wrefresh(my_menu_win);
            break;
          case 1:
            mvprintw(LINES -5,2, "Seleccionaste ver elementos");
            wrefresh(my_menu_win);

            exit(-1);
            break;
          case 2:
            mvprintw(LINES -5,2, "Seleccionaste cerrar sesion");
            wrefresh(my_menu_win);

            //sleep(3);
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

  // Remueve el menu y libera toda la memoria tomada 
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
};
*/

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

int isAllWhitespaceString(const char *str){
  while (*str) {
    if(!isspace((unsigned char)*str)){
      return 0; 
    };
    str++;
  };
  return 1;
}