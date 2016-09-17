/**
 * @file
 * Das Modul stellt Funktionalität zur Arbeit mit Vektoren zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System-Header einbinden
 * -------------------------------------------------------------------------- */
#include <math.h>
 
/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"
#include "vector.h"

/* ----------------------------------------------------------------------------
 * Funktionen
 * -------------------------------------------------------------------------- */

extern Vector2d vectorMake(double x, double y)
{
  Vector2d res;
  
  res.x = x;
  res.y = y;
  
  return res;
}

/**
 * Normiert den Vektor v.
 *
 * @param[in] v Vektor, der normiert werden soll.
 *
 * @return normierter Vektor.
 */
extern Vector2d vectorNorm(Vector2d v)
{
  /* Länge des Richtungsvektors von g */
  double len = sqrt(v.x * v.x + v.y * v.y);
  
  v.x /= len;
  v.y /= len;
  
  return v;
}

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u Vektor.
 * @param[in] v Skalar.
 *
 * @return Skalierter Vektor.
 */
extern Vector2d vectorScale(Vector2d u, double a)
{
  u.x *= a;
  u.y *= a;
  
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
extern Vector2d vectorAdd(Vector2d u, Vector2d v)
{
  u.x += v.x;
  u.y += v.y;
  
  return u;
}

/**
 * Berechnet die Differenz zwischen u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Differenz zwischen u und v, also u - v.
 */
extern Vector2d vectorSub(Vector2d u, Vector2d v)
{
  return vectorAdd(u, vectorScale(v, -1.0));
}

/**
 * Bestimmt einen zu u orthogonalen Vektor.
 *
 * @param[in] u Vektor, zu dem ein orthogonaler bestimmt werden soll.
 *
 * @return zu u orthogonaler Vektor.
 */
extern Vector2d vectorOrtho(Vector2d u)
{
  Vector2d res;
  
  res.x = - u.y;
  res.y =   u.x;
  
  return res;
}

/**
 * Rotiert den Winkel v um den Winkel angle.
 *
 * @param[in] v Vektor, der rotiert werden soll.
 * @param[in] a Rotationswinkel (im Bogenmaß).
 */
extern Vector2d vectorRotate(Vector2d v, double a)
{
  Vector2d res;
  
  double sin_a = sin(a)
       , cos_a = cos(a);
  
  res.x = v.x * cos_a - v.y * sin_a;
  res.y = v.x * sin_a + v.y * cos_a;
  
  return res;
}

/**
 * Berechnet die Länge des Vektors u.
 *
 * @param[in] u Vektor.
 *
 * @return Länge von u.
 */
extern double vectorLength(Vector2d u)
{
  return sqrt(u.x * u.x + u.y * u.y);
}
