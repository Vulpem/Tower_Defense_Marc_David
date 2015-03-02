#ifndef ENEMIGO
#define ENEMIGO

#include "mapa.h"

using namespace std; 

class enemigo
{     
      private:
      //Este enemigo se està utilizando?        
      bool activo;
      //Muerte
      bool dead;         //Este enemigo ha muerto?
      int deadAnim;      //Timer para la animacion de muerte
      bool giveGold;     //Booleano usado para que solo de oro una vez al morir
      int recompensa;    //Oro que da al morir
      int regeneracion;  //Vida que se cura con el tiempo
      int givePuntos;
      //Ralentizado
      bool isSlowed;     //Está ralentizado?
      int slowTime;      //Cuanto tiempo ha pasado ralentizado.
      int slowDuration;  //Duracion de la ralentizacion activa
      //Posición
      float X;           //Posicion en pixeles
      float Y;
      //Casilla actual
      int W;             //Posicion en la cuadricula
      int H;
      //Movimiento
      bool fly;
      bool direccionObtenida;//Ya ha obtenido la direccion en la que debe ir y no es necesario comprobarla
      int direccion;         //Direccion en la que se mueve
      float velocidad;       //Velocidad a la que se mueve
      //Combate
      int tipo;              //Tipo del enemigo
      int HP;                //Vida del enemigo
      int maxHP;
      //Animacion de los enemigos
      int animation;
      
      static int puntos;
      
      
      
      public:
      //Constructores
      enemigo () {X=-1000;Y=-1000; direccion=2; velocidad=1; activo=false; direccionObtenida=false; HP=1; tipo=0; dead=true; giveGold=false; recompensa=0; puntos=0;}
      
      //Getters
      float getX () const {return X;}
      float getY () const {return Y;}
      int getDir () const {return direccion;}
      int getTipo () const {return tipo;}
      bool getActivo () const {return activo;}
      bool getDead () const {return dead;}
      int getHP () const {return HP;}
      int getMaxHP () const {return maxHP;}
      bool getFly () const {return fly;}
      int getPuntos () {return puntos;}
      
      //Setters
      void setEnemigo (char _tipo, int dificultad, int _W, int _H)
      {
           if (_tipo != 'o')
           {       
           
             W=_W; X=_W*casillaW + casillaW/2; H=_H; Y=_H*casillaH + casillaH/2;
             tipo=_tipo;
             giveGold=false;
             activo=true;
             dead=false;
             deadAnim=0;
             fly=false;
             isSlowed=false;
             slowTime=0;
             animation=0;
             regeneracion=0;
             switch (_tipo)
             {    //0: Basico
                  case 0:
                  {
                       velocidad=2; HP=500+3000*dificultad; recompensa=50; givePuntos=50; break;
                  }
                  //1: Rapido
                  case 1:
                  {
                       velocidad=4+2*dificultad; HP=350+600*dificultad; recompensa=25; givePuntos=35; break;                   
                  }
                  case 2:
                  {
                       velocidad=1; HP=4000+6000*dificultad; recompensa=150; givePuntos=75; break;
                  }
                  case 3:
                  {
                       velocidad=3+1*dificultad; HP=600+750*dificultad; recompensa=75; fly=true; givePuntos=100; break;
                  }
                  case 4:
                  {
                       velocidad=2; HP=500+750*dificultad; recompensa=75; regeneracion=1+2*dificultad; givePuntos=50; break;
                  }
             }
             
           maxHP=HP;
          }
      }
      
      //Restart puntos
      void restartPuntos ()
      {
           puntos=0;
      }
      
      //Animate
      int Animate ()
      { animation++;
        if (animation>=28) {animation=0;}
        return animation/7;
      }
      
      
      //Obtener la casilla actual
      void Obtener_casilla ()
      {
           int W2=X/casillaW;
           int H2=Y/casillaH;
           if (W2!=W || H2!=H)
              {    W=W2;
                   H=H2;
                   direccionObtenida=false; //Si la casilla actual es distinta a la anterior, es posible que se deba cambiar la direccion
              }
      }      
      
      //Obtener dirección
      void obtenerDireccion ()
      {  
         if (direccionObtenida==false) //Si se necesita cambiar la direccion y se llega a la mitad de la nueva casilla
         {
           if (direccion==1)
           {    if (Y <=( H*casillaH + casillaH/2))
                {    direccion=Analizar_contiguas(); //Analizar contiguas y confirmar que ya se tiene direccion
                     direccionObtenida=true;
                }
           }
           else if (direccion==3)
           {    if (Y >= (H*casillaH + casillaH/2))
                {    direccion=Analizar_contiguas(); //Analizar contiguas y confirmar que ya se tiene direccion
                     direccionObtenida=true;
                }
           }
           else if (direccion==2)
           {    if (X >= (W*casillaW + casillaW/2))
                {    direccion=Analizar_contiguas(); //Analizar contiguas y confirmar que ya se tiene direccion
                     direccionObtenida=true;
                }
           }
           else 
           {    if (X <= (W*casillaW + casillaW/2))
                {    direccion=Analizar_contiguas(); //Analizar contiguas y confirmar que ya se tiene direccion
                     direccionObtenida=true;
                }
           }
         }  
      }
      
