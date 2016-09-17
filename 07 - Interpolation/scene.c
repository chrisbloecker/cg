/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalität (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * Header
 * ----------------------------------------------------------------------------
 * System
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>
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
#include "stringOutput.h"
#include "geometry.h"

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
 
/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Zeigt die Hilfe an.
 */
static void drawHelp(void)
{
  #define X0 (0.2)
  #define X1 (0.5)
  
  float textColor[3] = { 1.0, 1.0, 1.0 };
  
  drawString(X0, 0.25, textColor, "q/Q/ESC");
  drawString(X0, 0.30, textColor, "+");
  drawString(X0, 0.35, textColor, "-");
  drawString(X0, 0.40, textColor, "p/P");
  drawString(X0, 0.45, textColor, "o/O");
  drawString(X0, 0.50, textColor, "n/N");
  drawString(X0, 0.55, textColor, "b/B");
  drawString(X0, 0.60, textColor, "c/C");
  drawString(X0, 0.65, textColor, "Left Mouse");
  drawString(X0, 0.70, textColor, "Right Mouse");
  drawString(X0, 0.75, textColor, "h/H");
  
  drawString(X1, 0.25, textColor, "Programm beenden");
  drawString(X1, 0.30, textColor, "Interpolation verfeinern");
  drawString(X1, 0.35, textColor, "Interpolation vergroebern");
  drawString(X1, 0.40, textColor, "Punkt hinzufuegen");
  drawString(X1, 0.45, textColor, "Punkt entfernen");
  drawString(X1, 0.50, textColor, "Normalen Anzeigen");
  drawString(X1, 0.55, textColor, "Interpolationsart wechseln");
  drawString(X1, 0.60, textColor, "Konvexe Huelle zeichnen");
  drawString(X1, 0.65, textColor, "Punkte bewegen");
  drawString(X1, 0.70, textColor, "Punkte hinzufuegen");
  drawString(X1, 0.75, textColor, "Hilfe anzeigen");
  
  #undef X0
  #undef X1
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
extern void sceneDraw(void)
{
  Geometry g = logicGetGeometry();
  
  /* Textfarbe */
  static float textColor[3] = { 1.0f, 0.0f, 0.0f };
  
  /* Interpoaltionsmethode und -feinheit anzeigen */
  drawString(0.05, 0.10, textColor, "%s (%i)", matrixInterpolationMatrixToString(g.i), g.subDiv);
  
  /* Interpolationspolynom */
  if (!logicGetToggleable(LOGIC_TOGGLEABLE_CONVEXHULL))
  {
    /* Interpolationspolynom */
    glDrawElements(GL_LINE_STRIP, g.subDiv * (g.n - 3), GL_UNSIGNED_INT, g.indices);
    
    /* Normalen */
    if (logicGetToggleable(LOGIC_TOGGLEABLE_NORMALS))
      glDrawElements(GL_LINES, 2 * g.subDiv * (g.n - 3) - 2, GL_UNSIGNED_INT, g.normals);
  }
  /* Konvexe Hülle */
  else
    glDrawElements(GL_LINE_STRIP, g.nCH, GL_UNSIGNED_INT, g.indices);
  
  /* Punkte zeichnen */
  geometryDrawPoints(g);
  
  /* Hilfe */
  if (logicGetToggleable(LOGIC_TOGGLEABLE_HELP))
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
  
  /* z-Buffer */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  /* Normalen normalisieren */
  glEnable(GL_NORMALIZE);
  
  /* Vertex-Arrays aktivieren */
  glEnableClientState(GL_VERTEX_ARRAY);
  /*glEnableClientState(GL_COLOR_ARRAY);*/
  
  return 1;
}

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
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Wireframe mode %s.\n", wireframe ? "on" : "off");
  #endif
}
