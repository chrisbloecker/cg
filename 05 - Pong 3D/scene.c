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
#include <assert.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * Eigene
 * -------------------------------------------------------------------------- */
#include "scene.h"
#include "logic.h"
#include "vector.h"
#include "matrix.h"
#include "object_cg.h"
#include "drawing.h"
#include "stringOutput.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */

#define INFTY (10)
 
/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* Farben der Lichtquellen */
static float lightColor[1][3][4] = { { {0.1, 0.1, 0.1, 1.0}
                                     , {1.0, 1.0, 1.0, 1.0}
                                     , {1.5, 1.5, 1.5, 1.0}
                                     }
                                   };

#ifdef DEBUG
static Boolean zInfo = FALSE; /* z-Informationen anzeigen ? */
#endif

/* Objekt zur Schattenberechnung der Würfel */
static CGObject objCube;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Lichtquellen setzen und ein- bzw. ausschalten.
 */
static void setLights(void)
{
  /* Sonne */
  if (logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT0))
  {
    Vector3d light = logicGetObject(LOGIC_OBJECT_LIGHT)->t;
    
    CGVector4f v;
    
    v[0] = light.x;
    v[1] = light.y;
    v[2] = light.z;
    v[3] = 1.0;
    
    glLightfv(GL_LIGHT0, GL_POSITION, v);
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor[0][1]);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightColor[0][0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor[0][2]);
  }
  
  /* Ball */
  if (logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT1))
  {
    float lp[4];
    Object * o = logicGetObject(LOGIC_OBJECT_BALL);
    
    lp[0] = o->t.x;
    lp[1] = o->t.y;
    lp[2] = o->t.z;
    lp[3] = 1.0;
    
    glLightfv(GL_LIGHT1, GL_POSITION, lp);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightColor[0][1]);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  lightColor[0][0]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor[0][2]);
  }
  
  logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT0)
    ? glEnable(GL_LIGHT0)
    : glDisable(GL_LIGHT0);
  
  logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT1)
    ? glEnable(GL_LIGHT1)
    : glDisable(GL_LIGHT1);
}

/**
 * Schatten einer Kugel unter Benutzung eines Kegels mit dem oberen Radius s zeichnen.
 *
 * @param[in] s oberer Radius des Kegels.
 */
static void castSphereShadow(double s)
{
  /* aktuelle Einstellungen sichern */
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT |
               GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
               GL_STENCIL_BUFFER_BIT);

  glCullFace(GL_BACK);

  glEnable(GL_CULL_FACE);

  /* Beleuchtung deaktivieren */
  glDisable(GL_LIGHTING);

  /* Schreiben in den Tiefenpuffer deaktivieren */
  glDepthMask(GL_FALSE);
  glDepthFunc (GL_LEQUAL); /* !!! */

  /* Stenciltest aktivieren */
  glEnable(GL_STENCIL_TEST);

  /* Schreiben in den Farbpuffer deaktivieren */
  glColorMask(0, 0, 0, 0);

  /* Funktion und Referenz wert fuer den Stenciltest setzen.
     Hier: Alles in den Stencilpuffer schreiben. */
  glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

  /* Erster Durchlauf: Rendern der kompletten Shadow-Volumes,
     vom schattenwerfenden Objekt durch den (leeren) Raum
     bis zum Boden bzw. zur Wand.
     Der Stencil-Puffer (der vorher ueberall 0 ist) wird an
     den Stellen auf 1 gesetzt, wo ein Shadow-Volume vorhanden
     ist (GL_INCR).
     Die Face-Orientierung ist dabei auf CCW (CounterClockWise)
     gestellt, alle FRONTfaces erzeugen also das Shadow-Volume. */
  glFrontFace(GL_CCW);
  glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

  drawKegel(s, TEXTURE_EMPTY);

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
  glFrontFace(GL_CW);
  glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);

  drawKegel(s, TEXTURE_EMPTY);

  /* Face-Orientierung wieder auf "normal" setzen */
  glFrontFace(GL_CCW);
  /* Schreiben in den Farbpuffer wieder aktivieren */
  glColorMask(1, 1, 1, 1);

  /* "Schattierendes" Rechteck zeichnen, das die gesamte Szene
     ueberdeckt. Es wird nur an den Stellen mittels Blending
     mit den Werten im Farbpuffer verrechnet, an denen der
     Stencil-Puffer einen Wert <> 0 hat, wo also ein Schatten
     entstehen soll. */
  glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);

  /* Schattenfarbe */
  glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

  /* Blending aktivieren und setzen */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Projektionsmatrix veraendern */
  glMatrixMode(GL_PROJECTION);

  /* aktuelle Projektionsmatrix sichern */
  glPushMatrix();
  {
    /* neue orthogonale 2D-Projektionsmatrix erzeugen */
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    /* ModelView-Matrix veraendern */
    glMatrixMode(GL_MODELVIEW);

    /* aktuelle ModelView-Matrix sichern */
    glPushMatrix();
    {
      /* ModelView-Matrix zuruecksetzen */
      glLoadIdentity();

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
    glPopMatrix();

    /* Projektionsmatrix veraendern */
    glMatrixMode(GL_PROJECTION);

    /* alte Projektionsmatrix laden */
  }
  glPopMatrix();

  /* auf der ModelView-Matrix weiterarbeiten */
  glMatrixMode(GL_MODELVIEW);

  /* alte Einstellungen laden */
  glPopAttrib();
}

