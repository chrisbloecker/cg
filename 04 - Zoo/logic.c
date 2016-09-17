/**
 * @file
 *
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <math.h>
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "logic.h"
#include "drawing.h"
#include "displaylist.h"
#include "vector.h"
#include "level.h"
#include "texture.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define LOGIC_CAM_ANGLE_PS  (90.0)
#define LOGIC_CAM_HEIGHT_PS (2.5)
#define LOGIC_CAM_ZOOM_PS   (1.0)

#define LOGIC_ANIM_ANGLE_PS (180)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* Signumfunktion */
#define SIGNUM(x) (((x) > (0)) ? (+1) : (-1))

/* Holt den Wert x in die Grenzen [-b, +b] zurück, GEFÄHRLICHES MACRO! */
#define TOBOUNDS(x, b) (((fabs(x)) > (b)) ? ((SIGNUM(x)) * (b)) : (x))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Der "Level" */
static Level l;

/* Zuordnung der Zeichenfunktionen zu den Levels */
static DrawFunction functionsDrawing[DF_DUMMY] = 
  { drawGround       /* Ground        */
  , drawSocket       /* Socket        */
  , drawFence        /* Fence         */
  , drawFenceCeiling /* Fence Ceiling */
  , drawPool         /* Pool          */
  , drawWater        /* Water         */
  , drawPig          /* Pig           */
  , drawGiraffe      /* Giraffe       */
  , drawGiraffeHead  /* Giraffe Head  */
  , drawFish         /* Fish          */
  , drawSun          /* Sun           */
  , drawGiraffeCube  /* Giraffe Cube  */
  };

/* Bewegung der Kamera */
static Boolean camMovement[CAM_DUMMY] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

static double camRotation = 0.0 /* Aulenkung der Kamera */
            , camHeight   = 0.0 /* Höhe der Kamera      */
            , camZoom     = 1.0 /* Zoom der Kamera      */
            ;

static Boolean light[1]   = {TRUE} /* Einschaltstatus der Lichtquelle(n)      */
             , ignoreMove = TRUE   /* Nächstes Mausbewegungs-Event ignorieren */
             ;

static Vector3d center      = {0.0, 0.0, 0.0}  /* Zentrum der zene   */
              , eye         = {0.0, 1.0, 1.0}  /* Augpunkt           */
              , cam         = {0.0, 1.0, 1.8}  /* Kameraposition     */
              , n                              /* Blickrichtung      */
              , sizeGiraffe = {0.1, 0.0, 0.15} /* Größe der Giraffe  */
              , sizePig     = {0.1, 0.0, 0.15} /* Größe des Schweins */
              , sizeFish    = {0.1, 0.0, 0.05} /* Größe des Fisches  */
              ;

/* Levelgegenstände */
static LevelItem itemGround       /* Grasboden            */
               , itemSocket       /* Steinsockel          */
               , itemCeiling      /* Decke                */
               , itemFenceFront   /* Vorderes "Gitter"    */
               , itemFenceBack    /* Hinteres "Gitter"    */
               , itemFenceLeft    /* Linkes   "Gitter"    */
               , itemFenceRight   /* Rechtes  "Gitter"    */
               , itemPool         /* Becken für den Fisch */
               , itemWater        /* Wasser "im" Becken   */
               , itemPig          /* Das Schwein          */
               , itemGiraffe      /* Die Giraffe          */
               , itemGiraffeHead  /* Der Giraffenkopf     */
               , itemFish         /* Der Fisch            */
               , itemSun          /* Die Sonne            */
               , itemGiraffeCube  /* Giraffenwürfel       */
               ;
/* Objekt, das beim Bewegen der Maus verschoben werden soll */
static LevelItem * itemMove = (void *) 0;

/* Enthält die animierbaren Levelobjekte */
static Animation animBuf[ANIMATION_DUMMY];

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Bestimmt, ob die durch p gegebene Position sichtbar ist.
 * Dies ist der Fall, wenn sie sich innerhalb der y-Grenzen befindet.
 *
 * @param[in] p Position.
 *
 * @return true  wenn p sichtbar ist
 *         false sonst.
 */
Boolean isVisible(Vector3d p)
{
  return TRUE;
}

