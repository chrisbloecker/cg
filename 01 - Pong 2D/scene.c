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
#include <math.h>

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * Eigene
 * -------------------------------------------------------------------------- */
#include "scene.h"
#include "logic.h"
#include "types.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* Grad- ins Gradmaß umrechnen */
#define DEGTORAD(x) ((x)*(2)*(PI)/(360))

/* ----------------------------------------------------------------------------
 * Pseudo-Konstanten
 * -------------------------------------------------------------------------- */
static Rect stripTop    = {{-0.9, 0.8}, {0.9, 0.8}, {0.9, 0.9}, {-0.9, 0.9}}
          , stripBottom = {{-0.9, -0.8}, {0.9, -0.8}, {0.9, -0.9}, {-0.9, -0.9}}
          , stripRight  = {{0.8, 0.8}, {0.8, -0.8}, {0.9, -0.8}, {0.9, 0.8}};
          
static RGBColor stripColor  = {0.0, 1.0, 0.0}
              , racketColor = {0.0, 0.0, 1.0};

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet ein Rechteck.
 *
 * @param[in] rect Das Rechteck.
 * @param[in] rgb  Farbe, in der das Rechteck gezeichnet werden soll.
 */
static void drawRect(Rect rect, RGBColor rgb)
{
  unsigned i;

  /* Zeichenfarbe */
  glColor3f(rgb[0], rgb[1], rgb[2]);
  
  glBegin(GL_POLYGON);
  {
    for (i = 0; i < 4; ++i)
      glVertex2d(rect[i].x, rect[i].y);
  }
  glEnd();
}

/**
 * Zeichnet den oberen Balken.
 */
static void drawStripTop(void)
{
  drawRect(stripTop, stripColor);
}

/**
 * Zeichnet den unteren Balken.
 */
static void drawStripBottom(void)
{
  drawRect(stripBottom, stripColor);
}

/**
 * Zeichnet den rechten Balken.
 */
static void drawStripRight(void)
{  
  drawRect(stripRight, stripColor);
}

/**
 * Zeichnet den oberen Balken.
 */
static void drawRacket(void)
{
  drawRect(getRacket()->vertices, racketColor);
}

/* Zeichnet den Ball */
void drawBall(void)
{
  unsigned i;
  Ball ball = *getBall();
  
  /* Zeichenfarbe */
  glColor3f(ball.c[0], ball.c[1], ball.c[2]);
  
  glBegin(GL_POLYGON);
  {
    for (i = 0; i < BALL_ITERATIONS; ++i)
      glVertex2d(ball.vertices[i].x + ball.p.x, ball.vertices[i].y + ball.p.y);
  }
  glEnd();
}

/**
 * Zeichen-Funktion.
 * Stellt die Szene mit Balken, Schläger und Ball dar.
 */
void drawScene(void)
{
  /* Balken oben, unten, rechts */
  drawStripTop();
  drawStripBottom();
  drawStripRight();
  
  /* Schläger */
  drawRacket();
  
  /* Ball */
  drawBall();
}

/**
 * Initialisierung der Szene.
 */
int initScene(void)
{
  /* Hintergrundfarbe */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  /* Ball und Schläger initialisiern */
  initGame();
  initBall();
  initRacket();
  
  return 1;
}
