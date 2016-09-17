/**
 * @file
 *
 * Definiert Erweiterungen für Matrizen.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <assert.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "matrixExtender.h"

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

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine Interpolationsmatrix für den Interpolationstypen I.
 *
 * @param[in] I Interpolationstyp.
 *
 * @return Matrix, mit der der Interpolationstyp I realisiert werden kann.
 */
extern Matrix matrixMakeInterpolation(InterpolationMatrix I)
{
  Matrix M = matrixMake(4, 4);
  
  switch (I)
  {
    case MATRIX_BSPLINE:
      M = matrixSet(M, 0, 0, - 1.0 / 6);
      M = matrixSet(M, 1, 0,   3.0 / 6);
      M = matrixSet(M, 2, 0, - 3.0 / 6);
      M = matrixSet(M, 3, 0,   1.0 / 6);
      
      M = matrixSet(M, 0, 1,   3.0 / 6);
      M = matrixSet(M, 1, 1, - 6.0 / 6);
      M = matrixSet(M, 2, 1,   3.0 / 6);
      M = matrixSet(M, 3, 1,   0.0 / 6);
      
      M = matrixSet(M, 0, 2, - 3.0 / 6);
      M = matrixSet(M, 1, 2,   0.0 / 6);
      M = matrixSet(M, 2, 2,   3.0 / 6);
      M = matrixSet(M, 3, 2,   0.0 / 6);
      
      M = matrixSet(M, 0, 3,   1.0 / 6);
      M = matrixSet(M, 1, 3,   4.0 / 6);
      M = matrixSet(M, 2, 3,   1.0 / 6);
      M = matrixSet(M, 3, 3,   0.0 / 6);
      break;
    case MATRIX_BEZIER:
      M = matrixSet(M, 0, 0, - 1.0);
      M = matrixSet(M, 1, 0,   3.0);
      M = matrixSet(M, 2, 0, - 3.0);
      M = matrixSet(M, 3, 0,   1.0);
      
      M = matrixSet(M, 0, 1,   3.0);
      M = matrixSet(M, 1, 1, - 6.0);
      M = matrixSet(M, 2, 1,   3.0);
      M = matrixSet(M, 3, 1,   0.0);
      
      M = matrixSet(M, 0, 2, - 3.0);
      M = matrixSet(M, 1, 2,   3.0);
      M = matrixSet(M, 2, 2,   0.0);
      M = matrixSet(M, 3, 2,   0.0);
      
      M = matrixSet(M, 0, 3,   1.0);
      M = matrixSet(M, 1, 3,   0.0);
      M = matrixSet(M, 2, 3,   0.0);
      M = matrixSet(M, 3, 3,   0.0);
      break;
    case MATRIX_HERMITE:
      M = matrixSet(M, 0, 0,   2.0);
      M = matrixSet(M, 1, 0, - 2.0);
      M = matrixSet(M, 2, 0,   1.0);
      M = matrixSet(M, 3, 0,   1.0);
      
      M = matrixSet(M, 0, 1, - 3.0);
      M = matrixSet(M, 1, 1,   3.0);
      M = matrixSet(M, 2, 1, - 2.0);
      M = matrixSet(M, 3, 1, - 1.0);
      
      M = matrixSet(M, 0, 2,   0.0);
      M = matrixSet(M, 1, 2,   0.0);
      M = matrixSet(M, 2, 2,   1.0);
      M = matrixSet(M, 3, 2,   0.0);
      
      M = matrixSet(M, 0, 3,   1.0);
      M = matrixSet(M, 1, 3,   0.0);
      M = matrixSet(M, 2, 3,   0.0);
      M = matrixSet(M, 3, 3,   0.0);
      break;
    case MATRIX_CATROM:
      M = matrixSet(M, 0, 0, - 1.0 / 2);
      M = matrixSet(M, 1, 0,   3.0 / 2);
      M = matrixSet(M, 2, 0, - 3.0 / 2);
      M = matrixSet(M, 3, 0,   1.0 / 2);
      
      M = matrixSet(M, 0, 1,   2.0 / 2);
      M = matrixSet(M, 1, 1, - 5.0 / 2);
      M = matrixSet(M, 2, 1,   4.0 / 2);
      M = matrixSet(M, 3, 1, - 1.0 / 2);
      
      M = matrixSet(M, 0, 2, - 1.0 / 2);
      M = matrixSet(M, 1, 2,   0.0 / 2);
      M = matrixSet(M, 2, 2,   1.0 / 2);
      M = matrixSet(M, 3, 2,   0.0 / 2);
      
      M = matrixSet(M, 0, 3,   0.0 / 2);
      M = matrixSet(M, 1, 3,   2.0 / 2);
      M = matrixSet(M, 2, 3,   0.0 / 2);
      M = matrixSet(M, 3, 3,   0.0 / 2);
      break;
    default:
      assert(0);
  }
  
  return M;
}

/**
 * Gibt die Stringrepräsentation des Interpolationstypen I zurück.
 *
 * @param[in] I Interpolationstyp.
 *
 * @return Stringrepräsentation des Interpolationstypen I.
 */
extern char * matrixInterpolationMatrixToString(InterpolationMatrix I)
{
  char * res;
  
  switch (I)
  {
    case MATRIX_BSPLINE:
      res = "BSpline";
      break;
    case MATRIX_BEZIER:
      res = "Bezier";
      break;
    case MATRIX_HERMITE:
      res = "Hermite";
      break;
    case MATRIX_CATROM:
      res = "CatRom";
      break;
    default:
      assert(0);
  }
  
  return res;
}