/**
 * Berechnet die Position der Kamera und rotiert die Birne des Leuchtturms.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcCams(double interval)
{
  /* Kamerabewegung */
  if (camMovement[CAM_UP])
    camHeight += interval * LOGIC_CAM_HEIGHT_PS;
  if (camMovement[CAM_DOWN])
    camHeight -= interval * LOGIC_CAM_HEIGHT_PS;
  if (camMovement[CAM_LEFT])
    camRotation -= interval * LOGIC_CAM_ANGLE_PS;
  if (camMovement[CAM_RIGHT])
    camRotation += interval * LOGIC_CAM_ANGLE_PS;
  if (camMovement[CAM_FORWARD])
    camZoom -= interval * LOGIC_CAM_ZOOM_PS;
  if (camMovement[CAM_BACKWARD])
    camZoom += interval * LOGIC_CAM_ZOOM_PS;
  
  /* Rotationswinkel "kleinhalten" */
  while (camRotation < 0.0)
    camRotation += 360.0;
  
  while (camRotation > 360.0)
    camRotation -= 360.0;
  
  /* Augpunkt aus Kamerabewegung berechnen */
  eye = vectorRotateY( vectorScale( vectorMake(cam.x, cam.y + camHeight, cam.z)
                                  , camZoom
                                  , camZoom
                                  , camZoom
                                  )
                     , camRotation
                     );
}

/**
 * Initialisiert den Animationsbuffer.
 */
static void initAnimBuf(void)
{
  AnimationName a;
  
  /* Standardwerte für jedes Objekt setzen */
  for (a = ANIMATION_GIRAFFE; a < ANIMATION_DUMMY; ++a)
  {
    animBuf[a].isAnimated = FALSE;
    animBuf[a].stop       = FALSE;
    animBuf[a].animMin    =   0.0;
    animBuf[a].animMax    = 360.0;
  }
  
  /* Zeiger auf die zu verändernden Objekteigenschaften setzen */
  animBuf[ANIMATION_GIRAFFE].animProperty = &itemGiraffeHead.a;
  animBuf[ANIMATION_PIG].animProperty     = &itemPig.a;
  animBuf[ANIMATION_FISH].animProperty    = &itemFish.a;
}

/**
 * Prüft, ob sich das Rechteck, das durch v und size bestimmt ist, im Pool 
 * befindet.
 *
 * @param[in] v    Mittelpunkt des Rechtecks.
 * @param[in] size Ausdehnung  des Rechtecks.
 *
 * @return true  wenn sich v im Pool befindet
 *         false sonst.
 */
static Boolean inPool(Vector3d v, Vector3d size)
{
  #define POOL_X  (1.0 )
  #define POOL_Z1 (1.0 )
  #define POOL_Z2 (0.2 )
  
  return fabs(v.x) + size.x <= POOL_X
      &&      v.z  + size.z <= POOL_Z1
      &&      v.z  - size.z >= POOL_Z2
      ;
  
  #undef POOL_WALL_X
  #undef POOL_WALL_Z1
  #undef POOL_WALL_Z2
}

/**
 * Prüft, ob sich das Rechteck, das durch v und size bestimmt ist, im Käfig 
 * befindet.
 *
 * @param[in] v    Mittelpunkt des Rechtecks.
 * @param[in] size Ausdehnung  des Rechtecks.
 *
 * @return true  wenn sich v im Käfig befindet
 *         false sonst.
 */
static Boolean inCage(Vector3d v, Vector3d size)
{
  #define CAGE_X  (  1.0)
  #define CAGE_Z1 (- 1.0)
  #define CAGE_Z2 (  0.2)
  
  return fabs(v.x) + size.x <= CAGE_X
      &&      v.z  + size.z <= CAGE_Z2
      &&      v.z  - size.z >= CAGE_Z1
      ;
  
  #undef CAGE_X
  #undef CAGE_Z1
  #undef CAGE_Z2
}

/**
 * Prüft, ob sich Giraffe und Schwein so weit voneinander entfernt befinden,
 * dass sie eine leere Schnittmenge bilden.
 *
 * @param[in] giraffe     Mittelpunkt der Giraffe.
 * @param[in] pig         Mittelpunkt des Schweins.
 * @param[in] sizeGiraffe Mittelpunkt der Giraffe.
 * @param[in] sizePig     Mittelpunkt der Giraffe. 
 *
 * @return true  wenn sich v im Pool befindet
 *         false sonst.
 */
