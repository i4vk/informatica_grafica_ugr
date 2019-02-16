// Iván Garzón Segura

#include "aux.h" // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....
#include <iostream>

using namespace std;

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena() {
  Front_plane = 0.1;
  Back_plane = 2000.0;
  Observer_distance = 2.0;
  Observer_angle_x = 0.0;
  Observer_angle_y = 0.0;

  estadoRaton = 0;

  ejes.changeAxisSize(5000);

  // crear los objetos de las prácticas: Mallas o Jerárquicos....
  cubo = new Cubo();
  cubo_text = new Cubo(1);
  tetraedro = new Tetraedro();
  piramides = new CincoPiramides();
  ply = new ObjPLY("./plys/beethoven.ply");
  rev = new ObjRevolucion("./plys/peon.ply");
  cilindro = new Cilindro(3, 32);
  cono = new Cono(6, 32);
  esfera = new Esfera(32, 32);
  obj_jer = new ObjJerarquico();
  cuadro = new Cuadro();

  num_objetos = 11; // se usa al pulsar la tecla 'O' (rotar objeto actual)
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana,
// por tanto sí puede ejecutar ordenes de OpenGL) Principalmemnte, inicializa
// OpenGL y la transf. de vista y proyección
//**************************************************************************

vector<float> calcularAngulos(Tupla3f origen, Tupla3f nueva) {
  Tupla3f aux1 = {nueva(0), 0, nueva(2)};
  Tupla3f aux2 = {0, nueva(1), nueva(2)};

  float cos_x = (origen(0)*aux1(0) + origen(1)*aux1(1))
            /(sqrt(pow(origen(0),2) + pow(origen(1),2))
              + sqrt(pow(aux1(0),2) + pow(aux1(1),2)));

  float cos_y = (origen(0)*aux2(0) + origen(1)*aux1(1))
            /(sqrt(pow(origen(0),2) + pow(origen(1),2))
              + sqrt(pow(aux2(0),2) + pow(aux2(1),2)));

  float x = acos(cos_x)*180/M_PI;
  float y = acos(cos_y)*180/M_PI;
  cout << cos_x << " " << cos_y << endl;
  cout << x << " " << y << endl;

  vector<float> res;
  res.push_back(x);
  res.push_back(y);
  cout << "llego" << endl;
}

void Escena::inicializar(int UI_window_width, int UI_window_height) {
  glClearColor(1.0, 1.0, 1.0, 1.0); // se indica cual sera el color para limpiar
                                    // la ventana	(r,v,a,al)

  glEnable(GL_DEPTH_TEST); // se habilita el z-bufer
  Tupla4f posicion = {10,0,0,0};
  Tupla4f ambiente = {0.8, 0.8, 0.8, 1.0};
  Tupla4f diff = {1.0, 1.0, 1.0, 1.0};
  Tupla4f spec = {1.0, 1.0, 1.0, 1.0};
  luces.push_back(new Luz(GL_LIGHT1, ambiente, diff, spec, posicion));

  posicion = {-10,0,10,1};
  ambiente = {0.8, 0.0, 0.8, 1.0};
  diff = {1.0, 0.0, 1.0, 1.0};
  spec = {1.0, 0.0, 1.0, 1.0};
  luces.push_back(new Luz(GL_LIGHT2, ambiente, diff, spec, posicion));

  cuadro->prepara_textura("./text/willyrex.jpg");
  cubo_text->prepara_textura("./text/skybox_edit.jpg");

  float width, height;
  width = glutGet(GLUT_WINDOW_WIDTH);
  height = glutGet(GLUT_WINDOW_HEIGHT);

  Tupla3f eye = {0.0, 0.0, 2.5};
  Tupla3f at = {0.0, 0.0, 0.0};
  Tupla3f up = {0.0, 1.0, 0.0};
  camaras.push_back(new Camara(eye, at, up, true, float(width)/float(height), 0.1, 2000.0));
  eye = {0.0, 0.0, 3.0};
  camaras.push_back(new Camara(eye, at, up, false, float(width)/float(height), 0.1, 2000.0));

  redimensionar( UI_window_width, UI_window_height );
}

// **************************************************************************
// Funcion que dibuja el objeto activo actual, usando su método 'draw'
// (llamada desde Escena::dibujar)
// ***************************************************************************

void Escena::dibujar_objeto_actual() {
  using namespace std;

  // (1) configurar OpenGL para el modo actual (puntos/lineas/sólido)
  //    llamar glPolygonMode, glColor... (y alguna cosas más), según dicho modo
  // .........completar (práctica 1)

  glColor3f(1, 0, 0);
  glLineWidth(1.5);

  // (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
  // valor entero en 'objeto_actual'
  if (luces_activas && modo_smooth) {
    glShadeModel(GL_SMOOTH);
  } else {
    glShadeModel(GL_FLAT);
  }

  switch (objeto_actual) {
  case 0:
    if (cubo != nullptr)
      cubo->draw(num_modo, modo_diferido);
    break;
  case 1:
    if (cubo != nullptr)
      cubo_text->draw(num_modo, modo_diferido);
    break;
  case 2:
    if (tetraedro != nullptr)
      tetraedro->draw(num_modo, modo_diferido);
    break;
  case 3:
    if (piramides != nullptr)
      piramides->draw(2, modo_diferido);
    break;
  case 4:
    if (ply != nullptr)
      ply->draw(num_modo, modo_diferido);
    break;
  case 5:
    if (rev != nullptr)
      rev->draw(num_modo, modo_diferido);
    break;
  case 6:
    if (cilindro != nullptr)
      cilindro->draw(num_modo, modo_diferido);
    break;
  case 7:
    if (cono != nullptr)
      cono->draw(num_modo, modo_diferido);
    break;
  case 8:
    if (esfera != nullptr)
      esfera->draw(num_modo, modo_diferido);
    break;
  case 9:
    if (obj_jer != nullptr)
      obj_jer->draw(num_modo, modo_diferido);
    break;
  case 10:
    if (cuadro != nullptr)
      cuadro->draw(num_modo, modo_diferido);
    break;
  default:
    cout << "draw_object: el número de objeto actual (" << objeto_actual
         << ") es incorrecto." << endl;
    break;
  }
}

// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
  change_observer();
  ejes.draw();
  glEnable(GL_NORMALIZE);
  dibujar_objeto_actual();
}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

