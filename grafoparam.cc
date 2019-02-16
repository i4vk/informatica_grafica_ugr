// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.cc
// -- implementación del objeto jerárquico de la práctica 3
//
// #############################################################################


#include "grafoparam.h"

// -----------------------------------------------------------------------------
// constructor: crea mallas indexadas en los nodos terminales del grafo

GrafoParam::GrafoParam()
{
   cilindro = new Cilindro( 4, 16 );
   cubo     = new Cubo();
   esfera = new Esfera(16, 16);
   cono = new Cono(16, 16);
}
// -----------------------------------------------------------------------------
// actualizar valor efectivo de un parámetro (a partir de su valor no acotado)

void GrafoParam::actualizarValorEfe( const unsigned iparam, const float valor_na )
{
   assert( iparam < num_parametros );

   using namespace std ;
   //cout << "GrafoParam::actualizarValorEfe( " << iparam << ", " << valor_na << " )" << endl ;

   constexpr float vp = 2.5 ;

   switch( iparam )
   {
      case 0:
        ag_rotacion_plat = 10*valor_na;
        break;
      case 1:
        ag_rotacion_plat2 = -40 + 25*sin( 0.2*(2.0*M_PI*valor_na) );
        break;
      case 2:
        ag_rotacion_plat3 = 90 + 25*sin( 0.1*(2.0*M_PI*valor_na) );
        break;
      case 3:
        ag_rotacion_plat4 = 0 + 25*sin( 0.3*(2.0*M_PI*valor_na) );
        break;
      case 4:
        desplazamiento_plat = (altura_plat_3/3)*sin( 0.3*(2.0*M_PI*valor_na) );
        break;
      case 5:
        ag_giro_rueda = 20.0*sin( 0.25*(2.0*M_PI*valor_na) );
        break;
      case 6:
        ag_rotacion_rueda = 20*valor_na;
        break;
      case 7:
        ag_giro_sirena = 90*valor_na;
        break;
      case 8:
        altura_pata = 1 + 0.5*sin( 0.25*(2.0*M_PI*valor_na) );
        break;
      case 9:
        desplazamiento_pata = (2.0/3.0)*sin( 0.3*(2.0*M_PI*valor_na) );
        break;
      case 10:
        ag_desplazamiento_camion = 7.5*valor_na;
        break;
   }
}

// -----------------------------------------------------------------------------
// visualización del objeto Jerárquico con OpenGL,
// mediante llamadas a los métodos 'draw' de los sub-objetos

void GrafoParam::draw( const ModoVis p_modo_vis, const bool p_usar_diferido )
{
   // asegurarnos que está inicializado ok

   assert( cubo     != nullptr );
   assert( cilindro != nullptr );
   assert( esfera != nullptr );

   // guardar parametros de visualización para esta llamada a 'draw'
   // (modo de visualización, modo de envío)

   modo_vis      = p_modo_vis ;
   usar_diferido = p_usar_diferido ;

   // dibujar objetos

   constexpr float
      sep       = 1.5,  // separación entre centros de las columnas
      radio_cil = 0.5 , // radio del cilindro más fino
      radio_cil2 = radio_cil+0.1 ; // radio del cilindro más grueso

   camionGrua();
}
// -----------------------------------------------------------------------------


