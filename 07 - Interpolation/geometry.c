/**
 * @file
 *
 * Modul für die Geometrie. Hier werden die Punkte, Vertices und Normalen 
 * verwaltet.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include <assert.h>
#include <float.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "geometry.h"
#include "list.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define PI (3.1415926535897932384626433832795029)
#define DEFAULT_POINTCOUNT  ( 4)
#define INTERPOLATIONLEVEL  (25)
#define CLICKDISTANCE       (0.01)
#define POINTSCALE          (0.01)
#define START_X             (0.1 )
#define DELTA_X             (0.05)
#define NORM_BORDER         (0.1 )

/* ----------------------------------------------------------------------------
 * Macros
 * -------------------------------------------------------------------------- */

#define RANDOM_COORDINATE ((rand() % 100) * 0.01)
#define TOINTERVAL(v, min, max) (((v) < (min)) ? (min) : ((v) > (max)) ? (max) : (v))

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Bestimmt, ob i ein selektierter Punkt ist.
 *
 * @param[in] g Geometrie.
 * @param[in] i Punkt.
 *
 * @return true  wenn i selektirert ist
 *         false sonst.
 */
static Boolean isSelectedNode(Geometry g, unsigned i)
{
  return listIsIn(g.selected, (int) i);
}

/**
 * Berechnet das Interpolationspolynom unter Verwendung der in g definierten
 * Interpolationsmethode.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie, die Interpolationspolynom enthält.
 */
static Geometry interpolate(Geometry g)
{
  unsigned i
         , j
         , k
         ;
  
  Matrix M  = matrixMakeInterpolation(g.i) /* Interpolationsmatrix           */
       , Gx = matrixMake(1, 4)             /* Geometriematrix für x-Richtung */
       , Gy = matrixMake(1, 4)             /* Geometriematrix für y-Richtung */
       , T  = matrixMake(4, 1)             /* Monome */
       , R
       , Sx
       , Sy
       ;
  
  /* Zur Berechnung der Normalen */
  Vector3d u
         , v
         , n
         , zAxis = vector3dMake(0.0, 0.0, 1.0)
         ;
  
  /* Speicher für Vertices, Indizes und Indizes der Normalen reservieren */
  g.vertices = realloc(g.vertices,  2 * g.subDiv * (g.n - 3)      * sizeof(Vertex));
  g.indices  = realloc(g.indices,       g.subDiv * (g.n - 3)      * sizeof(GLuint));
  g.normals  = realloc(g.normals,  (2 * g.subDiv * (g.n - 3) - 2) * sizeof(GLuint));
  
  /* Jeweils 4 Punkte für Interpolation benutzen */
  for (i = 0; i <= g.n - 4; ++i)
  {
    /* Geometriematrizen */
    for (j = 0; j < 4; ++j)
    {
      Gx = matrixSet(Gx, 0, j, g.vectors[i + j].x);
      Gy = matrixSet(Gy, 0, j, g.vectors[i + j].y);
    }
    
    for (j = 0; j < g.subDiv; ++j)
    {
      /* Monome */
      for (k = 0; k < 4; ++k)
        T = matrixSet(T, k, 0, pow(j * 1.0 / g.subDiv, 3 - k));
      
      /**
       * Monome und Interpolationsmatrix sind für Interpolation in x- sowie
       * y-Richtung identisch
       */
      R = matrixMultiply(T, M);
      
      Sx = matrixMultiply(R, Gx);
      Sy = matrixMultiply(R, Gy);
      
      /* Ergebnis der Interpolation muss jeweils ein Skalar sein. */
      assert(Sx.dimX == 1);
      assert(Sx.dimY == 1);
      assert(Sy.dimX == 1);
      assert(Sy.dimY == 1);
      
      g.vertices[j + i * g.subDiv][VERTEX_ATTRIBUTE_X] = matrixAt(Sx, 0, 0);
      g.vertices[j + i * g.subDiv][VERTEX_ATTRIBUTE_Y] = matrixAt(Sy, 0, 0);
      
      /* Speicher wieder freigeben */
      R  = matrixDelete(R);
      Sx = matrixDelete(Sx);
      Sy = matrixDelete(Sy);
    }
  }
  
  /* Indizes setzen (sind linear) */
  for (i = 0; i < g.subDiv * (g.n - 3); ++i)
    g.indices[i] = i;
  
  /* Normalen setzen */
  for (i = 0; i < g.subDiv * (g.n - 3) - 1; ++i)
  {
    /* Aktueller und nächster Punkt */
    u = vector3dMake(g.vertices[i][VERTEX_ATTRIBUTE_X], g.vertices[i][VERTEX_ATTRIBUTE_Y], 0.0);
    v = vector3dMake(g.vertices[i + 1][VERTEX_ATTRIBUTE_X], g.vertices[i + 1][VERTEX_ATTRIBUTE_Y], 0.0);
    
    /**
     * Normale durch Kreuzprodukt mit der z-Achse bestimmen und zu den 
     * Koordinaten des zugehörigen Punktes hinzuaddieren
     */
    n = vector3dAdd(vector3dCross(zAxis, vector3dSub(v, u)), u);
    
    /* Endpunkt der Normalen setzem */
    g.vertices[g.subDiv * (g.n - 3) + i][VERTEX_ATTRIBUTE_X] = n.x;
    g.vertices[g.subDiv * (g.n - 3) + i][VERTEX_ATTRIBUTE_Y] = n.y;
    
    /* Indizes setzen */
    g.normals[2 * i   ] = i;
    g.normals[2 * i +1] = g.subDiv * (g.n - 3) + i;
  }
  
  /* Verwendeten Speicher wieder freigeben */
  M  = matrixDelete(M);
  Gx = matrixDelete(Gx);
  Gy = matrixDelete(Gy);
  T  = matrixDelete(T);
  
  return g;
}

