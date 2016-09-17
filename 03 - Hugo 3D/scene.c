/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalität (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * System
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * Eigene
 * -------------------------------------------------------------------------- */
#include "scene.h"
#include "logic.h"
#include "types.h"
#include "level.h"
#include "vector.h"
#include "displaylist.h"
#include "stringOutput.h"

#define DRAWING
#include "constants.h"
#undef DRAWING

#define PI (3.1415926535897932384626433832795029)
#define DEGTORAD(x) ((x)*(2)*(PI)/(360))

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/* Speed für Objekte */
#define SPEED_ENVIRONMENT (0.75)
#define SPEED_BUILDINGS   (0.40)

#define ENVIRONMENT_MIN (- 6)
#define ENVIRONMENT_MAX ( 10)

#define COUNT_LIGHTS (2)

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */


static float 
/* Positionen der Lichtquellen */
  lightPos[COUNT_LIGHTS][4] = { {0.0, 15.0, 0.0, 1.0}
                              , {0.0, 1.0, 0.0, 1.0}
                              }
                            
/* Farben der Lichtquellen */
, lightColor[COUNT_LIGHTS][3][4] = { { {0.1, 0.1, 0.1, 1.0}
                                     , {1.0, 1.0, 1.0, 1.0}
                                     , {1.5, 1.5, 1.5, 1.0}
                                     }
                                   ,
                                     { {0.1, 0.1, 0.1, 1.0}
                                     , {1.0, 1.0, 1.0, 1.0}
                                     , {1.5, 1.5, 1.5, 1.0}
                                     }
                                   }

/* Richtung der Lichtquellen */
, lightDir[COUNT_LIGHTS][3] = { {0.0, 0.0, 0.0}
                              , {0.0, 1.0, 5.0}
                              }

/* Augpunkt */
, cam[3] = {0.0, 1.0, 1.0}

/* Zentrum */
, center[3] = {0.0, 0.0, 0.0}

/* Up-Vektor */
, up[3]     = {0.0, 1.0, 0.0}
;

static Vector3d eyePos; /* aktueller Augpunkt */

static Boolean useDisplayLists = TRUE
             , lightRotation   = FALSE;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Ruft die Zeichenfunktion bzw. Displayliste zum Zeichnen der durch f gegebenen
 * Zeichenfunktion auf.
 *
 * @param[in] f DrawFunctionType.
 */
static void drawMyType(DrawFunctionType f)
{
  if (useDisplayLists)
    displaylistCall(f);
  else
    logicGetDrawFunction(f)();
}

/**
 * Zeichnet den Hintergrund der Szene.
 */
static void drawBackground(void)
{
  glPushMatrix();
    glScalef( SCALE_X_BACKGROUND
            , SCALE_Y_BACKGROUND
            , SCALE_Z_BACKGROUND
            );
    
    drawMyType(DF_BACKGROUND);
  glPopMatrix();
}

/**
 * Zeichnet den Boden der Szene.
 */
static void drawGround(void)
{
  glPushMatrix();
    glScalef( SCALE_X_GROUND
            , SCALE_Y_GROUND
            , SCALE_Z_GROUND
            );
    
    drawMyType(DF_GROUND);
  glPopMatrix();
}

/**
 * Zeichnet den Bodenbelag der Szene.

 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawSurface(double shift)
{
  int i;
  
  /* shift -> [0,2] */
  while (shift > 2.0)
    shift -= 2.0;
  
  /* shift -> [0,1] mit "Verlangsamung" */
  shift /= 2;
  
  /* Belag des Untergrundes */
  for (i = ENVIRONMENT_MIN; i < ENVIRONMENT_MAX; ++i)
  {
    glPushMatrix();
      glTranslatef( 0.0
                  , 0.0
                  , - (SPEED_ENVIRONMENT * (i - shift))
                  );
      
      drawMyType(DF_SURFACE);
    glPopMatrix();
  }
}

