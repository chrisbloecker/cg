/**
 * @file
 *
 * Matrix-Modul.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <float.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "matrix.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Prüft, ob die Matrix M leer ist.
 * Dies ist der Fall, wenn M in x- sowie y-Richtung die Dimension 0 hat und der
 * Zeiger auf die Daten NULL ist.
 *
 * @param[in] M Matrix, die auf "Leerheit" geprüft werden soll.
 *
 * @return 1 Wenn M leer ist
 *         0 Sons.
 */
static Boolean matrixIsEmpty(Matrix M)
{
  return M.dimX == 0
      && M.dimY == 0
      && !M.m
      ;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine Nullmatrix der Größe dimX * dimY.
 *
 * @param[in] dimX X-Dimension.
 * @param[in] dimY Y-Dimension.
 *
 * @return Nullmatrix, die die Dimension dimX * dimY besitzt.
 */
extern Matrix matrixMake(unsigned dimX, unsigned dimY)
{
  Matrix res;
  
  res.dimX = dimX;
  res.dimY = dimY;
  
  res.m = calloc(dimX * dimY, sizeof(double));
  
  if (!res.m) 
    perror("Error allocating memory in matrixMake.\n");
  
  return res;
}

/**
 * Erzeugt die leere Matrix.
 *
 * @return Die leere Matrix.
 */
extern Matrix matrixMakeEmpty(void)
{
  Matrix res;
  
  res.dimX = 0;
  res.dimY = 0;
  
  res.m = NULL;
  
  return res;
}

/**
 * Löscht die Matrix M, indem der belegte Speicher freigegeben wird.
 *
 * @param[in] M Zu löschende Matrix.
 *
 * @return Die leere Matrix.
 */
extern Matrix matrixDelete(Matrix M)
{
  if (M.m)
    free(M.m);
   
  M.m    = NULL;
  M.dimX = 0;
  M.dimY = 0;
  
  return M;
}

/**
 * Gibt den Wert von M an der Position M[x,y] zurück.
 *
 * @param[in] M Matrix.
 * @param[in] x Spalte.
 * @param[in] y Zeile.
 *
 * @pre x darf die Ausdehnung von M in x-Richtung nicht überschreiten.
 * @pre y darf die Ausdehnung von M in y-Richtung nicht überschreiten.
 *
 * @return Wert von M an der Position M[x,y].
 */
extern double matrixAt(Matrix M, unsigned x, unsigned y)
{
  assert(x < M.dimX);
  assert(y < M.dimY);
  
  return M.m[x + y * M.dimX];
}

/**
 * Setzt den Wert von M an der Position M[x,y] auf v.
 *
 * @param[in] M Matrix.
 * @param[in] x Spalte.
 * @param[in] y Zeile.
 * @param[in] v Wert.
 *
 * @pre x darf die Ausdehnung von M in x-Richtung nicht überschreiten.
 * @pre y darf die Ausdehnung von M in y-Richtung nicht überschreiten.
 *
 * @return Modifizierte Matrix M.
 */
extern Matrix matrixSet(Matrix M, unsigned x, unsigned y, double v)
{
  assert(x < M.dimX);
  assert(y < M.dimY);
  
  M.m[x + y * M.dimX] = v;
  
  return M;
}

/**
 * Prüft ob die Matrizen M und N gleich sind.
 * Dies ist der Fall, wenn M und N dieselben Ausdehnungen haben und an allen
 * Positionen übereinstimmen.
 *
 * @param[in] M Erste Matrix.
 * @param[in] N Zweite Matrix.
 *
 * @return 1 Wenn M und N gleich sind
 *         0 Sonst.
 */
extern Boolean matrixIsEqual(Matrix M, Matrix N)
{
  Boolean equal = TRUE;
  
  unsigned i = 0;
  
  /* Dimensionen müssen übereinstimmen */
  equal &= M.dimX == N.dimX && M.dimY == N.dimY;
  
  /* Alle Werte müssen übereinstimmen */
  for (i = 0; i < M.dimX * M.dimY && equal; ++i)
    equal &= fabs(M.m[i] - N.m[i]) < DBL_EPSILON;
  
  return equal;
}

/**
 * Kopiert die Matrix M und gibt die Kopie zurück.
 *
 * @param[in] M Matrix, die kopiert werden soll.
 *
 * @return Kopie von M.
 */
extern Matrix matrixCopy(Matrix M)
{
  Matrix res;
  
  if (matrixIsEmpty(M))
    res = matrixMakeEmpty();
  else
  {
    unsigned i;
    
    res = matrixMake(M.dimX, M.dimY);
    
    for (i = 0; i < M.dimX * M.dimY; ++i)
      res.m[i] = M.m[i];
  }
  
  return res;
}

/**
 * Berechnet das Ergebnis der Multiplikation M * N und gibt es zurück.
 *
 * @param[in] M Erste Matrix.
 * @param[in] N Zweite Matrix.
 *
 * @pre Die Dimension von M in x- und die Dimension von N in y- Richtung müssen
 *      übereinstimmen.
 *
 * @return Das Ergebnis der Multiplikation M * N.
 */
extern Matrix matrixMultiply(Matrix M, Matrix N)
{
  Matrix res;
  
  unsigned x
         , y
         , z
         ;
  
  double tmp;
  
  assert(M.dimX == N.dimY);
  
  res = matrixMake(N.dimX, M.dimY);
  
  for (x = 0; x < res.dimX; ++x)
    for (y = 0; y < res.dimY; ++y)
    {
      tmp = 0.0;
      
      for (z = 0; z < M.dimX; ++z)
        tmp += matrixAt(M, z, y) * matrixAt(N, x, z);
      
      res = matrixSet(res, x, y, tmp);
    }
  
  return res;
}

/**
 * Gibt die Matrix M auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] M      Auszugebende Matrix.
 */
extern void matrixPrint(FILE * stream, Matrix M)
{
  unsigned x
         , y
         ;
  
  for (y = 0; y < M.dimY; ++y)
  {
    for (x = 0; x < M.dimX; ++x)
      fprintf(stream, "%f ", matrixAt(M, x, y));
    fprintf(stream, "\n");
  }
}
