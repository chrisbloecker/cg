#ifndef __MATRIX_H__
#define __MATRIX_H__
/**
 * @file
 * Schnittstelle des Matrix-Moduls.
 * Das Modul kapselt einige Matrixberechnungen. Die Funktionen sind an die
 * entsprechenden OpenGL-Funktionen angelehnt.
 *
 * Bestandteil eines Beispielprogramms fuer Schatten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2008. All rights reserved.
 */

/* ---- Eigene Header einbinden ---- */
#include "types.h"

/**
 * Normalisiert den Vektor v.
 * @param v Vektor, der normalisiert werden soll.
 *          Vorbedingung: v ist ein Zeiger auf ein existierenden Vektor.
 *          Nachbedingung: v ist normalisiert.
 */
void VectorNormalize (CGVector3f * v);

/**
 * Multipliziert den Vektor v mit der Matrix m.
 * @param m Matrix, mit welcher der Vektor v multipliziert werden soll.
 * @param v Vektor, der mit der Matrix m multipliziert werden soll.
 *          Vorbedingung: v ist ein Zeiger auf ein existierenden Vektor.
 *          Nachbedingung: v wurde mit der Matrix m multipliziert.
 */
void VectorMatrixMult (CGMatrix16f m, CGVector4f * v);

/**
 * Liefert die Einheitsmatrix.
 * @param m die Einheitsmatrix.
 *          Vorbedingung: m ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m ist die Einheitsmatrix.
 */
void getIdentity (CGMatrix16f * m);

/**
 * Multipliziert die Matrix m2 von rechts an die Matrix m1 heran.
 * @param m1 linke Matrix der Multiplikation.
 *          Vorbedingung: m1 ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m1 enthaelt das Ergebnis der Multiplikation.
 * @param m2 rechte Matrix der Multiplikation.
 */
void MatrixMatrixMult (CGMatrix16f * m1, CGMatrix16f m2);

/**
 * Multipliziert die Rotationsmatrix der Rotation mit dem Winkel angle um die
 * Achse v von rechts an die Matrix m heran.
 * @param m Matrix, an welche die Rotationsmatrix heran multipliziert werden
 *          soll.
 *          Vorbedingung: m ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m enthaelt das Ergebnis der Multiplikation.
 * @param v Rotationsachse.
 * @param angle Rotationswinkel.
 */
void Rotate (CGMatrix16f * m, CGVector3f v, GLfloat angle);

/**
 * Multipliziert die Translationsmatrix der Translation um den Vektor v von
 * rechts an die Matrix m heran.
 * @param m Matrix, an welche die Translationsmatrix heran multipliziert werden
 *          soll.
 *          Vorbedingung: m ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m enthaelt das Ergebnis der Multiplikation.
 * @param v Translationsvektor.
 */
void Translate (CGMatrix16f * m, CGVector3f v);

/**
 * Multipliziert die Translationsmatrix der Translation um den Vektor v von
 * rechts an die Matrix m heran.
 * @param m Matrix, an welche die Translationsmatrix heran multipliziert werden
 *          soll.
 *          Vorbedingung: m ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m enthaelt das Ergebnis der Multiplikation.
 * @param v Translationsvektor.
 */
void Scale (CGMatrix16f * m, CGVector3f v);

#endif
