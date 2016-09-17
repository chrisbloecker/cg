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
#include <time.h>
#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "logic.h"
#include "drawing.h"
#include "vector.h"
#include "object.h"
#include "texture.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Seiten eines Würfels */
typedef enum {
  FACE_FRONT
, FACE_LEFT
, FACE_BACK
, FACE_RIGHT
, FACE_TOP
, FACE_BOTTOM
, FACE_DUMMY
} Face;

/** Informationen über Würfelseiten */
typedef enum {
  VERTEX_MIDDLE
, VERTEX_NORMAL
, VERTEX_DUMMY
} Vertex;

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define LOGIC_CAM_ANGLE_PS     ( 90.0)
#define LOGIC_CAM_HEIGHT_PS    (  2.5)
#define LOGIC_CAM_ZOOM_PS      (  1.0)

#define LOGIC_BALL_RADIUS      (  0.1)
#define LOGIC_BALL_ANGLE_MAX   ( 45  )
#define LOGIC_BALL_STEP_PS     (  1.5)
#define LOGIC_BALL_SPEED       (  1.0)

#define LOGIC_RACKET_STEP_PS   (  2.0)
#define LOGIC_RACKET_ANGLE_PS  (120.0)
#define LOGIC_RACKET_ANGLE_MAX ( 35.0)

#define LOGIC_CUBE_ANGLE_PS    ( 30.0)
#define LOGIC_CUBE_SCALE       (  0.3)
#define LCS (LOGIC_CUBE_SCALE)

#define LOGIC_LIGHT_STEP_PS    (  0.5)

#define LOGIC_LIFES            (  3  )

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* Signumfunktion */
#define SIGNUM(x) (((x) > (0)) ? (+1) : (-1))

/* Holt den Wert x in die Grenzen [-b, +b] zurück, GEFÄHRLICHES MACRO! */
#define TOBOUNDS(x, b) (((fabs(x)) > (b)) ? ((SIGNUM(x)) * (b)) : (x))

/* Ein Vorzeichen aus dem Bereich [-,+] */
#define RANDSIGN ((rand() % 2) & 1 ? (+1) : (-1))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Zuordnung der Zeichenfunktionen zu den Levels */
static DrawFunction functionsDrawing[DF_DUMMY] = 
  { drawCube
  , drawSphere
  , drawCylinder
  , drawNothing
  };

/* Umschaltbare Informationen */
static Boolean switchable[LOGIC_SWITCHABLE_DUMMY] =
  { TRUE  /* LIGHT0        */
  , FALSE /* LIGHT1        */
  , FALSE /* CHEAT         */
  , FALSE /* PAUSE         */
  , TRUE  /* CUBEROTATION  */
  , FALSE /* RACKETUP      */
  , FALSE /* RACKETDOWN    */
  , FALSE /* RACKETLEFT    */
  , FALSE /* RACKETRIGHT   */
  , FALSE /* CAMUP         */
  , FALSE /* CAMLEFT       */
  , FALSE /* CAMDOWN       */
  , FALSE /* CAMRIGHT      */
  , FALSE /* CAMFORWARD    */
  , FALSE /* CAMBACKWARD   */
  , FALSE /* LIGHTFORWARD  */
  , FALSE /* LIGHTBACKWARD */
  , FALSE /* LIGHTUP       */
  , FALSE /* LIGHTDOWN     */
  , FALSE /* LIGHTLEFT     */
  , FALSE /* LIGHTRIGHT    */
  , TRUE  /* SHADOWS       */
  , FALSE /* SHADOWVOLUMES */
  };

/* Vektoren */
static Vector3d vector[LOGIC_VECTOR_DUMMY] =
  {
    { 0.0, 0.0, 0.0 } /* CENTER */
  , { 0.0, 1.0, 1.0 } /* EYE    */
  , { 0.0, 1.0, 1.8 } /* CAM    */
  , { 0.0, 0.0, 0.0 } /* N      */
  };

/* Integers */
static int ints[LOGIC_INT_DUMMY] =
  { LOGIC_LIFES
  , 0
  };

static double camRotation = - 35.0 /* Aulenkung der Kamera */
            , camHeight   =    2.0 /* Höhe der Kamera      */
            , camZoom     =    0.8 /* Zoom der Kamera      */
            , ballSpeed   = LOGIC_BALL_SPEED
            ;

