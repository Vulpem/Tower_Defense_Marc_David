# include <iostream>
# include <allegro.h>
# include <fstream>

# include "torre.h"
# include "enemigos.h"
# include "rotar.h"
# include "mapa.h"
# include "oleadas.h"



using namespace std;

//Variable que almacena el numero maximo de torres que puede haber en el nivel según el mapa
int nMaxTorres=0;
const int FPS = 30; //FPS máximos a los que funcionará el juego
int enemigo::puntos=0;

//Funcion que contará los ticks, usados para regular los FPS
volatile int ticks = 0;
void ticker()
{ 
	ticks++;
} 
END_OF_FUNCTION(ticker)


bool init ()
{
     //Inicializando allegro
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    
    //Profundidad de color 
    set_color_depth(24);
    int ret;
    bool fullscreen=false;
    //Tamaño de la pantalla
    if (fullscreen == true)
    {
        ret = set_gfx_mode(GFX_AUTODETECT, 1280, 1024, 0, 0);
    }
    else
    {
        ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1152, 864, 0, 0);
    }
    //Se dará este error cuando la resolucion que se ha seleccionado sea mayor a la pantalla por la que se visualiza    
    if (ret !=0)
    {
           allegro_message("Could not set graphics mode");
           allegro_exit();
           return false;
    }
    return true;
}
END_OF_FUNCTION (init)