      //Analizar celdas contiguas para obtener la nueva direccion, siendo 1:arriba, 2:derecha, 3:abajo, 4:izquierda
      int Analizar_contiguas()
      {
          if (tipo!=3)
          {
              if (direccion!=1)
              {   if (mapGraphics[H+1][W]=='_'||mapGraphics[H+1][W]=='B')
                  {    return 3;
                  }
              }
              if (direccion!=2)
              {   if (mapGraphics[H][W-1]=='_'||mapGraphics[H][W-1]=='B')
                  {    return 4;
                  }
              }
              if (direccion!=3)
              {   if (mapGraphics[H-1][W]=='_'||mapGraphics[H-1][W]=='B')
                  {    return 1;
                  }
              }
              if (direccion!=4)
              {   if (mapGraphics[H][W+1]=='_'||mapGraphics[H][W+1]=='B')
                  {    return 2;
                  }
              }
          }
          else if (tipo==3)
          {
              if (direccion!=1)
              {   if (mapFly[H+1][W]=='_'||mapGraphics[H+1][W]=='B')
                  {    return 3;
                  }
              }
              if (direccion!=2)
              {   if (mapFly[H][W-1]=='_'||mapGraphics[H][W-1]=='B')
                  {    return 4;
                  }
              }
              if (direccion!=3)
              {   if (mapFly[H-1][W]=='_'||mapGraphics[H-1][W]=='B')
                  {    return 1;
                  }
              }
              if (direccion!=4)
              {   if (mapFly[H][W+1]=='_'||mapGraphics[H][W+1]=='B')
                  {    return 2;
                  }
              }   
          }
                    
      }
      
      //Mover
      void mover ()
      {  switch (direccion)
         {    case 1: {Y -= velocidad; break;}
              case 2: {X += velocidad; break;}
              case 3: {Y += velocidad; break;}
              case 4: {X -= velocidad; break;}
         };
      }
      
      
      //Hit
      void hit (int dano, bool end)
      {  HP-=dano;
         if (HP<=0) { velocidad=0; dead=true; HP=1; if(end==false) {giveGold=true;}} //El enemigo muere
      }
      
      int Gold () //Retornar el oro que recibirà el jugador, solo si ha muerto justo antes.
      {
          if (giveGold==true)
          {
             giveGold=false;
             puntos+=givePuntos;
             return recompensa;               
          }
          else 
          {
             return 0;  
          }
      }
      
      void regenerar ()
      {
           if (HP<maxHP && animation%2==0)
           {
           HP+=regeneracion;
           }
      }
      
      //Controla la animación de muerte
      int dye () 
      {
           if (dead==true && deadAnim==14) //Final de la animación de muerte
           {    deadAnim=0;
                X=-1000;Y=-1000; direccion=2; velocidad=0;
                direccionObtenida=false;  
                dead=false;              
                return 0;
           }
           else if (dead==true) //Animacion de muerte
           {
                if (deadAnim==6)
                {   activo=false; //Hacer desaparecer el tanque cuando la explosion es mayor
                }
                deadAnim++;
               return deadAnim-1;
                          
           }
           else {return 0;}
           
           
      }
      
      //Similar al "hit", pero provocando daños alrededor de una casilla y ralentizando el movimiento, para el tipo de torre 2
      void slow (int _dano, int _SlowDuration, int _W, int _H)
      {
           if (X>_W-1.6*casillaW && X<_W+1.6*casillaW && Y>_H-1.6*casillaH && Y<_H+1.6*casillaH)
           {
               slowTime=0;                  //Aunque ya esté ralentizado, el contador se pone a 0 para aumentar la duracion
               slowDuration=_SlowDuration;
               if (isSlowed==false && tipo!= 3)
               {
                  velocidad=velocidad/2;   //Solo se disminuye la velocidad a la mitad si no ha sido ralentizado antes.
                  isSlowed=true;
               }
               hit (_dano, false);         //Aplicar el daño normalmente                     
           }
      }
      
      //Timer de la ralentizacion, que marca cuando la velocidad vuelve a la normalidad
      void slowed ()
      {
        if (isSlowed==true)
        {   
           slowTime++;
           if (slowTime>=slowDuration)
           {    velocidad=velocidad*2;
                isSlowed=false;
           }
        }
      }
      
    
};


#endif
