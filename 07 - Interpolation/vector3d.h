#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__
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

/** Ein Vektor im R³ */
typedef struct {
  double x
       , y
       , z;
} Vector3d;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Gibt den Nullvektor zurück.
 *
 * @return Nullvektor.
 */
extern Vector3d vector3dMakeNull(void);

/**
 * Erzeugt einen Vektor und gibt ihn zurück.
 *
 * @param[in] x x-Komponente.
 * @param[in] y y-Komponente.
 * @param[in] z z-Komponente.
 *
 * @return Erzeugter Vektor.
 */
extern Vector3d vector3dMake(double x, double y, double z);

/**
 * Berechnet die Länge des Vektors u.
 *
 * @param[in] u Vektor.
 *
 * @return Länge von u.
 */
extern double vector3dLength(Vector3d u);

/**
 * Normiert den Vektor v.
 *
 * @param[in] v Vektor, der normiert werden soll.
 *
 * @return normierter Vektor.
 */
extern Vector3d vector3dNorm(Vector3d u);

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u Vektor.
 * @param[in] sx Skalar.
 * @param[in] sy Skalar.
 * @param[in] sz Skalar.
 *
 * @return Skalierter Vektor.
 */
extern Vector3d vector3dScale(Vector3d u, double sx, double sy, double sz);

/**
 * Berechnet die Summe von u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Summe von u und v, also u + v.
 */
extern Vector3d vector3dAdd(Vector3d u, Vector3d v);

/**
 * Berechnet die Differenz zwischen u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Differenz zwischen u und v, also u - v.
 */
extern Vector3d vector3dSub(Vector3d u, Vector3d v);

/**
 * Berechnet das Kreuzprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Kreuzprodukt von u und v.
 */
extern Vector3d vector3dCross(Vector3d u, Vector3d v);

/**
 * Berechnet das Skalarprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Skalarprodukt von u und v.
 */
extern double vector3dMult(Vector3d u, Vector3d v);

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
extern Vector3d vector3dRotateX(Vector3d v, double a);

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
extern Vector3d vector3dRotateY(Vector3d v, double a);

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
extern Vector3d vector3dRotateZ(Vector3d v, double a);

/**
 * Berechnet den Winkel zwischen den Vektoren u und v im Gradmaß.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Winkel zwischen den Vektoren u und v im Gradmaß.
 */
extern double vector3dAngle(Vector3d u, Vector3d v);

/**
 * Gibt den Vektor u auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] u      Vektor, der ausgegeben werden soll.
 */
extern void vector3dPrint(FILE * stream, Vector3d u);

#endif