int main ()
{   
    if (init ()==false)
    {return 0;}
    
    //Inicializar el "timer" que contará los ticks.
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker, BPS_TO_TIMER(FPS));
 
         
    //Creando el buffer 
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    //Creando el escenario
    BITMAP *escenario = create_bitmap(SCREEN_W, SCREEN_H);
    //Creando la interficie
    BITMAP *interficie = create_bitmap(SCREEN_W, SCREEN_H*3/mapH);
    rectfill(interficie, 0, 0, SCREEN_W, SCREEN_H*3/mapH, makecol(255,0,255));
    
    ////////
    //MAPA//
    ////////
    
    //Res: 1280, 1024
    //Size: 25x15
    //int mapH=15;
    //int mapW=25;
    
    //Obtener la altura y anchura de las casillas
    casillaH=SCREEN_H/mapH;
    casillaW=SCREEN_W/mapW;
    
    //Obtener las coordenadas del inicio y el final
    setStart ();


    //Cargando imágenes
    //Cursor
    BITMAP *Cursor = load_bitmap("Interfaz/Cursor.bmp", NULL);
    //Base de la interficie sobre la que se dibujaran otros elementos
    BITMAP *InterficieBase = load_bitmap("Interfaz/Interficie.bmp", NULL);
        
    //Reparar la base
    BITMAP *Repair = load_bitmap("Interfaz/Repair.bmp", NULL);
    
    //Escenario    
    BITMAP *Nieve = load_bitmap("Escenario/Nieve.bmp", NULL);

    //Torres    
    BITMAP *tower[5];
        //0: Basic
        tower[0] = load_bitmap("Torres/Basica.bmp", NULL);
        //1: Long
        tower[1] = load_bitmap("Torres/Larga.bmp", NULL);
        //2: Slow
        tower[2] = load_bitmap("Torres/Slow.bmp", NULL);
        //3: Base
        tower[3] = load_bitmap("Torres/Base.bmp", NULL);
        //4: Harvester
        tower[4] = load_bitmap("Torres/Harvester.bmp", NULL);
        
     //Enemigos   
    BITMAP *Enemigo[5];
           //0: Basic
        Enemigo[0] = load_bitmap("Enemigos/Basico.bmp", NULL);
          //1: Fast
        Enemigo[1] = load_bitmap("Enemigos/Fast.bmp", NULL);
          //2: Bomber
        Enemigo[2] = load_bitmap("Enemigos/Bomber.bmp", NULL);
          //3: Flyer
        Enemigo[3] = load_bitmap("Enemigos/Flyer.bmp", NULL);
          //4: Bot
        Enemigo[4] = load_bitmap("Enemigos/Bot.bmp", NULL);
         
    //Cargamos antes el disparo de la torre tipo 2 para estandarizar su tamaño. Tendrà siempre el mismo tamaño sin importar em tamaño de la imagen, puesto que a la hora de
    //dibujarlo debe ocupar siempre el mismo espacio: una casilla alrededor de la torre, su area de efecto.    
    BITMAP *Tipo2 = create_bitmap(casillaW*6, casillaH*6);
    BITMAP *Slowb = load_bitmap("Torres/Slowb.bmp", NULL);
    rectfill(Tipo2, 0,0,casillaW*6, casillaH*6, makecol(255,0,255)); //El fondo de la nueva bala debe de ser de este color para que allegro lo interprete como transparente.
    stretch_sprite(Tipo2, Slowb, 0, 0, casillaW*6, casillaH*6);  

    //Cargar las balas de todos los tipos de torres 
    BITMAP *Bullet[3];
        Bullet[0] = load_bitmap("Torres/Basicab.bmp", NULL);
        Bullet[1] = load_bitmap("Torres/Longb.bmp", NULL);
        Bullet[2] = Tipo2;
    //Animacion de muerte
    BITMAP *Muerte = load_bitmap ("Enemigos/Muerte.bmp", NULL);
    BITMAP *Crear = load_bitmap("Enemigos/Spawn.bmp", NULL);
    
    
    
    
    //Informacion sobre las torres
    BITMAP *t[5];
        //0: Basic
        t[0] = load_bitmap("Interfaz/Menus/t0.bmp", NULL);
        //1: Long
        t[1] = load_bitmap("Interfaz/Menus/t1.bmp", NULL);
        //2: Slow
        t[2] = load_bitmap("Interfaz/Menus/t2.bmp", NULL);
        //3: Base
        t[3] = load_bitmap("Interfaz/Menus/t3.bmp", NULL);
        //4: Harvester
        t[4] = load_bitmap("Interfaz/Menus/t4.bmp", NULL);
        
    //Informacion sobre los enemigos
    BITMAP *e[5];
        //0: Basic
        e[0] = load_bitmap("Interfaz/Menus/e0.bmp", NULL);
        //1: Long
        e[1] = load_bitmap("Interfaz/Menus/e1.bmp", NULL);
        //2: Slow
        e[2] = load_bitmap("Interfaz/Menus/e2.bmp", NULL);
        //3: Base
        e[3] = load_bitmap("Interfaz/Menus/e3.bmp", NULL);
        //4: Harvester
        e[4] = load_bitmap("Interfaz/Menus/e4.bmp", NULL);
     //MENUS(Inicio y ayudas)
    BITMAP *Inicio[3];
        //0: Inicio
        Inicio[0] = load_bitmap("Interfaz/menu.bmp", NULL);
        //1: Inicio ayuda
        Inicio[1] = load_bitmap("Interfaz/help1.bmp", NULL);
        //2: interficie ayuda
        Inicio[2] = load_bitmap("Interfaz/help2.bmp", NULL);
    
    //SONIDOS
    MIDI *menuMusic;
    menuMusic = load_midi ("Musica/menu.mid");
    MIDI *music;
    music = load_midi ("Musica/music.mid");
    SAMPLE *Rocketeer;
    Rocketeer = load_sample("Torres/Rocketeer.wav");
    SAMPLE *Pulse;
    Pulse = load_sample("Torres/Pulse.wav");
    SAMPLE *BaseM;
    BaseM = load_sample("Torres/Base.wav");
    SAMPLE *Railgun;
    Railgun = load_sample("Torres/Railgun.wav");
    SAMPLE *Spawn;
    Spawn = load_sample("Enemigos/Spawn.wav");
    SAMPLE *Harvester;
    Harvester = load_sample("Torres/Harvester.wav");
    
    
    ////////////////////////////////
    //Generar el escenario (fondo)//
    ////////////////////////////////
    
    //En este bucle también se añadirá uno al maximo de torres dibujables por cada "O" en el mapa.    
    for (int h=0; h<mapH; h++)
        {   for (int w=0; w<mapW; w++)
            {    char terrain = mapGraphics[h][w]; //Obtener la casilla
                 if (terrain=='O' || terrain=='X')
                 {
                   if (terrain=='O') //Terreno normal. Un aleatorio que dibuja cualquier de las cuatro posibles imagenes.
                   {nMaxTorres++;}
                   
                   //masked_stretch_blit(Nieve, escenario, source_x, source_y, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                   int t = (rand()%100);
                   if (t==0)
                   {
                      masked_stretch_blit(Nieve, escenario, 240, 80, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                   }
                   else if (t==1)
                   {
                     masked_stretch_blit(Nieve, escenario, 320, 0, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                   }
                   else if (t==2)
                   {
                     masked_stretch_blit(Nieve, escenario, 320, 80, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                   }
                   else if (t>=3)
                   {
                     masked_stretch_blit(Nieve, escenario, 240, 0, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                   }
                 }
                 else if (terrain=='_') //Si el terreno es carretera, obtener las carreteras contiguas y dibujar correspondientemente
                 {
                    if (mapGraphics[h][w+1]=='_' && mapGraphics[h][w-1]=='_')
                    {
                        masked_stretch_blit(Nieve, escenario, 0, 0, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    }
                    else if (mapGraphics[h+1][w]=='_' && mapGraphics[h-1][w]=='_')
                    {
                       masked_stretch_blit(Nieve, escenario, 0, 80, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    }
                    else if (mapGraphics[h-1][w]=='_' && mapGraphics[h][w+1]=='_')
                    {
                       masked_stretch_blit(Nieve, escenario, 80, 80, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    }    
                    else if (mapGraphics[h+1][w]=='_' && mapGraphics[h][w+1]=='_')
                    {
                       masked_stretch_blit(Nieve, escenario, 80, 0, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    }   
                    else if (mapGraphics[h+1][w]=='_' && mapGraphics[h][w-1]=='_')
                    {
                       masked_stretch_blit(Nieve, escenario, 160, 0, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    }
                    else if (mapGraphics[h-1][w]=='_' && mapGraphics[h][w-1]=='_')
                    {
                       masked_stretch_blit(Nieve, escenario, 160, 80, 80, 80, w*casillaW, h*casillaH, casillaW, casillaH);
                    } 
                 }
            else if (mapGraphics[h][w]=='A') //Inicio
            {
                 masked_stretch_blit(Nieve, escenario, 400, 0, 160, 80, w*casillaW+1, h*casillaH, casillaW*2, casillaH);
            } 
            else if (mapGraphics[h][w]=='B') //Final
            {
                 masked_stretch_blit(Nieve, escenario, 560, 0, 160, 160, (w-1)*casillaW+1, (h-1)*casillaH+1, casillaW*2-1, casillaH*2-1);
            }   
                     
                 rect(escenario, w*casillaW, h*casillaH, (w+1)*casillaW, (h+1)*casillaH, makecol(30,30,60));
            }
        }
    
    //Cargar la imagen que harà de puntero
    set_mouse_sprite(Cursor);
    //Centrar la imagen del cursor en el puntero
    set_mouse_sprite_focus(15,15);
    
    //Inicializando clases
    Base *base;
    base = new Base;
    torres *torre;
    torre = new torres[nMaxTorres];
    enemigo *enemigos;
    enemigos = new enemigo[40]; //No habrá más de 40 enemigos por pantalla a la vez.
    
    //Crear cuatro torres para obtener sus datos y almacenarlos. Seran destruidas antes de iniciar el juego.
    torre[0].setTower(0,0,0);
    torre[1].setTower(1,1,0);
    torre[2].setTower(2,2,0);
    base->setTower(BW,BH-1);
    torre[3].setTower(4,3,0);
    
    ///////////////////////
    //Variables del juego/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////
    bool play=false;
    bool help=false;
    
    int oro=1000;
    int vidas=15;
    int oleada=0;
    //Variables usadas al "Spawnear" enemigos
    int oleadaFunc=0;
    int p=-420; //Tiempo hasta el proximo enemigo
    int n=0;    //Numero del ulimo enemigo spawneado
    int numEnemigo=0;
    int dificultad=0;
    bool create=true; //Se debe dibujar la animacion de spawn?
    ////Variables usadas al "Spawnear" torres
    bool construir=false;//¿Se tiene seleccionada alguna torre para construirla?
    int torretipo=0;  //Tipo de la torre que se construira
    int torreN=0;     //Numero de la torre que se debe construir
    int notDestroy=0; //Tiempo durante el que no se pueden vender las torres tras construirlas
    
    //Datos de las torres (obtenidos de las creadas previamente)
    int coste[5];
    coste[0]=torre[0].getCoste();
    coste[1]=torre[1].getCoste();
    coste[2]=torre[2].getCoste();
    coste[3]=base->getCoste();
    coste[4]=torre[3].getCoste();
    int rango[5];
    rango[0]=torre[0].getRango();
    rango[1]=torre[1].getRango();
    rango[2]=torre[2].getRango();
    rango[3]=base->getRango();
    rango[4]=torre[3].getRango();
    
    float balaDifX;
    float balaDifY;
    
    float angle;


    //Cargar las fuentes que utilizaremos
    FONT* fuentes[2];
    fuentes [0] = load_font ("Interfaz/font1.pcx", NULL, NULL);
    fuentes [1]= load_font ("Interfaz/font2.pcx", NULL, NULL);
   
    ////////////////////////////////
    //Generar la interficie (base)//
    //////////////////////////////// 
    //Dibujar la base
    stretch_sprite(interficie, InterficieBase, 0, 0, SCREEN_W, SCREEN_H*0.2);
    //Dibujar las imagenes de las torres
    masked_stretch_blit(tower[0], interficie, 350, 0, 70, 70, SCREEN_W/10.23, SCREEN_H*0.2/2.45,casillaW+10, casillaW+10 );
    masked_stretch_blit(tower[1], interficie, 350, 0, 70, 70, SCREEN_W/4.96, SCREEN_H*0.2/2.9, casillaW+10, casillaW+10);
    masked_stretch_blit(tower[2], interficie, 350, 0, 70, 70, SCREEN_W/3.34, SCREEN_H*0.2/2.37, casillaW+10, casillaW+10);
    masked_stretch_blit(Repair, interficie, 0, 0, 99, 99, SCREEN_W/2.5-5, SCREEN_H*0.2/2.2-5, casillaW+15, casillaW+15); 
    masked_stretch_blit(tower[3], interficie, 350, 0, 70, 70, SCREEN_W/2.47, SCREEN_H*0.2/2.5, casillaW+10, casillaW+10);
    masked_stretch_blit(tower[4], interficie, 0, 0, 70, 70, SCREEN_W/2.0078, SCREEN_H*0.2/2.5, casillaW+10, casillaW+10);
    
    //Costes
    textprintf_ex(interficie, fuentes[0], SCREEN_W/16.2025, SCREEN_H*0.2/1.8618, makecol(161, 178, 200), -1, "%i", coste[0]);
    textprintf_ex(interficie, fuentes[0], SCREEN_W/6.1244, SCREEN_H*0.2/1.8618, makecol(161, 178, 200), -1, "%i", coste[1]);
    textprintf_ex(interficie, fuentes[0], SCREEN_W/3.7758, SCREEN_H*0.2/1.8618, makecol(161, 178, 200), -1, "%i", coste[2]);
    textprintf_ex(interficie, fuentes[0], SCREEN_W/2.75, SCREEN_H*0.2/1.8618, makecol(161, 178, 200), -1, "%i", coste[3]);
    textprintf_ex(interficie, fuentes[0], SCREEN_W/2.16, SCREEN_H*0.2/1.8618, makecol(161, 178, 200), -1, "%i", coste[4]); 
   
    //Nombres
     textout_ex(interficie, fuentes[0], "Railgun", SCREEN_W/16.2025, SCREEN_H*0.2/1.28, makecol(161, 178, 200), -1);
     textout_ex(interficie, fuentes[0], "Rocketeer", SCREEN_W/6.1244, SCREEN_H*0.2/1.28, makecol(161, 178, 200), -1);
     textout_ex(interficie, fuentes[0], "Pulse", SCREEN_W/3.7758, SCREEN_H*0.2/1.28, makecol(161, 178, 200), -1);
     textout_ex(interficie, fuentes[0], "Base", SCREEN_W/2.745, SCREEN_H*0.2/1.28, makecol(161, 178, 200), -1);
     textout_ex(interficie, fuentes[0], "Harvester", SCREEN_W/2.155, SCREEN_H*0.2/1.28, makecol(161, 178, 200), -1);
     
   
    
    //Limpiar las torres usadas para obtener sus datos
    torre[0].destroyTower();
    torre[1].destroyTower();
    torre[2].destroyTower();
    torre[3].destroyTower();
    
    
    //Mostrando el raton    
      show_mouse(screen);
      
      //Sound
      set_volume (140,180);
    play_midi(menuMusic, true);  
      ////////////////////
    //INICIO DEL BUCLE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    //////////////////// 
      
    do
    {              
    // Regulando los FPS. No hacer nada (rest) hasta que haya pasado por lo menos un tick entero.                    
    while(ticks == 0)
		{
			rest(1);
		}

    ticks--; //Restar un tick para voler a ponerlo a 0.
    
    //DIBUJAR una base
    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,0));
        
    //dibujar el FONDO
    draw_sprite(buffer, escenario, 0, 0);      
    
    if (play==true)
    {      
           help=false;
           
                 
    //SPAWN DE ENEMIGOS
    
    
        //Si las oleadas programadas llegan a su fin, volver a la primera
        if (oleadaFunc>=15) {oleadaFunc=0; dificultad++;}
        //Incrementar el "timer" de los enemigos
        p++;
        //Incrementar la oleada cuando esten a punto de llegar los enemigos
        if (p==-1){oleada++;}
        //Espawnear un enemigo cuando haya pasado 1 segundo
        if (p>=30) 
        {    
            //dificultad=(oleada-1)/15; //Cada vez que se resetean las oleadas aumenta la dificultad  de estas
            p=1;
            if (numEnemigo >= 14 || tipoEnemigo(oleadaFunc, numEnemigo)== 'o' )
            {
                numEnemigo = -1; oleadaFunc++; p=-870-dificultad*90;
                create=false;
            } //Termina la oleada cuando se han leido todos los enemigos o se encuentra con un 'o'
            else
            {
                 play_sample(Spawn,60,128,2000,false);
            }
            //Crear el enemigo
            enemigos[n].setEnemigo(tipoEnemigo(oleadaFunc, numEnemigo), dificultad, 0, 2);
            
            
            numEnemigo++;
            n++;
        }
        if (p>=20 && p<=30 && create==true)
        {
              masked_stretch_blit(Crear, buffer, (p-19)*100, 0, 100, 100, AW*casillaW, AH*casillaH, casillaW, casillaH);    
        }  
        create=true;    
        if (n>=39) //Como solo puede habar 40 enemigos, al llegar a 39 se vuelve a utilizar la clase enemigo[0]
        {n=0;}
        
        //Comprovar si hay algun enemigo vivo
        int activos=0;
        if (oleadaFunc>=1 && vidas>0 && p<-1)
        {
            for (int q=0; q<40; q++)
            {
                if (enemigos[q].getActivo()==true)
                {
                    activos++;
                }
            }
            //Si no lo hay, pasar a la siguiente oleada
            if (activos==0)
            {
               p=100; oleada++;          
            }
        }
    
    
    //FINAL DEL SPAWN DE ENEMIGOS
    
      } //Cerrando el "if (play==true)"
    
    //////////////////////////////                     
    //MOVIMIENTO DE LOS ENEMIGOS//
    /////////////////////////////
    
        for (int r=0; r<40; r++)
        { if (enemigos[r].getActivo()==true)
          {
              enemigos[r].slowed();            //Comprovar el "timer" de la ralentizacion
              enemigos[r].Obtener_casilla ();  //Comprovar si se ha cambiado de casilla
              enemigos[r].obtenerDireccion (); //Obtener una nueva direccion si se ha cambiado
              enemigos[r].mover ();            //Incrementar la posicion
              enemigos[r].regenerar();      //Regenerar la vida que sea necesaria
              for (int z=0; z<nMaxTorres; z++)
              {   if (torre[z].getActiva()==true)
                  {
                      if (enemigos[r].getDead()==true && torre[z].getTipo()==4 && (enemigos[r].getX()-torre[z].getCenterX())*(enemigos[r].getX()-torre[z].getCenterX())+(enemigos[r].getY()-torre[z].getCenterY())*(enemigos[r].getY()-torre[z].getCenterY()) <= torre[z].getRango()*torre[z].getRango())
                      { 
                          oro+=enemigos[r].Gold ()*5/4; //Incrementar el oro más de lo habitual si el enemigo ha muerto cerca de un "Harvester"
                          fastline (buffer, torre[z].getCenterX(), torre[z].getCenterY(), enemigos[r].getX(), enemigos[r].getY(), makecol(235,235,85));
                          fastline (buffer, torre[z].getCenterX()+20, torre[z].getCenterY()+20, enemigos[r].getX(), enemigos[r].getY(), makecol(235,235,85));
                          fastline (buffer, torre[z].getCenterX()+20, torre[z].getCenterY()-20, enemigos[r].getX(), enemigos[r].getY(), makecol(235,235,85));
                          fastline (buffer, torre[z].getCenterX()-20, torre[z].getCenterY()+20, enemigos[r].getX(), enemigos[r].getY(), makecol(235,235,85));
                          fastline (buffer, torre[z].getCenterX()-20, torre[z].getCenterY()-20, enemigos[r].getX(), enemigos[r].getY(), makecol(235,235,85));
                          play_sample(Harvester,80,128,1000,false);
                      }
                  }
              }
              oro+=enemigos[r].Gold ();        //Incrementar el oro si el enemigo ha muerto
              if (enemigos[r].getX()>casillaW*24)//Morir y perder una vida si el enemigo llega al margen derecho de la pantalla
              {
                  enemigos[r].hit(100000000, true);
                  if (enemigos[r].dye()==4)
                  {   //vidas--;
                      base->hit();
                  }
                                                    
              }
          }
        }
        //FINAL DEL MOVIMIENTO DE LOS ENEMIGOS
        
        
      ////////////////////
      //Torres apuntando//  
      ////////////////////
      
     //Obtener el objetivo, su posicion y incrementar el timer de recarga de las torres
      for (int r=0; r<nMaxTorres; r++)
      {     if (torre[r].getActiva()==true && torre[r].getTipo() != 4) //Comprovar que la torre está en uso
            {   
                  torre[r].PrepararApuntar();   //fijar el rango máximo
                  
                  if (torre[r].getEnRango () == true) //Si la torre ya tiene un objetivo en rango, seguir disparando a ese objetivo
                  { 
                      torre[r].DistanciaObjetivo( enemigos[torre[r].getEnemigoObjetivo()].getX(), enemigos[torre[r].getEnemigoObjetivo()].getY(), torre[r].getEnemigoObjetivo(), enemigos[torre[r].getEnemigoObjetivo()].getDead());
                      if(torre[r].recargar()==true) //Incrementar el "timer" que marca el tiempo entre disparos
                      {   switch (torre[r].getTipo())
                          {
                                 case 0: {play_sample(Railgun,30,128,2500,false); break;}
                                 case 1: {play_sample(Rocketeer,80,128,1000,false);break;}
                                 case 2: {play_sample(Pulse,80,128,2000,false);break;}  
                          }                
                      }  
                  }  
                  else  //Si no tiene un objetivo en rango, buscar el enemigo en rango más cercano
                  {
                      for (int e=0; e<=40; e++)
                      {    if (enemigos[e].getDead()==false)
                           {
                               torre[r].DistanciaObjetivo (enemigos[e].getX(), enemigos[e].getY(), e,enemigos[e].getDead());
                           }
                      }
                  }
            }
      }
      //LO MISMO DE LA BASE
      base->PrepararApuntar();   //fijar el rango máximo
                  
      if (base->getEnRango () == true) //Si la torre ya tiene un objetivo en rango, seguir disparando a ese objetivo
      { 
          base->DistanciaObjetivo( enemigos[base->getEnemigoObjetivo()].getX(), enemigos[base->getEnemigoObjetivo()].getY(), base->getEnemigoObjetivo(), enemigos[base->getEnemigoObjetivo()].getDead());
          if (base->recargar()==true)  //Incrementar el "timer" que marca el tiempo entre disparos
          {
              play_sample(BaseM,180,128,1000,false);                         
          }
      }  
      else  //Si no tiene un objetivo en rango, buscar el enemigo en rango más cercano
      {
          for (int e=0; e<=40; e++)
          {    if (enemigos[e].getDead()==false)
               {
                    base->DistanciaObjetivo (enemigos[e].getX(), enemigos[e].getY(), e,enemigos[e].getDead());
               }
          }
      }
      
       //Moviendo las balas y dibujandolas
      for (int t=0; t<nMaxTorres; t++)
      { if (torre[t].getActiva()==true && torre[t].getTipo() != 4)
        {
          for (int b=0; b<5; b++)
          {    //Proceder solo si la bala está activa y el objetivo tambien
              if (torre[t].getBalaActiva(b)==true && enemigos[torre[t].getBalaObj(b)].getDead()==false)
              {
                   if (torre[t].getTipo()!=2) //Obtener la direccion, mover y dibujar la bala normalmente
                   {
                        balaDifX = enemigos[torre[t].getBalaObj(b)].getX() - torre[t].getBalaX(b);
                        balaDifY = enemigos[torre[t].getBalaObj(b)].getY() - torre[t].getBalaY(b);
                        angle = atan2 (balaDifY, balaDifX);
                        torre[t].setBalaX(b, torre[t].getBalaX(b)+torre[t].getBalaV()*cos(angle));
                        torre[t].setBalaY(b, torre[t].getBalaY(b)+torre[t].getBalaV()*sin(angle));
                        torre[t].setVueloT(b, torre[t].getVueloT(b)+1);
                        //Dibujar la bala correspondiente
                        if (enemigos[torre[t].getBalaObj(b)].getActivo()==true) //Dibujar la bala solo si el enemigo estáactivo
                        {   //Rotar la bala para que siga la dirección del movimiento 
                            rotate_sprite(buffer, Bullet[torre[t].getTipo()], torre[t].getBalaX(b)-10, torre[t].getBalaY(b)-10, 
                            ftofix(angle*40) );
                        }
                        //Inflingi el daño una vez pasado el tiempo necesario para llegar hasta el enemigo
                        if (torre[t].getVueloT(b)>= torre[t].getBalaTiempo(b))
                        {    torre[t].setBalaActiva(b,false);
                             torre[t].setVueloT(b,0);
                             enemigos[torre[t].getBalaObj(b)].hit(torre[t].getDano(), false);
                        }
                   }
                   else if (torre[t].getTipo()==2)  //El tipo de bala de "onda expansiva" del tipo de torre 2
                   {
                        torre[t].setEscala(torre[t].getEscala()+torre[t].getBalaV());
                        pivot_scaled_sprite(buffer, Tipo2, torre[t].getCenterX(), torre[t].getCenterY(), casillaW*3, casillaH*3, 0, ftofix(torre[t].getEscala()));
                        if (torre[t].getEscala()>= 0.5)
                        {
                              for (int w=0; w<40; w++)
                              {
                                  enemigos[w].slow(torre[t].getDano(),torre[t].getSlowDuration(), torre[t].getCenterX(), torre[t].getCenterY());
                                        //En vez de llamar a "hit", llamamos a "slow" para que realentice al enemigo
                              }
                              torre[t].setBalaActiva(b,false);
                              torre[t].setEscala(0);                       
                        }
                   }
                   
              }
          }
        }  
      }
      
      //LO MISMO PARA LA BASE
      for (int b=0; b<5; b++)
          {    //Proceder solo si la bala está activa y el objetivo tambien
              if (base->getBalaActiva(b)==true && enemigos[base->getBalaObj(b)].getDead()==false)
              {
                   
                        float balaDifX = enemigos[base->getBalaObj(b)].getX() - base->getBalaX(b);
                        float balaDifY = enemigos[base->getBalaObj(b)].getY() - base->getBalaY(b);
                        float angle = atan2 (balaDifY, balaDifX);
                        base->setBalaX(b, base->getBalaX(b)+base->getBalaV()*cos(angle));
                        base->setBalaY(b, base->getBalaY(b)+base->getBalaV()*sin(angle));
                        base->setVueloT(b, base->getVueloT(b)+1);
                        //Dibujar la bala correspondiente
                        if (enemigos[base->getBalaObj(b)].getActivo()==true) //Dibujar la bala solo si el enemigo estáactivo
                        {   //Rotar la bala para que siga la dirección del movimiento 
                            rotate_sprite(buffer, Bullet[1], base->getBalaX(b)-10, base->getBalaY(b)-10, 
                            ftofix(angle*40) );
                        }
                        //Inflingi el daño una vez pasado el tiempo necesario para llegar hasta el enemigo
                        if (base->getVueloT(b)>= base->getBalaTiempo(b))
                        {    base->setBalaActiva(b,false);
                             base->setVueloT(b,0);
                             enemigos[base->getBalaObj(b)].hit(base->getDano(), false);
                        }
                                         
                        
                   
                   
              }
          }   
      // FINAL DE LAS TORRES APUNTANDO
      
 if (play==true)
 {     
      ///////////
      //JUGADOR///////////////////////////////////////////////////////////////////////
      //////////
      //Seleccionar la torre que se quiere construir
      if (mouse_b==1 && mouse_y<SCREEN_H*0.882+80 && mouse_y>SCREEN_H*0.882 && construir==false) //Si se pulsa el ratón y se està dentro del rango Y
      {
          if (mouse_x>SCREEN_W/10.23 && mouse_x<SCREEN_W/10.23+casillaW+10)//Tipo 0
          {
               if (oro-coste[0]>=0)
               {   oro-=coste[0];
                   torretipo=0;
                   construir=true;
               }  
               //stretch_sprite(buffer, t[0], 805, 7, 308, 118);                      
          }
          else if (mouse_x>SCREEN_W/4.96 && mouse_x<SCREEN_W/4.96+casillaW+10)//Tipo 1
          {
               if (oro-coste[1]>=0)
               {   oro-=coste[1];
                   torretipo=1;
                   construir=true;  
               }                         
          }
          else if (mouse_x>SCREEN_W/3.34 && mouse_x<SCREEN_W/3.34+casillaW+10)//Tipo 2
          {
               if (oro-coste[2]>=0)
               {   oro-=coste[2];
                   torretipo=2;
                   construir=true;  
               }                         
          }
          else if (mouse_x>SCREEN_W/2.47 && mouse_x<SCREEN_W/2.47+casillaW+10 && notDestroy>20 && base->getLvl() < 5)//Tipo 3 (base)
          {
               if (oro-coste[3]>=0)
               {   oro-=coste[3];
                   base->lvlUp(); 
                   notDestroy=0; 
               }                         
          }
          else if (mouse_x>SCREEN_W/2.0078 && mouse_x<SCREEN_W/2.0078+casillaW+10) //Tipo 4
          {
               if (oro-coste[4]>=0)
               {   oro-=coste[4];
                   torretipo=4;
                   construir=true;  
               }
          }
      }
      
      //Construir la torre seleccionada si la casilla está libre
      if(mouse_b==1 && construir==true && mouse_y<SCREEN_H-SCREEN_H*0.2)
      {
           bool SeConstruye = torre[torreN].setTower(torretipo,mouse_x/casillaW,mouse_y/casillaH);
           if (SeConstruye==true)
           {
             torreN++;
             notDestroy=0;
             construir=false;  
           }
           else if (SeConstruye==false) 
           {
               construir=false;
               oro+=coste[torretipo]; 
           }
      }
      // FINAL DEL JUGADOR
      
      
      
      
  } //Cerrando el "if (play==true)"
        
        
        
     //////////////// 
    //DIBUJAR///////////////////////
    ////////////////
    //Interficie
    draw_sprite(buffer, interficie, 0, SCREEN_H-(casillaH*3));
    if (base->getLvl()>=5)
    {
        masked_stretch_blit(Repair, interficie, 0, 0, 99, 99, SCREEN_W/2.5-5, SCREEN_H*0.2/2.2-5, casillaW+15, casillaW+15); 
    }
    
    vidas = base->getHP();
    //Elementos interactivos de la interficie
     /*Vidas*/ textprintf_ex(buffer, fuentes[1], SCREEN_W/1.3061, SCREEN_H/1.2278, makecol(161, 178, 200), -1, "%i", vidas);
     /*Oleada*/ textprintf_ex(buffer, fuentes[1], SCREEN_W/1.3061, SCREEN_H/1.1743, makecol(161, 178, 200), -1, "%i", oleada);
     /*Siguiente*/textprintf_ex(buffer, fuentes[1], SCREEN_W/1.3061, SCREEN_H/1.135, makecol(161, 178, 200), -1, "%i", (30-p)/30);
     /*Puntos*/textprintf_ex(buffer, fuentes[1], SCREEN_W/1.3061, SCREEN_H/1.08245, makecol(161, 178, 200), -1, "%i", enemigos[0].getPuntos());
     /*Oro*/   textprintf_ex(buffer, fuentes[1], SCREEN_W/12.8, SCREEN_H/1.19906, makecol(161, 178, 200), -1, "%i", oro);
    
    
    
      
      //Dibujando las torres
      for (int n=0; n<nMaxTorres; n++)
      {  if (torre[n].getActiva()==true)
        {
            
          //Torre
          if (torre[n].getTipo() != 4)
          {
              int number = rotar(  torre[n].getCenterX(),  torre[n].getCenterY() ,torre[n].getObjetivoX(),torre[n].getObjetivoY()  );
              masked_stretch_blit(tower[torre[n].getTipo()], buffer, number*70, 0, 70, 70, torre[n].getX(),  torre[n].getY(), casillaW+10, casillaW+10);
          }
          else
          {   int animation = (torre[n].Animate()/4)%23;
              masked_stretch_blit(tower[torre[n].getTipo()], buffer, animation*70, 0, 70, 70, torre[n].getX(),  torre[n].getY(), casillaW, casillaW);
          }
        }
          
      }
      //LO MISMO PARA LA BASE
      int number = rotar(  base->getCenterX(),  base->getCenterY() ,base->getObjetivoX(),base->getObjetivoY()  );
      masked_stretch_blit(tower[3], buffer, number*70, 0, 70, 70, base->getX(),  base->getY(), casillaW+10, casillaW+10);
      
      //Dibujando enemigos
      for (int r=0; r<40; r++)
      {  if (enemigos[r].getActivo()==true)
         { 
            if (enemigos[r].getTipo() != 4 && enemigos[r].getTipo() != 3)
            {
                switch (enemigos[r].getDir()) //Obtener la direccion del enemigo y dibujarlo correspondientemente
                {
                      case 1: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 0, 0, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      case 2: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 70, 0, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      case 3: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 140, 0, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      default:{masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 210, 0, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                }
            }
            else
            {
            
                switch (enemigos[r].getDir()) //Obtener la direccion del enemigo y dibujarlo correspondientemente
                {
                      case 1: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 0, enemigos[r].Animate()*70, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      case 2: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 70, enemigos[r].Animate()*70, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      case 3: {masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 140, enemigos[r].Animate()*70, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                      default:{masked_blit(Enemigo[enemigos[r].getTipo()], buffer, 210, enemigos[r].Animate()*70, enemigos[r].getX()-35, enemigos[r].getY()-35, 70, 70); break;}
                }
            }
            
         }
         masked_blit(Muerte, buffer, enemigos[r].dye()*40, 0, enemigos[r].getX()-20, enemigos[r].getY()-20, 40, 40); //Dibujar la explosion de muerte en caso de que sea necesaria.
         if(key[KEY_SPACE]) //Ayudas para desenvolupar el juego: Vida y Nº de enemigo
         {
             textprintf_ex(buffer, fuentes[0], enemigos[r].getX()-40, enemigos[r].getY()-40, makecol(0, 0, 0), -1, "HP:%i", enemigos[r].getHP());
         }
      }
      
      //Dibujando las barras de vida de los enemigos
      for (int r=0; r<40; r++)
      {  if (enemigos[r].getActivo()==true)
         {
             rectfill(buffer, enemigos[r].getX()-40, enemigos[r].getY()-24, enemigos[r].getX()+40, enemigos[r].getY()-25, makecol (45,45,45));
             rectfill(buffer, enemigos[r].getX()-enemigos[r].getHP()*40/enemigos[r].getMaxHP(), enemigos[r].getY()-24, enemigos[r].getX()+enemigos[r].getHP()*40/enemigos[r].getMaxHP(), enemigos[r].getY()-25, makecol (240,0,0));
         }
      }
      
      //Dibujar la torre seleccionada en el raton
      if (construir==true)
      {
          masked_stretch_blit(tower[torretipo], buffer, 350, 0, 70, 70, mouse_x-(casillaW+10)/2, mouse_y-(casillaW+10)/2, casillaW+10, casillaW+10 );
          circle(buffer, mouse_x, mouse_y, rango[torretipo] ,makecol(255,0,0));
          
      }
      
    //Visualizar detalles de las torres
     for (int r=0; r<nMaxTorres; r++)
      {  if (torre[r].getActiva()==true) //Comprovar que la torre está en uso
         {
             if (mouse_x<torre[r].getCenterX()+casillaW/2 && mouse_x>torre[r].getCenterX()-casillaW/2 && mouse_y>torre[r].getCenterY()-casillaH/2 && mouse_y<torre[r].getCenterY()+casillaH/2)
             {
                 rectfill(buffer, torre[r].getCenterX()-37, torre[r].getCenterY()+15, torre[r].getCenterX()+37, torre[r].getCenterY()+48, makecol (50, 50, 50));
                //Rango    
                circle(buffer, torre[r].getCenterX(), torre[r].getCenterY(), torre[r].getRango(),makecol(255,0,0));
    
                textprintf_ex(buffer, fuentes[0], torre[r].getCenterX()-34, torre[r].getCenterY()+15, makecol(255, 255, 255), -1, "Sell:");
                textprintf_ex(buffer, fuentes[0], torre[r].getCenterX()-34, torre[r].getCenterY()+30, makecol(255, 255, 255), -1, "%i", torre[r].getCoste()/2);
                if (mouse_b==1 && notDestroy>20) //Vender la torre y retornar la mitad del oro
                {
                   torre[r].destroyTower();
                   oro= oro + coste[torre[r].getTipo()]/2;     
                }            
             }                                
         }
      }
      
      if (help==false)
      {
          ////////////////////////////////////////////////////////////////////////
          //VER DETALLES DE LAS TORRES AL PASAR EL RATON POR ENCIMA DE SU IMAGEN//
          ////////////////////////////////////////////////////////////////////////
          if (mouse_y<SCREEN_H*0.882+80 && mouse_y>SCREEN_H*0.882) //Si se pulsa el ratón y se està dentro del rango Y
          {
              if (mouse_x>SCREEN_W/10.23 && mouse_x<SCREEN_W/10.23+casillaW+10)//Tipo 0
              {
                  stretch_sprite(buffer, t[0], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);              
              }
              else if (mouse_x>SCREEN_W/4.96 && mouse_x<SCREEN_W/4.96+casillaW+10)//Tipo 1
              {
                  stretch_sprite(buffer, t[1], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                         
              }
              else if (mouse_x>SCREEN_W/3.34 && mouse_x<SCREEN_W/3.34+casillaW+10)//Tipo 2
              {
                  stretch_sprite(buffer, t[2], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                            
              }
              else if (mouse_x>SCREEN_W/2.47 && mouse_x<SCREEN_W/2.47+casillaW+10 && notDestroy>50)//Tipo 3
              {
                  stretch_sprite(buffer, t[3], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                           
              }
              else if (mouse_x>SCREEN_W/2.0078 && mouse_x<SCREEN_W/2.0078+casillaW+10) //Tipo 4
              {
                  stretch_sprite(buffer, t[4], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);    
              }
          }
          ////////////////////////////////////////////////////////////////////////
          //VER DETALLES DE LOS ENEMIGOS AL PASAR EL RATON POR ENCIMA DE SU IMAGEN//
          ////////////////////////////////////////////////////////////////////////
          if (mouse_y>SCREEN_H/1.19906 -20 && mouse_y<SCREEN_H/1.19906+40) //Si se pulsa el ratón y se està dentro del rango Y
          {
              if (mouse_x>SCREEN_W/3.34 && mouse_x<SCREEN_W/3.34+casillaW)//Tipo 0
              {
                  stretch_sprite(buffer, e[0], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);              
              }
              else if (mouse_x>SCREEN_W/2.75 && mouse_x<SCREEN_W/2.75+casillaW)//Tipo 1
              {
                  stretch_sprite(buffer, e[1], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                         
              }
              else if (mouse_x>SCREEN_W/2.47 && mouse_x<SCREEN_W/2.47+casillaW)//Tipo 2
              {
                  stretch_sprite(buffer, e[2], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                            
              }
              else if (mouse_x>SCREEN_W/2.19 && mouse_x<SCREEN_W/2.19+casillaW && notDestroy>50)//Tipo 3
              {
                  stretch_sprite(buffer, e[4], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);                           
              }
              else if (mouse_x>SCREEN_W/2.0078 && mouse_x<SCREEN_W/2.0078+casillaW) //Tipo 4
              {
                  stretch_sprite(buffer, e[3], SCREEN_W/1.5366, SCREEN_H/1.235223, SCREEN_W/3.93846153, SCREEN_H/6.872483);    
              }
          }
          
      } //Cerrando "if (help==false);
      
      
      
      //LO MISMO PARA LA BASE
      if (mouse_x<base->getCenterX()+casillaW/2 && mouse_x>base->getCenterX()-casillaW/2 && mouse_y>base->getCenterY()-casillaH/2 && mouse_y<base->getCenterY()+casillaH/2)
             {
                //Rango    
                circle(buffer, base->getCenterX(), base->getCenterY(), base->getRango(),makecol(255,0,0));
                //Nivel
                textprintf_ex(buffer, fuentes[0], base->getX()-10, base->getY(), makecol(10, 10, 10), -1, "Lvl:%i", base->getLvl());            
             }
             
             
      notDestroy++; //Tiempo que debe pasar hasta que se pueda vender una torre después de construir una.
      if (construir==true) //No se pueden vender mientras se esté construyendo
      { notDestroy=15; }
            
            
      /////////////////      
      //FIN DEL JUEGO//
      ////////////////
      
      if (vidas<=0)
      {
                   
                   
         //Creación del fichero con datos
          ofstream fichero;
          fichero.open("Puntuaciones.txt", fstream::app);
          fichero << endl << endl;
          fichero << "Oleadas: "<< oleada << endl;
          fichero << "Puntos: "<< enemigos[0].getPuntos() << endl;
          fichero.close();
          
          
         
          for (int e=0; e<40; e++)
          {   if (enemigos[e].getActivo()==true)
              {
                  enemigos[e].hit (100000000, true);
              }
          }
          for (int t=0; t<nMaxTorres; t++)
          {    if (torre[t].getActiva()==true)
               {
                   torre[t].destroyTower();
               }
          }
          play=false;
          help=false;
          oro=1000;
          vidas=15;
          oleada=0;
          oleadaFunc=0;
          p=-420; //Tiempo hasta el proximo enemigo
          n=0;    //Numero del ulimo enemigo spawneado
          numEnemigo=0;
          dificultad=0;
          construir=false;//¿Se tiene seleccionada alguna torre para construirla?
          torretipo=0;  //Tipo de la torre que se construira
          torreN=0;     //Numero de la torre que se debe construir
          base->restart();
          enemigos[0].restartPuntos();
          stop_midi(); 
          play_midi(menuMusic, true); 
          
      }
      ///////////////////////////
      //MENU INICIAL Y AYUDAS////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////
      
      if (play==false)
      {  notDestroy++;
      
              if (mouse_b==1 && mouse_x>=casillaW*4 && mouse_x<=casillaW*9 && mouse_y>=casillaH*8 && mouse_y<=casillaH*10)
              {
                    play=true;
                    stop_midi(); 
                    play_midi(music, true);        
              }
              
              if (help==false)
              {
                      stretch_sprite(buffer, Inicio[0], casillaW*3, casillaH*2, casillaW*19, casillaH*8);
                      if (notDestroy>=20 && mouse_b==1 && mouse_x>=casillaW*16 && mouse_x<=casillaW*21 && mouse_y>=casillaH*8 && mouse_y<=casillaH*10)
                      {
                            help=true;
                            notDestroy=0;         
                      } 
                      //rectfill (buffer, casillaW*4, casillaH*8, casillaW*9, casillaH*10, makecol(100,0,0)); //Play
                      //rectfill (buffer, casillaW*16, casillaH*8, casillaW*21, casillaH*10, makecol(100,0,0)); //Help
              }  
              
              else if (help==true)
              {
                      stretch_sprite(buffer, Inicio[1], casillaW*3, casillaH*2, casillaW*19, casillaH*8);
                      stretch_sprite(buffer, Inicio[2], 0, SCREEN_H*0.8, SCREEN_W, SCREEN_H*0.2);
                      if (notDestroy>=20 && mouse_b==1 && mouse_x>casillaW*16 && mouse_x<casillaW*21 && mouse_y>casillaH*8 && mouse_y<casillaH*10)
                      {
                            help=false; 
                            notDestroy=0;        
                      }      
              }
         
      }
      
      
       //Dibujar el buffer
       stretch_sprite(screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
       //FINAL DE DIBUJAR
       
       
       
       //Limpiar la pantalla
       clear_bitmap(buffer);
               
       //No salir del bucle hasta que se presione "esc"
    } while (!key[KEY_ESC]);
    
    
     //Limpiar la pantalla
       clear_bitmap(buffer);
       
    //Borrar los objetos creados
    stop_midi();
    remove_mouse();
    rest (10);
     delete torre;
     delete enemigos;   
     delete base;      
     
     
    destroy_font (fuentes[0]);
    destroy_font (fuentes[1]);
    
    destroy_bitmap (interficie);
    destroy_bitmap (escenario);
    destroy_bitmap (Cursor);
    destroy_bitmap (Repair);
    destroy_bitmap (Nieve);
    //destroy_bitmap (InterficieBase);
    
    destroy_bitmap (Tipo2);
    destroy_bitmap (Slowb);
    destroy_bitmap (Bullet[0]);
    destroy_bitmap (Bullet[1]);
    destroy_bitmap (Bullet[2]);
    
    destroy_bitmap (Muerte);
     for (int n=0; n<5; n++)
    {
        destroy_bitmap (tower[n]);
        destroy_bitmap (Enemigo[n]);
        destroy_bitmap (t[n]);
        destroy_bitmap (e[n]);
    }
    
    destroy_bitmap (Inicio[0]);
    destroy_bitmap (Inicio[1]);
    destroy_bitmap (Inicio[2]);
    destroy_bitmap (Crear);             
    destroy_bitmap (buffer);
    
    
    destroy_midi(menuMusic);
    destroy_midi(music);
    destroy_sample(Rocketeer);
    destroy_sample(Pulse);
    destroy_sample(Harvester);
    destroy_sample(BaseM);
    destroy_sample(Railgun);
    destroy_sample(Spawn);
    
    
    //destroy_midi(bgMusic);
     
     
     //Salir de alegro
    allegro_exit();    
    //Cerrar el programa
    return 0;     
}

END_OF_MAIN ();  
