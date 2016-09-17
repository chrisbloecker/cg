/**
 * @file
 * Kann Level estellen.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"
#include "gen.h"
#include "vector.h"
#include "level.h"

/* ----------------------------------------------------------------------------
 * Konstanten / Macros
 * -------------------------------------------------------------------------- */

/* Platz, der nach einem Objekt frei bleiben muss */
#define GEN_SPACE_OBSTACLE (0.5)
#define GEN_SPACE_ITEM     (0.4)
#define GEN_SPACE_HUGO     (0.6)

/* "Ein bisschen Zufall" */
#define A_LITTLE_BIT_OF_RANDOM ((double) (0.01 * (rand() % 100)))

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Typ für die Straßenseite */
typedef enum {
  GEN_STREET_LEFT
, GEN_STREET_MIDDLE
, GEN_STREET_RIGHT
} GenStreetSide;

/* ----------------------------------------------------------------------------
 * Globale Daten
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Static
 * -------------------------------------------------------------------------- */

/**
 * Gibt den Level l aus.
 *
 * @param[in] l Level.
 */
void printLevelInfo(Level l)
{
  unsigned i;
  
  fprintf(stderr, "DEBUG :: LEVEL INFO ...\n");
  
  /* Die lieben Gegenstände */
  for (i = 0; i < l.itemLast; ++i)
    fprintf(stderr, "DEBUG :: %i : ITEM AT (%f, %f)\n", i, levelItemAt(l, i).x, levelItemAt(l, i).y);
  
  /* Die bösen Hindernisse */
  for (i = 0; i < l.obstacleLast; ++i)
    fprintf(stderr, "DEBUG :: %i : OBSTACLE AT (%f, %f)\n", i, levelObstacleAt(l, i).x, levelObstacleAt(l, i).y);
}

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Generiert einen Level.
 *
 * @return generierter Level.
 */
extern Level genLevel(void)
{
  double y = 0.0
       , next[3];
  
  GenStreetSide side
              , hugoSide = GEN_STREET_MIDDLE;
  
  Level l = levelInit();
  
  srand(time(NULL));
  
  for (side = GEN_STREET_LEFT; side <=  GEN_STREET_RIGHT; ++side)
    next[side] = 0.0;
  
  /* Genügend Hindernisse erzeugen */ 
  while (l.obstacleLast < LEVEL_COUNT_OBSTACLES)
  {
    /* Auf jeder Straßenseite versuchen, etwas zu erzeugen */
    for (side = GEN_STREET_LEFT; side <= GEN_STREET_RIGHT; ++side)
    {
      /**
       * Wenn bereits genügend Abstand gehalten wurde und Hugo diesen Platz
       * nicht beansprucht
       */
      if ((l.obstacleLast < LEVEL_COUNT_OBSTACLES)
      &&  (y > next[side])
      &&  (side != hugoSide)
      &&  (rand() % 2))
      {
        l = levelObstacleAppend(l, vectorMake(side, y));
        next[side] = y + GEN_SPACE_OBSTACLE;
      }
      
      /* Auch ein paar Items erstellen */
      if ((l.itemLast < LEVEL_COUNT_ITEMS)
      &&  (y > next[side])
      &&  (rand() % 2))
      {
        l = levelItemAppend(l, vectorMake(side, y));
        next[side] = y + GEN_SPACE_ITEM;
      }
    }
    
    if (rand() % 2)
    {
      next[hugoSide] = y + GEN_SPACE_HUGO;
      
      /* Hugo bewegen */
      switch (hugoSide)
      {
        case GEN_STREET_LEFT:
          if (rand() % 2)
            ++hugoSide;
            next[hugoSide] = y + GEN_SPACE_HUGO;
          break;
        case GEN_STREET_MIDDLE:
          hugoSide += (rand() % 2) ? - 1 : 1;
          break;
        case GEN_STREET_RIGHT:
          if (rand() % 2)
            --hugoSide;
            next[hugoSide] = y + GEN_SPACE_HUGO;
          break;
      }
    } 
    
    next[hugoSide] = y + GEN_SPACE_HUGO;
    
    y += 0.5 * A_LITTLE_BIT_OF_RANDOM;
  }
    
  return l;
}