static Boolean distanceGiraffePig(Vector3d giraffe, Vector3d pig, Vector3d sizeGiraffe, Vector3d sizePig)
{
  /* Giraffe geschnitten Schwein sei die Leere Menge */
  return fabs(giraffe.x - pig.x) > sizeGiraffe.x + sizePig.x
      || fabs(giraffe.z - pig.z) > sizeGiraffe.z + sizePig.z
      ;
}

/**
 * Prüft ob der Giraffenkopf mit dem Schwein oder dem Boden kollidiert.
 *
 * @param[in] giraffeHead Position des Kopfes der Giraffe.
 * @param[in] pig         Position des Schweins.
 * @param[in] angle       Winkel, um den der Kopf der Giraffe gedreht werden soll.
 *
 * @return true  wenn keine Kollision stattgefunden hat
 *         false sonst.
 */
static Boolean collisionGiraffeHead(Vector3d giraffeHead, Vector3d pig, double angle)
{
  /* Giraffenkopfposition, die durch die Rotation entsteht */
  Vector3d g = vectorRotateX(vectorMake(0, giraffeHead.y, 0), angle);
  
  /* Distand des Giraffenkopfes vom Schwein */
  double d1 = vectorLength(vectorSub(vectorAdd(giraffeHead, g), pig));
  
  /* Distand des Giraffenkopfes vom Schweinekopf */
  double d2 = vectorLength(vectorSub(vectorAdd(giraffeHead, g), vectorAdd(pig, vectorMake(0.0, 0.1, 0.1))));
  
  return g.y < - 0.2
      || d1  <   0.2
      || d2  <   0.2
      ;
}

/**
 * Animiert die Giraffe.
 *
 * @param[in] giraffe  Zeiger auf die Animationsinformationen.
 * @param[in] interval Zeit seit letztem Aufruf.
 */
static void animateGiraffe(Animation * giraffe, double interval)
{
  /* giraffe != NULL */
  assert(giraffe);
  
  /**
   * Vorwärtsanimation, wenn die Giraffe nicht durch eine Kollision gestoppt 
   * wurde
   */
  if (!giraffe->stop)
  {
    /* neuer Rotationswinkel für den Kopf */
    double newAngle = *giraffe->animProperty + interval * LOGIC_ANIM_ANGLE_PS;
    
    /* Wenn der Kopf kollidiert, dann die Animation umkehren */
    if (collisionGiraffeHead(itemGiraffeHead.t, itemPig.t, newAngle))
    {
      giraffe->stop = TRUE;
    }
    /* Sonst neuen Winkel setzen */
    else
    {
      *giraffe->animProperty = newAngle;
    }
  }
  /* Wenn die Animation wegen Kollision gestoppt wurde */
  else
  {
    /* "Rückwärtsanimation", Kopf wieder heben */
    *giraffe->animProperty -= interval * LOGIC_ANIM_ANGLE_PS;
    
    /* Wenn der Kopf wieder die Ausgangslage erreicht hat, Animation beenden */
    if (*giraffe->animProperty < giraffe->animMin)
    {
       giraffe->isAnimated   = FALSE;
       giraffe->stop         = FALSE;
      *giraffe->animProperty = giraffe->animMin;
    }
  }
}

/**
 * Animiert das Schwein.
 *
 * @param[in] pig      Zeiger auf die Animationsinformationen.
 * @param[in] interval Zeit seit letztem Aufruf.
 */
static void animatePig(Animation * pig, double interval)
{
  /* pig != NULL */
  assert(pig);
  
  /**
   * Das Schwein befinde sich weit genug von der Wand des Käfigs entfernt, um
   * sich nicht zu zersäbeln, außerdem sei es noch nicht vollständig rotiert.
   */
  if (fabs(itemPig.t.x) > 0.8 || *pig->animProperty > pig->animMax)
  {
     pig->isAnimated   = FALSE;
    *pig->animProperty = pig->animMin;
  }
  else
  {
    *pig->animProperty += interval * LOGIC_ANIM_ANGLE_PS;
  }
}

/**
 * Animiert den Fisch.
 *
 * @param[in] fish     Zeiger auf die Animationsinformationen.
 * @param[in] interval Zeit seit letztem Aufruf.
 */
