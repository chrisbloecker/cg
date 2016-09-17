#ifndef __VECTOR_H__
#define __VECTOR_H__
/**
 * @file
 * Das Modul stellt Funktionalität zur Arbeit mit Vektoren zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include "types.h"

/**
 * Erzeugt einen Vektor und gibt ihn zurück.
 *
 * @param[in] x x-Komponente.
 * @param[in] y y-Komponente.
 * @param[in] z z-Komponente.
 *
 * @return Erzeugter Vektor.
 */
extern Vector3d vectorMake(double x, double y, double z);

/**
 * Berechnet die Länge des Vektors u.
 *
 * @param[in] u Vektor.
 *
 * @return Länge von u.
 */
extern double vectorLength(Vector3d u);

/**
 * Normiert den Vektor v.
 *
 * @param[in] v Vektor, der normiert werden soll.
 *
 * @return normierter Vektor.
 */
extern Vector3d vectorNorm(Vector3d u);

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
extern Vector3d vectorScale(Vector3d u, double sx, double sy, double sz);

/**
 * Berechnet die Summe von u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Summe von u und v, also u + v.
 */
extern Vector3d vectorAdd(Vector3d u, Vector3d v);

/**
 * Berechnet die Differenz zwischen u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Differenz zwischen u und v, also u - v.
 */
extern Vector3d vectorSub(Vector3d u, Vector3d v);

/**
 * Berechnet das Kreuzprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Kreuzprodukt von u und v.
 */
extern Vector3d vectorCross(Vector3d u, Vector3d v);

/**
 * Berechnet das Skalarprodukt der Vektoren u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Skalarprodukt von u und v.
 */
extern double vectorMult(Vector3d u, Vector3d v);

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
extern Vector3d vectorRotateX(Vector3d v, double a);

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
extern Vector3d vectorRotateY(Vector3d v, double a);

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
extern Vector3d vectorRotateZ(Vector3d v, double a);

/**
 * Berechnet den Winkel zwischen den Vektoren u und v im Gradmaß.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Winkel zwischen den Vektoren u und v im Gradmaß.
 */
extern double vectorAngle(Vector3d u, Vector3d v);

#endif
