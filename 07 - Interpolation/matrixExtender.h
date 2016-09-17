#ifndef __MATRIXEXTENDER_H__
#define __MATRIXEXTENDER_H__
/**
 * @file
 *
 *
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "matrix.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Interpolationstypen */
typedef enum {
  MATRIX_BSPLINE
, MATRIX_BEZIER
, MATRIX_HERMITE
, MATRIX_CATROM
, MATRIX_DUMMY
} InterpolationMatrix;

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
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine Interpolationsmatrix für den Interpolationstypen I.
 *
 * @param[in] I Interpolationstyp.
 *
 * @return Matrix, mit der der Interpolationstyp I realisiert werden kann.
 */
extern Matrix matrixMakeInterpolation(InterpolationMatrix I);

/**
 * Gibt die Stringrepräsentation des Interpolationstypen I zurück.
 *
 * @param[in] I Interpolationstyp.
 *
 * @return Stringrepräsentation des Interpolationstypen I.
 */
extern char * matrixInterpolationMatrixToString(InterpolationMatrix I);

#endif