static void animateFish(Animation * fish, double interval)
{
  #define ANIMATION_RADIUS (0.25)
  
  /* fish != NULL */
  assert(fish);
  
  {
    Vector3d 
    /* alte Verschiebung des Fisches, berechnet mit altem Rotationswinkel */
    old     = vectorRotateY(vectorMake(0.0, 0.0, ANIMATION_RADIUS), *fish->animProperty)
    
    /* neue Verschiebung des Fisches, berechnet mit neuem Rotationswinkel */
  , new     = vectorRotateY( vectorMake(0.0, 0.0, ANIMATION_RADIUS)
                           , *fish->animProperty + interval * LOGIC_ANIM_ANGLE_PS)
  
    /* neue Position des Fisches */
  , fishPos = vectorAdd(vectorSub(itemFish.t, old), new)
  ;
    
    /**
     * Fisch animieren, wenn er noch nicht vollständig rotiert ist und wenn
     * sich die resultierende Position innerhalb des Pools befindet.
     */
    if (*fish->animProperty < fish->animMax && inPool(fishPos, sizeFish))
    {
       itemFish.t = fishPos;
      *fish->animProperty += interval * LOGIC_ANIM_ANGLE_PS;
    }
    /* Sonst Animation stoppen */
    else
    {
       fish->isAnimated   = FALSE;
      *fish->animProperty = fish->animMin;
    }
  }
  
  #undef ANIMATION_RADIUS
}

/**
 * Animationen durchführen.
 *
 * @param[in] Zeit seit letztem Aufruf.
 */
static void calcAnimation(double interval)
{
  if (animBuf[ANIMATION_GIRAFFE].isAnimated)
    animateGiraffe(animBuf + ANIMATION_GIRAFFE, interval);
  
  if (animBuf[ANIMATION_PIG].isAnimated)
    animatePig(animBuf + ANIMATION_PIG, interval);
  
  if (animBuf[ANIMATION_FISH].isAnimated)
    animateFish(animBuf + ANIMATION_FISH, interval);
  
}

/**
 * Objekte des Levels erstellen und einfügen.
 */
