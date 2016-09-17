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
#include <stdlib.h>
#include <GL/glu.h>
#include <assert.h>
#include <float.h>
#include <time.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "vertexCube.h"
#include "vector6d.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define SCALE_NODES (0.01)

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

#define RANDOM_COLOR ((rand() % 100 + 1) * 0.01)

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Display-List für die weißen Spheren */
static int sphereDL;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Berechnet die Anzahl der Knoten pro Linie.
 *
 * @param[in] s Unterteilungstiefe.
 *
 * @return Anzahl der Knoten pro Linie.
 */
static unsigned nodesPerLine(unsigned s)
{
  return (1 << s) + 1;
}

/**
 * Berechnet die Anzahl der Indizes, die zum Zeichnen des Würfels erforderlich
 * sind.
 *
 * @param[in] s Unterteilungstiefe.
 *
 * @return Anzahl der nötigen Indizes.
 */
static unsigned numOfIndices(unsigned s)
{
  return (1 << ((1 + s) << 1)) * 6;
}

/**
 * Berechnet den Zeiger auf den Vertex an Position (x/y/z).
 *
 * @param[in] c Vertex-Cube.
 * @param[in] x X-Index.
 * @param[in] y Y-Index.
 * @param[in] z Z-Index.
 * @param[in] l Länge einer Seite in Vertices.
 *
 * @pre x muss innerhalb der Grenzen liegen
 * @pre y muss innerhalb der Grenzen liegen
 * @pre z muss innerhalb der Grenzen liegen
 *
 * @return Zeiger auf den gefragten Vertex.
 */
static Vertex * at(VertexCube c, unsigned x, unsigned y, unsigned z)
{
  unsigned n = nodesPerLine(c.s);
  
  assert(x < n);
  assert(y < n);
  assert(z < n);
  
  return &c.v[z * n * n + y * n + x];
}

/**
 * Übernimmt die Werte des Vektors v für den Vertex an Position (x/y/z) des
 * Würfels c.
 *
 * @param[in] c Vertex-Cube.
 * @param[in] x X-Index.
 * @param[in] y Y-Index.
 * @param[in] z Z-Index.
 * @param[in] v Vektor, der die Informationen enthält.
 *
 * @return Aktualisierter Vertex Cube.
 */
static VertexCube set(VertexCube c, unsigned x, unsigned y, unsigned z, Vector6d v)
{
  Vertex * d = at(c, x, y, z);
  
  (*d)[0] = v.x;
  (*d)[1] = v.y;
  (*d)[2] = v.z;
  (*d)[3] = v.r;
  (*d)[4] = v.g;
  (*d)[5] = v.b;
  
  return c;
}

/**
 * Interpoliert linear zwischen u und v (mit dem Skalar 0 <= a <= 1).
 *
 * @param[in] u Erster Vektor.
 * @param[in] v Zweiter Vektor.
 * @param[in] a Skalar zur Interpolation.
 *
 * @pre a >= 0.
 * @pre a <= 1.
 *
 * @return Zwischen u und v linear interpolierter Wert.
 */
static Vector6d interpolate(Vector6d u, Vector6d v, double a)
{
  assert(a > - DBL_EPSILON);
  assert(a < 1.0 + DBL_EPSILON);
  
  return vector6dAdd( vector6dScale(u, 1.0 - a)
                    , vector6dScale(v, a)
                    );
}

/**
 * Interpoliert die Unterwürfel des Würfels c.
 *
 * @param[in] c Zu interpolierender Würfel.
 * @param[in] i Gibt an, wieviele Stufen interpoliert werden müssen.
 *
 * @return Gefüllter Würfel.
 */