static Vector3d middle[FACE_DUMMY] =
                { {  0.0,   0.0, + 0.5} /* Front  */
                , {- 0.5,   0.0,   0.0} /* Left   */
                , {  0.0,   0.0, - 0.5} /* Back   */
                , {+ 0.5,   0.0,   0.0} /* Right  */
                , {  0.0, + 0.5,   0.0} /* Top    */
                , {  0.0, - 0.5,   0.0} /* Bottom */
                }
              , normal[FACE_DUMMY] =
                { {  0.0,   0.0, + 1.0} /* Front  */
                , {- 1.0,   0.0,   0.0} /* Left   */
                , {  0.0,   0.0, - 1.0} /* Back   */
                , {+ 1.0,   0.0,   0.0} /* Right  */
                , {  0.0, + 1.0,   0.0} /* Top    */
                , {  0.0, - 1.0,   0.0} /* Bottom */
                }
              ;

/* Der "Level" */
static Objects objects[LOGIC_OBJECTS_DUMMY];

/* Levelgegenstände */
static Object object[LOGIC_OBJECT_DUMMY];

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine zufällige Ballrichtung.
 */
static Vector3d makeBallDirection(void)
{
  return vectorRotateY( vectorMake(0.0, 0.0, - 1.0)
                      , (rand() % (LOGIC_BALL_ANGLE_MAX + 1)) * RANDSIGN);
}

/**
 * Berechnet die Position der Kamera.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcCams(double interval)
{
  /* Kamerabewegung */
  if (switchable[LOGIC_SWITCHABLE_CAMUP])
    camHeight += interval * LOGIC_CAM_HEIGHT_PS;
  if (switchable[LOGIC_SWITCHABLE_CAMDOWN])
    camHeight -= interval * LOGIC_CAM_HEIGHT_PS;
  if (switchable[LOGIC_SWITCHABLE_CAMLEFT])
    camRotation -= interval * LOGIC_CAM_ANGLE_PS;
  if (switchable[LOGIC_SWITCHABLE_CAMRIGHT])
    camRotation += interval * LOGIC_CAM_ANGLE_PS;
  if (switchable[LOGIC_SWITCHABLE_CAMFORWARD])
    camZoom -= interval * LOGIC_CAM_ZOOM_PS;
  if (switchable[LOGIC_SWITCHABLE_CAMBACKWARD])
    camZoom += interval * LOGIC_CAM_ZOOM_PS;
  
  /* Rotationswinkel "kleinhalten" */
  while (camRotation < 0.0)
    camRotation += 360.0;
  
  while (camRotation > 360.0)
    camRotation -= 360.0;
  
  /* Augpunkt aus Kamerabewegung berechnen */
  vector[LOGIC_VECTOR_EYE] = vectorRotateY( vectorScale( vectorAdd( vector[LOGIC_VECTOR_CAM]
                                                                  , vectorMake(0, camHeight, 0)
                                                                  )
                                                       , camZoom
                                                       , camZoom
                                                       , camZoom
                                                       )
                                          , camRotation
                                          );
}

/**
 * Berechnet die Position der Lightquelle.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcLight(double interval)
{
  static Object * objectLight = &object[LOGIC_OBJECT_LIGHT];
  
  if (switchable[LOGIC_SWITCHABLE_LIGHTFORWARD])
    objectLight->t.z += interval * LOGIC_LIGHT_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_LIGHTBACKWARD])
    objectLight->t.z -= interval * LOGIC_LIGHT_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_LIGHTLEFT])
    objectLight->t.x -= interval * LOGIC_LIGHT_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_LIGHTRIGHT])
    objectLight->t.x += interval * LOGIC_LIGHT_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_LIGHTUP])
    objectLight->t.y += interval * LOGIC_LIGHT_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_LIGHTDOWN])
    objectLight->t.y -= interval * LOGIC_LIGHT_STEP_PS;
}

/**
 * Berechnet die Position des Schlägers.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcRacket(double interval)
{
  static Object * objectRacket     = &object[LOGIC_OBJECT_RACKET]
              , * objectWallCenter = &object[LOGIC_OBJECT_WALLCENTER]
              ;
  
  if (switchable[LOGIC_SWITCHABLE_RACKETUP])
    objectRacket->t.x -= interval * LOGIC_RACKET_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_RACKETDOWN])
    objectRacket->t.x += interval * LOGIC_RACKET_STEP_PS;
  if (switchable[LOGIC_SWITCHABLE_RACKETLEFT])
    objectRacket->a -= interval * LOGIC_RACKET_ANGLE_PS;
  if (switchable[LOGIC_SWITCHABLE_RACKETRIGHT])
    objectRacket->a += interval * LOGIC_RACKET_ANGLE_PS;
  
  objectRacket->t.x = TOBOUNDS(objectRacket->t.x, 1.0 - 0.5 * (objectRacket->s.x + objectWallCenter->s.z));
  objectRacket->a   = TOBOUNDS(objectRacket->a,   LOGIC_RACKET_ANGLE_MAX);
}

/**
 * Berechnet die Position des Balls.
 *
 * @param[in] interval Zeit seit dem letzen Aufruf.
 */
