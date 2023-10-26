#ifndef MODEL_H
#define MODEL_H

struct Usuario {
  int ID_usuario;
  char nombre [15];
  char usuario [25];
  char password [120];
};

struct Categoria{
  int ID_categoria;
  char descripcion [25];
};


struct Producto {
  int ID_producto;
  char nombre [20];
  int cantidad;
  char descripcion [100]; 
  long double precio;
  int ID_categoria;
};

struct Venta {
  int ID_ventas;
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  char metodo [12];
  long double isr;
  long double monto_total;
  int ID_usuario;
};

struct DetalleVenta {
  int ID_venta;
  int ID_producto;
  long double precio_unitario;
  int descuento;//por definir
  int cantidad;
};

struct Proveedor{
 int ID_proveedor;
 char descripcion [150];
};

struct adquisicion{
  int ID_provedor;
  int ID_producto;
  int ID_usuario;
  char fecha [19]; //HH:mm:ss-XX/XX/XXXX
  int cantidad;
};






#endif