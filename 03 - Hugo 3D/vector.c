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
 * Macros
 * -------------------------------------------------------------------------- */

#define PI (3.1415926535897932384626433832795029)
#define DEGTORAD(x) ((x)*(PI)/(180))
#define RADTODEG(x) ((180)*(x)/(PI))

/* ----------------------------------------------------------------------------
 * Funktionen
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt einen Vektor und gibt ihn zurück.
 *
 * @param[in] x x-Komponente.
 * @param[in] y y-Komponente.
 * @param[in] z z-Komponente.
 *
 * @return Erzeugter Vektor.
 */
extern Vector3d vectorMake(double x, double y, double z)
{
  Vector3d res;
  
  res.x = x;
  res.y = y;
  res.z = z;
  
  return res;
}

/**
 * Berechnet die Länge des Vektors u.
 *
 * @param[in] u Vektor.
 *
 * @return Länge von u.
 */
extern double vectorLength(Vector3d u)
{
  return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

/**
 * Normiert den Vektor v.
 *
 * @param[in] v Vektor, der normiert werden soll.
 *
 * @return normierter Vektor.
 */
extern Vector3d vectorNorm(Vector3d u)
{
  double len = vectorLength(u);
  
  u.x /= len;
  u.y /= len;
  u.z /= len;
  
  return u;
}

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u  Vektor.
 * @param[in] sx Skalar.
 * @param[in] sy Skalar.
 * @param[in] sz Skalar.
 *
 * @return Skalierter Vektor.
 */
extern Vector3d vectorScale(Vector3d u, double sx, double sy, double sz)
{
  u.x *= sx;
  u.y *= sy;
  u.z *= sz;
  
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
extern Vector3d vectorAdd(Vector3d u, Vector3d v)
{
  u.x += v.x;
  u.y += v.y;
  u.z += v.z;
  
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
extern Vector3d vectorSub(Vector3d u, Vector3d v)
{
  u.x -= v.x;
  u.y -= v.y;
  u.z -= v.z;
  
  return u;
}

/**
 * Berechnet das Kreuzprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Kreuzprodukt von u und v.
 */
extern Vector3d vectorCross(Vector3d u, Vector3d v)
{
  Vector3d res;
  
  res.x = u.y * v.z - u.z * v.y;
  res.y = u.z * v.x - u.x * v.z;
  res.z = u.x * v.y - u.y * v.x;
  
  return res;
}

/**
 * Berechnet das Skalarprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Skalarprodukt von u und v.
 */
extern double vectorMult(Vector3d u, Vector3d v)
{
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

/**
 * Rotiert den Vektor u um den Wikel a um die x-Achse.
 *
 * Rotationsmatrix:
 *
 * | 1   0    0 |
 * | 0 cos -sin |
 * | 0 sin  cos |
 *
 * @param[in] u Vektor.
 * @param[in] a Rotationswinkel im Gradmaß.
 *
 * @return Vektor, der durch die Rotation von u um den Winkel a um die x-Achse
 *         entsteht.
 */
extern Vector3d vectorRotateX(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vectorMake(u.x, cos(a) * u.y - sin(a) * u.z, sin(a) * u.y + cos(a) * u.z);
}

/**
 * Rotiert den Vektor u um den Wikel a um die y-Achse.
 *
 * Rotationsmatrix:
 *
 * |  cos 0  sin |
 * |    0 1    0 |
 * | -sin 0  cos |
 *
 * @param[in] u Vektor.
 * @param[in] a Rotationswinkel im Gradmaß.
 *
 * @return Vektor, der durch die Rotation von u um den Winkel a um die y-Achse
 *         entsteht.
 */
extern Vector3d vectorRotateY(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vectorMake(cos(a) * u.x + sin(a) * u.z, u.y, cos(a) * u.z - sin(a) * u.x);
}

/**
 * Rotiert den Vektor u um den Wikel a um die z-Achse.
 *
 * Rotationsmatrix:
 *
 * | cos -sin 0 |
 * | sin  cos 0 |
 * | 0      0 1 |
 *
 * @param[in] u Vektor.
 * @param[in] a Rotationswinkel im Gradmaß.
 *
 * @return Vektor, der durch die Rotation von u um den Winkel a um die z-Achse
 *         entsteht.
 */
extern Vector3d vectorRotateZ(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vectorMake(cos(a) * u.x - sin(a) * u.y, sin(a) * u.x + cos(a) * u.y , u.z);
}

/**
 * Berechnet den Winkel zwischen den Vektoren u und v im Gradmaß.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Winkel zwischen den Vektoren u und v im Gradmaß.
 */
extern double vectorAngle(Vector3d u, Vector3d v)
{
  return RADTODEG(vectorMult(u, v) / (vectorLength(u) * vectorLength(v)));
}