/**
 * Normiert die Geometrie derart, dass alle Punkte im sichtbaren Bereich liegen.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit normierten Punktkoordinaten.
 */
static Geometry normXY(Geometry g)
{
  unsigned i;
  
  /* Minimal- und Maximalkoordinaten */
  Vector3d   max = vector3dMakeNull()
         ,   min = vector3dMakeNull()
         , * tmp
         ;
  
  /* Minimal- und Maximalwerte bestimmen */
  for (i = 0; i < g.n; ++i)
  {
    tmp = &g.vectors[i];
    
    if (tmp->x > max.x)
      max.x = tmp->x;
    
    if (tmp->x < min.x)
      min.x = tmp->x;
    
    if (tmp->y > max.y)
      max.y = tmp->y;
    
    if (tmp->y < min.y)
      min.y = tmp->y;
  }
  
  /**
   * Normierung notwendig, falls Koordinaten außerhalb des zulässigen Bereiches
   * liegen
   */
  if (max.x > 1.0 || min.x < 0.0)
    for (i = 0; i < g.n; ++i)
      g.vectors[i].x = (g.vectors[i].x - min.x) / (max.x - min.x);
  
  if (max.y > 1.0 || min.y < 0.0)
    for (i = 0; i < g.n; ++i)
      g.vectors[i].y = (g.vectors[i].y - min.y) / (max.y - min.y);
  
  return g;
}

/**
 * Berechnet die Konvexe Hülle der in g verwalteten Punkte nach dem 
 * von Neumann'schen Algorithmus (welcher nur ein achsenvertauschter 
 * Jarvis-March ist).
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit berechneter konvexer Hülle.
 */
