#ifndef CAMARA_H
#define CAMARA_H

#include "aux.h"
#include <iostream>

using namespace std;

class Camara {
private:
  Tupla3f eye;
  Tupla3f at;
  Tupla3f up;
  bool tipo_perspectiva;      // 0=Ortogonal     1=Perspectiva
  float angulo_observador_x;
  float angulo_observador_y;
  float distancia_observador;
  float distancia_inicial;
  float left, right, top, bottom, near, far;
  float ratio_xy;
  float zoom;

public:
  Camara(Tupla3f _eye, Tupla3f _at, Tupla3f _up, bool tipo, float rat_xy, float Front_plane,
        float Back_plane, float rotacion_x_inicial = 0, float rotacion_y_inicial = 0) {
      eye = _eye;
      at = _at;
      up = _up;

      distancia_observador = eye(2);
      distancia_inicial = distancia_observador;

      zoom = 1;

      ratio_xy = rat_xy;

      tipo_perspectiva = tipo;
      float wy = 0.84*Front_plane;
      float wx = ratio_xy*wy ;

      left = -wx;
      right = wx;
      top = wy;
      bottom = -wy;
      near = Front_plane;
      far = Back_plane;

      angulo_observador_x = rotacion_x_inicial;
      angulo_observador_y = rotacion_y_inicial;
  }

  float getDistancia() {
    return distancia_observador;
  }
  float getDistanciaInicial() {
    return distancia_inicial;
  }

  float getAnguloX() {
    return angulo_observador_x;
  }

  float getAnguloY() {
    return angulo_observador_y;
  }

  void redimensionar(const float rat_xy) {
    if (rat_xy != ratio_xy) {
      ratio_xy = rat_xy;
      const float wy = 0.84*near,
      wx = ratio_xy*wy ;

      left = -wx;
      right = wx;
      top = wy;
      bottom = -wy;
    }
  }

  void setProyeccion() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (tipo_perspectiva == true) {
      glFrustum(left/zoom, right/zoom, bottom/zoom, top/zoom, near, far);
    }else {
      glOrtho(left*20/zoom, right*20/zoom, bottom*20/zoom, top*20/zoom, near, far);
    }
  }

  void setObservador() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye(0), eye(1), eye(2),
              at(0), at(1), at(2),
              up(0), up(1), up(2));
  }

  void rotar(int x, int y) {
    angulo_observador_x += x;
    angulo_observador_y += y;
  }

  void zoomIn() {
    zoom *= 1.2;
    setProyeccion();
  }

  void zoomOut() {
    if (zoom > 1) {
      zoom /= 1.2;
      setProyeccion();
    }
  }

  void aumentarDistancia(float multiplicador) {
    distancia_observador *= multiplicador;
  }

  void disminuirDistancia(float multiplicador) {
    distancia_observador /= multiplicador;
  }

  void girar(int x, int y) {
    at(0) += x/500;
    at(1) += y/500;
  }

  void activarCamara() {
    setObservador();
    setProyeccion();
  }
};


#endif
