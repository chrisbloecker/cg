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
#include "logic.h"
#include "types.h"

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */
static int displayLists[DF_DUMMY];

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
  displayLists[(DrawFunctionType) 0] = glGenLists(DF_DUMMY);
  
  #ifdef DEBUG
  fprintf(stderr, "DEBUG :: Display Lists generated.\n");
  #endif
}

/**
 * Befüllt die Displaylisten mit den aktuellen Objekten.
 */
extern void displaylistSet(void)
{
  if (displayLists[(DrawFunctionType) 0] != 0)
  {
    DrawFunctionType t;
    
    /* Erste */
    glNewList(displayLists[(DrawFunctionType) 0], GL_COMPILE);
      logicGetDrawFunction((DrawFunctionType) 0)();
    glEndList();
    
    /* Folgende */
    for (t = (DrawFunctionType) 1; t < DF_DUMMY; ++t)
    {
      displayLists[t] = displayLists[t-1] + 1;
      glNewList(displayLists[t], GL_COMPILE);
        logicGetDrawFunction(t)();
      glEndList();
    }
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
