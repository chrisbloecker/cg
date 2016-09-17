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
#include <math.h>
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#include <GL/glut.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "drawing.h"
#include "vector.h"
#include "material.h"
#include "displaylist.h"

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

#define even(n) (((n) % 2) ? ((n) - 1) : (n))

#define max(m,n) ((m) > (n) ? (m) : (n))
#define min(m,n) ((m) < (n) ? (m) : (n))

#define growNone         (0)
#define growConst(n)     (2)
#define growLinear(n)    (even(n))
#define growQuadratic(n) (even((n) * (n)))
#define growCubic(n)     (even((n) * (n) * (n)))

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
#define CYLINDER     (1.0)
#define KEGELDINGENS (0.9)

/** Level 1 */
#define SUBDIVIDE_STREET            (min(growQuadratic(drawSubdivides), 16))
#define SUBDIVIDE_STREET_BACKGROUND (min(growCubic(drawSubdivides), 16))
#define SUBDIVIDE_STREET_SURFACE    (min(growLinear(drawSubdivides), 8))
#define SUBDIVIDE_CAR               (min(growLinear(drawSubdivides), 4))
#define SUBDIVIDE_CAR_TYRE          (min(growLinear(drawSubdivides), 2))
#define SUBDIVIDE_TREE_TRUNK        (min(growLinear(drawSubdivides), 2))
#define SUBDIVIDE_TREE_CROWN        (min(growLinear(drawSubdivides), 4))
#define SUBDIVIDE_COIN              (growConst(drawSubdivides))
#define SUBDIVIDE_HOUSE             (min(growLinear(drawSubdivides), 8))
#define SUBDIVIDE_HUGO              (min(growLinear(drawSubdivides), 4))

/** Level 2 */
#define SUBDIVIDE_RIVER             (SUBDIVIDE_STREET)
#define SUBDIVIDE_RIVER_BACKGROUND  (SUBDIVIDE_STREET_BACKGROUND)
#define SUBDIVIDE_RIVER_SURFACE     (SUBDIVIDE_STREET_SURFACE)
#define SUBDIVIDE_SHIP              (SUBDIVIDE_CAR)
#define SUBDIVIDE_SHIP_SAIL         (SUBDIVIDE_CAR)
#define SUBDIVIDE_BUOY              (SUBDIVIDE_TREE_CROWN)
#define SUBDIVIDE_STARFISH          (SUBDIVIDE_COIN)

/** Level 3 */
#define SUBDIVIDE_BEACH             (SUBDIVIDE_STREET)
#define SUBDIVIDE_BEACH_BACKGROUND  (SUBDIVIDE_STREET_BACKGROUND)
#define SUBDIVIDE_BEACH_SURFACE     (SUBDIVIDE_STREET_SURFACE)
#define SUBDIVIDE_CRAB              (SUBDIVIDE_CAR)
#define SUBDIVIDE_PALM_TRUNK        (growNone)
#define SUBDIVIDE_PALM_LEAF         (growNone)
#define SUBDIVIDE_COCONUT           (growConst(drawSubdivides))
#define SUBDIVIDE_SHELL             (SUBDIVIDE_COIN)
#define SUBDIVIDE_LIGHTHOUSE        (min(growLinear(drawSubdivides), 4))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

static int drawMode       = GL_TRIANGLES
         , drawSubdivides = 0;

static Boolean normals = FALSE;

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet ein Dreieck mit den Eckpunkten p, q, r.
 *
 * @param[in] p Erster  Eckpunkt.
 * @param[in] q Zweiter Eckpunkt.
 * @param[in] r Dritter Eckpunkt.
 */
