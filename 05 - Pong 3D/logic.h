#ifndef __LOGIC_H__
#define __LOGIC_H__
/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include "types.h"
#include "object.h"

/** Umschaltbare Informationen */
typedef enum {
  LOGIC_SWITCHABLE_LIGHT0
, LOGIC_SWITCHABLE_LIGHT1
, LOGIC_SWITCHABLE_CHEAT
, LOGIC_SWITCHABLE_PAUSE
, LOGIC_SWITCHABLE_CUBEROTATION
, LOGIC_SWITCHABLE_RACKETUP
, LOGIC_SWITCHABLE_RACKETDOWN
, LOGIC_SWITCHABLE_RACKETLEFT
, LOGIC_SWITCHABLE_RACKETRIGHT
, LOGIC_SWITCHABLE_CAMUP
, LOGIC_SWITCHABLE_CAMLEFT
, LOGIC_SWITCHABLE_CAMDOWN
, LOGIC_SWITCHABLE_CAMRIGHT
, LOGIC_SWITCHABLE_CAMFORWARD
, LOGIC_SWITCHABLE_CAMBACKWARD
, LOGIC_SWITCHABLE_LIGHTFORWARD
, LOGIC_SWITCHABLE_LIGHTBACKWARD
, LOGIC_SWITCHABLE_LIGHTUP
, LOGIC_SWITCHABLE_LIGHTDOWN
, LOGIC_SWITCHABLE_LIGHTLEFT
, LOGIC_SWITCHABLE_LIGHTRIGHT
, LOGIC_SWITCHABLE_SHADOWS
, LOGIC_SWITCHABLE_SHADOWVOLUMES
, LOGIC_SWITCHABLE_DUMMY
} LogicSwitchable;

/** Vektoren */
typedef enum {
  LOGIC_VECTOR_CENTER
, LOGIC_VECTOR_EYE
, LOGIC_VECTOR_CAM
, LOGIC_VECTOR_N
, LOGIC_VECTOR_DUMMY
} LogicVector;

/** Integers */
typedef enum {
  LOGIC_INT_LIFES
, LOGIC_INT_POINTS
, LOGIC_INT_DUMMY
} LogicInt;

/** Objekte */
typedef enum {
  LOGIC_OBJECT_BALL
, LOGIC_OBJECT_WALLLEFT
, LOGIC_OBJECT_WALLRIGHT
, LOGIC_OBJECT_WALLCENTER
, LOGIC_OBJECT_WALLFRONT
, LOGIC_OBJECT_GROUND
, LOGIC_OBJECT_LIGHT
, LOGIC_OBJECT_RACKET
, LOGIC_OBJECT_CUBE0
, LOGIC_OBJECT_CUBE1
, LOGIC_OBJECT_DUMMY
} LogicObject;

/** Objektsammlungen */
typedef enum {
  LOGIC_OBJECTS_LEVEL
, LOGIC_OBJECTS_SHADOWABLEBYLIGHT
, LOGIC_OBJECTS_SHADOWABLEBYBALL
, LOGIC_OBJECTS_DUMMY
} LogicObjects;

/**
 * Initialisiert das Spiel und sagt "1".
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
 * Gibt die zum Objekttypen t zugeordnete Zeichenfunktion zurück.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t gehörende Zeichenfunktion.
 */
extern DrawFunction logicGetDrawFunction(DrawFunctionType t);

/**
 * Schaltet s um.
 *
 * @param[in] s Gibt an, was umgeschaltet werden soll.
 */
extern void logicSwitch(LogicSwitchable s);

/**
 * Gibt den Wert von s zurück.
 *
 * @param[in] s
 *
 * @return Wert von s.
 */
extern Boolean logicGetSwitchable(LogicSwitchable s);

/**
 * Gibt den Vektor v zurück.
 *
 * @param[in] v
 *
 * @return Wert von v.
 */
extern Vector3d logicGetVector(LogicVector v);

/**
 * Gibt den Wert von i zurück.
 *
 * @param[in] i
 *
 * @return Wert von i.
 */
extern int logicGetInt(LogicInt i);

/**
 * Gibt einen Zeiger auf das Objekt o zurück.
 *
 * @param[in] o
 *
 * @return Zeiger auf o.
 */
extern Object * logicGetObject(LogicObject o);

/**
 * Gibt einen Zeiger auf die Objekte os zurück.
 *
 * @param[in] os
 *
 * @return Zeiger auf os.
 */
extern Objects * logicGetObjects(LogicObjects os);

#endif