static VertexCube interpolateSubCubes(VertexCube c, unsigned i)
{
  unsigned ix /* Interpolation in x- */
         , iy /* y-                  */
         , iz /* und z-Richtung      */
         , x
         , y
         , z
         , n    = nodesPerLine(c.s) - 1 /* Anzahl der Knoten pro Linie im Würfel */
         , next = nodesPerLine(i)   - 1 /* Erster Knoten, der nicht interpoliert */
         ;                              /* werden muss                           */
  
  /* Vektoren, zwischen denen zu interpolieren ist */
  Vector6d v000
         , v100
         , v010
         , v110
         , v001
         , v101
         , v011
         , v111
         ;
  
  /* Interpolation über alle Unterwürfel, die bekannte Informationen enthalten */
  for (x = 0; x < n; x += next)
    for (y = 0; y < n; y += next)
      for (z = 0; z < n; z += next)
      {
        v000 = vertexMakeVector6dFromVertex(at(c, x,        y,        z       ));
        v100 = vertexMakeVector6dFromVertex(at(c, x + next, y,        z       ));
        v010 = vertexMakeVector6dFromVertex(at(c, x,        y + next, z       ));
        v110 = vertexMakeVector6dFromVertex(at(c, x + next, y + next, z       ));
        v001 = vertexMakeVector6dFromVertex(at(c, x,        y,        z + next));
        v101 = vertexMakeVector6dFromVertex(at(c, x + next, y,        z + next));
        v011 = vertexMakeVector6dFromVertex(at(c, x,        y + next, z + next));
        v111 = vertexMakeVector6dFromVertex(at(c, x + next, y + next, z + next));
        
        /* Zwischen den bekannten Vertices interpolieren */
        for (ix = 0; ix < 3; ++ix)
          for (iy = 0; iy < 3; ++iy)
            for (iz = 0; iz < 3; ++iz)
              /* Aber nur die Vertices, die es nötig haben */
              if (ix & 1 || iy & 1 || iz & 1)
                set( c
                   , x + ix * (next >> 1)
                   , y + iy * (next >> 1)
                   , z + iz * (next >> 1)
                   , interpolate( interpolate( interpolate(v000, v100, ix * 0.5)
                                             , interpolate(v010, v110, ix * 0.5)
                                             , iy * 0.5
                                             )
                                , interpolate( interpolate(v001, v101, ix * 0.5)
                                             , interpolate(v011, v111, ix * 0.5)
                                             , iy * 0.5
                                             )
                                , iz * 0.5
                                )
                   );
      }
  
  /* Falls notwendig, weitere Stufen interpolieren */
  if (i - 1)
    interpolateSubCubes(c, i - 1);
  
  return c;
}

/**
 * Reserviert Speicher für den Würfel c.
 *
 * @param[in] c Würfel, für den Speicher zu besorgen ist.
 *
 * @pre Es darf kein Speicher alloziiert werden, wenn der Würfel bereits über
 *      Speicher verfügt.
 *
 * @return Würfel, für den Speicher geholt wurde.
 */
static VertexCube getMem(VertexCube c)
{
  /* Gefahr von Speicherlecks */
  assert(c.v == NULL);
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Allocating space for %i nodes and %i indices\n"
                , vertexCubeNumOfNodes(c.s)
                , numOfIndices(c.s)
                );
  #endif
  
  c.v = malloc(vertexCubeNumOfNodes(c.s) * sizeof(Vertex)); /* Vertices */
  c.i = malloc(numOfIndices(c.s)         * sizeof(GLuint)); /* Indizes  */
  
  return c;
}

/**
 * Füllt einen Vertex-Cube mit Informationen.
 * Hierzu werden die Informationen für die Vertices aus den Informationen der
 * Vertices an den äußeren Ecken interpoliert.
 *
 * @param[in] d Ziel-Würfel.
 * @param[in] s Quell-Würfel.
 *
 * @return Gefüllter Würfel.
 */
static VertexCube fillNodes(VertexCube d, VertexCube s)
{
  unsigned dm
         , sm
         , n
         , x
         , y
         , z
         ;
  
  /* Anzahl der Knoten pro Linie beim kleineren Würfel */
  n = nodesPerLine(d.s < s.s ? d.s : s.s);
  
  /* Multiplikatoren für den Zugriff auf die Attribute bestimmen */
  dm = d.s < s.s ? 1 : 1 << abs(d.s - s.s);
  sm = d.s > s.s ? 1 : 1 << abs(d.s - s.s);
  
  /* Attribute vom Quellwürfel kopieren */
  for (x = 0; x < n; ++x)
    for (y = 0; y < n; ++y)
      for (z = 0; z < n; ++z)
        vertexCopy(at(s, sm * x, sm * y, sm * z), at(d, dm * x, dm * y, dm * z));
  
  /* Interpolation der fehlenden Werte (nur) bei Verfeinerung */
  if (d.s > s.s)  
    d = interpolateSubCubes(d, d.s - s.s);
  
  return d;
}

