#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
/**
 * @file
 *
 * Modul für die Geometrie. Hier werden die Punkte, Vertices und Normalen 
 * verwaltet.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "vertex.h"
#include "vector3d.h"
#include "matrixExtender.h"
#include "types.h"
#include "list.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Zeichenmodi */
typedef enum {
  DRAWMODE_INTERPOLATION
, DRAWMODE_CONVEXHULL
, DRAWMODE_DUMMY
} DrawMode;

/** Punktlöschmodi */
typedef enum {
  REMOVE_LAST
, REMOVE_SELECTED
, REMOVE_DUMMY
} RemovePoints;

/** Struktur einer Geometrie */
typedef struct {
  unsigned n             /* Anzahl der Punkte         */
         , subDiv        /* Interpolationsgenauigkeit */
         , nCH
         ;
  Vector3d * vectors;    /* Punkte                    */
  List selected;         /* Selected Nodes            */
  Vertex * vertices;     /* Interpolierte             */
  GLuint * indices       /* Indizes                   */
       , * normals
       ;
  InterpolationMatrix i; /* Interpolationsmatrix      */
  DrawMode d;            /* Zeichenmodus              */
} Geometry;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine neue Geometrie und gibt sie zurück.
 *
 * @return Erzeugte Geometrie.
 */
extern Geometry geometryInit(void);

/**
 * Löscht die Geometrie g.
 *
 * @param[in] g Zu löschende Geometrie.
 *
 * @return Gelöschte Geometrie.
 */
extern Geometry geometryDelete(Geometry g);

/**
 * Erzeugt einen zufälligen Punkt, der sich rechts vom letzten Punkt der 
 * Geometrie-Punkte befindet und fügt ihn am rechten Ende ein.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit neu erzeugtem Punkt.
 */
extern Geometry geometryAddPoint(Geometry g);

/**
 * Fügt an der Position v einen neuen Punkt in die Geometrie g ein.
 *
 * @param[in] g Geometrie.
 * @param[in] v Position, an der der neue Punkt eingefügt werden soll.
 *
 * @return Geometrie mit neu eingefügtem Punkt.
 */
extern Geometry geometryAddPointAt(Geometry g, Vector3d v);

/**
 * Verändert die Interpolationsfeinheit.
 *
 * @param[in] g Geometrie.
 * @param[in] i Neue Interpolationsfeinheit.
 *
 * @return Geometrie mit angepasster Interpolationsfeinheit, alle Daten
 *         (Interpolationpolynom, Normalen, konvexe Hülle) wurden bereits neu
 *         berechnet.
 */
extern Geometry geometrySetSubdivides(Geometry g, unsigned i);

/**
 * Wechselt die Interpolationsmethode.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie, die die "nächste" Interpolationsmethode benutzt.
 */
extern Geometry geometrySwitchInterpolation(Geometry g);

/**
 * Zeichnet die Punkte der Geometrie.
 *
 * @param[in] g Geometrie.
 */
extern void geometryDrawPoints(Geometry g);

/**
 * Wählt den Punkt an der Koordinate v in g aus.
 *
 * @param[in] g     Geometrie.
 * @param[in] v     Position, an der ein Punkt ausgewählt werden soll (falls es 
 *                  dort einen gibt).
 * @param[in] singe Gibt an, ob nur ein Punkt ausgewählt werden soll, oder ob
 *                  ein weiterer zur bisherigen Auswahl hinzuzufügen ist.
 *
 * @return Geometrie mit angepassten ausgewählen Punkten.
 */
extern Geometry geometrySelectNodeAt(Geometry g, Vector3d v, Boolean single);

/** 
 * Bewegt die in g ausgewählten Punkte um den Vektor v.
 *
 * @param[in] g Geometrie.
 * @param[in] v Translation, die auf die in g ausgewählten Punkte angewendet 
 *              werden soll.
 *
 * @return Geometrie mit verschobenen Punkten.
 */
extern Geometry geometryMoveSelectedNodes(Geometry g, Vector3d v);

/**
 * Entfernt Punkte aus g. r gibt dabei an, ob alle ausgewählten Punkte oder nur
 * der zuletzt hinzugefügte gelöscht werden sollen. Die initial vorhandenen
 * Punkte bleiben immer vorhanden und können nicht gelöscht werden.
 *
 * @param[in] g Geometrie.
 * @param[in] r Lösch-Modus.
 *
 * @return Geometrie "mit gelöschten Punkten".
 */
extern Geometry geometryRemovePoints(Geometry g, RemovePoints r);

/**
 * Wechselt den Zeichenmodus von g. Mögliche Zeichenmodi sind Interpolation und
 * Konvexe Hülle.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit gewechseltem Zeichenmodus.
 */
extern Geometry geometrySwitchDrawMode(Geometry g);

#endif