static void calcBall(double interval)
{
  static Object * objectBall = &object[LOGIC_OBJECT_BALL];
  
  double speed = interval * ballSpeed * LOGIC_BALL_STEP_PS;
  
  /* Rotation als Bewegungsrichtung missbrauchen */
  objectBall->t = vectorAdd( objectBall->t
                           , vectorScale(objectBall->r, speed, speed, speed)
                           );
  
  /* Neu erzeugen, wenn der Ball das Spielfeld verlässt */
  if (objectBall->t.z > 1.5)
  {
    objectBall->t          = vectorMake(0.0, 0.0, - 0.2);
    objectBall->r          = makeBallDirection();
    ballSpeed              = LOGIC_BALL_SPEED;
    
    if (--ints[LOGIC_INT_LIFES] < 1)
    {
      objectBall->t = vectorMakeNull();
      objectBall->r = vectorMakeNull();
      
      fprintf(stdout, "Punkte : %i\n", ints[LOGIC_INT_POINTS]);
      fprintf(stderr, "\n");
      fprintf(stdout, "Game Over!\n");
    }
    else
    {
      fprintf(stdout, "Punkte : %i\n", ints[LOGIC_INT_POINTS]);
      fprintf(stdout, "Leben  : %i\n", ints[LOGIC_INT_LIFES]);
      fprintf(stderr, "\n");
    }
  }
  
  if (ints[LOGIC_INT_LIFES] > 0)
    objectBall->r = vectorNorm(objectBall->r);
}

/**
 * Gibt den Mittelpunkt der Fläche f eines Würfels zurück.
 *
 * @param[in] f Fläche.
 */
static Vector3d getMiddle(Face f)
{
  return middle[f];
}

/**
 * Gibt die Normale der Fläche f eines Würfels zurück.
 *
 * @param[in] f Fläche.
 */
static Vector3d getNormal(Face f)
{
  return normal[f];
}

/**
 * Reflektiert den Ball am Objeckt o.
 *
 * @param[in] o Objekt, an dem der Ball reflektiert werden soll.
 */
extern void reflectOn(Object * o)
{
  static Object * objectBall = &object[LOGIC_OBJECT_BALL];
  
  /* Seit, an der reflektiert werden soll. */
  Face face;
  
  /* Kollision aufgetreten ? */
  Boolean collision = FALSE;
  
  /* "Alle" Seiten auf Kollision prüfen */
  for (face = FACE_FRONT; !collision && face <= FACE_RIGHT; ++face)
  {
    double d   /* Distanz von der Fläche */
               /**
                * |
                * |
                * |->  x
                * |
                * |
                */
         , dNN /* Distand vom Mittelpunkt der Fläche in Richtung der Flächenvektoren */
               /**
                * |---------|
                * |         |
                * |    ·  x |
                * |         |
                * |---------|
                */
         ;
    
    Vector3d middle         = getMiddle(face)                   /* Flächenmittelpunkt         */
           , normal         = getNormal(face)                   /* Flächennormale             */
           , neighborNormal = getNormal((face + 1) % FACE_TOP); /* Normale einer Nachbarseite */
           ;
    
    /* Mittelpunkt und Normalen affinieren, also dahin schieben, wo das Object ist */
    middle = vectorAdd(vectorRotateY(vectorScale(middle, o->s.x, o->s.y, o->s.z), o->a), o->t);
    normal = vectorRotateY(normal, o->a);
    neighborNormal = vectorRotateY(neighborNormal, o->a);
    
    /* Entfernungen berechnen */
    d   = vectorMult(vectorSub(objectBall->t, middle), normal);
    dNN = fabs(vectorMult(vectorSub(objectBall->t, middle), neighborNormal));
    
    /**
     * Wenn der Ball sich auf die Fläche zubewegt und die Distanzen stimmen, 
     * dann abprallen lassen
     */
    if (vectorMult(objectBall->r, normal) < 0.0 /* Ball in Richtung Fläche */
     && d   > 0.0                               /* Ball vor der Fläche     */
     && d   < LOGIC_BALL_RADIUS                 /* Ball knallt mangels Entfernung auf die Fläche */
     && dNN + LOGIC_BALL_RADIUS < ((face == FACE_LEFT || face == FACE_RIGHT) ? o->s.z : o->s.x)) /* und fliegt nicht vorbei */
    {
      Vector3d invBallDir = vectorScale(objectBall->r, -1.0, -1.0, -1.0);
      
      double scale = 2.0 * vectorMult(normal, invBallDir);
      
      objectBall->r = vectorSub(vectorScale(normal, scale, scale, scale), invBallDir);
      
      collision = TRUE;
    }
  }
}

