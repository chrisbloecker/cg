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
#include "vector.h"
#include "types.h"
#include "displaylist.h"
#include "material.h"
#include "texture.h"
#include "level.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Unterteilungsfeinheit */
static int drawSubdivides = 20;

/* Normalen anzeigen */
static Boolean normals = FALSE;

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet ein Quadrat im Ursprung.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawSquare(unsigned subdivides, TextureName t)
{
  unsigned x, y;
  
  /* Textur aufkleben */
  bindTexture(t);
  
  if (normals)
  {
    glDisable(GL_LIGHTING);
      glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 1.0);
      glEnd();
    glEnable(GL_LIGHTING);
  }
  
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
 * Zeichnet einen Würfel.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawCube(unsigned subdivides, TextureName t)
{
  /* Vorne */
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Rechts */
  glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Hinten */
  glPushMatrix();
    glRotatef(180, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Links */
  glPushMatrix();
    glRotatef(270, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Oben */
  glPushMatrix();
    glRotatef(- 90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Unten */
  glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
}

/**
 * Zeichnet einen Würfel von Innen und Außen, die Oberseite fehlt.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawCubeInvertedOpen(unsigned subdivides, TextureName t)
{
  /* Vorne */
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.5);
    glRotatef(180, 0.0, 1.0, 0.0);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Rechts */
  glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(270, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, - 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Hinten */
  glPushMatrix();
    glRotatef(180, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 0.0, - 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Links */
  glPushMatrix();
    glRotatef(270, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, - 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();

  /* Unten */
  glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, - 0.5);
    drawSquare(subdivides, t);
  glPopMatrix();
}

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] proportion Verhältnis des Radius des oberen Kreises zum unteren.
 * @param[in] t          Name der zu verwendenen Textur.
 */
static void drawCylinder(unsigned subdivides, double proportion, TextureName t)
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
  
  /* Kreis oben */
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(- 90, 1.0, 0.0, 0.0);
    gluDisk(q, 0.0, proportion, subdivides + 3, subdivides + 3);
  glPopMatrix();
  
  /* Kreis unten */
  glPushMatrix();
    glTranslatef(0.0, - 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluDisk(q, 0.0, 1.0, subdivides + 3, subdivides + 3);
  glPopMatrix();
  
  gluDeleteQuadric(q);
  
  bindTexture(TEXTURE_EMPTY);
}

/**
 * Zeichnet eine Einheitskugel mit dem Radius 1 im Ursprung.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] t          Name der zu verwendenen Textur.
 */
void drawSphere(unsigned subdivides, TextureName t)
{
  GLUquadric * q;
  q = gluNewQuadric();
  
  gluQuadricNormals(q, GL_SMOOTH);
  
  gluQuadricTexture(q, GL_TRUE);
  bindTexture(t);
  
  gluSphere(q, 1.0f, subdivides + 3, subdivides + 3);
  
  gluDeleteQuadric(q);
}

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Schaltete zwischen "Normalen anzeigen" und "Normalen nicht anzeigen".
 */
extern void drawingSetNormals(void)
{
  normals = !normals;
  
  displaylistSet();
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: normals %s\n", normals ? "on" : "off");
  #endif
}

/**
 * Setzt die Rekursionstiefe für das Zeichnen der Objekte und aktualisiert die
 * Displaylisten.
 *
 * @param[in] i Rekursionstiefe.
 */
extern void drawingSetSubdivides(int i)
{
  drawSubdivides += i;
  
  if (drawSubdivides < 0)
    drawSubdivides = 0;
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: subdivides : %i\n", drawSubdivides);
  #endif
    
  displaylistSet();
}

/* ----------------------------------------------------------------------------
 * Cage
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet eine Gitterwand.
 */
extern void drawFence(void)
{
  #define step (0.2)
  double d = 0.0;
  
  materialSet(MAT_FENCE);
  for (d = - 1.0; d <= 1.0; d += step)
  {
    glPushMatrix();
      glTranslatef(d, 0.5, 0.0);
      glScalef(0.01, 0.5,  0.01);
      drawCylinder(drawSubdivides, 1.0, TEXTURE_METAL);
    glPopMatrix();
  }
  
  glPushMatrix();
    glScalef(2.0, 0.01, 0.01);
    drawCube(drawSubdivides, TEXTURE_METAL);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(2.0, 0.01, 0.01);
    drawCube(drawSubdivides, TEXTURE_METAL);
  glPopMatrix();
}

/**
 * Zeichnet die Steindecke.
 */
extern void drawFenceCeiling(void)
{
  materialSet(MAT_STONE);
  
  glPushMatrix();
    glScalef(2.0, 0.05, 2.0);
    drawCube(drawSubdivides, TEXTURE_STONE);
  glPopMatrix();
}

/**
 * Zeichnet eine Glaswand.
 */
extern void drawGlass(void)
{
  materialSet(MAT_FENCE);
  glPushMatrix();
    glScalef(2.0, 0.01, 0.01);
    drawCube(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(2.0, 0.01, 0.01);
    drawCube(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(1.0, 0.5, 0.0);
    glScalef(0.01, 1.0, 0.01);
    drawCube(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
  
  materialSet(MAT_GLASS);
  glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(2.0, 1.0, 0.01);
    drawCube(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
}

/**
 * Zeichnet die Glasdecke.
 */
extern void drawGlassCeiling(void)
{
  materialSet(MAT_GLASS);
  
  glPushMatrix();
    glScalef(2.0, 0.01, 2.0);
    drawCube(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
}

/**
 * Zeichnet den Pool.
 */
extern void drawPool(void)
{
  materialSet(MAT_STONE);
  glPushMatrix();
    glScalef(1.98, 0.2, 0.78);
    drawCubeInvertedOpen(drawSubdivides, TEXTURE_STONE);
  glPopMatrix();
}

/**
 * Zeichnet das Wasser.
 */
extern void drawWater(void)
{
  materialSet(MAT_WATER);
  glPushMatrix();
    glScalef(1.98, 1.0, 0.78);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawSquare(drawSubdivides, TEXTURE_WATER);
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Animals
 * ----------------------------------------------------------------------------
 * Pig
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Kopf des Schweins.
 */
static void drawPigHead(void)
{
  drawSphere(drawSubdivides, TEXTURE_PAPER);
  
  /* Augen */
  materialSet(MAT_PIG_EYE);
  glPushMatrix();
    glTranslatef(0.4, 0.5, 0.7);
    glScalef(0.2, 0.2, 0.2);
    drawSphere(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.4, 0.5, 0.7);
    glScalef(0.2, 0.2, 0.2);
    drawSphere(drawSubdivides, TEXTURE_EMPTY);
  glPopMatrix();
  
  /* Schnauze */
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.9);
    glScalef(0.2, 0.2, 0.2);
    
    materialSet(MAT_PIG_NOSE);
    /* Nasenlöcher */
    glPushMatrix();
      glTranslatef(0.5, 0.0, 0.8);
      glRotatef(90, 1.0, 0.0, 0.0);
      glScalef(0.3, 0.3, 0.3);
      drawCylinder(drawSubdivides, 1.0, TEXTURE_EMPTY);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(- 0.5, 0.0, 0.8);
      glRotatef(90, 1.0, 0.0, 0.0);
      glScalef(0.3, 0.3, 0.3);
      drawCylinder(drawSubdivides, 1.0, TEXTURE_EMPTY);
    glPopMatrix();
    
    materialSet(MAT_PIG);
    
    glRotatef(90, 1.0, 0.0, 0.0);
    drawCylinder(drawSubdivides, 1.0, TEXTURE_EMPTY);
  glPopMatrix();
}

/**
 * Zeichnet das Schwein.
 */
extern void drawPig(void)
{
  glPushName(NAME_PIG_BODY);
    materialSet(MAT_PIG);
    
    /* Körper */
    glPushMatrix();
      glScalef(0.1, 0.1, 0.15);
      
      /* Beine */
      glPushMatrix();
        glTranslatef(0.5, - 1.0, 0.5);
        glScalef(0.2, 1.5, 0.2);
        drawCube(drawSubdivides, TEXTURE_PAPER);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(- 0.5, - 1.0, 0.5);
        glScalef(0.2, 1.5, 0.2);
        drawCube(drawSubdivides, TEXTURE_PAPER);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(0.5, - 1.0, - 0.5);
        glScalef(0.2, 1.5, 0.2);
        drawCube(drawSubdivides, TEXTURE_PAPER);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(- 0.5, - 1.0, - 0.5);
        glScalef(0.2, 1.5, 0.2);
        drawCube(drawSubdivides, TEXTURE_PAPER);
      glPopMatrix();
      
      drawSphere(drawSubdivides, TEXTURE_PAPER);
    glPopMatrix();
    
    /* Kopf */
    glPushName(NAME_PIG_HEAD);
      glPushMatrix();
        glTranslatef(0.0, 0.1, 0.1);
        glScalef(0.08, 0.08, 0.08);
        drawPigHead();
      glPopMatrix();
    glPopName();
    
    /* RINGEL-Schwanz */
    glPushMatrix();
      glTranslatef(0.0, 0.0, - 0.16);
      glScalef(0.02, 0.02, 0.02);
      glRotatef(270, 1.0, 0.0, 0.0);
      drawCylinder(drawSubdivides, 0.0, TEXTURE_PAPER);
    glPopMatrix();
  glPopName();
}

/* ----------------------------------------------------------------------------
 * Giraffe
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Kopf der Giraffe.
 */
extern void drawGiraffeHead(void)
{
  materialSet(MAT_GIRAFFE);
  glPushMatrix();
    glScalef(0.08, 0.08, 0.08);
    
    /* Hals */
    glPushMatrix();
      glScalef(0.2, 2.0, 0.2);
      glTranslatef(0.0, 1.0, 0.0);
      drawCylinder(drawSubdivides, 1.0, TEXTURE_GIRAFFE);
    glPopMatrix();
    
    /* Kopf */
    glTranslatef(0.0, 4.0, 0.0);
    glPushMatrix();
      glRotatef(45, 1.0, 0.0, 0.0);
      glScalef(0.8, 0.5, 1.0);
      drawSphere(drawSubdivides, TEXTURE_GIRAFFE);
    glPopMatrix();
    
    /* Augen */
    materialSet(MAT_PIG_EYE);
    glPushMatrix();
      glTranslatef(0.4, 0.6, 0.1);
      glScalef(0.2, 0.2, 0.2);
      drawSphere(drawSubdivides, TEXTURE_EMPTY);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(- 0.4, 0.6, 0.1);
      glScalef(0.2, 0.2, 0.2);
      drawSphere(drawSubdivides, TEXTURE_EMPTY);
    glPopMatrix();
    
    /* Nase */
    glPushMatrix();
      glTranslatef(0.0, 0.0, 0.7);
      glScalef(0.2, 0.2, 0.1);
      
      materialSet(MAT_GIRAFFE);
      drawSphere(drawSubdivides, TEXTURE_EMPTY);
    glPopMatrix();
  glPopMatrix();
}

/**
 * Zeichnet den Körper der Giraffe.
 */
extern void drawGiraffe(void)
{
  glPushName(NAME_GIRAFFE_BODY);
    materialSet(MAT_GIRAFFE);
    
    /* Körper */
    glPushMatrix();
      glScalef(0.1, 0.1, 0.15);
      
      /* Beine */
      glPushMatrix();
        glTranslatef(0.5, -1.0, 0.5);
        glScalef(0.2, 2.0, 0.2);
        drawCube(drawSubdivides, TEXTURE_GIRAFFE);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(- 0.5, - 1.0, 0.5);
        glScalef(0.2, 2.0, 0.2);
        drawCube(drawSubdivides, TEXTURE_GIRAFFE);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(0.5, - 1.0, -  0.5);
        glScalef(0.2, 2.0, 0.2);
        drawCube(drawSubdivides, TEXTURE_GIRAFFE);
      glPopMatrix();
      
      glPushMatrix();
        glTranslatef(- 0.5, - 1.0, - 0.5);
        glScalef(0.2, 2.0, 0.2);
        drawCube(drawSubdivides, TEXTURE_GIRAFFE);
      glPopMatrix();
      
      drawSphere(drawSubdivides, TEXTURE_GIRAFFE);
    glPopMatrix();    
  glPopName();
}

/* ----------------------------------------------------------------------------
 * Fish
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Fisch.
 */
extern void drawFish(void)
{
  materialSet(MAT_FISH);
  
  glPushName(NAME_FISH_BODY);
    glPushMatrix();
      glScalef(0.1, 0.06, 0.03);
      drawSphere(drawSubdivides, TEXTURE_FISH);
      
      glPushMatrix();
        glScalef(0.2, 0.5, 0.5);
        glTranslatef(3.0, 1.0, 1.0);
        drawSphere(drawSubdivides, TEXTURE_EMPTY);
      glPopMatrix();
      
      glPushMatrix();
        glScalef(0.2, 0.5, 0.5);
        glTranslatef(3.0, 1.0, -1.0);
        drawSphere(drawSubdivides, TEXTURE_EMPTY);
      glPopMatrix();
    glPopMatrix();
  glPopName();
}

/* ----------------------------------------------------------------------------
 * Rest
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Grasboden.
 */
extern void drawGround(void)
{
  materialSet(MAT_GRASS);
  
  glPushMatrix();
    glScalef(10.0, 1.0, 10.0);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawSquare(drawSubdivides, TEXTURE_GRASS);
  glPopMatrix();
  
  bindTexture(TEXTURE_EMPTY);
}

/**
 * Zeichnet den Steinsockel.
 */
extern void drawSocket(void)
{
  materialSet(MAT_STONE);
  
  glPushMatrix();
    glScalef(2.0, 0.2, 1.2);
    drawCube(drawSubdivides, TEXTURE_SOCKET);
  glPopMatrix();
}

/**
 * Zeichnet die Sonne.
 */
extern void drawSun(void)
{
  materialSet(MAT_SUN);
  
  drawSphere(drawSubdivides, TEXTURE_SUN);
}

/**
 * Zeichnet einen giraffigen Würfel.
 */
extern void drawGiraffeCube(void)
{
  materialSet(MAT_GIRAFFE);
  
  glDisable(GL_LIGHTING);
  
  glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    drawCube(drawSubdivides, TEXTURE_GIRAFFE);
  glPopMatrix();
  
  glEnable(GL_LIGHTING);
}

/**
 * Zeichnet nichts.
 */
extern void drawNothing(void)
{
}
