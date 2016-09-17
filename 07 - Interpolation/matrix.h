#ifndef __MATRIX_H__
#define __MATRIX_H__
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
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Matrizen */
typedef struct {
  unsigned dimX
         , dimY
         ;
  double * m;
} Matrix;

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
 * Erzeugt eine Nullmatrix der Größe dimX * dimY.
 *
 * @param[in] dimX X-Dimension.
 * @param[in] dimY Y-Dimension.
 *
 * @return Nullmatrix, die die Dimension dimX * dimY besitzt.
 */
extern Matrix matrixMake(unsigned dimX, unsigned dimY);

/**
 * Erzeugt die leere Matrix.
 *
 * @return Die leere Matrix.
 */
extern Matrix matrixMakeEmpty(void);

/**
 * Löscht die Matrix M, indem der belegte Speicher freigegeben wird.
 *
 * @param[in] M Zu löschende Matrix.
 *
 * @return Die leere Matrix.
 */
extern Matrix matrixDelete(Matrix M);

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
extern double matrixAt(Matrix M, unsigned x, unsigned y);

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
extern Matrix matrixSet(Matrix M, unsigned x, unsigned y, double v);

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
extern Boolean matrixIsEqual(Matrix M, Matrix N);

/**
 * Kopiert die Matrix M und gibt die Kopie zurück.
 *
 * @param[in] M Matrix, die kopiert werden soll.
 *
 * @return Kopie von M.
 */
extern Matrix matrixCopy(Matrix M);

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
extern Matrix matrixMultiply(Matrix M, Matrix N);

/**
 * Gibt die Matrix M auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] M      Auszugebende Matrix.
 */
extern void matrixPrint(FILE * stream, Matrix M);

#endif
