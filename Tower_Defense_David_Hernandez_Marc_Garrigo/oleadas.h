#ifndef OLEADAS
#define OLEADAS



char oleadas[15][15]=
{
  { 0 ,'o','o','o','o','o','o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 0 ,'o','o','o','o','o','o','o','o','o','o','o','o'},
  { 1 , 1 ,'o','o','o','o','o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 1 , 1 ,'o','o','o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 0 , 1 , 1 , 1 ,'o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 0 , 0 , 4 , 4 ,'o','o','o','o','o','o','o','o','o'},
  { 1 , 1 , 1 , 1 , 1 , 1 , 1 ,'o','o','o','o','o','o','o','o'},
  { 4 , 2 , 4 ,'o','o','o','o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'o','o','o','o','o','o','o'},
  { 0 , 0 , 0 , 0 , 3 , 1 , 1 ,'o','o','o','o','o','o','o','o'},
  { 3 , 0 , 0 , 0 , 0 , 3 ,'o','o','o','o','o','o','o','o','o'},
  { 0 , 0 , 4 , 0 , 0 , 4 , 0 , 0 , 4 , 1 , 1 , 1 , 1 , 1 ,'o'},
  { 4 , 3 , 4 , 0 , 0 , 3 , 3 ,'o','o','o','o','o','o','o','o'},
  { 0 , 0 , 0 , 2 , 3 , 3 , 1 , 1 , 1 ,'o','o','o','o','o','o'},
  { 2 , 3 , 4 , 4 , 4 , 3 , 4 , 4 , 4 , 3 , 4 , 4 , 4 ,'o','o'},
};


int tipoEnemigo(int _oleada, int _numEnemigo)
{
  return oleadas[_oleada][_numEnemigo];
}

END_OF_FUNCTION(tipoEnemigo)


#endif