static void initItems(void)
{
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Initializing Level.\n");
  #endif
  
  l = levelInit();
  
  itemGround = levelItemMake( vectorMake(0.0, 0.0, 0.0) /* p */
                            , vectorMake(0.0, 0.0, 0.0) /* r */
                            , vectorMake(0.0, 0.0, 0.0) /* t */
                            , DF_GROUND                 /* f */
                            , 0                         /* a */
                            , NAME_GROUND               /* n */
                            , 0                         /* z */
                            );
  
  itemSocket = levelItemMake( vectorMake(0.0, 0.1, -0.4) /* p */
                            , vectorMake(0.0, 0.0, 0.0) /* r */
                            , vectorMake(0.0, 0.1, -0.4) /* t */
                            , DF_SOCKET                 /* f */
                            , 0                         /* a */
                            , NAME_SOCKET               /* n */
                            , 1                         /* z */
                            );
  
  itemCeiling = levelItemMake( vectorMake(0.0, 2.0, 0.0) /* p */
                             , vectorMake(0.0, 0.0, 0.0) /* r */
                             , vectorMake(0.0, 1.0, 0.0) /* t */
                             , DF_CEILING                /* f */
                             , 0                         /* a */
                             , NAME_CEILING              /* n */
                             , 1                         /* z */
                             );
  
  itemFenceFront = levelItemMake( vectorMake(0.0, 1.0, 1.0) /* p */
                                , vectorMake(0.0, 0.0, 0.0) /* r */
                                , vectorMake(0.0, 0.0, 1.0) /* t */
                                , DF_CAGE                   /* f */
                                , 0                         /* a */
                                , NAME_FENCE_FRONT          /* n */
                                , 1                         /* z */
                                );
  
  itemFenceBack = levelItemMake( vectorMake(0.0, 1.0, -1.0) /* p */
                               , vectorMake(0.0, 1.0,  0.0) /* r */
                               , vectorMake(0.0, 0.0, -1.0) /* t */
                               , DF_CAGE                    /* f */
                               , 180                        /* a */
                               , NAME_FENCE_BACK            /* n */
                               , 1                          /* z */
                               );
  
  itemFenceLeft = levelItemMake( vectorMake(-1.0, 1.0, 0.0) /* p */
                               , vectorMake( 0.0, 1.0, 0.0) /* r */
                               , vectorMake(-1.0, 0.0, 0.0) /* t */
                               , DF_CAGE                    /* f */
                               , 270                        /* a */
                               , NAME_FENCE_LEFT            /* n */
                               , 1                          /* z */
                               );
  
  itemFenceRight = levelItemMake( vectorMake(1.0, 1.0, 0.0) /* p */
                                , vectorMake(0.0, 1.0, 0.0) /* r */
                                , vectorMake(1.0, 0.0, 0.0) /* t */
                                , DF_CAGE                   /* f */
                                , 90                        /* a */
                                , NAME_FENCE_RIGHT          /* n */
                                , 1                         /* z */
                                );
  
  itemPool = levelItemMake( vectorMake(0.0, 0.1, 0.0) /* p */
                          , vectorMake(0.0, 0.0, 0.0) /* r */
                          , vectorMake(0.0, 0.1, 0.6) /* t */
                          , DF_POOL                   /* f */
                          , 0                         /* a */
                          , NAME_POOL                 /* n */
                          , 1                         /* z */
                          );
  
  itemWater = levelItemMake( vectorMake(0.0, 0.2, 0.0) /* p */
                           , vectorMake(0.0, 0.0, 0.0) /* r */
                           , vectorMake(0.0, 0.2, 0.6) /* t */
                           , DF_WATER                  /* f */
                           , 0                         /* a */
                           , NAME_WATER                /* n */
                           , 1                         /* z */
                           );
  
  itemPig = levelItemMake( vectorMake(0.5, 0.4, 0.0) /* p */
                         , vectorMake(0.0, 0.0, 1.0) /* r */
                         , vectorMake(0.5, 0.4, 0.0) /* t */
                         , DF_PIG                    /* f */
                         , 0                         /* a */
                         , NAME_PIG                  /* n */
                         , 1                         /* z */
                         );
  
  itemGiraffe = levelItemMake( vectorMake(-0.5, 0.4, 0.0) /* p */
                             , vectorMake(0.0, 0.0, 0.0)  /* r */
                             , vectorMake(-0.5, 0.4, 0.0) /* t */
                             , DF_GIRAFFE                 /* f */
                             , 0                          /* a */
                             , NAME_GIRAFFE               /* n */
                             , 1                          /* z */
                             );
  
  itemGiraffeHead = levelItemMake( vectorMake(-0.5, 0.4, 0.1) /* p */
                                 , vectorMake(1.0, 0.0, 0.0)  /* r */
                                 , vectorMake(-0.5, 0.4, 0.1) /* t */
                                 , DF_GIRAFFE_HEAD            /* f */
                                 , 0                          /* a */
                                 , NAME_GIRAFFE_HEAD          /* n */
                                 , 1                          /* z */
                                 );
  
  itemFish = levelItemMake( vectorMake(0.0, -1.0, 0.0) /* p */
                          , vectorMake(0.0,  1.0, 0.0) /* r */
                          , vectorMake(0.0,  0.1, 0.5) /* t */
                          , DF_FISH                    /* f */
                          , 0                          /* a */
                          , NAME_FISH                  /* n */
                          , 1                          /* z */
                          );
  
  itemSun = levelItemMake( vectorMake(0.0, 0.0, 0.0) /* p */
                         , vectorMake(0.0, 0.0, 0.0) /* r */
                         , vectorMake(0.0, 5.0, 3.0) /* t */
                         , DF_SUN                    /* f */
                         , 0                         /* a */
                         , NAME_SUN                  /* n */
                         , 2                         /* z */
                         );
  
  itemGiraffeCube = levelItemMake( vectorMake(-1.5, 0.3, 0.0) /* p */
                                 , vectorMake(0.0, 0.0, 0.0)  /* r */
                                 , vectorMake(-1.5, 0.3, 0.0) /* t */
                                 , DF_GIRAFFE_CUBE            /* f */
                                 , 0                          /* a */
                                 , NAME_GIRAFFE_CUBE          /* n */
                                 , 1                          /* z */
                                 );
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Inserting Items.\n");
  #endif
  
  l = levelInsert(l, &itemGround,      eye, n);
  l = levelInsert(l, &itemSocket,      eye, n);
  l = levelInsert(l, &itemCeiling,     eye, n);
  l = levelInsert(l, &itemFenceFront,  eye, n);
  l = levelInsert(l, &itemFenceBack,   eye, n);
  l = levelInsert(l, &itemFenceLeft,   eye, n);
  l = levelInsert(l, &itemFenceRight,  eye, n);
  l = levelInsert(l, &itemPool,        eye, n);
  l = levelInsert(l, &itemWater,       eye, n);
  l = levelInsert(l, &itemPig,         eye, n);
  l = levelInsert(l, &itemGiraffe,     eye, n);
  l = levelInsert(l, &itemGiraffeHead, eye, n);
  l = levelInsert(l, &itemFish,        eye, n);
  l = levelInsert(l, &itemSun,         eye, n);
  l = levelInsert(l, &itemGiraffeCube, eye, n);
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
  n = vectorNorm(vectorSub(center, eye));
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Loading Textures.\n");
  #endif
  
  initTextures();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Creating Items.\n");
  #endif
  
  initItems();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Initializing Animation Buffer.\n");
  #endif
  
  initAnimBuf();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Setting Display Lists.\n");
  #endif
  displaylistInit();
  displaylistSet();
  
  return 1;
}