static void drawTriangle(Vector3d p, Vector3d q, Vector3d r)
{
  if (normals)
  {
    Vector3d n = vectorCross(vectorSub(q, p), vectorSub(r, p));
    
    double norm = 1.0 / vectorLength(n);
    
    n = vectorScale(n, norm, norm, norm);

    glDisable (GL_LIGHTING);
      glBegin (GL_LINES);
        glColor3f (0., 0., 1.);
        
        glVertex3f(q.x, q.y, q.z);
        glVertex3f(q.x + n.x, q.y + n.y, q.z + n.z);
      glEnd ();
    glEnable (GL_LIGHTING);
  }
  
  glBegin(drawMode);
    glVertex3f(p.x, p.y, p.z);
    glVertex3f(q.x, q.y, q.z);
    glVertex3f(r.x, r.y, r.z);
  glEnd();
}

/**
 * Wenn i > 0, so wird das durch p, q, r definierte Dreieck feiner unterteilt,
 * sonst wird es gezeichnet.
 *
 * @param[in] p Erster  Eckpunkt.
 * @param[in] q Zweiter Eckpunkt.
 * @param[in] r Dritter Eckpunkt.
 * @param[in] i Unterteilungstiefe ... Rekursion ...
 */
static void drawTriangleSubdivide(Vector3d p, Vector3d q, Vector3d r, unsigned i)
{
  if (i)
  {
    Vector3d s = vectorScale(vectorAdd(p, q), 0.5, 0.5, 0.5);
    
    --i;
    
    drawTriangleSubdivide(p, r, s, i);
    drawTriangleSubdivide(r, q, s, i);
  }
  else
  {
    drawTriangle(p, q, r);
  }
}

/**
 * Zeichnen eines Dreiecks mit den Eckpunkten p, q, r.
 * Berechnet die Normale dür das Dreieck und setzt sie.
 *
 * @param[in] p Erster  Eckpunkt.
 * @param[in] q Zweiter Eckpunkt.
 * @param[in] r Dritter Eckpunkt.
 * @param[in] i Unterteilungstiefe ... Rekursion ...
 */
static void drawTriangleRecursive(Vector3d p, Vector3d q, Vector3d r, unsigned i)
{
  Vector3d n = vectorCross(vectorSub(r, p), vectorSub(q, p));
  
  double norm = 1.0 / vectorLength(n);
  
  n = vectorScale(n, norm, norm, norm);
  if (normals)
  {
    glDisable (GL_LIGHTING);
      glBegin (GL_LINES);
        glColor3f (1., 0., 0.);

        glVertex3f(p.x, p.y, p.z);
        glVertex3f(p.x + n.x, p.y + n.y, p.z + n.z);
      glEnd ();
    glEnable (GL_LIGHTING);
  }
  
  glNormal3f(n.x, n.y, n.z);
  
  drawTriangleSubdivide(p, r, q, i);
}

/**
 * Zeichnet ein Quadrat im Ursprung.
 *
 * @param[in] subdivides Feinheit der Figur.
 */
static void drawSquare(unsigned subdivides)
{
  Vector3d a = vectorMake(- 1.0,   0.0,   1.0)
         , b = vectorMake(  1.0,   0.0,   1.0)
         , c = vectorMake(  1.0,   0.0, - 1.0)
         , d = vectorMake(- 1.0,   0.0, - 1.0)
         ;
    
  drawTriangleRecursive(a, c, b, subdivides);
  drawTriangleRecursive(c, a, d, subdivides);
}

/**
 * Zeichnet einen Würfel.
 *
 * @param[in] subdivides Feinheit der Figur.
 */