/**
 * Verändert die Farbe des Balls.
 *
 * @param[in] ball Der Ball.
 */
static void switchBallColor(Object * ball)
{
  Material matBall = ball->m;

  ball->m = (MAT_BALL0 + (rand() % (MAT_BALL5 - MAT_BALL0))) % MAT_BALL5;

  if (ball->m == matBall)
    ball->m += ball->m == MAT_BALL5
             ? -1
             : +1
             ;
}

/**
 * Berechnet alle Kollisionen und lässt den Ball reflektieren.
 */
static void calcCollision(void)
{
  static Object * objectWallCenter = &object[LOGIC_OBJECT_WALLCENTER]
              , * objectWallLeft   = &object[LOGIC_OBJECT_WALLLEFT]
              , * objectWallRight  = &object[LOGIC_OBJECT_WALLRIGHT]
              , * objectWallFront  = &object[LOGIC_OBJECT_WALLFRONT]
              , * objectRacket     = &object[LOGIC_OBJECT_RACKET]
              , * objectCube0      = &object[LOGIC_OBJECT_CUBE0]
              , * objectCube1      = &object[LOGIC_OBJECT_CUBE1]
              , * objectBall       = &object[LOGIC_OBJECT_BALL]
              ;
  
  Vector3d ballDir = objectBall->r;
  
  /* Falls Cheat, dann Ball fangen */
  if (switchable[LOGIC_SWITCHABLE_CHEAT])
    reflectOn(objectWallFront);
  else
  {
    reflectOn(objectRacket);
    
    if (vectorLength(vectorSub(ballDir, objectBall->r)) > 0.0)
    {
      ballSpeed *= 1.05;
      ++ints[LOGIC_INT_POINTS];
    }
  }
  
  reflectOn(objectWallCenter);
  reflectOn(objectWallLeft);
  reflectOn(objectWallRight);
  reflectOn(objectCube0);
  reflectOn(objectCube1);
  
  if (vectorLength(vectorSub(ballDir, objectBall->r)) > 0.0)
    switchBallColor(objectBall);
}

/**
 * Objekte des Levels erstellen und einfügen.
 */
