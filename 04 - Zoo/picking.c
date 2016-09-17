/**
 * @file
 *
 * Verarbeitet Picking-Ereignisse.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <assert.h>
#include <GL/glut.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "picking.h"
#include "scene.h"
#include "vector.h"
#include "logic.h"
#include "level.h"

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Verarbeitung der Picking-Ergebnisse.
 * Findet den Treffer, der dem Betrachter am naechsten liegt und gibt die
 * Namen dieses Treffers aus.
 * @param numHits Anzahl der getroffenen Objekte (In).
 * @param buffer Buffer, in dem die Treffer gespeichert sind (In).
 */
static void processHits(int numHits, unsigned buffer[], PickingAction a)
{
  /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
  int numOfClosestNames;
  /* Anzahl der Namen des Treffers, der am naechsten zum Betrachter ist */
  unsigned * ptrClosestNames;

  /* Laufvariable - Nummer des akt. verarbeiteten Treffers */
  int i;
  /* Laufvariable - Zeiger auf den Beginn des Hit-Records */
  unsigned * ptr = (unsigned *) buffer;

  /* DepthBuffer - Wert */
  unsigned minZ = 0xffffffff;

  if (numHits > 0)
  {
    /* Alle Treffer verarbeiten */
    for (i = 0; i < numHits; ++i)
    {
      /* Ist der Treffer naeher dran, als der bisher naechste? */
      if (*(ptr + 1) < minZ)
      {
        /* Anzahl der Namen merken */
        numOfClosestNames = *ptr;
        /* Zeiger auf den ersten Namen merken */
        ptrClosestNames = ptr + 3;
        /* als neuen minimalen Tiefenwert merken */
        minZ = *(ptr + 1);
      }
      /* zum Beginn des naechsten Hit-Records springen */
      /* Schrittweite: Anzahl-Feld + minZ-Feld + maxZ-Feld + Namen-Felder */
      ptr += 3 + numOfClosestNames;
    }
    
    #ifdef DEBUG
    fprintf(stderr, "DEBUG :: ");
    
    switch (a)
    {
      case ACTION_SELECT:
        fprintf(stderr, "Selecting ");
        break;
      case ACTION_RELEASE:
        fprintf(stderr, "Releasing ");
        /* CANNOT happen! */
        assert(0);
        break;
      case ACTION_ANIMATE:
        fprintf(stderr, "Animating ");
        break;
      default:
        fprintf(stderr, "PICKING ERROR!\n");
        /* CANNOT happen! */
        assert(0);
        break;
    }
    #endif
    
    switch (*ptrClosestNames)
    {
      case NAME_GIRAFFE:
      case NAME_GIRAFFE_HEAD:
      case NAME_GIRAFFE_BODY:
        #ifdef DEBUG
        fprintf(stderr, "Giraffe.\n");
        #endif
        
        switch (a)
        {
          case ACTION_SELECT:
            logicSelect(NAME_GIRAFFE, TRUE);
            break;
          case ACTION_RELEASE:
            /* CANNOT happen! */
            assert(0);
            break;
          case ACTION_ANIMATE:
            logicAnimate(ANIMATION_GIRAFFE);
            break;
        }
        break;
      case NAME_PIG:
      case NAME_PIG_HEAD:
      case NAME_PIG_BODY:
        #ifdef DEBUG
        fprintf(stderr, "Pig.\n");
        #endif
        
        switch (a)
        {
          case ACTION_SELECT:
            logicSelect(NAME_PIG, TRUE);
            break;
          case ACTION_RELEASE:
            /* CANNOT happen! */
            assert(0);
            break;
          case ACTION_ANIMATE:
            logicAnimate(ANIMATION_PIG);
            break;
        }
        break;
      case NAME_FISH:
      case NAME_FISH_HEAD:
      case NAME_FISH_BODY:
        #ifdef DEBUG
        fprintf(stderr, "Fish.\n");
        #endif
        
        switch (a)
        {
          case ACTION_SELECT:
            logicSelect(NAME_FISH, TRUE);
            break;
          case ACTION_RELEASE:
            /* CANNOT happen! */
            assert(0);
            break;
          case ACTION_ANIMATE:
            logicAnimate(ANIMATION_FISH);
            break;
        }
        break;
    }
  }
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Picking. Auswahl von Szenenobjekten durch Klicken mit der Maus.
 *
 * @param[in] x x-Koordinate.
 * @param[in] y y-Koordinate.
 * @param[in] a Auszuführende Aktion.
 */
extern void pick(int x, int y, PickingAction a)
{
  /** Groesse des Buffers fuer Picking Ergebnisse */
  #define SELECTBUFSIZE 512

  /* Viewport (Darstellungsbereich des Fensters) */
  GLint viewport[4];

  /* Puffer fuer Picking-Ergebnis */
  GLuint buffer[SELECTBUFSIZE];

  /* Anzahl der getroffenen Namen beim Picking */
  GLint numHits;

  /* aktuellen Viewport ermitteln */
  glGetIntegerv (GL_VIEWPORT, viewport);

  /* Puffer festlegen, Name-Stack leeren und Rendermode wechseln */
  glSelectBuffer (SELECTBUFSIZE, buffer);
  glInitNames ();
  glRenderMode (GL_SELECT);

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  gluPickMatrix (x, glutGet (GLUT_WINDOW_HEIGHT) - y, 5, 5, viewport);

  setProjection ((double) glutGet (GLUT_WINDOW_WIDTH) /
                 (double) glutGet (GLUT_WINDOW_HEIGHT));

  /* Nachfolgende Operationen beeinflussen Modelviewmatrix */
  glMatrixMode (GL_MODELVIEW);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  setCamera ();

  /* Zeichnen */
  sceneDraw (TRUE);

  /* Zeichnen beenden und auswerten */
  glFlush ();

  /* Nachfolgende Operationen beeinflussen Projektionsmatrix */
  glMatrixMode (GL_PROJECTION);
  /* Matrix zuruecksetzen - Einheitsmatrix laden */
  glLoadIdentity ();

  setProjection ((double) glutGet (GLUT_WINDOW_WIDTH) /
                 (double) glutGet (GLUT_WINDOW_HEIGHT));
  
  numHits = glRenderMode (GL_RENDER);
  
  processHits (numHits, buffer, a);
}
