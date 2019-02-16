// Iván Garzón Segura

#include "aux.h"
#include "malla.h"
#include "ply_reader.h"
#include <iostream>
#include <cmath>

using namespace std;

// *****************************************************************************
//
// Clase ObjMallaIndexada
//
// *****************************************************************************

// Visualización en modo inmediato con 'glDrawElements'

void ObjMallaIndexada::cargarMateriales() {
	string nombre = "Silver";
	Tupla4f mat_amb = {0.19225, 0.19225, 0.19225, 1.0};
  Tupla4f mat_dif = {0.50754, 0.50754, 0.50754, 1.0};
  Tupla4f mat_esp = {0.508273, 0.508273, 0.508273, 1.0};
  GLfloat shininess = 0.4;

	materiales.push_back(Material(nombre, mat_amb, mat_dif, mat_esp, shininess));

	nombre = "Esmerald";
	mat_amb = {0.0215, 0.1745, 0.0215, 1.0};
  mat_dif = {0.07568, 0.61424, 0.07568, 1.0};
  mat_esp = {0.633, 0.727811, 0.633, 1.0};
  shininess = 0.6;

	materiales.push_back(Material(nombre, mat_amb, mat_dif, mat_esp, shininess));

	nombre = "Red Rubber";
	mat_amb = {0.05, 0.0, 0.0, 1.0};
  mat_dif = {0.5, 0.4, 0.4, 1.0};
  mat_esp = {0.7, 0.04, 0.04, 1.0};
  shininess = 0.78125;

	materiales.push_back(Material(nombre, mat_amb, mat_dif, mat_esp, shininess));
}

void ObjMallaIndexada::draw_ModoInmediato(bool ajedrez) {
  // visualizar la malla usando glDrawElements,
  // completar (práctica 1)
  // ...
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	if (glIsEnabled(GL_LIGHTING)) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,0,normales_vertices.data());
		materiales[material_actual].activar();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (textura_id != 0) {
		glEnable(GL_TEXTURE_2D);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura_id);
		glTexCoordPointer(2, GL_FLOAT, 0, coord_text.data());
		glColor3f(1,1,1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

  if (!ajedrez || glIsEnabled(GL_LIGHTING)) {
		glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT,
                   triangulos.data());
  } else {
    glColor3f(1, 0, 0);
    glDrawElements(GL_TRIANGLES, triang_pares.size() * 3, GL_UNSIGNED_INT,
                   triang_pares.data());
    glColor3f(0, 1, 0);
    glDrawElements(GL_TRIANGLES, triang_pares.size() * 3, GL_UNSIGNED_INT,
                   triang_impares.data());
  }

  glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)

void ObjMallaIndexada::draw_ModoDiferido(bool ajedrez) {
	if (vbo_vertices == 0) {
    vbo_vertices = CrearVBO(
        GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data());
  }
  if (ids_vbo_tri[0] == 0) {
    ids_vbo_tri[0] =
        CrearVBO(GL_ELEMENT_ARRAY_BUFFER, triang_pares.size() * 3 * sizeof(int),
                 triang_pares.data());
    ids_vbo_tri[1] = CrearVBO(GL_ELEMENT_ARRAY_BUFFER,
                              triang_impares.size() * 3 * sizeof(int),
                              triang_impares.data());
  }
	if (glIsEnabled(GL_LIGHTING)) {
		if (vbo_normales == 0) {
			vbo_normales = CrearVBO(
	        GL_ARRAY_BUFFER, normales_vertices.size() * 3 * sizeof(float),
					 normales_vertices.data());
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo_normales);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,0,0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		materiales[material_actual].activar();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (textura_id != 0) {
		glEnable(GL_TEXTURE_2D);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura_id);
		glTexCoordPointer(2, GL_FLOAT, 0, coord_text.data());
		glColor3f(1,1,1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else {
		glDisable(GL_TEXTURE_2D);
	}

  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY);

  if (!ajedrez || glIsEnabled(GL_LIGHTING)) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids_vbo_tri[0]);
    glDrawElements(GL_TRIANGLES, triang_pares.size() * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids_vbo_tri[1]);
    glDrawElements(GL_TRIANGLES, triang_impares.size() * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  } else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids_vbo_tri[0]);
    glColor3f(1, 0, 0);
    glDrawElements(GL_TRIANGLES, triang_pares.size() * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids_vbo_tri[1]);
    glColor3f(0, 1, 0);
    glDrawElements(GL_TRIANGLES, triang_impares.size() * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}
// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido

void ObjMallaIndexada::draw(int modo, bool modo_diferido) {
	bool ajedrez = false;
	GLenum modo_actual;

	if (normales_caras.empty())
		calcular_normales();

  glPointSize(4);

	switch (modo) {
	case 0:
		modo_actual = GL_POINT;
		break;
	case 1:
		modo_actual = GL_LINE;
		break;
	case 2:
		modo_actual = GL_FILL;
		break;
	case 3:
		ajedrez = true;
		break;
	}

	glPolygonMode(GL_FRONT_AND_BACK, modo_actual);
	if (!triangulos_divididos)
		dividirTriangulosParidad();

	if (!modo_diferido)
  	draw_ModoInmediato(ajedrez);
	else
		draw_ModoDiferido(ajedrez);
}

GLuint ObjMallaIndexada::CrearVBO(GLuint tipo_vbo, GLuint tam, GLvoid *dir) {
  GLuint id_vbo;
  glGenBuffers(1, &id_vbo);
  glBindBuffer(tipo_vbo, id_vbo);

  glBufferData(tipo_vbo, tam, dir, GL_STATIC_DRAW);

  glBindBuffer(tipo_vbo, 0);
  return id_vbo;
}

void ObjMallaIndexada::dividirTriangulosParidad() {
  Tupla3i aux;
  for (int i = 0; i < triangulos.size(); i++) {
    if (i % 2 == 0) {
      for (int j = 0; j < 3; j++)
        aux[j] = triangulos[i][j];
      triang_pares.push_back(aux);
    } else {
      for (int j = 0; j < 3; j++)
        aux[j] = triangulos[i][j];
      triang_impares.push_back(aux);
    }
  }

	triangulos_divididos = true;
}
// -----------------------------------------------------------------------------
// Recalcula la tabla de normales de vértices (el contenido anterior se pierde)

void ObjMallaIndexada::calcular_normales() {
	Tupla3f inicio = {0,0,0};
	normales_caras.assign(triangulos.size(), inicio);
	normales_vertices.assign(vertices.size(), inicio);
	Tupla3i aux_tri;
	Tupla3f a, b, normal;
	for (int i = 0; i < triangulos.size(); i++) {
		aux_tri = triangulos[i];
		a = vertices[aux_tri(1)] - vertices[aux_tri(0)];
		b = vertices[aux_tri(2)] - vertices[aux_tri(0)];
		normal = a.cross(b);
		normal = normal.normalized();
		normales_caras[i] = normal;

		normales_vertices[aux_tri(0)] = normales_vertices[aux_tri(0)] + normal;
		normales_vertices[aux_tri(1)] = normales_vertices[aux_tri(1)] + normal;
		normales_vertices[aux_tri(2)] = normales_vertices[aux_tri(2)] + normal;
	}

	for (int i = 0; i < normales_vertices.size(); i++) {
		if (normales_vertices[i].lengthSq() != 0)
			normales_vertices[i] = normales_vertices[i].normalized();
	}
}

