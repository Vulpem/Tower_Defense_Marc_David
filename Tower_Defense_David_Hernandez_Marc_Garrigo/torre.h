#ifndef TORRES
#define TORRES


#include <math.h>
#include "mapa.h"


using namespace std; 

/*Tipos:
  0: Basica
  1: Long
  2: Slow
        
        */

class torres
{     
      private:
      //Animacion
      int animation;        //Temporizador para las animaciones
      
      
      //Variables usadas al apuntar a los enemigos
      int distanciaObjetivo;  
      int distanciaObjetivo2;
      int objetivoX;         //Posicion del enemigo objetivo
      int objetivoY;
      bool enRango; 
      int enemigoObjetivo;  //Numero del enemigo que se ha fijado
      
      //Balas
      int espera;           //Tiempo transcurrido desde el ultimo disparo
      
      bool balaActiva[5];   //Està siendo utilizada la bala?
      float balaTiempo[5];  //Tiempo que lleva activa
      float balaX[5];       //Posicion en pixeles
      float balaY[5];
      int balaObj[5];       //Objetivo de la bala
      int n;
      float vueloT[5];      //Tiempo que tarda la bala hasta llegar al objetivo
      float escala;         //Tamaño de la onda expansiva de las torres del Tipo 2
      
      float difX;           //Diferencia entre la posicion de la bala y el objetivo
      float difY;
      int direccion;        //Direccion de las balas
      
      
      protected:
      bool activa;          //¿Está la torre en uso?
      int nivel;            //Nivel de la torre
      
      //Posicion            
      int X, Y;             //Posicion en pixeles
      int W, H;             //Posicion en la cuadricula
      
      //Atributos           
      int tipo;             //Tipo de la torre
      int rango;
      int coste;
      int dano;
      int slowDuration;     //Duracion de la ralentizacion
      int retraso;          //Tiempo entre dos disparos
      float balaV;          //Velocidad de las balas
      
      
      public:
      //Constructores
      torres () { X=1; Y=1; rango=1; coste=1; dano=1; retraso=1; espera=0; n=0; vueloT[0]=0; vueloT[1]=0; vueloT[2]=0; vueloT[3]=0; vueloT[4]=0; activa=false; nivel=1; animation=0;}
      
      
      
      
      
      //Getters
      bool getActiva () const {return activa;}
      int getNivel () const {return nivel;}
      
      int getX () const {return X;}
      int getY () const {return Y;}
      int getCenterX () const {return X+(casillaW+10)/2;}
      int getCenterY () const {return Y+(casillaW+10)/2;}
      int getTipo () const {return tipo;}
      int getRango () {return rango;}
      
      int getObjetivoX () const {return objetivoX;}
      int getObjetivoY () const {return objetivoY;}
      int getEnemigoObjetivo () const {return enemigoObjetivo;}
      bool getEnRango () const {return enRango;}
      
      bool getBalaActiva (int b) {return balaActiva[b];}
      int getBalaObj (int b) {return balaObj[b];}
      float getBalaX (int b) {return balaX[b];}
      float getBalaY (int b) {return balaY[b];}
      float getBalaV () const {return balaV;}
      float getVueloT (int b) {return vueloT[b];}
      float getBalaTiempo (int b) {return balaTiempo[b];}
      int getEspera () {return espera;}
      float getEscala () const {return escala;}
      
      
      
      int getCoste () {return coste;}
      int getDano () const {return dano;}
      int getSlowDuration () const {return slowDuration;}
      
      //Animate
      int Animate ()
      { animation++;
        return animation;
      }
      
      //Setters
      void setNivel () {nivel++;}
      
      void setBalaActiva (int b, bool active) {balaActiva[b]=active;}
      void setVueloT (int b, float _vuelo) {vueloT[b]=_vuelo;}
      void setBalaX (int b, float _X) {balaX[b]=_X;}
      void setBalaY (int b, float _Y) {balaY[b]=_Y;}
      void setEscala (float _escala) {escala=_escala;}
      
