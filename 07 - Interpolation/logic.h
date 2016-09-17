#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System-Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"
#include "vector3d.h"
#include "geometry.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */
 
/** Ein- und ausschaltbare Informationen */
typedef enum {
  LOGIC_SWITCHABLE_UNKNOWN
, LOGIC_SWITCHABLE_DUMMY
} LogicSwitchable;

/** Umschaltbare Informationen */
typedef enum {
  LOGIC_TOGGLEABLE_HELP
, LOGIC_TOGGLEABLE_NORMALS
, LOGIC_TOGGLEABLE_CONVEXHULL
, LOGIC_TOGGLEABLE_DUMMY
} LogicToggleable;

/** Vektoren */
typedef enum {
  LOGIC_VECTOR_UNKNOWN
, LOGIC_VECTOR_DUMMY
} LogicVector;

/** Ints */
typedef enum {
  LOGIC_INT_UNKNOWN
, LOGIC_INT_DUMMY
} LogicInt;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert die Logik und sagt "1".
 *
 * @return 1.
 */
extern int logicInit(void);

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval);

/**
 * Schaltet s um.
 *
 * @param[in] s Gibt an, was umgeschaltet werden soll.
 */
extern void logicSwitch(LogicSwitchable s);

/**
 * Schaltet t um.
 *
 * @param[in] t Gibt an, was umgeschaltet werden soll.
 */
extern void logicToggle(LogicToggleable t);

/**
 * Gibt den Wert von s zurück.
 *
 * @param[in] s Gefragte Information.
 *
 * @return Wert von s.
 */
extern Boolean logicGetSwitchable(LogicSwitchable s);

/**
 * Gibt den Wert von t zurück.
 *
 * @param[in] t Gefragte Information.
 *
 * @return Wert von t.
 */
extern Boolean logicGetToggleable(LogicToggleable t);

/**
 * Gibt den Vektor v zurück.
 *
 * @param[in] v Gefragter Vektor.
 *
 * @return Wert von v.
 */
extern Vector3d logicGetVector(LogicVector v);

/**
 * Gibt den Wert von i zurück.
 *
 * @param[in] i Gefragte Information.
 *
 * @return Wert von i.
 */
extern Boolean logicGetInt(LogicInt i);

/**
 * Verändert die Interpolationsfeinheit des verwalteten Geometrie.
 *
 * @param[in] i Veränderung der Interpolationsfeinheit.
 */
extern void logicSetSubdivides(int i);

/**
 * Räumt auf, indem der von der Geometrie verwendete Speicher freigegeben wird.
 */
extern void logicCleanUp(void);

/**
 * Gibt die verwaltete Geometrie zurück.
 */
extern Geometry logicGetGeometry(void);

/**
 * Wechselt die Interpolationsmethode der verwalteten Geometrie.
 */
extern void logicSwitchInterpolation(void);

/**
 * Fügt einen Punkt an der Position v in die verwaltete Geometrie ein.
 *
 * @param[in] v Position.
 */
extern void logicAddPointAt(Vector3d v);

/**
 * Wählt einen Punkt an der Position v in der verwalteten Geometrie aus.
 *
 * @param[in] v      Position.
 * @param[in] single Gibt an, ob nur ein Punkt ausgewählt werden soll,
 *                   oder ob Punkte zur bisherigen Auswahl hinzugefügt werden
 *                   sollen
 */
extern void logicSelectNodeAt(Vector3d v, Boolean single);

/**
 * Bewegt die in der verwalteten Geometrie ausgewählten Punkte um den Vektor v.
 *
 * @param[in] v Translation für ausgewählte Punkte.
 */
extern void logicMoveSelectedNodes(Vector3d v);

/**
 * Löscht die in der verwalteten Geometrie ausgewählten Punkte.
 *
 * @param[in] r Löschtmodus.
 */
extern void logicRemovePoints(RemovePoints r);

/**
 * Fügt der verwalteten Geometrie einen weiteren Punkt hinzu.
 */
extern void logicAddPoint(void);

/**
 * Wechselt den Zeichenmodus der verwalteten Geometrie.
 */
extern void logicSwitchDrawMode(void);

#endif