void ObjMallaIndexada::prepara_textura (char* nombre_imagen)
{
   std::vector<unsigned char> data;

   CImg<unsigned char> logo;
   logo.load(nombre_imagen);

   // empaquetamos bien los datos
   for (long y = 0; y < logo.height(); y ++)
      for (long x = 0; x < logo.width(); x ++)
      {
				 unsigned char *r = logo.data(x, y, 0, 0);
				 unsigned char *g = logo.data(x, y, 0, 1);
				 unsigned char *b = logo.data(x, y, 0, 2);
				 data.push_back(*r);
				 data.push_back(*g);
				 data.push_back(*b);
      }

   glGenTextures(1, &textura_id);
   glBindTexture(GL_TEXTURE_2D, textura_id);

	 glActiveTexture(GL_TEXTURE0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // TRASFIERE LOS DATOS A GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, logo.width(), logo.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

// *****************************************************************************
//
// Clase Cubo (práctica 1)
//
// *****************************************************************************
void Cubo::text_en_malla() {
	coord_text = {
		{0.0, 0.0}, // 0
		{0.0, 3.0}, // 1
		{0.0, 0.0}, // 2
		{0.0, 0.0}, // 3
		{0.0, 0.0}, // 4
		{3.0, 3.0}, // 5
		{0.0, 0.0}, // 6
		{3.0, 0.0}  // 7
	};
}

Cubo::Cubo() {

  // inicializar la tabla de vértices
  vertices = {
      {-0.5, -0.5, -0.5}, // 0
      {-0.5, -0.5, +0.5}, // 1
      {-0.5, +0.5, -0.5}, // 2
      {-0.5, +0.5, +0.5}, // 3
      {+0.5, -0.5, -0.5}, // 4
      {+0.5, -0.5, +0.5}, // 5
      {+0.5, +0.5, -0.5}, // 6
      {+0.5, +0.5, +0.5}  // 7
  };

  // inicializar la tabla de caras o triángulos:
  // (es importante en cada cara ordenar los vértices en sentido contrario
  //  de las agujas del reloj, cuando esa cara se observa desde el exterior del
  //  cubo)
	triangulos = {{0, 2, 4}, {4, 2, 6}, {1, 5, 3},
	{3, 5, 7}, {1, 3, 0}, {0, 3, 2},
	{5, 4, 7}, {7, 4, 6}, {1, 0, 5},
	{5, 0, 4}, {3, 7, 2}, {2, 7, 6}};

	cargarMateriales();
}

Cubo::Cubo(int) {
	vertices = {
      {-0.5, -0.5, -0.5}, // 0
      {-0.5, -0.5, +0.5}, // 1
      {-0.5, +0.5, -0.5}, // 2
      {-0.5, +0.5, +0.5}, // 3
      {+0.5, -0.5, -0.5}, // 4
      {+0.5, -0.5, +0.5}, // 5
      {+0.5, +0.5, -0.5}, // 6
      {+0.5, +0.5, +0.5}  // 7
  };

	vertices.push_back(vertices[4]);  // 8
	vertices.push_back(vertices[6]);  // 9
	vertices.push_back(vertices[2]);  // 10
	vertices.push_back(vertices[6]);  // 11
	vertices.push_back(vertices[0]);  // 12
	vertices.push_back(vertices[4]);  // 13

  triangulos = {{0, 2, 4}, {4, 2, 6}, {1, 5, 3},
								{3, 5, 7}, {1, 3, 0}, {0, 3, 2},
								{5, 8, 7}, {7, 8, 9}, {1, 12, 5},
								{5, 12, 13}, {3, 7, 10}, {10, 7, 11}};

	coord_text = {
		{0.25, 0.5}, // 0
		{0.5, 0.5}, // 1
		{0.25, 0.25}, // 2
		{0.5, 0.25}, // 3
		{0, 0.5}, // 4
		{0.75, 0.5}, // 5
		{0, 0.25}, // 6
		{0.75, 0.25}, // 7
		{1.0, 0.5},
		{1.0, 0.25},
		{0.5, 0.0},
		{0.75, 0.0},
		{0.5, 0.75},
		{0.75, 0.75}
	};

	cargarMateriales();
}

// *****************************************************************************
//
// Clase Tetraedro (práctica 1)
//
// *****************************************************************************

// completar .....
Tetraedro::Tetraedro() {
  vertices = {
      {0, 0, 0}, // 0
      {1, 0, 0}, // 1
      {0, 1, 0}, // 2
      {0, 0, 1}  // 3
  };

  triangulos = {{0, 2, 1}, {1, 2, 3}, {0, 3, 2}, {0, 1, 3}};
	cargarMateriales();
}

Cuadro::Cuadro() {
	vertices = {
		{-2.5, 0, 0}, //0
		{2.5, 0, 0},  //1
		{-2.5, 5, 0}, //2
		{2.5, 5, 0}   //3
	};

	triangulos = {{0, 1, 2}, {2, 1, 3}};

	coord_text = {
		{0.0, 1.0},
		{1.0, 1.0},
		{0.0, 0.0},
		{1.0, 0.0}
	};
	cargarMateriales();
}

Piramide::Piramide() {
	vertices = {
		{-0.5, 0, 0.5}, //0
		{0.5, 0, 0.5},  //1
		{0.5, 0, -0.5},  //2
		{-0.5, 0, -0.5}, //3
		{0, 1, 0} //4
	};

	triangulos = {
		{0, 3, 1}, {1, 3, 2},
		{4, 0, 1},
		{4, 1, 2},
		{4, 2, 3},
		{4, 3, 0}
	};

	cargarMateriales();
}

CincoPiramides::CincoPiramides() {
	piramide = new Piramide();
	for (int i = 0; i < 5; i++) {
		selecciones[i] = 0;
	}
}

void CincoPiramides::draw(const int modo_vis, const bool p_usar_diferido) {
	num_modo = modo_vis;
	usar_diferido = p_usar_diferido;
	glLineWidth(2);

	glPushMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, -0.5);
			glPushMatrix();
				if (selecciones[0])
					glColor3ub(255, 255, 0);
				else
					glColor3ub(211, 84, 0);
				piramide->draw(num_modo, usar_diferido);
				if (num_modo == 2) {
					glColor3ub(0, 0, 0);
					piramide->draw(1, usar_diferido);
				}
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, 0.5);
			glPushMatrix();
				if (selecciones[1])
					glColor3ub(255, 255, 0);
				else
					glColor3ub(211, 84, 0);
				piramide->draw(num_modo, usar_diferido);
				if (num_modo == 2) {
					glColor3ub(0, 0, 0);
					piramide->draw(1, usar_diferido);
				}
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5, 0, -0.5);
			glPushMatrix();
				if (selecciones[2])
					glColor3ub(255, 255, 0);
				else
					glColor3ub(211, 84, 0);
				piramide->draw(num_modo, usar_diferido);
				if (num_modo == 2) {
					glColor3ub(0, 0, 0);
					piramide->draw(1, usar_diferido);
				}
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5, 0, 0.5);
			glPushMatrix();
				if (selecciones[3])
					glColor3ub(255, 255, 0);
				else
					glColor3ub(211, 84, 0);
				piramide->draw(num_modo, usar_diferido);
				if (num_modo == 2) {
					glColor3ub(0, 0, 0);
					piramide->draw(1, usar_diferido);
				}
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0, 1, 0);
			glPushMatrix();
				if (selecciones[4])
					glColor3ub(255, 255, 0);
				else
					glColor3ub(211, 84, 0);
				piramide->draw(num_modo, usar_diferido);
				if (num_modo == 2) {
					glColor3ub(0, 0, 0);
					piramide->draw(1, usar_diferido);
				}
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void CincoPiramides::drawSeleccion(const int modo_vis, const bool p_usar_diferido) {
	num_modo = modo_vis;
	usar_diferido = p_usar_diferido;

	glDisable(GL_DITHER);

	glLineWidth(2);

	glPushMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, -0.5);
			glColor3ub(0, 0, 0);
			piramide->draw(num_modo, usar_diferido);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, 0.5);
			glColor3ub(25, 25, 25);
			piramide->draw(num_modo, usar_diferido);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5, 0, -0.5);
			glColor3ub(50, 50, 50);
			piramide->draw(num_modo, usar_diferido);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5, 0, 0.5);
			glColor3ub(75, 75, 75);
			piramide->draw(num_modo, usar_diferido);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0, 1, 0);
			glColor3ub(100, 100, 100);
			piramide->draw(num_modo, usar_diferido);
		glPopMatrix();
	glPopMatrix();

	glEnable(GL_DITHER);
}