static void drawCube(unsigned subdivides)
{
  /* Oben */
  glPushMatrix ();
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
  
  /* Unten */
  glPushMatrix ();
    glRotatef(180, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
  
  /* Vorne */
  glPushMatrix ();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
  
  /* Hinten */
  glPushMatrix ();
    glRotatef(-90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
  
  /* Rechts */
  glPushMatrix ();
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
  
  /* Links */
  glPushMatrix ();
    glRotatef(-90, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);
    drawSquare(subdivides);
  glPopMatrix ();
}

/**
 * Zeichnet ein Tetraeder.
 *
 * @param[in] subdivides Feinheit der Figur.
 */
static void drawTetraeder(unsigned subdivides)
{
  Vector3d a = vectorMake(  0.0,   1.0,   0.0)
         , b = vectorMake(- 1.0, - 1.0,   1.0)
         , c = vectorMake(  1.0, - 1.0,   1.0)
         , d = vectorMake(  0.0, - 1.0, - 1.0)
         ;
  
  drawTriangleRecursive(a, c, b, subdivides);
  drawTriangleRecursive(a, d, c, subdivides);
  drawTriangleRecursive(a, b, d, subdivides);
  drawTriangleRecursive(d, b, c, subdivides);
}

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] subdivides Feinheit der Figur.
 * @param[in] proportion Verhältnis des Radius des oberen Kreises zum unteren.
 */
static void drawCylinder(unsigned subdivides, double proportion)
{
  /*#define PIECES (8)
  
  if (!subdivides)
    subdivides += 2;

  drawNShape(subdivides * PIECES, subdivides, proportion);
  
  #undef PIECES*/
  GLUquadric * q;
  q = gluNewQuadric();
  
  gluQuadricNormals(q, GL_SMOOTH);
  
  glPushMatrix();
    glRotatef(- 90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, - 1.0);
    gluCylinder(q, 1.0, proportion, 2.0, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(- 90, 1.0, 0.0, 0.0);
    gluDisk(q, 0.0, proportion, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, - 1.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluDisk(q, 0.0, 1.0, 20, 20);
  glPopMatrix();
  
  gluDeleteQuadric(q);
}

/**
 * Zeichnet eine Einheitskugel mit dem Radius 1 im Ursprung.
 *
 * @param[in] subdivides Feinheit der Figur.
 */
static void drawSphere(unsigned subdivides)
{
  /*#define PIECES (12)
  
  unsigned i
         , j
         , iter
         ;
  
  Vector3d a = vectorMake(0.0, 0.0, 1.0)
         , b
         , c
         , d
         , e
         , f
         ;
  
  if (!subdivides)
    ++subdivides;
  
  iter = PIECES * subdivides;
  
  for (i = 0; i < iter; ++i)
  {
    b = vectorRotateY(a, 360.0 / iter);
    
    for (j = 0; j < iter; ++j)
    {
      c = vectorRotateX(a, j * 360.0 / iter);
      d = vectorRotateX(b, j * 360.0 / iter);
      e = vectorRotateX(c,     360.0 / iter);
      f = vectorRotateX(d,     360.0 / iter);
      
      drawTriangleRecursive(d, e, c, 0);
      drawTriangleRecursive(f, e, d, 0);
    }
    
    a = vectorRotateY(a, 360.0 / iter);
  }
  
  #undef PIECES*/
  GLUquadric * q;
  q = gluNewQuadric();
  
  gluQuadricNormals(q, GL_SMOOTH);
  
  gluSphere(q, 1.0f, 20, 20);
  
  gluDeleteQuadric(q);
}

/**
 * Setzt den Zeichenmodus.
 *
 * @param[in] mode Zeichenmodus.
 */
extern void drawingSetMode(int mode)
{
  drawMode = mode;
  
  displaylistSet();
}

/**
 * Setzt den Zeichenmodus.
 *
 * @param[in] mode Zeichenmodus.
 */
extern void drawingSetNormals(void)
{
  normals = !normals;
  
  displaylistSet();
  
  fprintf(stderr, "DEBUG :: normals %s\n", normals ? "on" : "off");
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
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Help
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet einen Palmenast mit Blättern.
 *
 * @param[in] subdivides Feinheit der Figur.
 */
static void drawPalmLeaf(unsigned subdivides)
{
  Vector3d a = vectorMake(- 1.0,   0.0,    0.0)
         , b = vectorMake(  1.0,   0.0,    0.0)
         , c = vectorMake(  0.0,   0.75, - 1.0)
         , m = vectorMake(  0.0,   0.75,   0.0)
         ; 
  
  glPushMatrix();
    drawTriangleRecursive(a, c, m, subdivides);
    drawTriangleRecursive(m, c, b, subdivides);
    glTranslatef(0.0, 0.0, - 1.0);
    drawTriangleRecursive(a, c, m, subdivides);
    drawTriangleRecursive(m, c, b, subdivides);
    glTranslatef(0.0, 0.0, - 1.0);
    drawTriangleRecursive(a, c, m, subdivides);
    drawTriangleRecursive(m, c, b, subdivides);
    glTranslatef(0.0, 0.0, - 1.0);
    drawTriangleRecursive(a, c, m, subdivides);
    drawTriangleRecursive(m, c, b, subdivides);
    glTranslatef(0.0, 0.0, - 1.0);
    drawTriangleRecursive(a, c, m, subdivides);
    drawTriangleRecursive(m, c, b, subdivides);
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Ground
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet die Straße mit den Begrenzungslinien.
 */
extern void drawStreet(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Street, ");
  #endif
  
  /* Belag */
  materialSet(MAT_STREET);
  drawSquare(SUBDIVIDE_STREET);
  
  /* Mit Fahrbahnmarkierung */
  materialSet(MAT_STREET_SURFACE);
  glPushMatrix();
    glTranslatef(- 1.0, 0.01, 0.0);
    glScalef(0.03, 1.0, 1.0);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(1.0, 0.01, 0.0);
    glScalef(0.03, 1.0, 1.0);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet den Fluss
 */
extern void drawRiver(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing River, ");
  #endif
  
  /* Belag */
  materialSet(MAT_RIVER);
  drawSquare(SUBDIVIDE_RIVER);
  
  /* Mit Rand */
  materialSet(MAT_RIVER_SURFACE);
  glPushMatrix();
    glTranslatef(- 1.0, 0.01, 0.0);
    glScalef(0.03, 1.0, 1.0);
    drawSquare(SUBDIVIDE_RIVER_SURFACE);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(1.0, 0.01, 0.0);
    glScalef(0.03, 1.0, 1.0);
    drawSquare(SUBDIVIDE_RIVER_SURFACE);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet den Strand
 */
extern void drawBeach(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Beach, ");
  #endif
  
  /* Gelb-oranger Sand */
  materialSet(MAT_BEACH);
  drawSquare(SUBDIVIDE_BEACH);
  
  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Background
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Hintergrund der Strasse
 * Dieser ist statisch als Hintergrund zur Szene
 */
extern void drawStreetBackground(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Street Background, ");
  #endif
  
  materialSet(MAT_STREET_BACKGROUND);
  
  glPushMatrix();
    glTranslatef(0.0, - 0.01, 0.0);
    glScalef(2.0, 1.0, 2.0);
    drawSquare(SUBDIVIDE_STREET_BACKGROUND);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet den Hintergrund vom Fluss
 */
extern void drawRiverBackground(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing River Background, ");
  #endif
  
  /* Blaues Wasser */
  materialSet(MAT_RIVER_BACKGROUND);
  
  glPushMatrix();
    glTranslatef(0.0, - 0.01, 0.0);
    glScalef(2.0, 1.0, 2.0);
    drawSquare(SUBDIVIDE_RIVER_BACKGROUND);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet den Hintergrund des Beach
 */
extern void drawBeachBackground(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Beach Background, ");
  #endif
  
  materialSet(MAT_STREET_BACKGROUND);
  
  glPushMatrix();
    glTranslatef(0.0, - 0.01, 0.0);
    glScalef(2.0, 1.0, 2.0);
    drawSquare(SUBDIVIDE_BEACH_BACKGROUND);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Surface
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet den Strassenbelag
 */
extern void drawStreetSurface(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Street Surface, ");
  #endif
  
  materialSet(MAT_STREET_SURFACE);
  
  glPushMatrix();
    glTranslatef(- 0.2, 0.01, 0.0);
    glScalef(0.018, 1.0, 0.15);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.2, 0.01, 0.0);
    glScalef(0.018, 1.0, 0.15);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet die Wellen auf dem Fluss
 */
extern void drawRiverSurface(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing River Surface, ");
  #endif
  
  materialSet(MAT_RIVER_SURFACE);
  glPushMatrix();
    glTranslatef(- 0.25, 0.01, 0.0);
    glRotatef(45, 0.0, 1.0, 0.0);
    glScalef(0.02, 1.0, 0.2);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  materialSet(MAT_RIVER_SURFACE);
  glPushMatrix();
    glTranslatef(- 0.0, 0.01, 0.0);
    glRotatef(315, 0.0, 1.0, 0.0);
    glScalef(0.02, 1.0, 0.2);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  materialSet(MAT_RIVER_SURFACE);
  glPushMatrix();
    glTranslatef(0.25, 0.01, 0.0);
    glRotatef(45, 0.0, 1.0, 0.0);
    glScalef(0.02, 1.0, 0.2);
    drawSquare(SUBDIVIDE_STREET_SURFACE);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet den Sand am Strand
 */
extern void drawBeachSurface(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Beach Surface, ");
  #endif
  
  materialSet(MAT_BEACH_SURFACE);
  glPushMatrix();
    glTranslatef(- 0.2, 0.001, - 0.2);
    glScalef(0.03, 0.03, 0.03);
    drawSphere(SUBDIVIDE_BEACH_SURFACE);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.2, -0.001, - 0.2);
    glScalef(0.03, 0.03, 0.03);
    drawSphere(SUBDIVIDE_BEACH_SURFACE);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.2, -0.001, 0.2);
    glScalef(0.03, 0.03, 0.03);
    drawSphere(SUBDIVIDE_BEACH_SURFACE);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.2, -0.001, 0.2);
    glScalef(0.03, 0.03, 0.03);
    drawSphere(SUBDIVIDE_BEACH_SURFACE);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Environment
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet einen Baum im Ursprung
 */
extern void drawTree(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Tree, ");
  #endif
  
  materialSet(MAT_TREE_TRUNK);
  
  /* Stamm */
  glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(0.2, 0.5, 0.2);
    drawCylinder(SUBDIVIDE_TREE_TRUNK, CYLINDER);
  glPopMatrix();
  
  materialSet(MAT_TREE_CROWN);
  
  /* Krone */
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(SUBDIVIDE_TREE_CROWN);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet eine Boje
 */
extern void drawBuoy(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Buoy, ");
  #endif
  
  materialSet(MAT_BUOY);
  
  glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(0.5, 0.3, 0.5);
    drawSphere(SUBDIVIDE_BUOY);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet eine Palme im Ursprung
 */
extern void drawPalm(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Palm, ");
  #endif
  
  /* Stamm */
  materialSet(MAT_PALM_TRUNK_DARK);
  
  glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.2, 0.1, 0.2);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_PALM_TRUNK_LIGHT);
  
  glPushMatrix();
    glTranslatef(0.0, 0.4, 0.0);
    glScalef(0.18, 0.1, 0.18);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_PALM_TRUNK_DARK);
  
  glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.162, 0.1, 0.162);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_PALM_TRUNK_LIGHT);
  
  glPushMatrix();
    glTranslatef(0.0, 0.8, 0.0);
    glScalef(0.1458, 0.1, 0.1458);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_PALM_TRUNK_DARK);
  
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.13122, 0.1, 0.13122);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_PALM_TRUNK_LIGHT);
  
  glPushMatrix();
    glTranslatef(0.0, 1.2, 0.0);
    glScalef(0.118098, 0.1, 0.118098);
    drawCylinder(SUBDIVIDE_PALM_TRUNK, 0.9);
  glPopMatrix();
  
  materialSet(MAT_COCONUT);
  
  /* Nuss */
  glPushMatrix();
    glTranslatef(0.0, 1.1, 0.1);
    glScalef(0.1, 0.1, 0.1);
    drawSphere(SUBDIVIDE_COCONUT);
  glPopMatrix();
  
  materialSet(MAT_PALM_LEAF);
  
  /* Blätter */
  glPushMatrix();
    glTranslatef(0.0, 1.301, 0.0);
    glScalef(0.2, 0.2, 0.2);
    drawPalmLeaf(SUBDIVIDE_PALM_LEAF);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.301, 0.0);
    glScalef(0.2, 0.2, 0.2);
    drawPalmLeaf(SUBDIVIDE_PALM_LEAF);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(2 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.301, 0.0);
    glScalef(0.2, 0.2, 0.2);
    drawPalmLeaf(SUBDIVIDE_PALM_LEAF);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(3 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.301, 0.0);
    glScalef(0.2, 0.2, 0.2);
    drawPalmLeaf(SUBDIVIDE_PALM_LEAF);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(4 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.301, 0.0);
    glScalef(0.2, 0.2, 0.2);
    drawPalmLeaf(SUBDIVIDE_PALM_LEAF);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Items
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet eine Münze im Ursprung
 */
extern void drawCoin(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Coin, ");
  #endif
  
  materialSet(MAT_COIN);
  
  glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.75, 0.1, 0.75);
    drawCylinder(SUBDIVIDE_COIN, CYLINDER);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet einen Seestern im Ursprung
 */
extern void drawStarfish(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Starfish, ");
  #endif
  
  materialSet(MAT_STARFISH);
  
  glPushMatrix();
    glTranslatef(0.0, 0.5, - 0.5);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawTetraeder(SUBDIVIDE_STARFISH);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.5, - 0.5);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawTetraeder(SUBDIVIDE_STARFISH);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(2 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.5, - 0.5);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawTetraeder(SUBDIVIDE_STARFISH);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(3 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.5, - 0.5);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawTetraeder(SUBDIVIDE_STARFISH);
  glPopMatrix();
  
  glPushMatrix();
    glRotatef(4 * 72, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.5, - 0.5);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(270, 1.0, 0.0, 0.0);
    drawTetraeder(SUBDIVIDE_STARFISH);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glScalef(0.35, 0.3, 0.35);
    drawCylinder(SUBDIVIDE_STARFISH, CYLINDER);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet ein Eis im Ursprung
 */
extern void drawIce(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Shell, ");
  #endif
    
  materialSet(MAT_ICE_BANANA);
  glPushMatrix();
    glTranslatef(0.1, 1.5, 0.1);
    glScalef(0.25, 0.25, 0.25);
    drawSphere(SUBDIVIDE_SHELL);
  glPopMatrix();
  
  materialSet(MAT_ICE_STRAWBERRY);
  glPushMatrix();
    glTranslatef(- 0.1, 1.5, 0.1);
    glScalef(0.25, 0.25, 0.25);
    drawSphere(SUBDIVIDE_SHELL);
  glPopMatrix();
  
  materialSet(MAT_ICE_CHOCOLATE);
  glPushMatrix();
    glTranslatef(0.0, 1.5, - 0.1);
    glScalef(0.25, 0.25, 0.25);
    drawSphere(SUBDIVIDE_SHELL);
  glPopMatrix();

  /* Waffel */
  materialSet(MAT_ICE_WAFFLE);
  glPushMatrix();
    glTranslatef(0.0, 0.8, 0.0);
    glRotatef(180, 1.0, 0.0, 0.0);
    glScalef(0.35, 0.6, 0.35);
    drawCylinder(SUBDIVIDE_SHELL, 0.0);
  glPopMatrix();

  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Obstacles
 * -------------------------------------------------------------------------- */
 
/**
 * Zeichnet ein Auto
 */
extern void drawCar(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Car, ");
  #endif
  
  materialSet(MAT_CAR);
  
  /* Karosserie */
  glPushMatrix();
    glTranslatef(0.0, 0.4, 0.0);
    glScalef(1.0, 0.3, 1.0);
    drawCube(SUBDIVIDE_CAR);
  glPopMatrix();
  
  /* Kabine */
  materialSet(MAT_CAR_CABIN);
  
  glPushMatrix();
    glTranslatef(0.0, 0.8, - 0.2);
    glScalef(1.0, 0.2, 0.7);
    drawCube(SUBDIVIDE_CAR);
  glPopMatrix();
  
  materialSet(MAT_CAR_FRONTLIGHT);
  
  /* Scheinwerfer */
  glPushMatrix();
    glTranslatef(0.8, 0.5, 1.0);
    glScalef(0.1, 0.05, 0.05);
    drawCube(0);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.8, 0.5, 1.0);
    glScalef(0.1, 0.05, 0.05);
    drawCube(0);
  glPopMatrix();
  
  materialSet(MAT_CAR_TYRE);
  
  /* Räder */
  glPushMatrix();
    glTranslatef(1.0, 0.4, 0.5);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(0.25, 0.1, 0.25);
    drawCylinder(SUBDIVIDE_CAR_TYRE, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(1.0, 0.4, - 0.5);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(0.25, 0.1, 0.25);
    drawCylinder(SUBDIVIDE_CAR_TYRE, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 1.0, 0.4, 0.5);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(0.25, 0.1, 0.25);
    drawCylinder(SUBDIVIDE_CAR_TYRE, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 1.0, 0.4, - 0.5);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(0.25, 0.1, 0.25);
    drawCylinder(SUBDIVIDE_CAR_TYRE, CYLINDER);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet ein Schiff im Ursprung
 */
extern void drawShip(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Ship, ");
  #endif
  
  materialSet(MAT_SHIP);
  
  /* Hölzers */
  glPushMatrix();
    glTranslatef(- 0.75, 0.1, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.25, 1.0, 0.25);
    drawCylinder(SUBDIVIDE_SHIP, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.25, 0.1, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.25, 1.0, 0.25);
    drawCylinder(SUBDIVIDE_SHIP, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.25, 0.1, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.25, 1.0, 0.25);
    drawCylinder(SUBDIVIDE_SHIP, CYLINDER);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(0.75, 0.1, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.25, 1.0, 0.25);
    drawCylinder(SUBDIVIDE_SHIP, CYLINDER);
  glPopMatrix();
  
  /* Mast */
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glScalef(0.1, 1.5, 0.1);
    drawCube(SUBDIVIDE_SHIP);
  glPopMatrix();
  
  materialSet(MAT_SHIP_SAIL);
  
  /* Segel */
  glPushMatrix();
    glTranslatef(0.0, 1.8, 0.1);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.7, 0.01, 0.5);
    drawCube(SUBDIVIDE_SHIP_SAIL);
  glPopMatrix();

  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/**
 * Zeichnet einen Sonnenschirm im Ursprung
 */
extern void drawSunshade(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Crab, ");
  #endif
  
  materialSet(MAT_SUNSHADE_STICK);
  glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(180, 1.0, 0.0, 0.0);
    glScalef(0.1, 1.0, 0.1);
    drawCylinder(SUBDIVIDE_CRAB, CYLINDER);
  glPopMatrix();
  
  materialSet(MAT_SUNSHADE_TOP_OUTER);
  glPushMatrix();
    glTranslatef(0.0, 2.0, 0.0);
    glScalef(1.0, 0.1, 1.0);
    drawCylinder(SUBDIVIDE_CRAB, 0.0);
  glPopMatrix();
  
  materialSet(MAT_SUNSHADE_TOP_INNER);
  glPushMatrix();
    glTranslatef(0.0, 2.1, 0.0);
    glScalef(0.4, 0.1, 0.4);
    drawCylinder(SUBDIVIDE_CRAB, 0.0);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Hugo
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet Hugo
 */
extern void drawHugo(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Hugo, ");
  #endif
  
  /* Körper */
  materialSet(MAT_HUGO_BODY);
  
  glPushMatrix();
    glTranslatef(0.0, 1.3, 0.0);
    glScalef(0.3, 0.5, 0.18);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  /* Beine */
  materialSet(MAT_HUGO_LEGS);
  
  glPushMatrix();
    glTranslatef(0.2, 0.4, 0.0);
    glScalef(0.1, 0.5, 0.1);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.2, 0.4, 0.0);
    glScalef(0.1, 0.5, 0.1);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  /* Füße */
  materialSet(MAT_HUGO_FEET);
  
  glPushMatrix();
    glTranslatef(0.2, 0.0, - 0.15);
    glScalef(0.1, 0.1, 0.2);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.2, 0.0, - 0.15);
    glScalef(0.1, 0.1, 0.2);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  /* Arme */
  materialSet(MAT_HUGO_ARMS);
  
  glPushMatrix();
    glTranslatef(0.3, 1.4, 0.0);
    glScalef(0.1, 0.35, 0.1);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  glPushMatrix();
    glTranslatef(- 0.3, 1.4, 0.0);
    glScalef(0.1, 0.35, 0.1);
    drawCube(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  /* Kopf */
  materialSet(MAT_HUGO_HEAD);
  
  glPushMatrix();
    glTranslatef(0.0, 2.0, 0.0);
    glScalef(0.15, 0.2, 0.15);
    drawSphere(SUBDIVIDE_HUGO);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}

/* ----------------------------------------------------------------------------
 * Buildings
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet nichts
 */
extern void drawNothing(void)
{
}

/**
 * Zeichnet den Leuchtturm
 */
extern void drawLighthouse(void)
{
  #ifdef DEBUG
  int time = glutGet(GLUT_ELAPSED_TIME);
  
  fprintf(stderr, "DEBUG :: Drawing Lighthouse, ");
  #endif
  
  /* Die Birne ... */
  materialSet(MAT_CAR_FRONTLIGHT);
  glPushMatrix();
    glTranslatef(0.0, 0.95, 0.0);
    glScalef(0.05, 0.05, 0.05);
    drawSphere(2);
  glPopMatrix();
  
  materialSet(MAT_LIGHTHOUSE_RED);
  
  glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.25, 0.1, 0.25);
    drawCylinder(SUBDIVIDE_LIGHTHOUSE, 0.8);
  glPopMatrix();
  
  materialSet(MAT_LIGHTHOUSE_WHITE);
  
  glPushMatrix();
    glTranslatef(0.0, 0.4, 0.0);
    glScalef(0.2, 0.1, 0.2);
    drawCylinder(SUBDIVIDE_LIGHTHOUSE, 0.8);
  glPopMatrix();
  
  materialSet(MAT_LIGHTHOUSE_RED);
  
  glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.16, 0.1, 0.16);
    drawCylinder(SUBDIVIDE_LIGHTHOUSE, 0.8);
  glPopMatrix();
  
  materialSet(MAT_LIGHTHOUSE_WHITE);
  
  glPushMatrix();
    glTranslatef(0.0, 0.8, 0.0);
    glScalef(0.128, 0.1, 0.128);
    drawCylinder(SUBDIVIDE_LIGHTHOUSE, 0.8);
  glPopMatrix();
  
  materialSet(MAT_LIGHTHOUSE_RED);
  
  glPushMatrix();
    glTranslatef(0.0, 1.1, 0.0);
    glScalef(0.128, 0.1, 0.128);
    drawCylinder(SUBDIVIDE_LIGHTHOUSE, 0.2);
  glPopMatrix();
  
  #ifdef DEBUG
  fprintf(stderr, "\t\ttook %fs\n", (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0);
  #endif
}
