#include "View.h"

void showAuthPage(struct Usuario user){

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

        if (ch == '\n' || ch == KEY_ENTER)
            break;

        if (contrasena_index < sizeof(usuario) - 1) {
            usuario[contrasena_index++] = ch;
            waddch(win, ch);
            wrefresh(win);
        }
    }
    usuario[contrasena_index] = '\0';
    contrasena_index =0;
    // Capturar la contraseña y mostrar asteriscos
    while (1) {
        ch = mvwgetch(win, 3, 15 + contrasena_index);

        if (ch == '\n' || ch == KEY_ENTER)
            break;

        if (contrasena_index < sizeof(contrasena) - 1) {
            contrasena[contrasena_index++] = ch;
            waddch(win, '*');
            wrefresh(win);
        }
    }
    contrasena[contrasena_index] = '\0';

    // Simular la autenticación
    if (strcmp(usuario, "") == 0 && strcmp(contrasena, "") == 0) {
        mvwprintw(win, 5, 2, "Inicio de sesión exitoso!");
    } else {
        mvwprintw(win, 5, 2, "Inicio de sesión fallido. Intente de nuevo.");
    }

    wrefresh(win);
    wgetch(win);

    delwin(win);
    endwin(); // Finalizar ncurses
    return;

}
