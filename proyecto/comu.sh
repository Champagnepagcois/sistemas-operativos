#!/bin/bash
#chmod +x compilar.sh //Para poder utilizarlo en terminal
clear
echo -e "Compilando el programa....\n\n"
nombre="main"

gcc "$nombre.c" -o "$nombre" controller.c model.c view.c -lncurses -lmenu -lform

if [ $? -eq 0 ]; then
  echo  "programa compilado!"
  #./$nombre
else
  echo -e "\n\nError en la compilacion!"
fi