/**
 * Berechnet die Reihenfolge, in der die Vertices gezeichnet werden müssen.
 *
 * @param[in] c Würfel.
 *
 * @return Würfel mit berechneter Zeichenreihenfolge der Vertices.
 */
static VertexCube fillIndices(VertexCube c)
{
  unsigned i = 0
         , n = nodesPerLine(c.s) - 1
         , x
         , y
         , z
         ;
  
  x = 0;
  for (z = 0; z < n; ++z)
    for (y = 0; y < n; ++y)
    {
      c.i[i++] = (GLuint) (at(c, x,     y, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y,     z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y + 1, z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y + 1, z    ) - c.v);
    }
  
  x = n;
  for (z = 0; z < n; ++z)
    for (y = 0; y < n; ++y)
    {
      c.i[i++] = (GLuint) (at(c, x,     y, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y + 1, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y + 1, z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x, y,     z + 1) - c.v);
    }
  
  y = 0;
  for (z = 0; z < n; ++z)
    for (x = 0; x < n; ++x)
    {
      c.i[i++] = (GLuint) (at(c, x,     y, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y, z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x,     y, z + 1) - c.v);
    }
  
  y = n;
  for (z = 0; z < n; ++z)
    for (x = 0; x < n; ++x)
    {
      c.i[i++] = (GLuint) (at(c, x,     y, z    ) - c.v);
      c.i[i++] = (GLuint) (at(c, x,     y, z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y, z + 1) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y, z    ) - c.v);
    }
  
  z = 0;
  for (y = 0; y < n; ++y)       
    for (x = 0; x < n; ++x)
    {
      c.i[i++] = (GLuint) (at(c, x,     y,     z) - c.v);
      c.i[i++] = (GLuint) (at(c, x    , y + 1, z) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y + 1, z) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y,     z) - c.v);
    }
  
  z = n;
  for (y = 0; y < n; ++y)       
    for (x = 0; x < n; ++x)
    {
      c.i[i++] = (GLuint) (at(c, x,     y,     z) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y,     z) - c.v);
      c.i[i++] = (GLuint) (at(c, x + 1, y + 1, z) - c.v);
      c.i[i++] = (GLuint) (at(c, x    , y + 1, z) - c.v);
    }
  
  return c;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Berechnet die Anzahl der Knoten des Würfels.
 *
 * @param[in] s Unterteilungstiefe.
 *
 * @return Anzahl der Knoten des Würfels.
 */
extern unsigned vertexCubeNumOfNodes(unsigned s)
{
  return ((1 << s) + 1) * ((1 << s) + 1) * ((1 << s) + 1);
}

/**
 * Initialisiert den Vertex-Cube c.
 *
 * @param[in] c Zu initialisierender Vertex-Cube.
 */
extern VertexCube vertexCubeInit(VertexCube c)
{
  unsigned x
         , y
         , z
         ;
  
  GLUquadric * sphere;
  
  /* "Zufallszahlen" erzeugen */
  srand(time(NULL));
  
  /* Würfel initialisieren und Speicher besorgen */
  c.s = 0;
  c = getMem(c);
  
  /* Eckpunkte füllen */
  for (x = 0; x <= 1; ++x)
    for (y = 0; y <= 1; ++y)
      for (z = 0; z <= 1; ++z)
        c = set(c, x, y, z, vector6dMake(x, y, z, x, y, z));
  
  /* Zeichenreihenfolge berechnen */
  c = fillIndices(c);
  
  /* Display-List für Kugeln erzeugen */
  sphereDL = glGenLists(1);
  
  /* Kugel erzeugen */
  sphere = gluNewQuadric();
  gluQuadricNormals(sphere, GL_SMOOTH);
  
  /* Display-Liste befüllen */
  glNewList(sphereDL, GL_COMPILE);
    glColor3f(1.0, 1.0, 1.0);
    gluSphere(sphere, 1.0f, 15, 15);
  glEndList();
  
  gluDeleteQuadric(sphere);
  
  return c;
}

