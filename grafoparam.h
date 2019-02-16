// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.h
// -- declaraciones de clase para el objeto jerárquico de la práctica 3
//
// #############################################################################

#ifndef GRAFOPARAM_H_INCLUDED
#define GRAFOPARAM_H_INCLUDED

#include "malla.h" // añadir .h de cualquier objetos malla indexada usados....

constexpr int num_parametros = 11 ; // número de parámetros o grados de libertad
                                   // de este modelo

typedef int ModoVis;

class GrafoParam
{
   public:

   // crea mallas indexadas (nodos terminales del grafo)
   GrafoParam();

   // función principal de visualización
   void draw( const ModoVis p_modo_vis, const bool p_usar_diferido );

   // actualizar valor efectivo de un parámetro (a partir de su valor no acotado)
   void actualizarValorEfe( const unsigned iparam, const float valor_na );

   // devuelve el número de parámetros
   unsigned numParametros() { return num_parametros ; }

   private:

   // métodos de dibujo de subgrafos
   void columna( const float altura, const float ag_rotacion,
                             const float radio_cil );

    void cuerpoConRuedas();
    void rueda();
    void cuatroRuedas();
    void plataforma();
    void cilindroConBola(const float altura, const float ancho);
    void camionGrua();
    void sirena();
    void pataApoyo();
    void cuatroPatas();


   // objetos tipo malla indexada (nodos terminales)

   Cilindro * cilindro = nullptr ;
   Cubo *     cubo     = nullptr ;
   Esfera * esfera = nullptr;
   Cono * cono = nullptr;

   // parámetros de la llamada actual (o última) a 'draw'
   ModoVis modo_vis ;      // modo de visualización
   bool    usar_diferido ; // modo de envío (true -> diferido, false -> inmediato)


   // valores efectivos de los parámetros (angulos, distancias, factores de
   // escala, etc.....) calculados a partir de los valores no acotados

   float ag_rotacion_plat,
         ag_rotacion_plat2,
         ag_rotacion_plat3,
         ag_rotacion_plat4,
         desplazamiento_plat,
         ag_giro_rueda,
         ag_giro_sirena,
         altura_pata,
         desplazamiento_pata,
         ag_rotacion_rueda,
         ag_desplazamiento_camion ; // ángulo en grados de rotación (2)

    float altura_base_plat = 1;
    float altura_plat_2 = 2;
    float altura_plat_3 = 2.5;
    float altura_plat_4 = 1;
    float altura_camion = 1;

} ;

#endif
