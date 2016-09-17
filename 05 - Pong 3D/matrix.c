/**
 * @file
 * Matrix-Modul.
 * Das Modul kapselt einige Matrixberechnungen.
 *
 * Bestandteil eines Beispielprogramms fuer Schatten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2008. All rights reserved.
 */

/* ---- System Header einbinden ---- */

#include <math.h>


/* ---- Eigene Header einbinden ---- */

#include "matrix.h"


/* ---- Makros ---- */

#ifndef M_PI
/** Die mathematische Konstante PI */
#define M_PI 3.14159265358979323846
#endif

#ifndef DEG2RAD
/** Winkelumrechnung von Grad nach Radiant */
#define DEG2RAD(x) ((x)/180.0*M_PI)
/** Winkelumrechnung von Radiant nach Grad */
#define RAD2DEG(x) ((x)/M_PI*180.0)
#endif


/* ---- Konstanten ---- */


/* ---- Globale Daten ---- */


/* ---- Funktionen ---- */


/**
 * Normalisiert den Vektor v.
 * @param v Vektor, der normalisiert werden soll.
 *          Vorbedingung: v ist ein Zeiger auf ein existierenden Vektor.
 *          Nachbedingung: v ist normalisiert.
 */
void
VectorNormalize (CGVector3f * v)
{
  GLfloat len = (GLfloat) sqrt ((*v)[0] * (*v)[0] +
                      (*v)[1] * (*v)[1] +
                      (*v)[2] * (*v)[2]);

  (*v)[0] /= len;
  (*v)[1] /= len;
  (*v)[2] /= len;
}

/**
 * Multipliziert den Vektor v mit der Matrix m.
 * @param m Matrix, mit welcher der Vektor v multipliziert werden soll.
 * @param v Vektor, der mit der Matrix m multipliziert werden soll.
 *          Vorbedingung: v ist ein Zeiger auf ein existierenden Vektor.
 *          Nachbedingung: v wurde mit der Matrix m multipliziert.
 */
void
VectorMatrixMult (CGMatrix16f m, CGVector4f * v)
{
  CGVector4f res = { 0.0f, 0.0f, 0.0f, 0.0f };

  res[0] = m[0] * (*v)[0] + m[4] * (*v)[1] + m[8] * (*v)[2] + m[12] * (*v)[3];
  res[1] = m[1] * (*v)[0] + m[5] * (*v)[1] + m[9] * (*v)[2] + m[13] * (*v)[3];
  res[2] = m[2] * (*v)[0] + m[6] * (*v)[1] + m[10] * (*v)[2] + m[14] * (*v)[3];
  res[3] = m[3] * (*v)[0] + m[7] * (*v)[1] + m[11] * (*v)[2] + m[15] * (*v)[3];

  (*v)[0] = res[0];
  (*v)[1] = res[1];
  (*v)[2] = res[2];
  (*v)[3] = res[3];
}

/**
 * Liefert die Einheitsmatrix.
 * @param m die Einheitsmatrix.
 *          Vorbedingung: m ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m ist die Einheitsmatrix.
 */
void
getIdentity (CGMatrix16f * m)
{
  (*m)[0] = 1; (*m)[4] = 0;  (*m)[8] = 0; (*m)[12] = 0;
  (*m)[1] = 0; (*m)[5] = 1;  (*m)[9] = 0; (*m)[13] = 0;
  (*m)[2] = 0; (*m)[6] = 0; (*m)[10] = 1; (*m)[14] = 0;
  (*m)[3] = 0; (*m)[7] = 0; (*m)[11] = 0; (*m)[15] = 1;
}

/**
 * Multipliziert die Matrix m2 von rechts an die Matrix m1 heran.
 * @param m1 linke Matrix der Multiplikation.
 *          Vorbedingung: m1 ist ein Zeiger auf ein existierende Matrix.
 *          Nachbedingung: m1 enthaelt das Ergebnis der Multiplikation.
 * @param m2 rechte Matrix der Multiplikation.
 */