void CincoPiramides::setSeleccion(int num_piramide) {
	selecciones[num_piramide] = !selecciones[num_piramide];
}

// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************

ObjPLY::ObjPLY(const std::string &nombre_archivo) {
  // leer la lista de caras y vértices
  ply::read(nombre_archivo, vertices, triangulos);
	cargarMateriales();
}

// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion(const std::string &nombre_ply_perfil) {
		std::vector<Tupla3f> perfil;
		ply::read_vertices(nombre_ply_perfil, perfil);
		crearMalla(perfil, 64);
		cargarMateriales();
}

void ObjRevolucion::crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf) {
	int M = perfil_original.size();
	int N = num_instancias_perf;

	Tupla3f v;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			v = perfil_original[j];
			v(0) = (perfil_original[j](0)*cos(2*M_PI*i/N)) + (perfil_original[j](2)*sin(2*M_PI*i/N));
			v(2) = (perfil_original[j](0)*(-sin(2*M_PI*i/N))) + (perfil_original[j](2)*cos(2*M_PI*i/N));

			vertices.push_back(v);
		}
	}

	Tupla3i aux;
	int a, b;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M-1; j++) {
			a = M*i + j;
			b = M*((i+1)%N) + j;
			aux = {a,b,b+1};
			triangulos.push_back(aux);
			aux = {a,b+1,a+1};
			triangulos.push_back(aux);
		}
	}

	if ((vertices[0](0) != 0) || (vertices[0](2) != 0)) {
		Tupla3f polo_sur;
		polo_sur(0) = 0;
		polo_sur(1) = vertices[0](1);
		polo_sur(2) = 0;
		vertices.push_back(polo_sur);

		for (int i = 0; i < N; i++) {
			aux(1) = N*M;
			aux(0) = M*i;
			aux(2) = M*((i+1)%N);
			triangulos.push_back(aux);
		}
	}

	if ((vertices[M-1](0) != 0) || (vertices[M-1](2) != 0)) {
		Tupla3f polo_norte;
		polo_norte(0) = 0;
		polo_norte(1) = vertices[M-1](1);
		polo_norte(2) = 0;
		vertices.push_back(polo_norte);

		for (int i = 0; i < N; i++) {
			aux(1) = N*M + 1;
			aux(0) = (M*(i+2) - 1) % (M*N);
			aux(2) = M*(i+1) - 1;
			triangulos.push_back(aux);
		}
	}
}