static void initObjects(void)
{
  Vector3d * eye = &vector[LOGIC_VECTOR_EYE]
         , * n   = &vector[LOGIC_VECTOR_N]
         ;
  
  Object * objectBall       = &object[LOGIC_OBJECT_BALL]
       , * objectWallLeft   = &object[LOGIC_OBJECT_WALLLEFT]
       , * objectWallRight  = &object[LOGIC_OBJECT_WALLRIGHT]
       , * objectWallCenter = &object[LOGIC_OBJECT_WALLCENTER]
       , * objectWallFront  = &object[LOGIC_OBJECT_WALLFRONT]
       , * objectGround     = &object[LOGIC_OBJECT_GROUND]
       , * objectLight      = &object[LOGIC_OBJECT_LIGHT]
       , * objectRacket     = &object[LOGIC_OBJECT_RACKET]
       , * objectCube0      = &object[LOGIC_OBJECT_CUBE0]
       , * objectCube1      = &object[LOGIC_OBJECT_CUBE1]
       ;
  
  Objects * level             = &objects[LOGIC_OBJECTS_LEVEL]
        , * shadowableByLight = &objects[LOGIC_OBJECTS_SHADOWABLEBYLIGHT]
        , * shadowableByBall  = &objects[LOGIC_OBJECTS_SHADOWABLEBYBALL]
        ;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Initializing Objects.\n");
  #endif
  
  objects[LOGIC_OBJECTS_LEVEL]             = objectsInit();
  objects[LOGIC_OBJECTS_SHADOWABLEBYLIGHT] = objectsInit();
  objects[LOGIC_OBJECTS_SHADOWABLEBYBALL]  = objectsInit();
  
  *objectBall = objectMakeSphere( vectorMake(0.0, 0.0, - 0.2)
                                , makeBallDirection()
                                , vectorMake(LOGIC_BALL_RADIUS, LOGIC_BALL_RADIUS, LOGIC_BALL_RADIUS)
                                , 0.0
                                , TEXTURE_WATER
                                , MAT_BALL0
                                , "Ball"
                                );
  
  *objectWallLeft = objectMakeCube( vectorMake(- 1.0, 0.0, 0.0)
                                  , vectorMake(  0.0, 1.0, 0.0)
                                  , vectorMake(  2.0, 2.0, 0.1)
                                  , 90
                                  , TEXTURE_STONE
                                  , MAT_STONE
                                  , "Wall Left"
                                  );
  
  *objectWallRight = objectMakeCube( vectorMake(1.0, 0.0, 0.0)
                                   , vectorMake(0.0, 1.0, 0.0)
                                   , vectorMake(2.0, 2.0, 0.1)
                                   , 90
                                   , TEXTURE_STONE
                                   , MAT_STONE
                                   , "Wall Right"
                                   );
  
  *objectWallCenter = objectMakeCube( vectorMake(0.0, 0.0, - 1.0)
                                    , vectorMakeNull()
                                    , vectorMake(2.1, 2.0,   0.1)
                                    , 0
                                    , TEXTURE_STONE
                                    , MAT_STONE
                                    , "Wall Center"
                                    );
  
  *objectWallFront = objectMakeCube( vectorMake(0.0, 0.0, 1.0)
                                   , vectorMakeNull()
                                   , vectorMake(2.0, 2.0, 0.1)
                                   , 0
                                   , TEXTURE_STONE
                                   , MAT_STONE
                                   , "Wall Front"
                                   );
  
  *objectGround = objectMakeCube( vectorMake(0.0, - 1.0, 0.0 )
                                , vectorMake(1.0,   0.0, 0.0 )
                                , vectorMake(2.0,   2.0, 0.01)
                                , 90
                                , TEXTURE_STONE
                                , MAT_GRASS
                                , "Ground"
                                );
  
  *objectLight = objectMakeSphere( vectorMake(0.5, 1.2, 1.2)
                                 , vectorMakeNull()
                                 , vectorMake(0.1, 0.1, 0.1)
                                 , 0
                                 , TEXTURE_SUN
                                 , MAT_SUN
                                 , "Light"
                                 );
  
  *objectRacket = objectMakeCube( vectorMake(0.0, 0.0, 1.0)
                                , vectorMake(0.0, 1.0, 0.0)
                                , vectorMake(0.5, 0.3, 0.1)
                                , 0
                                , TEXTURE_METAL
                                , MAT_METAL
                                , "Wall Center"
                                );
  
  {
    Vector3d c1p = vectorMakeNull()
           , c2p = vectorMakeNull()
           ;
    
    /* Nicht direkt vor den Schläger */
    while (fabs(c1p.x) < LCS || fabs(c1p.z) > 1.0 - 2 * LCS)
      c1p = vectorMake( 0.01 * RANDSIGN * (rand() % (int)(100 * (1.0 - LCS)))
                      , 0
                      , 0.01 * RANDSIGN * (rand() % (int)(100 * (1.0 - LCS)))
                      );
    
    *objectCube0 = objectMakeCube( c1p
                                 , vectorMake(0.0, 1.0, 0.0)
                                 , vectorMake(LCS, LCS, LCS)
                                 , rand() % 90
                                 , TEXTURE_METAL
                                 , MAT_METAL
                                 , "Cube 1"
                                 );
    
    /* Nicht in den anderen Würfel und auch nicht direkt vor den Schläger */
    while (vectorLength(vectorSub(c1p, c2p)) < 2 * LCS
        || fabs(c2p.x) < LCS
        || fabs(c2p.z) > 1.0 - 2 * LCS)
      c2p = vectorMake( 0.01 * RANDSIGN * (rand() % (int)(100 * (1.0 - LCS)))
                      , 0
                      , 0.01 * RANDSIGN * (rand() % (int)(100 * (1.0 - LCS)))
                      );
    
    *objectCube1 = objectMakeCube( c2p
                                 , vectorMake(  0.0, 1.0, 0.0)
                                 , vectorMake(  0.3, 0.3, 0.3)
                                 , rand() % 90
                                 , TEXTURE_METAL
                                 , MAT_METAL
                                 , "Cube 2"
                                 );
  }
  
  *level = *objectsInsert(level, objectBall,       *eye, *n);
  *level = *objectsInsert(level, objectWallLeft,   *eye, *n);
  *level = *objectsInsert(level, objectWallRight,  *eye, *n);
  *level = *objectsInsert(level, objectWallCenter, *eye, *n);
  *level = *objectsInsert(level, objectGround,     *eye, *n);
  *level = *objectsInsert(level, objectLight,      *eye, *n);
  *level = *objectsInsert(level, objectRacket,     *eye, *n);
  *level = *objectsInsert(level, objectCube0,      *eye, *n);
  *level = *objectsInsert(level, objectCube1,      *eye, *n);
  
  *shadowableByLight = *objectsInsert(shadowableByLight, objectBall,   *eye, *n);
  *shadowableByLight = *objectsInsert(shadowableByLight, objectCube0,  *eye, *n);
  *shadowableByLight = *objectsInsert(shadowableByLight, objectCube1,  *eye, *n);
  *shadowableByLight = *objectsInsert(shadowableByLight, objectRacket, *eye, *n);
  
  *shadowableByBall = *objectsInsert(shadowableByBall, objectCube0,  *eye, *n);
  *shadowableByBall = *objectsInsert(shadowableByBall, objectCube1,  *eye, *n);
  *shadowableByBall = *objectsInsert(shadowableByBall, objectRacket, *eye, *n);
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
         , * n      = &vector[LOGIC_VECTOR_N]
         ;
  
  srand(time(NULL));
  
  /* Blickrichtung normieren */
  *n = vectorNorm(vectorSub(*center, *eye));
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Loading Textures.\n");
  #endif
  initTextures();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Init : Creating Items.\n");
  #endif
  initObjects();
  
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
                , * n      = &vector[LOGIC_VECTOR_N]
                ;
  
  static Object * objectCube0 = &object[LOGIC_OBJECT_CUBE0]
              , * objectCube1 = &object[LOGIC_OBJECT_CUBE1]
              ;
  
  static Objects * level = &objects[LOGIC_OBJECTS_LEVEL];
  
  /* Kameraposition berechnen */
  calcCams(interval);
  calcLight(interval);
  
  /* Pausenmodus */
  if (!switchable[LOGIC_SWITCHABLE_PAUSE])
  {
    if (switchable[LOGIC_SWITCHABLE_CUBEROTATION])
    {
      objectCube0->a += interval * LOGIC_CUBE_ANGLE_PS;
      objectCube1->a += interval * LOGIC_CUBE_ANGLE_PS;
    }
    
    calcRacket(interval);
    calcBall(interval);
    calcCollision();
  }
    
  /* Blickrichtung normieren */
  *n = vectorNorm(vectorSub(*center, *eye));
  
  /* Level anhand des Augpunktes und der Blickrichtung sortieren */
  *level = *objectsSort(level, *eye, *n);
}