/**
 * Schatten einer Kugel zeichnen.
 *
 * @param[in] light Lichtquelle.
 * @param[in] o     Kugeliges, schattenwerfendes Objekt.
 */
static void castSphereShadows(Vector3d light, Object * o)
{
  Vector3d lightToBall /* ... */
         , axis        /* Rotation Axis */
         ;
  
  double d;            /* Distance Light -> Ball */
  
  lightToBall = vectorSub(o->t, light);
  axis        = vectorCross(lightToBall, vectorMake(0,1,0));
  
  d = vectorLength(lightToBall);
  
  glPushMatrix();
    /* affinieren */
    materialSet(MAT_GLASS);
    glTranslatef(o->t.x, o->t.y, o->t.z);
    glRotatef(vectorAngle(lightToBall, vectorMake(0,-1,0)), axis.x, axis.y, axis.z);
    glScalef(o->s.x + INFTY / d, 2 * INFTY, o->s.z + INFTY / d);
    glTranslatef(0, -1, 0);
    
    /* Schatten zeichnen */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_SHADOWS))
    {
      castSphereShadow(o->s.x / (o->s.x + INFTY / d));
    }
    
    /* Shadow Volume zeichnen */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_SHADOWVOLUMES))
    {
      /* aktuelle Einstellungen sichern */
      glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);

      glDisable(GL_LIGHTING);

      materialSet(MAT_GLASS);
      
      drawKegel(o->s.x / (o->s.x + INFTY / d), TEXTURE_EMPTY);
      
      glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      
      drawKegel(o->s.x / (o->s.x + INFTY / d), TEXTURE_EMPTY);

      /* alte Einstellungen laden */
      glPopAttrib();
    }
  glPopMatrix();
}

/**
 * Zeichnet die Schatten aller schattenwerfenden Objekte.
 */
static void castCubeShadows(Vector3d light, Object * o)
{
  CGVector4f lp;

  CGMatrix16f minv;
  CGVector3f v;

  /* Zur Berechnung von Schatten wird die Lichtposition relativ zum
     schattenwerfenden Objekt benoetigt. Da hierbei auch die Orientierung des
     Objektes relevant ist, werden alle Transformationen, die auf das Objekt
     angewendet werden, invertiert und in umgekehrter Reihenfolge auf die
     Position der Lichtquelle ausgefuehrt. Hierzu werden die entsprechenden
     Transformationsmatrizen ausgehen von der Einheitsmatrix miteinander
     multiplziert. Wird die Lichtposition mit der daraus resultierenden Matrix
     multiplziert erhaelt man die Lichtposition im lokalen Koordinatensystem des
     Objektes. */

  /* Lichtposition in homogene Koordinaten "umwandeln" */
  lp[0] = light.x;
  lp[1] = light.y;
  lp[2] = light.z;
  lp[3] = 1;
  
  /* Einheitsmatrix erzeugen */
  getIdentity(&minv);
  
  v[0] = 1.0 / o->s.x;
  v[1] = 1.0 / o->s.y;
  v[2] = 1.0 / o->s.z;
  Scale(&minv, v);

  /* invertierte Rotationen des Objekt anwenden */
  v[0] = 0.0f;
  v[1] = 1.0f;
  v[2] = 0.0f;
  Rotate(&minv, v, -o->a);

  /* Translation des Objektes negiert anwenden */
  v[0] = -o->t.x;
  v[1] = -o->t.y;
  v[2] = -o->t.z;
  Translate(&minv, v);

  /* Inverse Transformation der Lichtposition ergibt deren Position im lokalen
     Koordinatensystem des Objektes */
  VectorMatrixMult(minv, &lp);

  /* Ende der Lichtpositionsberechnung */

  /* Sichtbarkeit der Flaechen von der Lichtquelle aus berechnen */
  calcVisibility(&objCube, lp);

  /* Schatten des Objekts zeichnen */
  glPushMatrix();
  {
    glTranslatef(o->t.x, o->t.y, o->t.z);
    glRotatef(o->a, 0.0, 1.0, 0.0);
    glScalef(o->s.x, o->s.y, o->s.z);
    
    /* Schatten zeichnen */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_SHADOWS))
    {
      castShadow(objCube, lp);
    }

    /* ggf. Schattenvolumen zeichnen */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_SHADOWVOLUMES))
    {
      /* aktuelle Einstellungen sichern */
      glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);

      glDisable(GL_LIGHTING);

      materialSet(MAT_GLASS);
      
      drawShadowVolume(objCube, lp);

      glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      
      drawShadowVolume(objCube, lp);

      /* alte Einstellungen laden */
      glPopAttrib();
    }
  }
  glPopMatrix();
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet die Szene.
 *
 * @param[in] shadow Gibt an, ob Schatten gezeichnet werden soll.
 */
