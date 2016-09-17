#ifndef __OBJECT_CG_H__
#define __OBJECT_CG_H__
/**
 * @file
 * Schnittstelle des Objekt-Moduls.
 * Das Modul kapselt Funktionen, die auf einem Objekt, das aus zahlreichen
 * Dreiecksflaechen besteht, durchgefuehrt werden koennen.
 * Die Funktionen dieses Moduls sind stark inspiriert von der Lesson 27 bei
 * http://nehe.gamedev.net
 *
 * Bestandteil eines Beispielprogramms fuer Schatten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2008. All rights reserved.
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdlib.h>


/* ---- Eigene Header einbinden ---- */
#include "types.h"


/* ---- Makros ---- */

#define EMPTY_CG_OBJECT { 0, 0, NULL, NULL}


/* ---- Typedeklarationen ---- */

/** Ebenen-Gleichung (Koordinatenform). */
typedef struct
{
  /** Erster Koeffizient. */
  GLfloat a;

  /** Zweiter Koeffizient. */
  GLfloat b;

  /** Dritter Koeffizient. */
  GLfloat c;

  /** Vierter Koeffizient. */
  GLfloat d;
} CGPlaneEq;

/** Dreiecksflaeche. */
typedef struct
{
  /** Indizes der Eckpunkte. */
  GLuint pointIdx[3];

  /** Normale der Flaeche */
  CGVector3f normal;

  /**
   * Indizes der Nachbarflaechen.
   * -1 bedeutet keine Nachbarflaeche an der entsprechenden Kante.
   */
  GLint neighIdx[3];

  /** Ebenen-Gleichung */
  CGPlaneEq PlaneEq;

  /* Sichtbarkeits-Flag: ist diese Flaeche dem Licht zu- oder abgewandt? */
  GLboolean visible;
} CGPlane;

/** Objekt. */
typedef struct
{
  /** Anzahl der Eckpunkte. */
  GLuint nPlanes;

  /** Anzahl der Dreiecksflaechen. */
  GLuint nPoints;

  /** Punkte */
  CGVector3f *points;

  /** Flaechen */
  CGPlane *planes;
} CGObject;


/* ---- Funktionsdeklarationen ---- */

/**
 * Initialisiert das Objekt obj als ein kreuzartiges Beispielobjekt. Speicher
 * fuer die Punkte und Flaechen wird allokiert, Nachbarschaftsbeziehungen und
 * Ebenengleichungen der Flaechen werden berechnet.
 * @param obj Objekt, das initialisiert werden soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: obj wurde vollstaendig initialisiert.
 * @return Erfolgsstatus der Initialisierung.
 */
extern GLboolean initObject(CGObject * obj);

/**
 * Gibt den fuer das Objekt obj allokierten Speicher frei.
 * @param obj Objekt, des Speicher freigegeben werden soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: der Speicher fuer obj wurde freigegeben.
 */
void freeObject (CGObject * obj);

/**
 * Zeichnet die Schatten, die das Objekt obj aufgrund der Position lightPos
 * einer Punktlichtquelle wirft. Schatten werden mithilfe von Shadow-Volumes
 * erzeugt.
 * @param obj Objekt, dessen Schatten gezeichnet werden soll.
 *            Vorbedingung: die Sichtbarkeit der Flaechen von der Position der
 *                          Lichtquelle aus ist bereits berechnet.
 * @param lightPos Position der Lichtquelle, relativ zum Nullpunkt des Objekts
 *                 obj (also in Objektkoordinaten).
 */
void castShadow (CGObject obj, CGVector4f lightPos);

/**
 * Berechnet fuer jede Flaeche des Objekts obj, ob diese von der Position pos
 * aus sichtbar sind. Flaechen sind sichtbar, wenn deren Vorderseite der
 * Position zugewandt ist.
 * @param obj Objekt, fuer dessen Flaechen die Sichtbarkeit berechnet werden
 *            soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: die Sichtbarkeit der Flaechen von obj von der
 *                           von der Position pos aus wurde berechnet.
 * @param pos Position, fuer welche die Sichtbarkeit berechnet werden soll.
 */
void calcVisibility (CGObject * obj, CGVector4f pos);

/**
 * Zeichnet das Schattenvolumen, welches das Objekts obj fuer eine
 * Punktlichtquelle mit der Position lightPos erzeugt.
 * @param obj Objekt, dessen Schattenvolumen gezeichnet werden soll.
 *            Vorbedingung: die Sichtbarkeit der Flaechen von der Position der
 *                          Lichtquelle aus ist bereits berechnet.
 * @param lightPos Position der Lichtquelle, relativ zum Nullpunkt des Objekts
 *                 obj (also in Objektkoordinaten).
 */
void drawShadowVolume (CGObject obj, CGVector4f lightPos);

#endif