bool Escena::teclaPulsada(unsigned char tecla, int x, int y) {
  using namespace std;
  cout << "Tecla pulsada: '" << tecla << "'" << endl;

  if (objeto_actual == 9){
    switch (tecla) {
      case 'p':
      case 'P':
        obj_jer->siguienteParametro();
        break;
      case 'a':
      case 'A':
        conmutarAnimaciones();
        break;
      case 'Z':
        // hacer if para que solo funcione con las animaciones desactivadas
        obj_jer->incrementaParamAct();
        break;
      case 'z':
        // hacer if para que solo funcione con las animaciones desactivadas
        obj_jer->decrementaParamAct();
        break;
      case '>':
        obj_jer->acelerar();
        break;
      case '<':
        obj_jer->decelerar();
        break;
    }
  }

  switch (toupper(tecla)) {
  case 'Q':
    // salir
    return true;
    break;
  case 'O':
    // activar siguiente objeto
    objeto_actual = (objeto_actual + 1) % num_objetos;
    cout << "objeto actual == " << objeto_actual << endl;
    break;
  case 'M':
    // cambiar modo de vista
    if (luces_activas) {
      modo_smooth = !modo_smooth;
    } else {
      num_modo = (num_modo + 1) % 4;
    }
    break;
  case 'V':
    modo_diferido = !modo_diferido;
    if (modo_diferido)
      cout << "Actualmente está en modo diferido" << endl;
    else
      cout << "Actualmente está en modo inmediato" << endl;
    break;
  case 'I':
    luces_activas = !luces_activas;
    if (luces_activas) {
      glEnable(GL_LIGHTING);
      for (int i = 0; i < luces.size(); i++) {
        luces[i]->activarLuz();
      }
    } else {
      glDisable(GL_LIGHTING);
    }
    break;
  case 'U':
    if (luces_activas) {
      luces[1]->rotar(0,1,0);
    }
    break;
  }

  return false;
}
//**************************************************************************