Cilindro::Cilindro( const int num_vert_perfil, const int num_instancias_perf ) {
	vector<Tupla3f> perfil;

	float tramo = h/(num_vert_perfil-1);

	for (int i = 0; i < num_vert_perfil; i++) {
		perfil.push_back({r, tramo*i, 0});
	}

	crearMalla(perfil, num_instancias_perf);
	cargarMateriales();
}

void Cono::crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf) {
	int M = perfil_original.size()-1;
	int N = num_instancias_perf;

	Tupla3f v;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			v = perfil_original[j];
			v(0) = (perfil_original[j](0)*cos(2*M_PI*i/N)) + (perfil_original[j](2)*sin(2*M_PI*i/N));
			v(2) = (perfil_original[j](0)*(-sin(2*M_PI*i/N))) + (perfil_original[j](2)*cos(2*M_PI*i/N));

			vertices.push_back(v);
		}
	}

	Tupla3i aux;
	int a, b;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M-1; j++) {
			a = M*i + j;
			b = M*((i+1)%N) + j;
			aux = {a,b,b+1};
			triangulos.push_back(aux);
			aux = {a,b+1,a+1};
			triangulos.push_back(aux);
		}
	}

	Tupla3f polo_sur;
	polo_sur(0) = 0;
	polo_sur(1) = vertices[0](1);
	polo_sur(2) = 0;
	vertices.push_back(polo_sur);

	for (int i = 0; i < N; i++) {
		aux(1) = N*M;
		aux(0) = M*i;
		aux(2) = M*((i+1)%N);
		triangulos.push_back(aux);
	}

	Tupla3f polo_norte = perfil_original[M];
	vertices.push_back(polo_norte);

	for (int i = 0; i < N; i++) {
		aux(1) = N*M + 1;
		aux(0) = (M*(i+2) - 1) % (M*N);
		aux(2) = M*(i+1) - 1;
		triangulos.push_back(aux);
	}
}