extern Geometry convexHull(Geometry g)
{
  int i
    , j
    , current    /* Aktueller Punkt */
    , next       /* Nächster  Punkt */
    , bottommost /* Unterster Punkt */
    ;
  
  double angleCurrent /* Winkel zum aktuellen            Punkt */
       , angleNext    /* Winkel zum nächsten             Punkt */
       , angleTmp     /* Winkel zum aktuell untersuchten Punkt */
       ;
  
  List points = listMakeEmpty() /* Noch zu untersuchende Punkte */
     , ch     = listMakeEmpty() /* Bereits berechnete    Punkte */
     ;
  
  /* Untersten Punkt bestimmen und Punkte in Liste einfügen */
  bottommost = 0;
  for (i = g.n - 1; i >= 0; --i)
  {
    if (g.vectors[i].y < g.vectors[bottommost].y)
      bottommost = i;
    
    points = listCons(i, points);
  }
  
  /* Unterster Punkt gehört zur konvexen Hülle */
  ch = listCons(bottommost, ch);
  
  /* Aktueller Punkt ist der unterste */
  current = bottommost;
  next    = current + 1;
  
  angleCurrent = 0.0;
  
  /* So lange Punkte untersuchen, bin man beim Startpunkt angelangt */
  while (next != bottommost)
  {
    angleNext = 2 * PI;
    
    next = bottommost;
    
    /* Alle noch nicht hinzugefügten Punkte untersuchen */
    for (i = 0; i < (int) listLength(points); ++i)
    {
      j = listAt(points, i);
      angleTmp = acos(vector3dNorm(vector3dSub(g.vectors[j], g.vectors[current])).x);
      
      if (g.vectors[j].y < g.vectors[current].y)
        angleTmp = 2 * PI - angleTmp;
      
      if (j != current && angleTmp < angleNext && angleTmp > angleCurrent)
      {
        angleNext = angleTmp;
        next  = j;
      }
    }
    
    points = listRemove(points, next);
    ch     = listCons(next, ch);
    
    current = next;
    angleCurrent = angleNext;
  }
  
  /* Speicher für Punkte der Konvexen Hülle reservieren */
  g.nCH = listLength(ch);
  g.vertices = realloc(g.vertices, g.n   * sizeof(Vertex));
  g.indices  = realloc(g.indices,  g.nCH * sizeof(GLuint));
  
  /* Punkte speichern */
  for (i = 0; i < (int) g.n; ++i)
  {
    g.vertices[i][VERTEX_ATTRIBUTE_X] = g.vectors[i].x;
    g.vertices[i][VERTEX_ATTRIBUTE_Y] = g.vectors[i].y;
  }
  
  /* Indizes setzen */
  g.indices[0] = listHead(ch);
  ch = listRemoveHead(ch);
  
  i = 0;
  while (!listIsEmpty(ch))
  {
    g.indices[++i] = listHead(ch);
    
    ch = listRemoveHead(ch);
  }
  
  /* Aufräumen */
  points = listRemoveAll(points);
  
  return g;
}

/**
 * Adaptiert die Geometrie g an die vorgegebene Konfiguration.
 *
 * @param[in] g Geometrie.
 *
 * @return Adaptiert Geometrie (d.h. mit neu berechneter Interpolation, Normalen
 *                              und konvexer Hülle).
 */
