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

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define COLOR_TEXT           (glColor3f(0.0, 0.0, 0.0))
#define COLOR_HELPBACKGROUND (glColor3f(0.8, 0.8, 0.8))

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Hintergrund der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f Zeichenfunktion.
 */
static void drawBackground(DrawFunction f)
{
  glPushMatrix();
    f();
  glPopMatrix();
}

/**
 * Zeichnet den Boden der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f Zeichenfunktion.
 */
static void drawGround(DrawFunction f)
{
  glPushMatrix();
    glScalef( SCALE_GROUND
            , 1.0
            , 1.0
            );
    f();
  glPopMatrix();
}

/**
 * Zeichnet den Bodenbelag der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f     Zeichenfunktion.
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawSurface(DrawFunction f, double shift)
{
  int i;
  
  /* shift -> [0,2] */
  while (shift > 2.0)
    shift -= 2.0;
  
  /* shift -> [0,1] mit "Verlangsamung" */
  shift /= 2;
  
  /* Belag des Untergrundes */
  for (i = - 1; i < 3; ++i)
  {
    glPushMatrix();
      glTranslatef( 0.0
                  , SPEED_ENVIRONMENT * (i - shift)
                  , 0.0
                  );
      glScalef( SCALE_GROUND
              , SCALE_GROUND
              , 1.0
              );
      f();
    glPopMatrix();
  }
}

/**
 * Zeichnet die Umgebung der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f     Zeichenfunktion.
 * @param[in] g     Animationsfunktion für die Umgebung.
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawEnvironment(DrawFunction f, AnimationFunction g, double shift)
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
  for (i = -1; i < 3; ++i)
  {
    glPushMatrix();
      glTranslatef( - TRANSLATION_X_ENVIRONMENT
                  , SPEED_ENVIRONMENT * (i - shift)
                  , 0.0
                  );
      g(xShift);
      glScalef( SCALE_ENVIRONMENT
              , SCALE_ENVIRONMENT
              , SCALE_ENVIRONMENT
              );
      f();
    glPopMatrix();
  }
  
  /* Rechts */
  for (i = -1; i < 3; ++i)
  {
    glPushMatrix();
      glTranslatef( TRANSLATION_X_ENVIRONMENT
                  , SPEED_ENVIRONMENT * (i - shift)
                  , 0.0
                  );
      g(- xShift);
      glScalef( SCALE_ENVIRONMENT
              , SCALE_ENVIRONMENT
              , SCALE_ENVIRONMENT
              );
      f();
    glPopMatrix();
  }
}

/**
 * Zeichnet die Gegenstände der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f     Zeichenfunktion.
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawItems(DrawFunction f, double shift)
{
  Vector2d item;
  
  /* Iterator für die Gegenstände initialisieren */
  logicItemIterInit();
  
  glPushMatrix();
    /* Und jeden Zeichnen */
    while(logicItemHasNext())
    {
      item = logicItemNext();
      
      glPushMatrix();
        glTranslatef( TRANSLATION_X_ITEM * (item.x - 1)
                    , item.y - shift
                    , 0.0
                    );
        glScalef( SCALE_ITEM
                , SCALE_ITEM
                , SCALE_ITEM
                );
        f();
      glPopMatrix();
    }
  glPopMatrix();
}

/**
 * Zeichnet den Bodenbelag der Szene mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f     Zeichenfunktion.
 * @param[in] shift Verschiebung der Szene in y-Richtung.
 */
static void drawObstacles(DrawFunction f, double shift)
{
  Vector2d obstacle;
   
  /* Iterator für die Hindernisse initialisieren */
  logicObstacleIterInit();
  
  glPushMatrix();
    /* Und jedes Zeichnen */
    while(logicObstacleHasNext())
    {
      obstacle = logicObstacleNext();
      
      glPushMatrix();
        glTranslatef( TRANSLATION_X_OBSTACLE * (obstacle.x - 1)
                    , obstacle.y - shift
                    , 0.0
                    );
        glScalef( SCALE_OBSTACLE
                , SCALE_OBSTACLE
                , SCALE_OBSTACLE
                );
        f();
      glPopMatrix();
    }
  glPopMatrix();
}

/**
 * Zeichnet Hugo mit Hilfe der Zeichenfunktion f.
 *
 * @param[in] f         Zeichenfunktion.
 * @param[in] hugoShift Hugos Verschiebung in x-Richtung.
 */
static void drawHugo(DrawFunction f, double hugoShift)
{
  glPushMatrix();
    glTranslatef( 0.65 * SCALE_GROUND * hugoShift
                , TRANSLATION_Y_HUGO
                , 0.0
                );
    glScalef( SCALE_HUGO
            , SCALE_HUGO
            , SCALE_HUGO
            );
    f();
  glPopMatrix();
}

/**
 * Zeichnet den String s an der Stelle (x/y).
 *
 * @param[in] x x-Position.
 * @param[in] y y-Position.
 * @param[in] s String.
 */
static void drawString(float x, float y, char * s)
{
  glRasterPos2f(x, y);
  
  while (*s)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *s);
    ++s;
  }
}

/**
 * Zeichnet die Level-Infos in die Szene.
 *
 * @param[in] t Infos der Szene.
 */
static void drawLevelInfo(Text t)
{
  glPushMatrix();
  
  COLOR_TEXT;
  
  drawString(- 0.95, - 0.8,  "Punkte : ");
  drawString(- 0.95, - 0.85, "Leben  : ");
  drawString(- 0.95, - 0.9,  "Level  : ");

  drawString(- 0.75, - 0.8,  t.points);
  drawString(- 0.75, - 0.85, t.lives);
  drawString(- 0.75, - 0.9,  t.level);
  glPopMatrix();
  
  logicCleanText(t);
}

/**
 * Zeichnet ein Rechteck mit den Hilfetexten.
 */
static void drawHelp(void)
{
  COLOR_HELPBACKGROUND;
  
  /* Kästchen */
  glBegin(GL_QUADS);
    glVertex2f(  0.5,   0.25);
    glVertex2f(  0.5, - 0.25);
    glVertex2f(- 0.5, - 0.25);
    glVertex2f(- 0.5,   0.25);
  glEnd();
  
  COLOR_TEXT;
  
  /* Texte */
  drawString(- 0.4,   0.1, "Hilfe : ");
  drawString(- 0.4,   0.0, "Beenden : ");
  drawString(- 0.4, - 0.1, "Steuerung : ");
  
  drawString(0.0,   0.1, "h / H");
  drawString(0.0,   0.0, "ESC / q / Q");
  drawString(0.0, - 0.1, "Pfeile Rechts und Links");
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet die Szene
 */
extern void drawScene(void)
{
  double shift     = logicGetShift()
       , hugoShift = logicGetHugoShift();
  
  drawBackground(logicGetDrawFunction(BACKGROUND));
  drawGround(logicGetDrawFunction(GROUND));
  drawSurface(logicGetDrawFunction(SURFACE), shift);
  drawHugo(logicGetDrawFunction(HUGO), hugoShift);
  drawEnvironment(logicGetDrawFunction(ENVIRONMENT), logicGetAnimationFunction(), shift);
  drawItems(logicGetDrawFunction(ITEM), shift);
  drawObstacles(logicGetDrawFunction(OBSTACLE), shift);
  drawLevelInfo(logicGetText());
  
  if (logicShowHelp())
    drawHelp();
}

/**
 * Initialisierung der Szene.
 */
extern int initScene(void)
{
  /* Hintergrundfarbe */
  glClearColor( 0.0f
              , 0.0f
              , 0.0f
              , 0.0f
              );
  
  return 1;
}
