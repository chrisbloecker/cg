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
#include "vertexCube.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */
 
/** Ein- und ausschaltbare Informationen */
typedef enum {
  LOGIC_SWITCHABLE_CAMUP
, LOGIC_SWITCHABLE_CAMLEFT
, LOGIC_SWITCHABLE_CAMDOWN
, LOGIC_SWITCHABLE_CAMRIGHT
, LOGIC_SWITCHABLE_CAMFORWARD
, LOGIC_SWITCHABLE_CAMBACKWARD
, LOGIC_SWITCHABLE_DUMMY
} LogicSwitchable;

/** Umschaltbare Informationen */
typedef enum {
  LOGIC_TOGGLEABLE_SPHERES
, LOGIC_TOGGLEABLE_EDGES
, LOGIC_TOGGLEABLE_HELP
, LOGIC_TOGGLEABLE_DUMMY
} LogicToggleable;

/** Vektoren */
typedef enum {
  LOGIC_VECTOR_CENTER
, LOGIC_VECTOR_EYE
, LOGIC_VECTOR_VIEW
, LOGIC_VECTOR_CAM
, LOGIC_VECTOR_DUMMY
} LogicVector;

/** Ints */
typedef enum {
  LOGIC_INT_SELECTEDNODE
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
 * Verändert die Feinheit des verwalteten Vertex-Cube um i.
 *
 * @param[in] i Veränderung der Feinheit des verwalteten Vertex-Cubes.
 */
extern void logicSetSubdivides(int i);

/**
 * Gibt den verwalteten Vertex Cube zurück.
 */
extern VertexCube logicGetCube(void);

/**
 * Bewegt die Kamera um v.
 * Dabei geben x/y die vertikale sowie horizontale Auslenkung gegenüber der
 * aktuellen Auslenkung an und z den Zoomfaktor.
 *
 * @param[in] v Veränderung der Kameraeinstellungen.
 */
extern void logicMove(Vector3d v);

/**
 * Verändert den Ausgewählten Vertex um i.
 */
extern void logicSwitchNode(int i);

/**
 * Verändert die Farbe des ausgewählten Vertex.
 */
extern void logicSwitchColor(void);

#endif