static Geometry adapt(Geometry g)
{
  switch (g.d)
  {
    case DRAWMODE_INTERPOLATION:
      #ifdef DEBUG
      fprintf(stderr, "ADAPTING :: Interpolation.\n");
      #endif
      
      g = interpolate(g);
      break;
    case DRAWMODE_CONVEXHULL:
      #ifdef DEBUG
      fprintf(stderr, "ADAPTING :: Convex Hull.\n");
      #endif
      
      g = convexHull(g);
      break;
    default:
      assert(0);
  }
  
  /* Neue Vertices an die Grafikkarte schicken */
  glVertexPointer(2, GL_DOUBLE, sizeof(Vertex), g.vertices[0]);
  
  return g;
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt eine neue Geometrie und gibt sie zurück.
 *
 * @return Erzeugte Geometrie.
 */
extern Geometry geometryInit(void)
{
  Geometry res;
  
  unsigned i;
  
  res.n        = DEFAULT_POINTCOUNT;
  res.nCH      = 0;
  res.i        = MATRIX_BSPLINE;
  res.subDiv   = INTERPOLATIONLEVEL;
  res.d        = DRAWMODE_INTERPOLATION;
  
  res.vectors  = malloc(res.n * sizeof(Vector3d));
  res.vertices = NULL;
  res.indices  = NULL;
  res.normals  = NULL;
  res.selected = listMakeEmpty();
  
  /* Initialpunkte erzeugen */
  for (i = 0; i < res.n; ++i)
    res.vectors[i] = vector3dMake(START_X + i * DELTA_X, RANDOM_COORDINATE, 0.0);
  
  return adapt(res);
}

/**
 * Löscht die Geometrie g.
 *
 * @param[in] g Zu löschende Geometrie.
 *
 * @return Gelöschte Geometrie.
 */
extern Geometry geometryDelete(Geometry g)
{
  if (g.vectors)
    free(g.vectors);
    
  if (g.vertices)
    free(g.vertices);
  
  if (g.indices)
    free(g.indices);
  
  if (g.normals)
    free(g.normals);
  
  g.vectors  = NULL;
  g.vertices = NULL;
  g.indices  = NULL;
  g.normals  = NULL;
  
  g.selected = listRemoveAll(g.selected);
  
  g.n      = 0;
  g.subDiv = 0;
  
  return g;
}

/**
 * Erzeugt einen zufälligen Punkt, der sich rechts vom letzten Punkt der 
 * Geometrie-Punkte befindet und fügt ihn am rechten Ende ein.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit neu erzeugtem Punkt.
 */
extern Geometry geometryAddPoint(Geometry g)
{
  Vector3d v = vector3dMake(g.vectors[g.n - 1].x + DELTA_X, RANDOM_COORDINATE, 0.0);
  
  return adapt(normXY(geometryAddPointAt(g, v)));
}

/**
 * Fügt an der Position v einen neuen Punkt in die Geometrie g ein.
 *
 * @param[in] g Geometrie.
 * @param[in] v Position, an der der neue Punkt eingefügt werden soll.
 *
 * @return Geometrie mit neu eingefügtem Punkt.
 */
extern Geometry geometryAddPointAt(Geometry g, Vector3d v)
{
  ++g.n;
  
  /* Speichergröße anpassen */
  g.vectors = realloc(g.vectors, g.n * sizeof(Vector3d));
  
  g.vectors[g.n - 1] = v;
  
  return adapt(g);
}

/**
 * Verändert die Interpolationsfeinheit.
 *
 * @param[in] g Geometrie.
 * @param[in] i Neue Interpolationsfeinheit.
 *
 * @return Geometrie mit angepasster Interpolationsfeinheit, alle Daten
 *         (Interpolationpolynom, Normalen, konvexe Hülle) wurden bereits neu
 *         berechnet.
 */
extern Geometry geometrySetSubdivides(Geometry g, unsigned i)
{
  g.subDiv = i;
  
  return adapt(g);
}

/**
 * Wechselt die Interpolationsmethode.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie, die die "nächste" Interpolationsmethode benutzt.
 */
extern Geometry geometrySwitchInterpolation(Geometry g)
{
  g.i = ++g.i % MATRIX_DUMMY;
  
  return adapt(g);
}

/**
 * Zeichnet die Punkte der Geometrie.
 *
 * @param[in] g Geometrie.
 */
extern void geometryDrawPoints(Geometry g)
{
  unsigned i
         , j
         ;
  
  float color[3] = { 1.0, 0.0, 0.0 }
      , plus     =   0.55
      ;
  
  GLUquadricObj * q;
  q = gluNewQuadric();
  
  for (i = 0; i < g.n; ++i)
  {
    /* Selektierte Punkte alle in unterschiedlicher Farbe zeichnen */
    if (isSelectedNode(g, i))
    {
      glColor3fv(color);
      
      color[2] += plus;
      color[1] += color[2] > 1.0 ? plus : 0.0;
      color[0] += color[1] > 1.0 ? plus : 0.0;
      
      for (j = 0; j < 3; ++j)
        while (color[j] > 1.0)
          color[j] -= 1.0;
    }
    /* Nicht-selektierte in weiß */
    else
      glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
      glTranslatef(g.vectors[i].x, g.vectors[i].y, 0);
      glScalef(POINTSCALE, POINTSCALE, 0);
      gluDisk(q, 0.0, 1.0, 12, 1); 
    glPopMatrix();
    
    glColor3f(1.0, 1.0, 1.0);
  }
  
  gluDeleteQuadric(q);
}

/**
 * Wählt den Punkt an der Koordinate v in g aus.
 *
 * @param[in] g     Geometrie.
 * @param[in] v     Position, an der ein Punkt ausgewählt werden soll (falls es 
 *                  dort einen gibt).
 * @param[in] singe Gibt an, ob nur ein Punkt ausgewählt werden soll, oder ob
 *                  ein weiterer zur bisherigen Auswahl hinzuzufügen ist.
 *
 * @return Geometrie mit angepassten ausgewählen Punkten.
 */
extern Geometry geometrySelectNodeAt(Geometry g, Vector3d v, Boolean single)
{
  unsigned i;
  
  /* Zu Beginn ist kein Punkt getroffen */
  double d = 2 * CLICKDISTANCE;
  
  /* Wenn nur ein Punkt ausgewählt werden soll, zunächst Auswahl löschen */
  if (single)
    g.selected = listRemoveAll(g.selected);
  
  /* Für alle Punkte prüfen, ob sie getroffen wurden */
  for (i = 0; i < g.n && (d > CLICKDISTANCE || !single); ++i)
    if ((d = vector3dLength(vector3dSub(v, g.vectors[i]))) < CLICKDISTANCE)
      g.selected = listInsert(g.selected, i);
  
  return g;
}

/** 
 * Bewegt die in g ausgewählten Punkte um den Vektor v.
 *
 * @param[in] g Geometrie.
 * @param[in] v Translation, die auf die in g ausgewählten Punkte angewendet 
 *              werden soll.
 *
 * @return Geometrie mit verschobenen Punkten.
 */
extern Geometry geometryMoveSelectedNodes(Geometry g, Vector3d v)
{
  unsigned i
         , at
         ;
  
  /* Alle ausgewählten Punkt verschieben */
  for (i = 0; i < listLength(g.selected); ++i)
  {
    at = listAt(g.selected, i);
    
    /* Verschiebung durchführen */
    g.vectors[at] = vector3dAdd(g.vectors[at], v);
    
    /* In zulässigen Bereich zurückholen */
    g.vectors[at].x = TOINTERVAL(g.vectors[at].x, 0.0, 1.0);
    g.vectors[at].y = TOINTERVAL(g.vectors[at].y, 0.0, 1.0);
  }
  
  return adapt(g);
}

/**
 * Entfernt Punkte aus g. r gibt dabei an, ob alle ausgewählten Punkte oder nur
 * der zuletzt hinzugefügte gelöscht werden sollen. Die initial vorhandenen
 * Punkte bleiben immer vorhanden und können nicht gelöscht werden.
 *
 * @param[in] g Geometrie.
 * @param[in] r Lösch-Modus.
 *
 * @return Geometrie "mit gelöschten Punkten".
 */
extern Geometry geometryRemovePoints(Geometry g, RemovePoints r)
{
  g.selected = listRemove(g.selected, 0);
  g.selected = listRemove(g.selected, 1);
  g.selected = listRemove(g.selected, 2);
  g.selected = listRemove(g.selected, 3);
  
  switch (r)
  {
    case REMOVE_SELECTED:
      {
        unsigned i;
        
        /**
         * Alle ausgewählten Punkte entfernen, indem die nachfolgenen nach vorne
         * verschoben werden
         */
        while (!listIsEmpty(g.selected) && g.n > DEFAULT_POINTCOUNT)
        {
          for (i = listHead(g.selected); i < g.n - 1; ++i)
            g.vectors[i] = g.vectors[i + 1];
            
          --g.n;
          
          g.selected = listRemoveHead(g.selected);
        }
      }
      break;
    case REMOVE_LAST:
      /* Nur letzen Punkt löschen */
      if (g.n > DEFAULT_POINTCOUNT)
        --g.n;
      break;
    default:
      assert(0);
  }
  
  /* Speichergröße anpassen */
  g.vectors = realloc(g.vectors, g.n * sizeof(Vector3d));
  
  return adapt(g);
}

/**
 * Wechselt den Zeichenmodus von g. Mögliche Zeichenmodi sind Interpolation und
 * Konvexe Hülle.
 *
 * @param[in] g Geometrie.
 *
 * @return Geometrie mit gewechseltem Zeichenmodus.
 */
extern Geometry geometrySwitchDrawMode(Geometry g)
{
  g.d = ++g.d % DRAWMODE_DUMMY;
  
  return adapt(g);
}
