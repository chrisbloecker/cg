/**
 * @file
 *
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "logic.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define LOGIC_CAM_ANGLE_PS  (60.0)
#define LOGIC_CAM_ZOOM_PS   ( 1.5)

#define LOGIC_SUBDIVIDES_MIN (  1)
#define LOGIC_SUBDIVIDES_MAX (100)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

#define SIGNUM(x) (((x) > (0)) ? (+1) : (-1))
#define TOBOUNDS(x, b) (((fabs(x)) > (b)) ? ((SIGNUM(x)) * (b)) : (x))
#define TOINTERVAL(v, min, max) (((v) < (min)) ? (min) : ((v) > (max)) ? (max) : (v))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Umschaltbare Informationen */
static Boolean switchable[LOGIC_SWITCHABLE_DUMMY] =
  { FALSE /* ??? */
  };

static Boolean toggleable[LOGIC_TOGGLEABLE_DUMMY] =
  { FALSE /* HELP       */
  , FALSE /* NORMALS    */
  , FALSE /* CONVEXHULL */
  };

/* Vektoren */
static Vector3d vector[LOGIC_VECTOR_DUMMY] =
  {
    { 0.0, 0.0, 0.0 } /* ??? */
  };

static int ints[LOGIC_INT_DUMMY] =
  { 0 /* SUBDIVIDES */
  };

static Geometry g;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert das Spiel und sagt "1".
 *
 * @return 1.
 */
extern int logicInit(void)
{
  srand(time(NULL));
  
  g = geometryInit();
  
  return 1;
}

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval)
{
}

/**
 * Schaltet s um.
 *
 * @param[in] s Gibt an, was umgeschaltet werden soll.
 */
extern void logicSwitch(LogicSwitchable s)
{
  assert(s < LOGIC_SWITCHABLE_DUMMY);
  
  switchable[s] = !switchable[s];
}

/**
 * Schaltet t um.
 *
 * @param[in] t Gibt an, was umgeschaltet werden soll.
 */
extern void logicToggle(LogicToggleable t)
{
  assert(t < LOGIC_TOGGLEABLE_DUMMY);
  
  toggleable[t] = !toggleable[t];
}

/**
 * Gibt den Wert von s zurück.
 *
 * @param[in] s Gefragte Information.
 *
 * @return Wert von s.
 */
extern Boolean logicGetSwitchable(LogicSwitchable s)
{
  assert(s < LOGIC_SWITCHABLE_DUMMY);
  
  return switchable[s];
}

/**
 * Gibt den Wert von t zurück.
 *
 * @param[in] t Gefragte Information.
 *
 * @return Wert von t.
 */
extern Boolean logicGetToggleable(LogicToggleable t)
{
  assert(t < LOGIC_TOGGLEABLE_DUMMY);
  
  return toggleable[t];
}

/**
 * Gibt den Wert von i zurück.
 *
 * @param[in] i Gefragte Information.
 *
 * @return Wert von i.
 */
extern Boolean logicGetInt(LogicInt i)
{
  assert(i < LOGIC_INT_DUMMY);
  
  return ints[i];
}

/**
 * Gibt den Vektor v zurück.
 *
 * @param[in] v Gefragter Vektor.
 *
 * @return Wert von v.
 */
extern Vector3d logicGetVector(LogicVector v)
{
  assert(v < LOGIC_VECTOR_DUMMY);
  
  return vector[v];
}

/**
 * Verändert die Interpolationsfeinheit des verwalteten Geometrie.
 *
 * @param[in] i Veränderung der Interpolationsfeinheit.
 */
extern void logicSetSubdivides(int i)
{
  if ((int) g.subDiv + i >= LOGIC_SUBDIVIDES_MIN
   && (int) g.subDiv + i <= LOGIC_SUBDIVIDES_MAX)
  {
    #ifdef DEBUG
    fprintf(stderr, "DEBUG :: Setting interpolation steps to %i\n", g.subDiv + i);
    #endif
    
    g = geometrySetSubdivides(g, g.subDiv + i);
  }
}

/**
 * Räumt auf, indem der von der Geometrie verwendete Speicher freigegeben wird.
 */
extern void logicCleanUp(void)
{
  g = geometryDelete(g);
}

/**
 * Gibt die verwaltete Geometrie zurück.
 */
extern Geometry logicGetGeometry(void)
{
  return g;
}

/**
 * Wechselt die Interpolationsmethode der verwalteten Geometrie.
 */
extern void logicSwitchInterpolation(void)
{
  g = geometrySwitchInterpolation(g);
}

/**
 * Fügt einen Punkt an der Position v in die verwaltete Geometrie ein.
 *
 * @param[in] v Position.
 */
extern void logicAddPointAt(Vector3d v)
{
  g = geometryAddPointAt(g, v);
}

/**
 * Wählt einen Punkt an der Position v in der verwalteten Geometrie aus.
 *
 * @param[in] v      Position.
 * @param[in] single Gibt an, ob nur ein Punkt ausgewählt werden soll,
 *                   oder ob Punkte zur bisherigen Auswahl hinzugefügt werden
 *                   sollen
 */
extern void logicSelectNodeAt(Vector3d v, Boolean single)
{
  g = geometrySelectNodeAt(g, v, single);
}

/**
 * Bewegt die in der verwalteten Geometrie ausgewählten Punkte um den Vektor v.
 *
 * @param[in] v Translation für ausgewählte Punkte.
 */
extern void logicMoveSelectedNodes(Vector3d v)
{
  g = geometryMoveSelectedNodes(g, v);
}

/**
 * Löscht die in der verwalteten Geometrie ausgewählten Punkte.
 *
 * @param[in] r Löschtmodus.
 */
extern void logicRemovePoints(RemovePoints r)
{
  g = geometryRemovePoints(g, r);
}

/**
 * Fügt der verwalteten Geometrie einen weiteren Punkt hinzu.
 */
extern void logicAddPoint(void)
{
  g = geometryAddPoint(g);
}

/**
 * Wechselt den Zeichenmodus der verwalteten Geometrie.
 */
extern void logicSwitchDrawMode(void)
{
  logicToggle(LOGIC_TOGGLEABLE_CONVEXHULL);
  
  g = geometrySwitchDrawMode(g);
}
