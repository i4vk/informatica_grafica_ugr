// Iván Garzón Segura

// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: ObjMallaIndexada.h
// -- declaraciones de clase ObjMallaIndexada (mallas indexadas) y derivados
//
// #############################################################################

#ifndef OBJETOS_H_INCLUDED
#define OBJETOS_H_INCLUDED

#include "aux.h"
#include "CImg.h"
#include <string>
using namespace cimg_library;

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

struct Material {
  std::string nombre;
  Tupla4f ambiente;
  Tupla4f difusa;
  Tupla4f especular;
  GLfloat brillo;

  Material() {};
  Material(std::string nom, Tupla4f amb, Tupla4f dif, Tupla4f esp, GLfloat shi) {
    ambiente = amb;
    difusa = dif;
    especular = esp;
    brillo = shi;
    nombre = nom;
  }

  void activar() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
    //std::cout << "Cargado material: " << nombre << std::endl;
  }
};


class ObjMallaIndexada {
public:
  // dibuja el objeto en modo inmediato
  void draw_ModoInmediato(bool ajedrez);

  // dibuja el objeto en modo diferido (usando VBOs)
  void draw_ModoDiferido(bool ajedrez);

  // función que redibuja el objeto
  // está función llama a 'draw_MI' (modo inmediato)
  // o bien a 'draw_MD' (modo diferido, VBOs)
  void draw(int modo, bool modo_diferido);
  GLuint CrearVBO(GLuint tipo_vbo, GLuint tam, GLvoid *dir);
  void dividirTriangulosParidad();
  void cargarMateriales();
  void prepara_textura(char* nombre_imagen);

protected:
  void
  calcular_normales(); // calcula tabla de normales de vértices (práctica 3)

  std::vector<Tupla3f> vertices; // tabla de coordenadas de vértices (una tupla
                                 // por vértice, con tres floats)
  std::vector<Tupla3i>
      triangulos; // una terna de 3 enteros por cada cara o triángulo

  std::vector<Tupla3i> triang_pares;
  std::vector<Tupla3i> triang_impares;
  bool triangulos_divididos = false;

  GLuint vbo_vertices = 0;
  GLuint *ids_vbo_tri = new GLuint[2] {0, 0};
  GLuint vbo_normales = 0;
  GLuint vbo_textura = 0;

  std::vector<Tupla3f> normales_caras;
  std::vector<Tupla3f> normales_vertices;

  std::vector<Material> materiales;
  int material_actual = 0;
  std::vector<Tupla2f> coord_text;
  GLuint textura_id = 0;
  // completar: tabla de colores, tabla de normales de vértices
};

// *****************************************************************************
//
// clases derivadas de ObjMallaIndexada (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad
// (tiene 9 vertices y 6 caras)

class Cubo : public ObjMallaIndexada {
private:
  void text_en_malla();
public:
  Cubo();
  Cubo(int);
};

class Tetraedro : public ObjMallaIndexada {
public:
  Tetraedro();
};

class Cuadro : public ObjMallaIndexada {
public:
  Cuadro();
};

class Piramide : public ObjMallaIndexada {
public:
  Piramide();
};

class CincoPiramides {
private:
  Piramide* piramide = nullptr;
  int num_modo;
  bool usar_diferido;
  bool selecciones[5];
public:
  CincoPiramides();
  void draw(const int modo_vis, const bool p_usar_diferido);
  void drawSeleccion(const int modo_vis, const bool p_usar_diferido);
  void setSeleccion(int num_piramide);
};

// *****************************************************************************
// objeto leído de un archivo PLY

class ObjPLY : public ObjMallaIndexada {
public:
  ObjPLY(const std::string &nombre_archivo);
};

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

class ObjRevolucion : public ObjMallaIndexada {
public:
  ObjRevolucion(const std::string &nombre_ply_perfil);
  ObjRevolucion() {};
protected:
  void crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf);
};


class Cilindro : public ObjRevolucion {
private:
  float r = 1;
  float h = 1;
public:
  Cilindro( const int num_vert_perfil, const int num_instancias_perf );
} ;


class Cono : public ObjRevolucion {
private:
  float r = 1;
  float h = 1;

  void crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf);
public:
  Cono( const int num_vert_perfil, const int num_instancias_perf );
} ;


class Esfera : public ObjRevolucion {
private:
  float r = 1;
  void crearPerfil(std::vector<Tupla3f> &perfil, const int num_vert_perfil);
  void crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf);
public:
  Esfera( const int num_vert_perfil, const int num_instancias_perf );
} ;

#endif
