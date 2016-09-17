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
#include <math.h>
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

#define LOGIC_SUBDIVIDES_MIN (0)
#define LOGIC_SUBDIVIDES_MAX (5)

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
  { FALSE /* CAMUP       */
  , FALSE /* CAMLEFT     */
  , FALSE /* CAMDOWN     */
  , FALSE /* CAMRIGHT    */
  , FALSE /* CAMFORWARD  */
  , FALSE /* CAMBACKWARD */
  };

static Boolean toggleable[LOGIC_TOGGLEABLE_DUMMY] =
  { TRUE  /* SPHERES */
  , TRUE  /* PLANES  */
  , FALSE /* HELP    */
  };

/* Vektoren */
static Vector3d vector[LOGIC_VECTOR_DUMMY] =
  {
    { 0.0, 0.0, 0.0 } /* CENTER       */
  , { 0.0, 1.0, 1.0 } /* EYE          */
  , { 0.0, 0.0, 0.0 } /* N            */
  , { 0.0, 0.0, 1.5 } /* CAM          */
  };

static int ints[LOGIC_INT_DUMMY] =
  { 0 /* SELECTEDNODE */
  };

static VertexCube cube;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Berechnet die Position der Kamera.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcCams(double interval)
{
  logicMove(vector3dMake( switchable[LOGIC_SWITCHABLE_CAMRIGHT] - switchable[LOGIC_SWITCHABLE_CAMLEFT]
                        , 0
                        , 0
                        )
           );
  
  /* Augpunkt aus Kamerabewegung berechnen */
  vector[LOGIC_VECTOR_EYE] 
    = vector3dScale(vector3dRotateY( vector3dRotateX(vector3dMake(0.0, 0.0, - 1.0)
                                                    , vector[LOGIC_VECTOR_CAM].y
                                                    )
                                   , vector[LOGIC_VECTOR_CAM].x
                                   )
                   , vector[LOGIC_VECTOR_CAM].z
                   , vector[LOGIC_VECTOR_CAM].z
                   , vector[LOGIC_VECTOR_CAM].z
                   );
}

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
  Vector3d * eye    = &vector[LOGIC_VECTOR_EYE]
         , * center = &vector[LOGIC_VECTOR_CENTER]
         , * view   = &vector[LOGIC_VECTOR_VIEW]
         ;
  
  srand(time(NULL));
  
  /* Würfel initialisieren */
  cube = vertexCubeInit(cube);
  
  /* Blickrichtung normieren */
  *view = vector3dNorm(vector3dSub(*center, *eye));
  
  return 1;
}

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval)
{
  static Vector3d * eye    = &vector[LOGIC_VECTOR_EYE]
                , * center = &vector[LOGIC_VECTOR_CENTER]
                , * n      = &vector[LOGIC_VECTOR_VIEW]
                ;
  
  /* Kameraposition berechnen */
  calcCams(interval);
    
  /* Blickrichtung normieren */
  *n = vector3dNorm(vector3dSub(*center, *eye));
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
  return vector[v];
}

/**
 * Verändert die Feinheit des verwalteten Vertex-Cube um i.
 *
 * @param[in] i Veränderung der Feinheit des verwalteten Vertex-Cubes.
 */
extern void logicSetSubdivides(int i)
{
  if (cube.s + i >= LOGIC_SUBDIVIDES_MIN
   && cube.s + i <= LOGIC_SUBDIVIDES_MAX)
  {
    cube = vertexCubeMake(cube, cube.s + i);
    ints[LOGIC_INT_SELECTEDNODE] = 0;
  }
}

/**
 * Gibt den verwalteten Vertex Cube zurück.
 */
extern VertexCube logicGetCube(void)
{
  return cube;
}

/**
 * Bewegt die Kamera um v.
 * Dabei geben x/y die vertikale sowie horizontale Auslenkung gegenüber der
 * aktuellen Auslenkung an und z den Zoomfaktor.
 *
 * @param[in] v Veränderung der Kameraeinstellungen.
 */
extern void logicMove(Vector3d v)
{
  static Vector3d * cam = &vector[LOGIC_VECTOR_CAM];
  
  *cam = vector3dAdd(*cam, v);
  
  cam->x = TOBOUNDS(cam->x, 360.0);
  cam->y = TOBOUNDS(cam->y,  90.0);
}

/**
 * Verändert den Ausgewählten Vertex um i.
 */
extern void logicSwitchNode(int i)
{
  static int * selected = &ints[LOGIC_INT_SELECTEDNODE];
  
  *selected = (*selected + i) % vertexCubeNumOfNodes(cube.s);
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Selected node is %i.\n", *selected);
  #endif
}

/**
 * Verändert die Farbe des ausgewählten Vertex.
 */
extern void logicSwitchColor(void)
{
  cube = vertexCubeSwitchColor(cube, ints[LOGIC_INT_SELECTEDNODE]);
}