/**
 * Zeichnet die Umgebung der Szene.
 * 
 * @param[in] g     Animationsfunktion für die Umgebung.
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawEnvironment(AnimationFunction g, double shift)
{
  int i;
  
  double xShift;
  
  /* shift -> [0,2] */
  while (shift > 2.0)
    shift -= 2.0;
  
  /* shift -> [0,1] mit "Verlangsamung" */
  shift /= 2;
  
  /**
   * Auslenkung in x-Richtung noch geringer machen und in einem Intervall von
   * 0.5 -> 0 -> 0.5 -> 0 ... rotieren lassen
   */
  xShift = 0.1 * fabs(shift - 0.5);
  
  /* Links */
  for (i = ENVIRONMENT_MIN; i < ENVIRONMENT_MAX; ++i)
  {
    glPushMatrix();
      glTranslatef( - TRANSLATION_X_ENVIRONMENT
                  , 0.0
                  , - (SPEED_ENVIRONMENT * (i - shift))
                  );
      /* Animation */
      g(xShift);
      glScalef( SCALE_X_ENVIRONMENT
              , SCALE_Y_ENVIRONMENT
              , SCALE_Z_ENVIRONMENT
              );
      
      drawMyType(DF_ENVIRONMENT);
    glPopMatrix();
  }
  
  /* Rechts */
  for (i = ENVIRONMENT_MIN; i < ENVIRONMENT_MAX; ++i)
  {
    glPushMatrix();
      glTranslatef( TRANSLATION_X_ENVIRONMENT
                  , 0.0
                  , - (SPEED_ENVIRONMENT * (i - shift))
                  );
      /* Animation */
      g(- xShift);
      glScalef( SCALE_X_ENVIRONMENT
              , SCALE_Y_ENVIRONMENT
              , SCALE_Z_ENVIRONMENT
              );
      
      drawMyType(DF_ENVIRONMENT);
    glPopMatrix();
  }
}

/**
 * Zeichnet die Gegenstände der Szene.
 *
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawItems(double shift)
{
  Vector3d item;
  
  /* Iterator für die Gegenstände initialisieren */
  logicItemIterInit();
  
  /* Und jeden Zeichnen */
  while(logicItemHasNext())
  {
    item = logicItemNext();
    
    glPushMatrix();
      glTranslatef( TRANSLATION_X_ITEM * (item.x - 1)
                  , 0.0
                  , - item.z + shift
                  ); 
      glScalef( SCALE_X_ITEM
              , SCALE_Y_ITEM
              , SCALE_Z_ITEM
              );
      
      drawMyType(DF_ITEM);
    glPopMatrix();
  }
}

/**
 * Zeichnet die Hindernisse der Szene.

 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawObstacles(double shift)
{
  Vector3d obstacle;
  
  /* Iterator für die Hindernisse initialisieren */
  logicObstacleIterInit();
  
  /* Und jedes Zeichnen */
  while(logicObstacleHasNext())
  {
    obstacle = logicObstacleNext();
    
    glPushMatrix();
      glTranslatef( TRANSLATION_X_OBSTACLE * (obstacle.x - 1)
                  , 0.0
                  , - obstacle.z + shift
                  );
      glScalef( SCALE_X_OBSTACLE
              , SCALE_Y_OBSTACLE
              , SCALE_Z_OBSTACLE
              );
      
      drawMyType(DF_OBSTACLE);
    glPopMatrix();
  }
}

/**
 * Zeichnet Hugo.

 * @param[in] hugoShift Hugos Verschiebung in x-Richtung.
 */
static void drawHugo(double hugoShift)
{
  glPushMatrix();
    glTranslatef( 0.65 * SCALE_X_GROUND * hugoShift
                , 0.0
                , - TRANSLATION_Z_HUGO
                );
    glScalef( SCALE_X_HUGO
            , SCALE_Y_HUGO
            , SCALE_Z_HUGO
            );
    
    drawMyType(DF_HUGO);
  glPopMatrix();
}

