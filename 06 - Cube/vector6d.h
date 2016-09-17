#ifndef __VECTOR6D_H__
#define __VECTOR6D_H__
/**
 * @file
 * Das Modul stellt Funktionalität zur Arbeit mit Vektoren zur Verfügung.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System-Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Ein Vektor im RxRxRxRxRxR */
typedef struct {
  double x
       , y
       , z
       , r
       , g
       , b
       ;
} Vector6d;

/* ----------------------------------------------------------------------------
 * Funktionen
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt einen Vektor und gibt ihn zurück.
 *
 * @param[in] x x-Komponente.
 * @param[in] y y-Komponente.
 * @param[in] z z-Komponente.
 * @param[in] r r-Komponente.
 * @param[in] g g-Komponente.
 * @param[in] b b-Komponente.
 *
 * @return Erzeugter Vektor.
 */
extern Vector6d vector6dMake(double x, double y, double z, double r, double g, double b);

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u Vektor.
 * @param[in] s Skalar.
 *
 * @return Skalierter Vektor.
 */
extern Vector6d vector6dScale(Vector6d u, double s);

/**
 * Berechnet die Summe von u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Summe von u und v, also u + v.
 */
extern Vector6d vector6dAdd(Vector6d u, Vector6d v);

/**
 * Gibt den Vektor u auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] u      Vektor, der ausgegeben werden soll.
 */
extern void vector6dPrint(FILE * stream, Vector6d u);

#endif
