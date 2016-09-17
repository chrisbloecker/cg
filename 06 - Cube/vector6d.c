/**
 * @file
 * Das Modul stellt Funktionalität zur Arbeit mit Vektoren zur Verfügung.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System-Header einbinden
 * -------------------------------------------------------------------------- */
#include <math.h>
#include <stdio.h>
 
/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "vector6d.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

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
extern Vector6d vector6dMake(double x, double y, double z, double r, double g, double b)
{
  Vector6d res;
  
  res.x = x;
  res.y = y;
  res.z = z;
  res.r = r;
  res.g = g;
  res.b = b;
  
  return res;
}

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u Vektor.
 * @param[in] s Skalar.
 *
 * @return Skalierter Vektor.
 */
extern Vector6d vector6dScale(Vector6d u, double s)
{
  u.x *= s;
  u.y *= s;
  u.z *= s;
  u.r *= s;
  u.g *= s;
  u.b *= s;
  
  return u;
}

/**
 * Berechnet die Summe von u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Summe von u und v, also u + v.
 */
extern Vector6d vector6dAdd(Vector6d u, Vector6d v)
{
  u.x += v.x;
  u.y += v.y;
  u.z += v.z;
  u.r += v.r;
  u.g += v.g;
  u.b += v.b;
  
  return u;
}

/**
 * Gibt den Vektor u auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] u      Vektor, der ausgegeben werden soll.
 */
extern void vector6dPrint(FILE * stream, Vector6d u)
{
  fprintf(stderr, "(%f, %f, %f, %f, %f, %f)", u.x, u.y, u.z, u.r, u.g, u.b);
}