void GrafoParam::camionGrua() {
  glPushMatrix();
    glRotatef(ag_desplazamiento_camion, 0, 1, 0);
    glTranslatef(0, 0, 2);
    glPushMatrix();
      glTranslatef(0, 0.2+desplazamiento_pata*0.25, 0);
      cuerpoConRuedas();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1, 0.2+0.35+altura_camion+(desplazamiento_pata*0.25), 0);
      plataforma();
    glPopMatrix();
    glPushMatrix();
      cuatroPatas();
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::cuerpoConRuedas() {
  glPushMatrix();
    glTranslatef(0, 0.35, 0);
    glPushMatrix();
      glPushMatrix();
        glColor3f(204.0/255.0, 6.0/255.0, 5.0/255.0);
        glTranslatef(-1, 0.5, 0);
        glScalef(5, 1, 2);
        cubo->draw(modo_vis, usar_diferido);
      glPopMatrix();
      glPushMatrix();
        glColor3f(179.0/255.0, 40.0/255.0, 33.0/255.0);
        glTranslatef(2.5, 1, 0);
        glScalef(2, 2, 2);
        cubo->draw(modo_vis, usar_diferido);
        glPushMatrix();
          glTranslatef(0, 0.5, 0);
          sirena();
        glPopMatrix();
      glPopMatrix();
      cuatroRuedas();
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::sirena() {
  glPushMatrix();
    glTranslatef(0, 0.05, 0);
    glPushMatrix();
      glColor3f(0.5098, 0.5098, 0.5098);
      glTranslatef(0, -0.05, 0);
      glScalef(0.1, 0.05, 0.1);
      cilindro->draw(modo_vis, usar_diferido);
    glPopMatrix();
    glPushMatrix();
      glColor3f(1, 0, 0);
      glRotatef(ag_giro_sirena, 0, 1, 0);
      glTranslatef(0, 0.1, 0);
      glPushMatrix();
        glTranslatef(0, -0.1, 0);
        glScalef(0.1, 0.1, 0.1);
        cono->draw(modo_vis, usar_diferido);
      glPopMatrix();
      glPushMatrix();
        glPushMatrix();
          glColor3f(0.0235, 0.2235, 0.4431);
          glTranslatef(-0.05, 0, 0);
          glRotatef(-90, 0, 0, 1);
          glTranslatef(0, -0.05, 0);
          glScalef(0.035, 0.1, 0.035);
          cono->draw(modo_vis, usar_diferido);
        glPopMatrix();
        glPushMatrix();
          glColor3f(0.0235, 0.2235, 0.4431);
          glTranslatef(0.05, 0, 0);
          glRotatef(90, 0, 0, 1);
          glTranslatef(0, -0.05, 0);
          glScalef(0.035, 0.1, 0.035);
          cono->draw(modo_vis, usar_diferido);
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::rueda() {
  glPushMatrix();
    glColor3f(0.1569, 0.1569, 0.1569);
    glRotatef(-ag_rotacion_rueda, 0, 0, 1);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.35, 0.15, 0.35);
    cilindro->draw( modo_vis, usar_diferido );
  glPopMatrix();
}

void GrafoParam::cuatroRuedas() {
  glPushMatrix();
    glPushMatrix();
      glTranslatef(2, 0, -1);
      glRotatef(ag_giro_rueda, 0, 1, 0);
      rueda();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-2, 0, -1);
      rueda();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(2, 0, 1.15);
      glRotatef(ag_giro_rueda, 0, 1, 0);
      rueda();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-2, 0, 1.15);
      rueda();
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::plataforma() {
  glPushMatrix();
    glColor3f(0.5098, 0.5098, 0.5098);
    glRotatef(ag_rotacion_plat, 0, 1, 0);
    glTranslatef(0, altura_base_plat, 0);
    glPushMatrix();
      glTranslatef(0, -altura_base_plat, 0);
      cilindroConBola(altura_base_plat, 0.35);
    glPopMatrix();
    glPushMatrix();
      glRotatef(ag_rotacion_plat2, 0, 0, 1);
      glRotatef(-30, 0, 0, 1);
      glTranslatef(0, altura_plat_2, 0);
      glPushMatrix();
        glTranslatef(0, -altura_plat_2, 0);
        cilindroConBola(altura_plat_2, 0.35);
      glPopMatrix();
      glPushMatrix();
        glRotatef(ag_rotacion_plat3, 0, 0, 1);
        glTranslatef(0, altura_plat_3, 0);
        glPushMatrix();
          glTranslatef(0, -altura_plat_3, 0);
          glScalef(0.35, altura_plat_3, 0.35);
          cilindro->draw(modo_vis, usar_diferido);
        glPopMatrix();
        glPushMatrix();
          glColor3f(0.5608, 0.5608, 0.5608);
          glTranslatef(0, desplazamiento_plat, 0);
          glTranslatef(0, altura_plat_3/2, 0);
          glPushMatrix();
            glTranslatef(0, -altura_plat_3, 0);
            glScalef(0.25, altura_plat_3, 0.25);
            cilindro->draw(modo_vis, usar_diferido);
          glPopMatrix();
          glPushMatrix();
            glRotatef(ag_rotacion_plat4, 0, 0, 1);
            glTranslatef(-altura_plat_4, 0, 0);
            glPushMatrix();
              glRotatef(-90, 0, 0, 1);
              cilindroConBola(altura_plat_4, 0.25);
            glPopMatrix();
            glPushMatrix();
              glColor3f(0.0627, 0.1725, 0.3137);
              glRotatef(-ag_rotacion_plat4, 0, 0, 1);
              glScalef(1.25, 1.25, 1.25);
              glTranslatef(-0.5, 0.5, 0);
              cubo->draw(modo_vis, usar_diferido);
            glPopMatrix();
          glPopMatrix();
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::cilindroConBola(const float altura, const float ancho) {
  glPushMatrix();
    glPushMatrix();
      glScalef(ancho, altura, ancho);
      cilindro->draw( modo_vis, usar_diferido );
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, altura, 0);
      glScalef(ancho, ancho, ancho);
      esfera->draw(modo_vis, usar_diferido);
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::pataApoyo() {
  glColor3f(1, 1, 0);
  glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatef(0, 0.25, 0);
    glPushMatrix();
      glPushMatrix();
        glScalef(0.25, 2, 0.25);
        cilindro->draw(modo_vis, usar_diferido);
      glPopMatrix();
      glTranslatef(0, desplazamiento_pata+1, 0);
      glPushMatrix();
        glTranslatef(0, 2, 0);
        glPushMatrix();
          glTranslatef(0, -2, 0);
          glScalef(0.35, 2, 0.35);
          cilindro->draw(modo_vis, usar_diferido);
        glPopMatrix();
        glPushMatrix();
          glRotatef(45, 0, 0, 1);
          glTranslatef(0, 1.5, 0);
          glPushMatrix();
            glRotatef(-45, 0, 0, 1);
            glTranslatef(-1, 0, 0);
            glRotatef(-90, 0, 0, 1);
            glColor3f(1,1,0);
            cilindroConBola(1, 0.35);
          glPopMatrix();
          glPushMatrix();
            glRotatef(180, 0, 0, 1);
            glColor3f(1,1,0);
            cilindroConBola(1.5, 0.35);
          glPopMatrix();
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
    glPushMatrix();
      glColor3f(0.5098, 0.5098, 0.5098);
      glTranslatef(0, -0.25/2, 0);
      glScalef(1, 0.25, 1);
      cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
  glPopMatrix();
}

void GrafoParam::cuatroPatas() {
  glPushMatrix();
    glPushMatrix();
      glTranslatef(1.5, 0, -1.5);
      glRotatef(90, 0, 1, 0);
      pataApoyo();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.5, 0, -1.5);
      glRotatef(90, 0, 1, 0);
      pataApoyo();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(1.5, 0, 1.5);
      glRotatef(-90, 0, 1, 0);
      pataApoyo();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.5, 0, 1.5);
      glRotatef(-90, 0, 1, 0);
      pataApoyo();
    glPopMatrix();
  glPopMatrix();
}
