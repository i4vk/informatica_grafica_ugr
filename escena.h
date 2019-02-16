// Iván Garzón Segura

#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "jerarquico.h"
#include "malla.h"
#include "luz.h"
#include "camara.h"

class Escena {

private:
  Ejes ejes;

  // variables que definen la posicion de la camara en coordenadas polares
  GLfloat Observer_distance;
  GLfloat Observer_angle_x;
  GLfloat Observer_angle_y;

  // variables que controlan la ventana y la transformacion de perspectiva
  GLfloat Width, Height, Front_plane, Back_plane;

  int estadoRaton;

  std::vector<Camara*> camaras;
  int camaraActiva = 0;

  int xant = 0;
  int yant = 0;
  int xantR = 0;
  int yantR = 0;

  void clear_window();
  void dibujar_objeto_actual();

  // Transformación de cámara
  void change_projection(const float ratio_xy);
  void change_observer();
  void conmutarAnumaciones();

  bool animaciones_activas = false;

  int objeto_actual = 0, // objeto actual (el que se visualiza)
      num_objetos = 0; // número de objetos (actualizado al crear los objetos en
                       // el constructor)

  int num_modo = 0;
  bool modo_ajedrez = false;
  bool modo_diferido = false;

  std::vector<Luz*> luces;
  bool luces_activas = false;
  bool modo_smooth = false;

  // Objetos de la escena
  Cubo *cubo = nullptr; // es importante inicializarlo a 'nullptr'
  Cubo *cubo_text = nullptr;
  Tetraedro *tetraedro = nullptr;
  CincoPiramides *piramides = nullptr;
  ObjPLY *ply = nullptr;
  ObjRevolucion *rev = nullptr;
  Cilindro *cilindro = nullptr;
  Cono *cono = nullptr;
  Esfera *esfera = nullptr;
  ObjJerarquico *obj_jer = nullptr;
  Cuadro *cuadro = nullptr;

  // completar: añadir punteros a tetraedro u otros (práctica 1),
  //            y a un objeto PLY y de revolución (práctica 2),
  //           y a un objeto jerarquico (práctica 3).
  // ......

public:
  Escena();
  void inicializar(int UI_window_width, int UI_window_height);
  void redimensionar(int newWidth, int newHeight);

  // Dibujar
  void dibujar();

  // Interacción con la escena
  bool teclaPulsada(unsigned char Tecla1, int x, int y);
  void teclaEspecial(int Tecla1, int x, int y);
  void clickRaton(int boton, int estado, int x, int y);
  void ratonMovido(int x, int y);

  void mgeDesocupado();
  void conmutarAnimaciones();
};
#endif
