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
  drawString(X0, 0.40, textColor, "l/L");
  drawString(X0, 0.45, textColor, "a/A");
  drawString(X0, 0.50, textColor, "d/D");
  drawString(X0, 0.55, textColor, "c/C");
  drawString(X0, 0.60, textColor, "s/S");
  drawString(X0, 0.65, textColor, "Left Mouse");
  drawString(X0, 0.70, textColor, "Right Mouse");
  drawString(X0, 0.75, textColor, "h/H");
  
  drawString(X1, 0.25, textColor, "Programm beenden");
  drawString(X1, 0.30, textColor, "Wuerfel verfeinern");
  drawString(X1, 0.35, textColor, "Wuerfel vergroebern");
  drawString(X1, 0.40, textColor, "Linien-/Flaechendarstellung");
  drawString(X1, 0.45, textColor, "Voriger Vertex");
  drawString(X1, 0.50, textColor, "Naechster Vertex");
  drawString(X1, 0.55, textColor, "Zufaellige Farbe zuweisen");
  drawString(X1, 0.60, textColor, "Knoten ein/aus");
  drawString(X1, 0.65, textColor, "Bewegen");
  drawString(X1, 0.70, textColor, "Zoomen");
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
  VertexCube cube = logicGetCube();
  
  if (logicGetToggleable(LOGIC_TOGGLEABLE_SPHERES))
    vertexCubeDrawNodes(cube, logicGetInt(LOGIC_INT_SELECTEDNODE));
  
  if (logicGetToggleable(LOGIC_TOGGLEABLE_EDGES))
    vertexCubeDrawPlanes(cube);
  
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
  glEnableClientState(GL_COLOR_ARRAY);
  
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