extern void sceneDraw(Boolean shadow)
{
  /*static float textColor[3] = { 1.0f, 0.0f, 0.0f };*/
  
  Objects * os = logicGetObjects(LOGIC_OBJECTS_LEVEL);
  Object  * o;

  int i = 0
    , z = 0;
  
  setLights();
  
  i = os->last - 1;
  
  /* Back -> Front */
  while (z < os->zMax && i >= 0)
  {
    o = objectsAt(os, i);
    
    /* z-BUffer bei neuer z-Ebene löschen */
    if (z != o->z)
    {
      #ifdef DEBUG
      if (zInfo)
        fprintf(stderr, "DEBUG :: Clearing Z-Buffer.\n");
      #endif
      
      glClear(GL_DEPTH_BUFFER_BIT);
      
      ++z;
    }
    
    objectDraw(o);
    
    --i;
  }
  
  /* letzte z-Ebene */
  while (i >= 0)
  {
    o = objectsAt(os, i);
    
    objectDraw(o);
    
    --i;
  }
  
  #ifdef DEBUG
  if (zInfo)
    fprintf(stderr, "\n");
  #endif
  
  /* Schatten zeichnen */
  if (shadow)
  {
    /* "Lampe" */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT0))
    {
      Vector3d light = logicGetObject(LOGIC_OBJECT_LIGHT)->t;
      
      os = logicGetObjects(LOGIC_OBJECTS_SHADOWABLEBYLIGHT);
      
      for (i = 0; i < os->last; ++i)
      {
        o = objectsAt(os, i);
        
        switch (o->f)
        {
          case FIGURE_CUBE:
            castCubeShadows(light, o);
            break;
          case FIGURE_SPHERE:
            castSphereShadows(light, o);
            break;
          default:
            assert(0);
            break;
        }
      }
    }
    
    /* Ball */
    if (logicGetSwitchable(LOGIC_SWITCHABLE_LIGHT1))
    {
      Vector3d light = logicGetObject(LOGIC_OBJECT_BALL)->t;
      
      os = logicGetObjects(LOGIC_OBJECTS_SHADOWABLEBYBALL);
      
      for (i = 0; i < os->last; ++i)
      {
        o = objectsAt(os, i);
        
        switch (o->f)
        {
          case FIGURE_CUBE:
            castCubeShadows(light, o);
            break;
          case FIGURE_SPHERE:
            castSphereShadows(light, o);
            break;
          default:
            assert(0);
            break;
        }
      }
    }
  }
  
  /* Infos anzeigen 
  drawString(0.05, 0.10, textColor, "Lifes: %i", logicGetInt(LOGIC_INT_LIFES));
  drawString(0.05, 0.15, textColor, "Points: %i", logicGetInt(LOGIC_INT_POINTS));*/
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
  
  /* z-Buffer */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  
  /* Licht */
  glEnable(GL_LIGHTING);
  
  /* Culling */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  /* Normalen normalisieren */
  glEnable(GL_NORMALIZE);
  
  /* Blending */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  /* Texturierung */
  glEnable(GL_TEXTURE_2D);
  
  initObject(&objCube);
  
  return 1;
}

#ifdef DEBUG
/**
 * Schaltet das Anzeigen von z-Informationen ein bzw. aus.
 */
extern void sceneSetZInfo(void)
{
  zInfo = !zInfo;
  
  fprintf(stderr, "DEBUG :: Z-Info %s.\n", zInfo ? "on" : "off");
}
#endif

/**
 * Schaltet um zwischen Wireframe Mode und Füllmode.
 */
extern void sceneSwitchWireframeMode(void)
{
  /* Flag: Wireframe: ja/nein */
  static GLboolean wireframe = GL_FALSE;

  /* Modus wechseln */
  wireframe = !wireframe;

  if (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * Räumt auf.
 */
extern void sceneCleanup(void)
{
  freeObject(&objCube);
}
