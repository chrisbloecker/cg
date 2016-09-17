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
#include "vector3d.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

#define PI (3.1415926535897932384626433832795029)
#define DEGTORAD(x) ((x)*(PI)/(180))
#define RADTODEG(x) ((180)*(x)/(PI))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/** Der Nullvektor */
static Vector3d vectorNull = { 0.0, 0.0, 0.0 };

/* ----------------------------------------------------------------------------
 * Funktionen
 * -------------------------------------------------------------------------- */

/**
 * Gibt den Nullvektor zurück.
 *
 * @return Nullvektor.
 */
extern Vector3d vector3dMakeNull(void)
{
  return vectorNull;
}

/**
 * Erzeugt einen Vektor und gibt ihn zurück.
 *
 * @param[in] x x-Komponente.
 * @param[in] y y-Komponente.
 * @param[in] z z-Komponente.
 *
 * @return Erzeugter Vektor.
 */
extern Vector3d vector3dMake(double x, double y, double z)
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
extern double vector3dLength(Vector3d u)
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
extern Vector3d vector3dNorm(Vector3d u)
{
  double len = vector3dLength(u);
  
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
extern Vector3d vector3dScale(Vector3d u, double sx, double sy, double sz)
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
extern Vector3d vector3dAdd(Vector3d u, Vector3d v)
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
extern Vector3d vector3dSub(Vector3d u, Vector3d v)
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
extern Vector3d vector3dCross(Vector3d u, Vector3d v)
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
extern double vector3dMult(Vector3d u, Vector3d v)
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
extern Vector3d vector3dRotateX(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vector3dMake(u.x, cos(a) * u.y - sin(a) * u.z, sin(a) * u.y + cos(a) * u.z);
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
extern Vector3d vector3dRotateY(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vector3dMake(cos(a) * u.x + sin(a) * u.z, u.y, cos(a) * u.z - sin(a) * u.x);
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
extern Vector3d vector3dRotateZ(Vector3d u, double a)
{
  a = DEGTORAD(a);
  
  return vector3dMake(cos(a) * u.x - sin(a) * u.y, sin(a) * u.x + cos(a) * u.y , u.z);
}

/**
 * Berechnet den Winkel zwischen den Vektoren u und v im Gradmaß.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Winkel zwischen den Vektoren u und v im Gradmaß.
 */
extern double vector3dAngle(Vector3d u, Vector3d v)
{
  return RADTODEG(acos(vector3dMult(u, v) / (vector3dLength(u) * vector3dLength(v))));
}

/**
 * Gibt den Vektor u auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] u      Vektor, der ausgegeben werden soll.
 */
extern void vector3dPrint(FILE * stream, Vector3d u)
{
  fprintf(stderr, "(%f, %f, %f)", u.x, u.y, u.z);
}