/**
 * Gibt den vom Vertex-Cube c belegten Speicher frei.
 *
 * @param[in] c Zu löschender Vertex-Cube.
 */
extern VertexCube vertexCubeClear(VertexCube c)
{
  if (c.v)
    free(c.v);
  
  if (c.i)
    free(c.i);
  
  c.v = NULL;
  c.i = NULL;
  c.s = 0;
  
  return c;
}

/**
 * Erzeugt einen Würfel der "Größe" s und übernimmt dabei die vorhandenen
 * Informationen aus c.
 *
 * @param[in] c Würfel, der bereits Informationen enthält.
 * @param[in] s "Größe" des neuen Würfels.
 */
extern VertexCube vertexCubeMake(VertexCube c, unsigned s)
{
  VertexCube res;
  
  res.v = NULL;
  res.s = s;
  
  /* Attribute retten, berechnen und interpolieren */
  res = fillIndices(fillNodes(getMem(res), c));
  
  /* Alten Würfel löschen */
  vertexCubeClear(c);
  
  return res;
}

/**
 * Zeichnet die Knoten des Würfels c.
 *
 * @param[in] c        Würfel.
 * @param[in] selectes Index des ausgewählten Knotens.
 */
extern void vertexCubeDrawNodes(VertexCube c, unsigned selected)
{
  unsigned i;
  
  Vertex * v;
  
  GLUquadric * sphere;
  
  for (i = 0; i < vertexCubeNumOfNodes(c.s); ++i)
  {
    v = c.v + i;
    
    glPushMatrix();
      glTranslatef((*v)[0] - 0.5, (*v)[1] - 0.5, (*v)[2] - 0.5);
      glScalef(SCALE_NODES, SCALE_NODES, SCALE_NODES);
      
      if (i != selected)
        glCallList(1);
    glPopMatrix();
  }
  
  v = c.v + selected;
  
  sphere = gluNewQuadric();
  gluQuadricNormals(sphere, GL_SMOOTH);
  glColor3f(1.0, 0.0, 0.0);
  
  glPushMatrix();
    glTranslatef((*v)[0] - 0.5, (*v)[1] - 0.5, (*v)[2] - 0.5);
    glScalef(SCALE_NODES, SCALE_NODES, SCALE_NODES);
    gluSphere(sphere, 1.0f, 15, 15);
  glPopMatrix();
  
  gluDeleteQuadric(sphere);
}

/**
 * Zeichnet die Seiten des Würfels anhand der zuvor berechneten 
 * Zeichenreihenfolge.
 *
 * @param[in] c Zu zeichnender Würfel.
 */
extern void vertexCubeDrawPlanes(VertexCube c)
{
  /* Zeiger auf Geometrie- sowie Farbinformationen setzen */
  glVertexPointer(3, GL_DOUBLE, sizeof(Vertex), c.v[0]);
  glColorPointer( 3, GL_DOUBLE, sizeof(Vertex), c.v[0] + 3);
  
  /* Vertices zeichnen */
  glPushMatrix();
    glTranslatef(- 0.5, - 0.5, - 0.5);
    glDrawElements(GL_QUADS, numOfIndices(c.s), GL_UNSIGNED_INT, c.i);
  glPopMatrix();
}

/**
 * Weist dem ausgewählten Vertex eine zufällige Farbe zu.
 *
 * @param[in] c Würfel.
 * @param[in] i Index des ausgewählten Vertex.
 *
 * @return Würfel, bei dem der i-te Vertex eine neue Farbe erhalten hat.
 */
extern VertexCube vertexCubeSwitchColor(VertexCube c, unsigned i)
{
  c.v[i][VERTEX_ATTRIBUTE_R] = RANDOM_COLOR;
  c.v[i][VERTEX_ATTRIBUTE_G] = RANDOM_COLOR;
  c.v[i][VERTEX_ATTRIBUTE_B] = RANDOM_COLOR;
  
  return c;
}