void Escena::teclaEspecial(int Tecla1, int x, int y) {
  switch (Tecla1) {
  case GLUT_KEY_LEFT:
    camaras[camaraActiva]->rotar(0,-1);
    break;
  case GLUT_KEY_RIGHT:
    camaras[camaraActiva]->rotar(0,1);
    break;
  case GLUT_KEY_UP:
    camaras[camaraActiva]->rotar(-1,0);
    break;
  case GLUT_KEY_DOWN:
    camaras[camaraActiva]->rotar(1,0);
    break;
  case GLUT_KEY_PAGE_UP:
    camaras[camaraActiva]->aumentarDistancia(1.2);
    break;
  case GLUT_KEY_PAGE_DOWN:
    camaras[camaraActiva]->disminuirDistancia(1.2);
    break;
  case GLUT_KEY_F1:
    camaraActiva = 0;
    camaras[camaraActiva]->activarCamara();
    break;
  case GLUT_KEY_F2:
    camaraActiva = 1;
    camaras[camaraActiva]->activarCamara();
    break;
  }
}

void Escena::clickRaton(int boton, int estado, int x, int y) {
  if (boton == GLUT_LEFT_BUTTON && objeto_actual == 3) {
    if (estado == GLUT_DOWN) {
      int invert_y = Height - y;
      unsigned int pixel;
      glClear(GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
      change_observer();
      piramides->drawSeleccion(2, modo_diferido);
      glReadPixels(x, invert_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

      unsigned int azul = (pixel & 0x00FF0000) >> 16;
      unsigned int verde = (pixel & 0x0000FF00) >> 8;
      unsigned int rojo = (pixel & 0x000000FF);

      switch(rojo) {
        case 0:
          piramides->setSeleccion(0);
          break;
        case 25:
          piramides->setSeleccion(1);
          break;
        case 50:
          piramides->setSeleccion(2);
          break;
        case 75:
          piramides->setSeleccion(3);
          break;
        case 100:
          piramides->setSeleccion(4);
          break;
      }
    }
  }

  if (boton == GLUT_RIGHT_BUTTON) {
    if (estado == GLUT_DOWN) {
      estadoRaton = 1;
      xant = y;
      yant = x;
    }else {
      estadoRaton = 0;
    }

  }else if (boton == 3) {
    camaras[camaraActiva]->zoomIn();
  }else if (boton == 4) {
    camaras[camaraActiva]->zoomOut();
  }

  glutPostRedisplay();
}

void Escena::ratonMovido(int y, int x) {
  if (estadoRaton == 1) {
    camaras[camaraActiva]->rotar(x-xant, y-yant);
    xant = x;
    yant = y;
  }

  glutPostRedisplay();
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection(const float ratio_xy) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  for (int i = 0; i < camaras.size(); i++) {
    camaras[i]->redimensionar(ratio_xy);
  }
  camaras[camaraActiva]->setProyeccion();
  // const float wy = 0.84*Front_plane,
  // wx = ratio_xy*wy ;
  // glFrustum( -wx, +wx, -wy, +wy, Front_plane, Back_plane );
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar(int newWidth, int newHeight) {
  Width = newWidth;
  Height = newHeight;
  change_projection( float(Width)/float(Height) );
  glViewport( 0, 0, Width, Height );
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  camaras[camaraActiva]->setObservador();

  float distancia_inicial = camaras[camaraActiva]->getDistanciaInicial();
  /*
    **IDEA** Hacer el translate multiplicando el vector que va desde el origen
    de coordenadas hasta la posicion de la cámara.

    **IDEA** El translate hacerlo en la clase cámara modificando el eye. Con
    la misma idea de arriba.
  */
  glTranslatef(0.0, 0.0, -camaras[camaraActiva]->getDistancia()+distancia_inicial);
  glRotatef(camaras[camaraActiva]->getAnguloX(), 1, 0, 0);
  glRotatef(camaras[camaraActiva]->getAnguloY(), 0, 1, 0);
}

void Escena::mgeDesocupado() {
  obj_jer->actualizarEstado();
  glutPostRedisplay();
}

void Escena::conmutarAnimaciones() {
  if (objeto_actual == 9) {
    animaciones_activas = !animaciones_activas;
  }else {
    std::cout << "Este objeto no tiene animaciones" << std::endl;
  }

  if (animaciones_activas) {
    obj_jer->inicioAnimaciones();
    glutIdleFunc(funcion_desocupado);
  }else {
    glutIdleFunc(nullptr);
  }
}