/**
 * Gibt die zum Objekttypen t zugeordnete Zeichenfunktion zurück.
 *
 * @param[in] t Zu zeichnender Objekttyp.
 *
 * @return zu t gehörende Zeichenfunktion.
 */
extern DrawFunction logicGetDrawFunction(DrawFunctionType t)
{
  return functionsDrawing[t];
}

/**
 * Schaltet s um.
 *
 * @param[in] s Gibt an, was umgeschaltet werden soll.
 */
extern void logicSwitch(LogicSwitchable s)
{
  switchable[s] = !switchable[s];
}

/**
 * Gibt den Wert von s zurück.
 *
 * @param[in] s
 *
 * @return Wert von s.
 */
extern Boolean logicGetSwitchable(LogicSwitchable s)
{
  return switchable[s];
}

/**
 * Gibt den Vektor v zurück.
 *
 * @param[in] v
 *
 * @return Wert von v.
 */
extern Vector3d logicGetVector(LogicVector v)
{
  return vector[v];
}

/**
 * Gibt den Wert von i zurück.
 *
 * @param[in] i
 *
 * @return Wert von i.
 */
extern int logicGetInt(LogicInt i)
{
  return ints[i];
}

/**
 * Gibt einen Zeiger auf das Objekt o zurück.
 *
 * @param[in] o
 *
 * @return Zeiger auf o.
 */
extern Object * logicGetObject(LogicObject o)
{
  return &object[o];
}

/**
 * Gibt einen Zeiger auf die Objekte os zurück.
 *
 * @param[in] os
 *
 * @return Zeiger auf os.
 */
extern Objects * logicGetObjects(LogicObjects os)
{
  return &objects[os];
}