/**
 * Berechnet die Szene neu.
 *
 * @param[in] interval Zeit, die seit dem letzten Aufruf vergangen ist.
 */
extern void logicCalcScene(double interval)
{
  /* Kameraposition berechnen */
  calcCams(interval);
  
  /* Animationen durchführen */
  calcAnimation(interval);
  
  /* Blickrichtung normieren */
  n = vectorNorm(vectorSub(center, eye));
  
  /* Level anhand des Augpunktes und der Blickrichtung sortieren */
  l = levelSort(l, eye, n);
}

/**
 * Gibt den Level zurück.
 *
 * @return Level.
 */
extern Level logicGetLevel(void)
{
  return l;
}

/**
 * Gibt die zum Objekttypen t zugeordnete Zeichenfunktion zurück.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t gehörende Zeichenfunktion.
 */
DrawFunction logicGetDrawFunction(DrawFunctionType t)
{
  return functionsDrawing[t];
}

/**
 * Setzt den Bewegungsstatus der Kamera in Richtung cd auf b.
 *
 * @param[in] cd Bewegungsrichtung für die Kamera.
 * @param[in] b  Status der Bewegung in Richtung cd.
 */
extern void logicSetCamMovement(LogicCamDirection cd, Boolean b)
{
  camMovement[cd] = b;
}

/**
 * Gibt die Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß
 * zurück.
 *
 * @return Auslenkung der Kamera bezogen auf die Grundposition im Gradmaß.
 */
extern int logicGetCamRotation(void)
{
  return camRotation;
}

/**
 * Gibt die Höhe der Kamera bezogen auf die Grundposition zurück.
 *
 * @return Höhe der Kamera bezogen auf die Grundposition.
 */
extern double logicGetCamHeight(void)
{
  return camHeight;
}

/**
 * Schaltet das Licht mit der Nummer i ein bzw. aus.
 */
extern void logicSetLight(unsigned i)
{
  light[i] = !light[i];
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Light %i %s.\n", i, light[i] ? "on" : "off");
  #endif
}

/**
 * @return Gibt zurück, ob Licht i eingeschaltet ist.
 */
extern Boolean logicGetLight(unsigned i)
{
  return light[i];
}

/**
 * @return Gibt den Augpunkt zurück.
 */
extern Vector3d logicGetEye(void)
{
  return eye;
}

/**
 * @return Gibt den Mittelpunkt der Szene zurück.
 */
extern Vector3d logicGetCenter(void)
{
  return center;
}

/**
 * Schaltet zwischen Gitter, Glas und Nichts um.
 */
extern void logicSetGlass(void)
{
  DrawFunction f = functionsDrawing[DF_CAGE]
             , g
             ;
  
  if (f == drawFence)
  {
    f = drawGlass;
    g = drawGlassCeiling;
  }
  else if (f == drawGlass)
  {
    f = drawNothing;
    g = drawNothing;
  }
  else
  {
    f = drawFence;
    g = drawFenceCeiling;
  }
  
  functionsDrawing[DF_CAGE]    = f;
  functionsDrawing[DF_CEILING] = g;
  
  displaylistSet();
}

