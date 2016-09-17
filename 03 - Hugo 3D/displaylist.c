/**
 * @file
 *
 * Das Modul stellt Displaylisten zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */
 
/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */ 
#include <GL/gl.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "displaylist.h"
#include "types.h"
#include "logic.h"
#include "drawing.h"

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */
static int displayLists[DF_BUILDING + 1];

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */
 
/**
 * Initialisiert die Displaylisten.
 */
extern void displaylistInit(void)
{
  displayLists[DF_GROUND] = glGenLists(DF_BUILDING + 1);
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Display Lists generated.\n");
  #endif
}

/**
 * Befüllt die Displaylisten mit den aktuellen Objekten.
 */
extern void displaylistSet(void)
{
  if (displayLists[DF_GROUND] != 0)
  {
    /* Boden */
    glNewList(displayLists[DF_GROUND], GL_COMPILE);
      logicGetDrawFunction(DF_GROUND)();
    glEndList();

    /* Hintergrund */
    displayLists[DF_BACKGROUND] = displayLists[DF_GROUND] + 1;
    glNewList(displayLists[DF_BACKGROUND], GL_COMPILE);
      logicGetDrawFunction(DF_BACKGROUND)();
    glEndList();
    
    /* Bodenbelag */
    displayLists[DF_SURFACE] = displayLists[DF_BACKGROUND] + 1;
    glNewList(displayLists[DF_SURFACE], GL_COMPILE);
      logicGetDrawFunction(DF_SURFACE)();
    glEndList();
    
    /* Umgebung */
    displayLists[DF_ENVIRONMENT] = displayLists[DF_SURFACE] + 1;
    glNewList(displayLists[DF_ENVIRONMENT], GL_COMPILE);
      logicGetDrawFunction(DF_ENVIRONMENT)();
    glEndList();
    
    /* Gegenstände */
    displayLists[DF_ITEM] = displayLists[DF_ENVIRONMENT] + 1;
    glNewList(displayLists[DF_ITEM], GL_COMPILE);
      logicGetDrawFunction(DF_ITEM)();
    glEndList();
    
    /* Hindernisse */
    displayLists[DF_OBSTACLE] = displayLists[DF_ITEM] + 1;
    glNewList(displayLists[DF_OBSTACLE], GL_COMPILE);
      logicGetDrawFunction(DF_OBSTACLE)();
    glEndList();
    
    /* Hugo */
    displayLists[DF_HUGO] = displayLists[DF_OBSTACLE] + 1;
    glNewList(displayLists[DF_HUGO], GL_COMPILE);
      logicGetDrawFunction(DF_HUGO)();
    glEndList();
    
    /* Gebäude */
    displayLists[DF_BUILDING] = displayLists[DF_HUGO] + 1;
    glNewList(displayLists[DF_BUILDING], GL_COMPILE);
      logicGetDrawFunction(DF_BUILDING)();
    glEndList();
  }
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Display Lists set.\n");
  #endif
}

/**
 * Ruft die Displayliste für den Zeichenfunktionstyp d auf.
 *
 * @param[in] d Zeichenfunktionstyp.
 */
extern void displaylistCall(DrawFunctionType d)
{
  glCallList(d + 1);
}
