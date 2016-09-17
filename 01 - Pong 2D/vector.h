#ifndef __VECTOR_H__
#define __VECTOR_H__
/**
 * @file
 * Das Modul stellt Funktionalität zur Arbeit mit Vektoren zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/**
 * Normiert den Vektor v.
 *
 * @param[in] v Vektor, der normiert werden soll.
 *
 * @return normierter Vektor.
 */
Vector2d vectorNorm(Vector2d v);

/**
 * Skaliert den Vektor u mit a.
 *
 * @param[in] u Vektor.
 * @param[in] v Skalar.
 *
 * @return Skalierter Vektor.
 */
Vector2d vectorScale(Vector2d u, double a);

/**
 * Berechnet die Summe von u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Summe von u und v, also u + v.
 */
Vector2d vectorAdd(Vector2d u, Vector2d v);

/**
 * Berechnet die Differenz zwischen u und v.
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 *
 * @return Differenz zwischen u und v, also u - v.
 */
Vector2d vectorSub(Vector2d u, Vector2d v);

/**
 * Bestimmt einen zu u orthogonalen Vektor.
 *
 * @param[in] u Vektor, zu dem ein orthogonaler bestimmt werden soll.
 *
 * @return zu u orthogonaler Vektor.
 */
Vector2d vectorOrtho(Vector2d u);

/**
 * Berechnet den Parameter, der in h eingesetzt werden muss, um den Schnittpunkt
 * von g und h zu erhalten.
 *
 * @param[in] g Erste Gerade.
 * @param[in] h Zweite Gerade.
 *
 * @return Parameter, der in h eingesetzt werden muss, um den Schnittpunkt von
 *         g und h zu erhalten.
 */
double pointDistance(Line g, Line h);

/**
 * Rotiert den Winkel v um den Winkel angle.
 *
 * @param[in] v Vektor, der rotiert werden soll.
 * @param[in] a Rotationswinkel (im Bogenmaß).
 */
Vector2d vectorRotate(Vector2d v, double a);

#endif