      //Crear una torre
      virtual bool setTower (int _tipo, int _W, int _H)
      {  if (mapGraphics[_H][_W]=='O' ) //Solo construir si el terreno está libre
         {  
               W=_W; H=_H; X=_W*casillaW ; Y=_H*casillaH ; //Cargar la posicion segun los datos ingresados
               tipo=_tipo;
               activa=true;
               mapGraphics[_H][_W]='X'; //Marcar el terreno como ocupado
               //0:Basic ; 1:Long ; 2:Slow; 3: ; 4:Harvester
               switch (_tipo)
               {      case 0:
                      {    rango=casillaH*2.6; dano=8; coste=100; balaV=8; retraso=10; 
                           break;
                      }
                      case 1:
                      {    rango=casillaH*4.5; dano=250; coste=350; balaV=15; retraso=160; 
                           break;
                      }
                      case 2:
                      {    rango=casillaH*1.5; dano=30; coste=300; balaV=0.08; retraso=100; escala=0; slowDuration=101; 
                           break;
                       }
                      case 4:
                      {   
                            rango=casillaH*1.7; dano=0; coste=200; balaV=100; retraso=1; 
                            break;
                       } 
               }
               return true;
               
         }
         else if (mapGraphics[_H][_W]=='X' || mapGraphics[_H][_W]=='_' || mapGraphics[_H][_W]=='A' || mapGraphics[_H][_W]=='B')
         {
              return false;
         }
      }
      
      //Destruye una torre
      void destroyTower ()
      {
           if (mapGraphics[H][W]='X')
           {mapGraphics[H][W]='O';}
           X=0; Y=0; rango=100; coste=100; dano=3; retraso=50; espera=0; n=0; vueloT[0]=0; vueloT[1]=0; vueloT[2]=0; vueloT[3]=0; vueloT[4]=0; activa=false; nivel=1;
       }
      
      //Apuntar
      //reiniciar la distancia objetivo màxima
      void PrepararApuntar ()
      {    distanciaObjetivo2=rango*rango;
      }
      
      void DistanciaObjetivo (float _ObjetivoX, float _ObjetivoY, int _enemigoObjetivo, bool dead)
      {    difX=_ObjetivoX-(X+((casillaW+10)/2));
           difY=_ObjetivoY-(Y+((casillaW+10)/2));
           distanciaObjetivo= difX*difX + difY * difY;
           
           //En caso de que el objetivo actual deje de estar en rango
           if (enemigoObjetivo==_enemigoObjetivo && distanciaObjetivo>=rango)
           {  enRango=false;
           }
           
           
           if (distanciaObjetivo < distanciaObjetivo2) //Siendo al distanciaObjetivo2=rango al inicio, solo se seleccionara el objetivo más cercano si este está dentro del rango
           {  enRango=true;
              enemigoObjetivo=_enemigoObjetivo;
              distanciaObjetivo2=distanciaObjetivo;
              objetivoX=_ObjetivoX;
              objetivoY=_ObjetivoY;
           }

       }    
      
      
      //Timer que indica el tiempo entre disparo y disparo
      bool recargar ()
      {   espera++;
          if (espera>=retraso)
          {   
              disparar();              
              espera=0;
              return true;
          }
          else
          {
              return false;
          }
      }
      //Se genera una bala
      void disparar ()
      {
           if (tipo!=2) //Balas normales
           {
               if (n>4 || n<0) {n=0;}
               balaObj[n]=enemigoObjetivo;
               balaX[n]=X+(casillaW+10)/2;
               balaY[n]=Y+(casillaW+10)/2;
               balaActiva[n]=true; 
               balaTiempo[n] = (  sqrt( difX*difX + difY*difY)) / balaV;
               n++;
           } 
           else if (tipo==2) //Torre tipo 2
           {
               balaObj[n]=enemigoObjetivo;
               balaActiva[n]=true; 
               escala=0;
           }     
      }
      
};




// TORRE DE LA BASE
class Base : public torres
{
      private:
      int HP;
      int lvl;
      
      public:
      Base () {}
      //Getters
      int getHP () const {return HP;}
      int getLvl () const {return lvl;}
      
      //Set torre       
      bool setTower (int _W, int _H)
      {    
               lvl=1;
               HP=15;
               W=_W; H=_H; X=_W*casillaW ; Y=_H*casillaH-23 ; //Cargar la posicion segun los datos ingresados
               tipo=3;
               activa=true;
               rango=casillaH*4.5; dano=100; coste=500; balaV=12; retraso=100; 
         
      }
      void restart ()
      {
           lvl=1;
           HP=15;
           rango=casillaH*4.5; dano=100; coste=500; balaV=12; retraso=100; 
      }
      
      void lvlUp ()
      {
           if (lvl<5)
           {
              lvl++;
              rango=rango*1.15; dano=dano*1.65; retraso=retraso*0.9; HP+=5; balaV=balaV*1.25;
           }
      }
      
      bool hit ()
      {
           HP--;
           if (HP>0)
           {
              return false;
           }
           else
           {
               return true;
           }
      }
               
};

# endif