/**
 * Zeichnet die Gebäude der Szene.

 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawBuildings(double shift)
{
  glPushMatrix();
    glTranslatef( TRANSLATION_X_BUILDING
                , 0.0
                , TRANSLATION_Z_BUILDING + SPEED_BUILDINGS * shift
                );
    
    drawMyType(DF_BUILDING);
  glPopMatrix();
}

/**
 * Zeichnet die Level-Infos in die Szene.
 *
 * @param[in] t Infos der Szene.
 */
static void drawLevelInfo(Text t)
{
  float textColor[3] = {1.0, 0.0, 0.0};
  
  drawString(0.05, 0.10, textColor, "Level: %i", t.level);
  drawString(0.05, 0.15, textColor, "Leben: %i", t.lives);
  drawString(0.05, 0.20, textColor, "Punkte: %i", t.points);
}

static void setLights(void)
{
  /* Kameraposition */
  eyePos  = vectorRotateY(vectorMake(cam[0], cam[1] + logicGetCamHeight(), cam[2]), logicGetCamRotation());
  
  /* Sonne */
  if (logicGetLight(0))
  {
    /* Globale Lichtquelle rotieren  */
    if (lightRotation)
    {
      Vector3d light = vectorRotateY(vectorMake(lightPos[0][0], lightPos[0][1], lightPos[0][2]), 2.0);
      
      lightPos[0][0] = light.x;
      lightPos[0][1] = light.y;
      lightPos[0][2] = light.z;
    }
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor[0][1]);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightColor[0][0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor[0][2]);
  }
  
  /* Spotlight */
  if (logicGetLight(1))
  {
    float spotPos[4];
    
    spotPos[0] = lightPos[1][0] + TRANSLATION_X_BUILDING;
    spotPos[1] = lightPos[1][1];
    spotPos[2] = lightPos[1][2] + TRANSLATION_Z_BUILDING + SPEED_BUILDINGS * logicGetShift();
    spotPos[3] = lightPos[1][3];
    
    glPushMatrix();
      glTranslatef(spotPos[0], spotPos[1], spotPos[2]);
      glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);

      glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightColor[1][1]);
      glLightfv(GL_LIGHT1, GL_AMBIENT,  lightColor[1][0]);
      glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor[1][2]);
      
      glRotatef(logicGetLighthouseRotation(), 0.0, 1.0, 0.0);
      
      glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir[1]);
      glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 45);
    glPopMatrix();
  }
  
  logicGetLight(0)
    ? glEnable(GL_LIGHT0)
    : glDisable(GL_LIGHT0);
  
  logicGetLight(1)
    ? glEnable(GL_LIGHT1)
    : glDisable(GL_LIGHT1);
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet die Szene
 */
extern void sceneDraw(void)
{
  double shift     = logicGetShift()
       , hugoShift = logicGetHugoShift();
    
  gluLookAt( eyePos.x,  eyePos.y,  eyePos.z
           , center[0], center[1], center[2]
           , up[0],     up[1],     up[2]
           );
  
  setLights();
  
  drawBackground();
  drawGround();
  drawSurface(shift);
  drawHugo(hugoShift);
  drawEnvironment(logicGetAnimationFunction(), shift);
  drawItems(shift);
  drawObstacles(shift);
  drawBuildings(shift);
  drawLevelInfo(logicGetText());
  
  if (logicShowHelp())
    drawHelp();
}

/**
 * Initialisierung der Szene.
 */
extern int sceneInit(void)
{
  /* Hintergrundfarbe */
  glClearColor( 0.0f
              , 0.0f
              , 0.0f
              , 0.0f
              );
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  
  glEnable(GL_NORMALIZE);
  
  return 1;
}

/**
 * Legt fest, ob Displaylisten zum Zeichnen verwendet werden sollen oder nicht.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetDisplayListUsage(void)
{
  useDisplayLists = !useDisplayLists;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Display Lists %s.\n", useDisplayLists ? "on" : "off");
  #endif
}

/**
 * Legt fest, ob die globale Lichtquelle um den Ursprung rotiert werden soll.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetLightRotation(void)
{
  lightRotation = !lightRotation;
}
