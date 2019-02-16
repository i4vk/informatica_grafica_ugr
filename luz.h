#ifndef LUZ_H
#define LUZ_H

#include "aux.h"

class Luz {
private:
  Tupla4f caf, cdf, csf, pos;
  GLenum id;
  float angulo_rotado = 0;

  void actualizar() {
    glLightfv(id, GL_AMBIENT, caf);
    glLightfv(id, GL_DIFFUSE, cdf);
    glLightfv(id, GL_SPECULAR, csf);
    glLightfv(id, GL_POSITION, pos);
  }
public:
  Luz(GLenum luz, Tupla4f ambiente, Tupla4f difusa, Tupla4f especular, Tupla4f posicion) {
    id = luz;
    caf = ambiente;
    cdf = difusa;
    csf = especular;
    pos = posicion;

    actualizar();
  }

  void activarLuz() {
    actualizar();
    glEnable(id);
  }

  void apagarLuz() {
    glDisable(id);
  }

  Tupla4f getPos() {
    return pos;
  }

  void setPos(Tupla4f posicion) {
    pos = posicion;
    actualizar();
  }

  void rotar(int x, int y, int z) {
    angulo_rotado += 10;

    glPushMatrix();
      glRotatef(angulo_rotado, x, y, z);
      apagarLuz();
      activarLuz();
    glPopMatrix();
  }
};

#endif