/**
 * Setzt die Kamera auf Kante c.
 *
 * @param[in] c Kante, auf die die Kamera gesetzt werden soll.
 */
extern void logicSetToCorner(Corner c)
{
  camRotation = 90 * c;
  
  camHeight   = 0.4;
  camZoom     = 0.7;
}

/**
 * Prüft, ob n ein Element bezeichnet, welches auswählbar sein soll.
 *
 * @param[in] n Objektname.
 *
 * @return true  wenn n auswählbar sein soll
 *         false sonst.
 */
extern Boolean logicIsPickable(PickingName n)
{
  return n == NAME_GIRAFFE
      || n == NAME_GIRAFFE_HEAD
      || n == NAME_FISH
      || n == NAME_PIG
      ;
}

/**
 * Wählt ein auswählbares Objekt aus oder lässt es los.
 *
 * @param[in] n Name des Objektes.
 * @param[in] b Auswahlstatus.
 */
extern void logicSelect(PickingName n, Boolean b)
{
  /* Auswählen */
  if (b)
  {
    switch (n)
    {
      case NAME_GIRAFFE:
        itemMove = &itemGiraffe;
        break;
      case NAME_PIG:
        itemMove = &itemPig;
        break;
      case NAME_FISH:
        itemMove = &itemFish;
        break;
      default:
        break;
    }
  }
  /* Loslassen */
  else
  {
    ignoreMove = TRUE;       /* Nächstes Move-Ereignis ignorieren */
    itemMove   = (void *) 0; /* Kein Objekt ist ausgewählt        */
  }
}

/**
 * Bewegt das ausgewählte Objekt.
 * Die Bewegung wird anhand von v berechnet.
 *
 * @param[in] v Gibt die auszuführende Verschiebung an.
 */
extern void logicMove(Vector3d v)
{
  /* Ersten Move ignorieren */
  if (!ignoreMove)
  {
    /* itemMove != NULL */
    if (itemMove)
    {
      /**
       * Verschiebungsrichtung an die Kamerarotation anpassen und resultierende
       * Position bestimmen
       */
      Vector3d move = vectorAdd( itemMove->t
                               , vectorRotateY( vectorScale(v, 0.01, 0.01, 0.01)
                                              , camRotation
                                              )
                               );
      
      /* Giraffe bewegen */
      if (itemMove == &itemGiraffe)
      {
        /**
         * Position muss innerhalb des Käfigs liegen und ein Sicherheitsabstand
         * zum Schwein muss eingehalten werden.
         */
        if (inCage(move, sizeGiraffe)
        &&  distanceGiraffePig(move, itemPig.t, sizeGiraffe, sizePig))
        {
          /* Giraffe und Giraffenkopf bewegen */
          itemGiraffe.t     = move;
          itemGiraffeHead.t = vectorAdd( itemGiraffeHead.t
                                       , vectorRotateY( vectorScale(v, 0.01, 0.01, 0.01)
                                                      , camRotation
                                                      )
                                       );
        }
      }
      /* Schwein bewegen */
      else if (itemMove == &itemPig)
      {
        /**
         * Position muss innerhalb des Käfigs liegen und ein Sicherheitsabstand
         * zur Giraffe muss eingehalten werden.
         */
        if (inCage(move, sizePig)
        && distanceGiraffePig(itemGiraffe.t, move, sizeGiraffe, sizePig))
        {
          /* Schwein bewegen */
          itemPig.t = move;
        }
      }
      /* Fisch bewegen */
      else if (itemMove == &itemFish)
      {
        /* Position muss innerhalb des Pools liegen */
        if (inPool(move, sizeFish))
        {
          /* Fisch bewegen */
          itemFish.t = move;
        }
      }
    }
  }
  /* Erster Move ignoriert, folgende beachten */
  else
  {
    ignoreMove = FALSE;
  }
}

/**
 * Animation für das mit n bezeichnete Objekt einschalten.
 *
 * @param[in] n Name des zu animierenden Objektes.
 */
extern void logicAnimate(AnimationName n)
{
  animBuf[n].isAnimated = TRUE;
}
