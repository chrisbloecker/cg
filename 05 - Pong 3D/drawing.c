/**
 * @file
 *
 * Das Modul stellt Zeichenfunktionen zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"
#include "material.h"
#include "texture.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Unterteilungsfeinheit */
static int subdivides = 10;

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet ein Quadrat im Ursprung.
 *
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawSquare(TextureName t)
{
  int x, y;
  
  /* Textur aufkleben */
  bindTexture(t);
  
  for (y = 0; y <= subdivides; ++y)
  {
    glBegin(GL_QUAD_STRIP);
      glNormal3f(0.0, 0.0, 1.0);
      for (x = 0; x <= subdivides + 1; ++x)
      {
        glTexCoord2f(x / (subdivides + 1.0), y / (subdivides + 1.0));
        glVertex3f( - 0.5 + x / (subdivides + 1.0)
                  , 0.5 - y / (subdivides + 1.0)
                  , 0.0);

        glTexCoord2f(x / (subdivides + 1.0), (y + 1) / (subdivides + 1.0));
        glVertex3f(- 0.5 + x / (subdivides + 1.0)
                  ,  0.5 - (y + 1) / (subdivides + 1.0)
                  ,  0.0);
      }
    glEnd();
  }
  
  /* Danach leere Textur verwenden */
  bindTexture(TEXTURE_EMPTY);
}

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] proportion Verhältnis des Radius des oberen Kreises zum unteren.
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawCylinder1(double proportion, TextureName t)
{
  GLUquadric * q;
  q = gluNewQuadric();
  
  gluQuadricNormals(q, GL_SMOOTH);
  gluQuadricTexture(q, GL_TRUE);
  
  bindTexture(t);
  
  /* Zylinder */
  glPushMatrix();
    glRotatef(- 90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, - 1.0);
    gluCylinder(q, 1.0, proportion, 2.0, subdivides + 3, subdivides + 3);
  glPopMatrix();
  
  bindTexture(TEXTURE_EMPTY);
  
  gluDeleteQuadric(q);
}

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Setzt die Rekursionstiefe für das Zeichnen der Objekte und aktualisiert die
 * Displaylisten.
 *
 * @param[in] i Rekursionstiefe.
 */
extern void drawingSetSubdivides(int i)
{
  subdivides += i;
  
  if (subdivides < 0)
    subdivides = 0;
    
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: subdivides : %i\n", subdivides);
  #endif
}

/**
 * Zeichnet einen Würfel.
 *
 * @param[in] t Name der zu verwendenen Textur.
 */
extern void drawCube(TextureName t)
{
  /* Vorne */
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();

  /* Rechts */
  glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();

  /* Hinten */
  glPushMatrix();
    glRotatef(180, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();

  /* Links */
  glPushMatrix();
    glRotatef(270, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();

  /* Oben */
  glPushMatrix();
    glRotatef(- 90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();

  /* Unten */
  glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(t);
  glPopMatrix();
}

/**
 * Zeichnet eine Einheitskugel mit dem Radius 1 im Ursprung.
 *
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawSphere(TextureName t)
{
  GLUquadric * q;
  q = gluNewQuadric();
  
  gluQuadricNormals(q, GL_SMOOTH);
  gluQuadricTexture(q, GL_TRUE);
  
  bindTexture(t);
  gluSphere(q, 1.0, subdivides + 3, subdivides + 3);
  bindTexture(TEXTURE_EMPTY);
  
  gluDeleteQuadric(q);
}

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawCylinder(TextureName t)
{
  drawCylinder1(1.0, t);
}

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] proportion Verhältnis des Radius des oberen Kreises zum unteren.
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawKegel(double proportion, TextureName t)
{
  drawCylinder1(proportion, t);
}

/**
 * Zeichnet nichts.
 * @param[in] t Name der zu verwendenen Textur.
 */
extern void drawNothing(TextureName t)
{
}