Cono::Cono(const int num_vert_perfil, const int num_instancias_perf) {
	vector<Tupla3f> perfil;
	float x = r;
	float y = 0;

	float tramo_eje_x = h/(num_vert_perfil-1);
	float tramo_eje_y = h/(num_vert_perfil-1);

	for (int i = 0; i < num_vert_perfil; i++) {
		perfil.push_back({x,y,0});
		x -= tramo_eje_x;
		y += tramo_eje_y;
	}

	crearMalla(perfil, num_instancias_perf);
	cargarMateriales();
}

void Esfera::crearPerfil(std::vector<Tupla3f> &perfil, const int num_vert_perfil) {
	// Cambiar a metodo de angel
	float tramo = M_PI/(num_vert_perfil-1);
	float angulo = M_PI;
	Tupla3f v = {0,-r,0};

	for (int i = 0; i < num_vert_perfil; i++) {
		perfil.push_back(v);
		angulo -= tramo;
		v(0) = r*sin(angulo);
		v(1) = r*cos(angulo);
		v(2) = 0;
	}

	cout << perfil[perfil.size() - 1] << endl;

	// for (int i = 0; i <= num_vert_perfil; i++) {
	// 	v(0) = r*sin(M_PI*i/(num_vert_perfil-1));
	// 	v(1) = r*cos(M_PI*i/(num_vert_perfil-1));
	// 	v(2) = 0;
	// 	aux.push_back(v);
	// }

	// for (int i = aux.size()-1; i >= 0; i--) {
	// 	perfil.push_back(aux[i]);
	// }
}

void Esfera::crearMalla(const std::vector<Tupla3f> &perfil_original, const int num_instancias_perf) {
	int M = perfil_original.size()-2;
	int N = num_instancias_perf;

	Tupla3f v;

	for (int i = 0; i < N; i++) {
		for (int j = 1; j < M+1; j++) {
			v = perfil_original[j];
			v(0) = (perfil_original[j](0)*cos(2*M_PI*i/N)) + (perfil_original[j](2)*sin(2*M_PI*i/N));
			v(2) = (perfil_original[j](0)*(-sin(2*M_PI*i/N))) + (perfil_original[j](2)*cos(2*M_PI*i/N));

			vertices.push_back(v);
		}
	}

	Tupla3i aux;
	int a, b;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M-1; j++) {
			a = M*i + j;
			b = M*((i+1)%N) + j;
			aux = {a,b,b+1};
			triangulos.push_back(aux);
			aux = {a,b+1,a+1};
			triangulos.push_back(aux);
		}
	}

	Tupla3f polo_sur = perfil_original[0];
	vertices.push_back(polo_sur);

	for (int i = 0; i < N; i++) {
		aux(1) = N*M;
		aux(0) = M*i;
		aux(2) = M*((i+1)%N);
		triangulos.push_back(aux);
	}

	Tupla3f polo_norte = perfil_original[M+1];
	vertices.push_back(polo_norte);

	for (int i = 0; i < N; i++) {
		aux(1) = N*M + 1;
		aux(0) = (M*(i+2) - 1) % (M*N);
		aux(2) = M*(i+1) - 1;
		triangulos.push_back(aux);
	}
}

Esfera::Esfera(const int num_vert_perfil, const int num_instancias_perf) {
	vector<Tupla3f> perfil;
	crearPerfil(perfil, num_vert_perfil);
	crearMalla(perfil, num_instancias_perf);
	cargarMateriales();
}
