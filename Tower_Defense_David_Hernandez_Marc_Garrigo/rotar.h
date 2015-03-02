#ifndef ROTAR
#define ROTAR


# include <math.h>

//Funcion utilizada para obtener la imagen que se debe imprimir en las torres segun la posicion de su objetivo
int rotar (int torreX, int torreY, int objetivoX, int objetivoY)
{
       float angle = atan2(torreY-objetivoY, torreX-objetivoX); //Obtener el angulo entre la torre y el enemigo, a partir de las diferencias de posicion entre estos
       float grados = angle*180/M_PI;                             //Pasar el angulo a grados.
       if ( grados>22.5 && grados<=67.5)
          { return 7; }
       else if (grados>67.5 && grados<=112.5)
          { return 0; }
       else if (grados>112.5 && grados<=157.5)
          { return 1; }
       else if (grados>-22.5 && grados<=22.5)
          { return 6; }
       else if (grados>-67.5 && grados<=-22.5)
          { return 5; }
       else if (grados>-112.5 && grados<=-67.5)
          { return 4; }
       else if (grados>-157.5 && grados<=-112.5)
          { return 3; }
       else 
          { return 2; }
}      //Siempre retorna la imagen que se debe imprimir, siendo 1: arriba y girando en sentido horario

#endif
