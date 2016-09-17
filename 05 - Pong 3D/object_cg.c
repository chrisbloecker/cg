/**
 * @file
 * Objekt-Modul.
 * Das Modul kapselt Funktionen, die auf einem Objekt, das aus zahlreichen
 * Dreiecksflaechen besteht, durchgefuehrt werden koennen.
 * Die Funktionen dieses Moduls sind stark inspiriert von der Lesson 27 bei
 * http://nehe.gamedev.net
 *
 * Bestandteil eines Beispielprogramms fuer Schatten mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2008. All rights reserved.
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <GL/glu.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "object_cg.h"
#include "vector.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

/** Laenge der Kanten des Schattenvolumens */
#define INFINITY (25)

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Berechnet die Nachbarschaftsbeziehungen der Flaechen des Objekts obj.
 * Angelehnt an den Algorithmus von http://www.gamasutra.com/
 * @param obj Objekt, fuer dessen Flaechen die Nachbarschaftsbeziehungen
 *            berechnet werden sollen.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: die Nachbarschaftsbeziehungen der Flaechen von obj
 *                           wurden berechnet.
 */
static void setConnectivity(CGObject * obj)
{
  unsigned int p1i, p2i, p1j, p2j;
  unsigned int P1i, P2i, P1j, P2j;
  unsigned int i, j, ki, kj;

  /* Fuer alle Flaechen des Objekts */
  for (i = 0; i < obj->nPlanes - 1; i++)
    {
      /* Fuer alle noch folgenden Flaechen */
      for (j = i + 1; j < obj->nPlanes; j++)
        {
          /* Fuer jede Kante der Flaeche */
          for (ki = 0; ki < 3; ki++)
            {
              /* Wenn noch kein Nachbar gesetzt wurde */
              if (obj->planes[i].neighIdx[ki] == -1)
                {
                  /* Fuer jede Kante der nachfolgenden Flaeche */
                  for (kj = 0; kj < 3; kj++)
                    {
                      /* Index des ersten Punktes der Kante ki der Flaeche i
                         im Array der Eckpunkte der Flaeche i */
                      p1i = ki;
                      /* Index des zweiten Punktes der Kante ki der Flaeche i
                         im Array der Eckpunkte der Flaeche i */
                      p2i = (ki + 1) % 3;
                      /* Index des ersten Punktes der Kante kj der Flaeche j
                         im Array der Eckpunkte der Flaeche j */
                      p1j = kj;
                      /* Index des zweiten Punktes der Kante kj der Flaeche j
                         im Array der Eckpunkte der Flaeche j */
                      p2j = (kj + 1) % 3;

                      /* Index des ersten Punktes der Kante ki der Flaeche i
                         im Array der Punkte des Objekts */
                      p1i = obj->planes[i].pointIdx[p1i];
                      /* Index des zweiten Punktes der Kante ki der Flaeche i
                         im Array der Punkte des Objekts */
                      p2i = obj->planes[i].pointIdx[p2i];
                      /* Index des ersten Punktes der Kante kj der Flaeche j
                         im Array der Punkte des Objekts */
                      p1j = obj->planes[j].pointIdx[p1j];
                      /* Index des zweiten Punktes der Kante kj der Flaeche j
                         im Array der Punkte des Objekts */
                      p2j = obj->planes[j].pointIdx[p2j];

                      /* Indizes so umrechnen, das P1 immer kleiner P2 ist */
                      P1i = ((p1i + p2i) - abs (p1i - p2i)) / 2;
                      P2i = ((p1i + p2i) + abs (p1i - p2i)) / 2;
                      P1j = ((p1j + p2j) - abs (p1j - p2j)) / 2;
                      P2j = ((p1j + p2j) + abs (p1j - p2j)) / 2;

                      /* Die Flaechen sind benachbart */
                      if ((P1i == P1j) && (P2i == P2j))
                        {
                          obj->planes[i].neighIdx[ki] = j;
                          obj->planes[j].neighIdx[kj] = i;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Zeichnet das Schattenvolumen, welches das Objekts obj fuer eine
 * Punktlichtquelle mit der Position lightPos erzeugt.
 * @param obj Objekt, dessen Schattenvolumen gezeichnet werden soll.
 *            Vorbedingung: die Sichtbarkeit der Flaechen von der Position der
 *                          Lichtquelle aus ist bereits berechnet.
 * @param lightPos Position der Lichtquelle, relativ zum Nullpunkt des Objekts
 *                 obj (also in Objektkoordinaten).
 */
void drawShadowVolume(CGObject obj, CGVector4f lightPos)
{
  unsigned int i, j;
  int k;

  unsigned int p1, p2;
  CGVector3f v1, v2;

  /* fuer alle Flaechen des Objekts */
  for (i = 0; i < obj.nPlanes; i++)
    {
      /* wenn die Flaeche dem Licht zugewandt ist */
      if (obj.planes[i].visible)
        {
          /* Fuer alle Kanten der Flaeche */
          for (j = 0; j < 3; j++)
            {
              /* Index der Nachbarflaeche */
              k = obj.planes[i].neighIdx[j];

              /* Wenn es eine Nachbarflaeche gibt und diese vom Licht abgewandt
                 ist */
              if ((k != -1) && (!obj.planes[k].visible))
                {
                  /* Diese beiden Flaechen haben eine dazwischenliengende
                     Kante, die fuer die Silhouette relevant ist. */
                  /* Index des ersten Punktes dieser Kante in der Liste der
                     Punkte des Objektes */
                  p1 = obj.planes[i].pointIdx[j];
                  /* Index des zweiten Punktes dieser Kante in der Liste der
                     Punkte des Objektes */
                  p2 = obj.planes[i].pointIdx[(j + 1) % 3];

                  /* Vektoren von der Lichtquelle aus ueber die beiden Punkte
                     der Kante hinaus berechnen. Ihre Laenge haengt vom
                     eingestellten Wert fuer INFINITY ab */
                  v1[0] = (obj.points[p1][0] - lightPos[0]) * INFINITY;
                  v1[1] = (obj.points[p1][1] - lightPos[1]) * INFINITY;
                  v1[2] = (obj.points[p1][2] - lightPos[2]) * INFINITY;

                  v2[0] = (obj.points[p2][0] - lightPos[0]) * INFINITY;
                  v2[1] = (obj.points[p2][1] - lightPos[1]) * INFINITY;
                  v2[2] = (obj.points[p2][2] - lightPos[2]) * INFINITY;

                  /* Schattenflaeche zeichnen. Ergibt sich aus den Punkten
                     der Kante und die zu ihnen addierten Vektoren */
                  glBegin (GL_TRIANGLE_STRIP);
                  {
                    glVertex3f (obj.points[p1][0],
                                obj.points[p1][1],
                                obj.points[p1][2]);
                    glVertex3f (obj.points[p1][0] + v1[0],
                                obj.points[p1][1] + v1[1],
                                obj.points[p1][2] + v1[2]);
                    glVertex3f (obj.points[p2][0],
                                obj.points[p2][1],
                                obj.points[p2][2]);
                    glVertex3f (obj.points[p2][0] + v2[0],
                                obj.points[p2][1] + v2[1],
                                obj.points[p2][2] + v2[2]);
                  }
                  glEnd ();
                }
            }
        }
    }
}

/**
 * Berechnet fuer jede Flaeche des Objekts obj, ob diese von der Position pos
 * aus sichtbar sind. Flaechen sind sichtbar, wenn deren Vorderseite der
 * Position zugewandt ist.
 * @param obj Objekt, fuer dessen Flaechen die Sichtbarkeit berechnet werden
 *            soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: die Sichtbarkeit der Flaechen von obj von der
 *                           von der Position pos aus wurde berechnet.
 * @param pos Position, fuer welche die Sichtbarkeit berechnet werden soll.
 */
void calcVisibility(CGObject * obj, CGVector4f pos)
{
  unsigned int i;
  float side;

  /* Fuer alle Flaechen des Objektes */
  for (i = 0; i < obj->nPlanes; i++)
    {
      /* Pruefen, ob die Flaeche der Position zu- oder abgewandt ist.
         Dazu wird berechnet, wird die Position in Ebenen-Gleichung
         eingesetzt. */
      side = obj->planes[i].PlaneEq.a * pos[0] +
             obj->planes[i].PlaneEq.b * pos[1] +
             obj->planes[i].PlaneEq.c * pos[2] +
             obj->planes[i].PlaneEq.d * pos[3];

      /* Flaeche ist dem Licht zugewandt, wenn Ergebnis groesser 0 */
      obj->planes[i].visible = (side > 0);
    }
}

/**
 * Zeichnet die Schatten, die das Objekt obj aufgrund der Position lightPos
 * einer Punktlichtquelle wirft. Schatten werden mithilfe von Shadow-Volumes
 * erzeugt.
 * @param obj Objekt, dessen Schatten gezeichnet werden soll.
 *            Vorbedingung: die Sichtbarkeit der Flaechen von der Position der
 *                          Lichtquelle aus ist bereits berechnet.
 * @param lightPos Position der Lichtquelle, relativ zum Nullpunkt des Objekts
 *                 obj (also in Objektkoordinaten).
 */
void castShadow(CGObject obj, CGVector4f lightPos)
{
  /* aktuelle Einstellungen sichern */
  glPushAttrib (GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT |
                GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

  glCullFace (GL_BACK);

  glEnable (GL_CULL_FACE);

  /* Beleuchtung deaktivieren */
  glDisable (GL_LIGHTING);

  /* Schreiben in den Tiefenpuffer deaktivieren */
  glDepthMask (GL_FALSE);
  glDepthFunc (GL_LEQUAL); /* !!! */

  /* Stenciltest aktivieren */
  glEnable (GL_STENCIL_TEST);

  /* Schreiben in den Farbpuffer deaktivieren */
  glColorMask (0, 0, 0, 0);

  /* Funktion und Referenz wert fuer den Stenciltest setzen.
     Hier: Alles in den Stencilpuffer schreiben. */
  glStencilFunc (GL_ALWAYS, 1, 0xffffffff);

  /* Erster Durchlauf: Rendern der kompletten Shadow-Volumes,
     vom schattenwerfenden Objekt durch den (leeren) Raum
     bis zum Boden bzw. zur Wand.
     Der Stencil-Puffer (der vorher ueberall 0 ist) wird an
     den Stellen auf 1 gesetzt, wo ein Shadow-Volume vorhanden
     ist (GL_INCR).
     Die Face-Orientierung ist dabei auf CCW (CounterClockWise)
     gestellt, alle FRONTfaces erzeugen also das Shadow-Volume. */
  glFrontFace (GL_CCW);
  glStencilOp (GL_KEEP, GL_KEEP, GL_INCR);

  drawShadowVolume (obj, lightPos);

  /* Zweiter Durchlauf: Erneutes Rendern der kompletten
     Shadow-Volumes.
     Der Stencil-Puffer (ueberall 0, 1 an Stellen mit Schatten)
     wird an den Stellen zurueck auf 0 gesetzt, wo ein Schadow-
     Volume vorhanden ist (GL_DECR).
     Die Face-Orientierung ist dabei auf CW (ClockWise)
     gestellt, alle BACKfaces erzeugen also das Shadow-Volume.
     Die "Differenz" zwischen dem Shadow-Volume der Front-
     und dem der Backfaces ergibt dann den eigentlichen Schatten
     auf Boden und Waenden. */
  glFrontFace (GL_CW);
  glStencilOp (GL_KEEP, GL_KEEP, GL_DECR);

  drawShadowVolume (obj, lightPos);

  /* Face-Orientierung wieder auf "normal" setzen */
  glFrontFace (GL_CCW);
  /* Schreiben in den Farbpuffer wieder aktivieren */
  glColorMask (1, 1, 1, 1);

  /* "Schattierendes" Rechteck zeichnen, das die gesamte Szene
     ueberdeckt. Es wird nur an den Stellen mittels Blending
     mit den Werten im Farbpuffer verrechnet, an denen der
     Stencil-Puffer einen Wert <> 0 hat, wo also ein Schatten
     entstehen soll. */
  glStencilFunc (GL_NOTEQUAL, 0, 0xffffffff);

  /* Schattenfarbe */
  glColor4f (0.0f, 0.0f, 0.0f, 0.4f);

  /* Blending aktivieren und setzen */
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Projektionsmatrix veraendern */
  glMatrixMode (GL_PROJECTION);

  /* aktuelle Projektionsmatrix sichern */
  glPushMatrix ();
  {

    /* neue orthogonale 2D-Projektionsmatrix erzeugen */
    glLoadIdentity ();
    gluOrtho2D (0.0, 1.0, 0.0, 1.0);

    /* ModelView-Matrix veraendern */
    glMatrixMode (GL_MODELVIEW);

    /* aktuelle ModelView-Matrix sichern */
    glPushMatrix ();
    {

      /* ModelView-Matrix zuruecksetzen */
      glLoadIdentity ();

      glBegin(GL_TRIANGLE_STRIP);
      {
        glVertex2f(0.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, 0.0f);
      }
      glEnd();

      /* alte ModelView-Matrix laden */
    }
    glPopMatrix ();

    /* Projektionsmatrix veraendern */
    glMatrixMode (GL_PROJECTION);

    /* alte Projektionsmatrix laden */
  }
  glPopMatrix ();

  /* auf der ModelView-Matrix weiterarbeiten */
  glMatrixMode (GL_MODELVIEW);

  /* alte Einstellungen laden */
  glPopAttrib ();
}

/**
 * Erzeugt das Objekt obj als ein kreuzartiges Beispielobjekt. Speicher fuer die
 * Punkte und Flaechen des Objekts werden allokiert. Punktkoordianten und
 * Eckpunkte der Flaechen werden gesetzt.
 * @param obj Objekt, das initialisiert werden soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: obj wurde erzeugt.
 * @return Erfolgsstatus der Erzeugung.
 */
static GLboolean createObject(CGObject * obj)
{
  unsigned int i = 0;

  /* Anzahl Punkte */
  obj->nPoints = 8;

  obj->points = calloc (obj->nPoints, sizeof (CGVector3f));

  if (obj->points == NULL)
    {
      freeObject (obj);
      return GL_FALSE;
    }

  obj->points[0][0] = -0.5f;
  obj->points[0][1] = -0.5f;
  obj->points[0][2] = +0.5f;

  obj->points[1][0] = +0.5f;
  obj->points[1][1] = -0.5f;
  obj->points[1][2] = +0.5f;

  obj->points[2][0] = +0.5f;
  obj->points[2][1] = +0.5f;
  obj->points[2][2] = +0.5f;

  obj->points[3][0] = -0.5f;
  obj->points[3][1] = +0.5f;
  obj->points[3][2] = +0.5f;

  obj->points[4][0] = -0.5f;
  obj->points[4][1] = -0.5f;
  obj->points[4][2] = -0.5f;

  obj->points[5][0] = +0.5f;
  obj->points[5][1] = -0.5f;
  obj->points[5][2] = -0.5f;

  obj->points[6][0] = +0.5f;
  obj->points[6][1] = +0.5f;
  obj->points[6][2] = -0.5f;

  obj->points[7][0] = -0.5f;
  obj->points[7][1] = +0.5f;
  obj->points[7][2] = -0.5f;

  /* Anzahl Dreiecksflaechen */
  obj->nPlanes = 12;

  obj->planes = calloc (obj->nPlanes, sizeof (CGPlane));

  if (obj->planes == NULL)
    {
      freeObject (obj);
      return GL_FALSE;
    }
  
  obj->planes[0].pointIdx[0] = 0;
  obj->planes[0].pointIdx[1] = 2;
  obj->planes[0].pointIdx[2] = 1;
  obj->planes[0].normal[0] = +0.0f;
  obj->planes[0].normal[1] = +1.0f;
  obj->planes[0].normal[2] = +0.0f;

  obj->planes[1].pointIdx[0] = 0;
  obj->planes[1].pointIdx[1] = 3;
  obj->planes[1].pointIdx[2] = 2;
  obj->planes[1].normal[0] = +0.0f;
  obj->planes[1].normal[1] = +1.0f;
  obj->planes[1].normal[2] = +0.0f;

  obj->planes[2].pointIdx[0] = 4;
  obj->planes[2].pointIdx[1] = 5;
  obj->planes[2].pointIdx[2] = 6;
  obj->planes[2].normal[0] = +0.0f;
  obj->planes[2].normal[1] = -1.0f;
  obj->planes[2].normal[2] = +0.0f;

  obj->planes[3].pointIdx[0] = 4;
  obj->planes[3].pointIdx[1] = 6;
  obj->planes[3].pointIdx[2] = 7;
  obj->planes[3].normal[0] = +0.0f;
  obj->planes[3].normal[1] = -1.0f;
  obj->planes[3].normal[2] = +0.0f;

  obj->planes[4].pointIdx[0] = 4;
  obj->planes[4].pointIdx[1] = 3;
  obj->planes[4].pointIdx[2] = 0;
  obj->planes[4].normal[0] = -1.0f;
  obj->planes[4].normal[1] = +0.0f;
  obj->planes[4].normal[2] = +0.0f;

  obj->planes[5].pointIdx[0] = 4;
  obj->planes[5].pointIdx[1] = 7;
  obj->planes[5].pointIdx[2] = 3;
  obj->planes[5].normal[0] = -1.0f;
  obj->planes[5].normal[1] = +0.0f;
  obj->planes[5].normal[2] = +0.0f;

  obj->planes[6].pointIdx[0] = 2;
  obj->planes[6].pointIdx[1] = 5;
  obj->planes[6].pointIdx[2] = 1;
  obj->planes[6].normal[0] = +1.0f;
  obj->planes[6].normal[1] = +0.0f;
  obj->planes[6].normal[2] = +0.0f;

  obj->planes[7].pointIdx[0] = 2;
  obj->planes[7].pointIdx[1] = 6;
  obj->planes[7].pointIdx[2] = 5;
  obj->planes[7].normal[0] = +1.0f;
  obj->planes[7].normal[1] = +0.0f;
  obj->planes[7].normal[2] = +0.0f;

  obj->planes[8].pointIdx[0] = 4;
  obj->planes[8].pointIdx[1] = 0;
  obj->planes[8].pointIdx[2] = 1;
  obj->planes[8].normal[0] = +0.0f;
  obj->planes[8].normal[1] = +0.0f;
  obj->planes[8].normal[2] = -1.0f;

  obj->planes[9].pointIdx[0] = 4;
  obj->planes[9].pointIdx[1] = 1;
  obj->planes[9].pointIdx[2] = 5;
  obj->planes[9].normal[0] = +0.0f;
  obj->planes[9].normal[1] = +0.0f;
  obj->planes[9].normal[2] = -1.0f;

  obj->planes[10].pointIdx[0] = 2;
  obj->planes[10].pointIdx[1] = 3;
  obj->planes[10].pointIdx[2] = 7;
  obj->planes[10].normal[0] = +0.0f;
  obj->planes[10].normal[1] = +0.0f;
  obj->planes[10].normal[2] = +1.0f;

  obj->planes[11].pointIdx[0] = 2;
  obj->planes[11].pointIdx[1] = 7;
  obj->planes[11].pointIdx[2] = 6;
  obj->planes[11].normal[0] = +0.0f;
  obj->planes[11].normal[1] = +0.0f;
  obj->planes[11].normal[2] = +1.0f;

  /* Alle Flaechen des Objekts initialisieren */
  for (i = 0; i < obj->nPlanes; i++)
    {
      obj->planes[i].visible = GL_TRUE;

      obj->planes[i].neighIdx[0] = -1;
      obj->planes[i].neighIdx[1] = -1;
      obj->planes[i].neighIdx[2] = -1;

      obj->planes[i].PlaneEq.a = 0;
      obj->planes[i].PlaneEq.b = 0;
      obj->planes[i].PlaneEq.c = 0;
      obj->planes[i].PlaneEq.d = 0;
    }

  return GL_TRUE;
}

/**
 * Berechnet die Ebenengleichungen der Flaechen des Objekts obj.
 * @param obj Objekt, dessen Ebenengleichungen berechnet werden sollen.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: Ebenengleichungen fuer obj wurden berechnet.
 */
static void calcPlanes(CGObject * obj)
{
  unsigned int i = 0, j = 0;
  CGVector3f v[3];
  CGPlane *plane;

  /* Fuer alle Objektflaechen */
  for (i = 0; i < obj->nPlanes; i++)
    {
      plane = &(obj->planes[i]);

      /* Eckpunkte der Flaeche speichern */
      for (j = 0; j < 3; j++)
        {
          v[j][0] = obj->points[plane->pointIdx[j]][0];
          v[j][1] = obj->points[plane->pointIdx[j]][1];
          v[j][2] = obj->points[plane->pointIdx[j]][2];
        }

      plane->PlaneEq.a =   v[0][1] * (v[1][2] - v[2][2]) +
                           v[1][1] * (v[2][2] - v[0][2]) +
                           v[2][1] * (v[0][2] - v[1][2]);
      plane->PlaneEq.b =   v[0][2] * (v[1][0] - v[2][0]) +
                           v[1][2] * (v[2][0] - v[0][0]) +
                           v[2][2] * (v[0][0] - v[1][0]);
      plane->PlaneEq.c =   v[0][0] * (v[1][1] - v[2][1]) +
                           v[1][0] * (v[2][1] - v[0][1]) +
                           v[2][0] * (v[0][1] - v[1][1]);
      plane->PlaneEq.d = -(v[0][0] * (v[1][1] * v[2][2] - v[2][1] * v[1][2]) +
                           v[1][0] * (v[2][1] * v[0][2] - v[0][1] * v[2][2]) +
                           v[2][0] * (v[0][1] * v[1][2] - v[1][1] * v[0][2]));
    }
}

/**
 * Initialisiert das Objekt obj als ein kreuzartiges Beispielobjekt. Speicher
 * fuer die Punkte und Flaechen wird allokiert, Nachbarschaftsbeziehungen und
 * Ebenengleichungen der Flaechen werden berechnet.
 * @param obj Objekt, das initialisiert werden soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: obj wurde vollstaendig initialisiert.
 * @return Erfolgsstatus der Initialisierung.
 */
GLboolean initObject(CGObject * obj)
{
  /* Objekt erzeugen und Punkte und Flaechen initialisieren */
  if (createObject(obj))
  {
    /* Nachbarschaftsbeziehungen setzen */
    setConnectivity(obj);
    
    /* Ebenengleichungen berechnen */
    calcPlanes(obj);
    
    return GL_TRUE;
  }
  else
  {
    return GL_FALSE;
  }
}

/**
 * Gibt den fuer das Objekt obj allokierten Speicher frei.
 * @param obj Objekt, des Speicher freigegeben werden soll.
 *            Vorbedingung: obj ist ein Zeiger auf ein existierendes Objekt.
 *            Nachbedingung: der Speicher fuer obj wurde freigegeben.
 */
void freeObject(CGObject * obj)
{
  obj->nPoints = 0;
  free (obj->points);
  obj->points = NULL;

  obj->nPlanes = 0;
  free (obj->planes);
  obj->planes = NULL;
}