void
MatrixMatrixMult (CGMatrix16f * m1, CGMatrix16f m2)
{
  CGMatrix16f res;
  unsigned int i = 0;

  res[0] = (*m1)[0] * m2[0] + (*m1)[4] * m2[1] +
           (*m1)[8] * m2[2] + (*m1)[12] * m2[3];
  res[1] = (*m1)[1] * m2[0] + (*m1)[5] * m2[1] +
           (*m1)[9] * m2[2] + (*m1)[13] * m2[3];
  res[2] = (*m1)[2] * m2[0] + (*m1)[6] * m2[1] +
           (*m1)[10] * m2[2] + (*m1)[14] * m2[3];
  res[3] = (*m1)[3] * m2[0] + (*m1)[7] * m2[1] +
           (*m1)[11] * m2[2] + (*m1)[15] * m2[3];

  res[4] = (*m1)[0] * m2[4] + (*m1)[4] * m2[5] +
           (*m1)[8] * m2[6] + (*m1)[12] * m2[7];
  res[5] = (*m1)[1] * m2[4] + (*m1)[5] * m2[5] +
           (*m1)[9] * m2[6] + (*m1)[13] * m2[7];
  res[6] = (*m1)[2] * m2[4] + (*m1)[6] * m2[5] +
           (*m1)[10] * m2[6] + (*m1)[14] * m2[7];
  res[7] = (*m1)[3] * m2[4] + (*m1)[7] * m2[5] +
           (*m1)[11] * m2[6] + (*m1)[15] * m2[7];

  res[8] = (*m1)[0] * m2[8] + (*m1)[4] * m2[9] +
           (*m1)[8] * m2[10] + (*m1)[12] * m2[11];
  res[9] = (*m1)[1] * m2[8] + (*m1)[5] * m2[9] +
           (*m1)[9] * m2[10] + (*m1)[13] * m2[11];
  res[10] = (*m1)[2] * m2[8] + (*m1)[6] * m2[9] +
            (*m1)[10] * m2[10] + (*m1)[14] * m2[11];
  res[11] = (*m1)[3] * m2[8] + (*m1)[7] * m2[9] +
            (*m1)[11] * m2[10] + (*m1)[15] * m2[11];

  res[12] = (*m1)[0] * m2[12] + (*m1)[4] * m2[13] +
            (*m1)[8] * m2[14] + (*m1)[12] * m2[15];
  res[13] = (*m1)[1] * m2[12] + (*m1)[5] * m2[13] +
            (*m1)[9] * m2[14] + (*m1)[13] * m2[15];
  res[14] = (*m1)[2] * m2[12] + (*m1)[6] * m2[13] +
            (*m1)[10] * m2[14] + (*m1)[14] * m2[15];
  res[15] = (*m1)[3] * m2[12] + (*m1)[7] * m2[13] +
            (*m1)[11] * m2[14] + (*m1)[15] * m2[15];

  for (i = 0; i < 16; i++)
    (*m1)[i] = res[i];
}

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
void
Rotate (CGMatrix16f * m, CGVector3f rotAxis, GLfloat angle)
{
  GLfloat c, s, omc, oms; /* cosinus, sinus, 1 - cosinus, 1 - sinus */
  CGMatrix16f mRot;
  CGVector3f v;
  
  v[0] = rotAxis[0];
  v[1] = rotAxis[1];
  v[2] = rotAxis[2];

  /* Cosinus und Sinus einmal berechnen und oefters benutzen */
  c = (GLfloat) cos (DEG2RAD (angle));
  s = (GLfloat) sin (DEG2RAD (angle));
  omc = 1.0f - c;
  oms = 1.0f - s;

  /* Rotationsachse normieren */
  VectorNormalize (&v);

  /* Rotationsmatrix erstellen */
  mRot[0] = v[0] * v[0] * omc + c;
  mRot[4] = v[0] * v[1] * omc - v[2] * s;
  mRot[8] = v[0] * v[2] * omc + v[1] * s;
  mRot[12] = 0.0f;

  mRot[1] = v[1] * v[0] * omc + v[2] * s;
  mRot[5] = v[1] * v[1] * omc + c;
  mRot[9] = v[1] * v[2] * omc - v[0] * s;
  mRot[13] = 0.0f;

  mRot[2] = v[0] * v[2] * omc - v[1] * s;
  mRot[6] = v[1] * v[2] * omc + v[0] * s;
  mRot[10] = v[2] * v[2] * omc + c;
  mRot[14] = 0.0f;

  mRot[3] = 0.0f;
  mRot[7] = 0.0f;
  mRot[11] = 0.0f;
  mRot[15] = 1.0f;

  /* Rotation ausfuehren */
  MatrixMatrixMult (m, mRot);
}

/**
 * Fuehrt eine Translation auf der Matrix m mit dem 
 * Verschiebungsvektor v durch.
 * @param m Matrix, auf der transliert werden soll (In)
 * @param v Verschiebungsvektor in 3D
 * @return Ergebnismatrix in m (Out)
 */
void
Translate (CGMatrix16f * m, CGVector3f v)
{
  CGMatrix16f mTrans;

  /* Translationsmatrix erstellen */
  mTrans[0] = 1.0f;
  mTrans[4] = 0.0f;
  mTrans[8] = 0.0f;
  mTrans[12] = v[0];

  mTrans[1] = 0.0f;
  mTrans[5] = 1.0f;
  mTrans[9] = 0.0f;
  mTrans[13] = v[1];

  mTrans[2] = 0.0f;
  mTrans[6] = 0.0f;
  mTrans[10] = 1.0f;
  mTrans[14] = v[2];

  mTrans[3] = 0.0f;
  mTrans[7] = 0.0f;
  mTrans[11] = 0.0f;
  mTrans[15] = 1.0f;

  /* Translation ausfuehren */
  MatrixMatrixMult (m, mTrans);
}

void Scale (CGMatrix16f * m, CGVector3f v)
{
  CGMatrix16f mScale;

  /* Translationsmatrix erstellen */
  mScale[0] = v[0];
  mScale[4] = 0.0f;
  mScale[8] = 0.0f;
  mScale[12] = 0.0f;

  mScale[1] = 0.0f;
  mScale[5] = v[1];
  mScale[9] = 0.0f;
  mScale[13] = 0.0f;

  mScale[2] = 0.0f;
  mScale[6] = 0.0f;
  mScale[10] = v[2];
  mScale[14] = 0.0f;

  mScale[3] = 0.0f;
  mScale[7] = 0.0f;
  mScale[11] = 0.0f;
  mScale[15] = 1.0f;
  
  /* Translation ausfuehren */
  MatrixMatrixMult (m, mScale);
}
