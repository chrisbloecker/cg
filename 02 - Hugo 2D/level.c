/**
 * @file
 * Definiert die Datenstrukturen für Level und Spielstand.
 * Bietet Funktionen zur Arbeit mit Levels.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <assert.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "level.h"

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Initialisiert den Level.
 *
 * @return initialisierter Level.
 */
extern Level levelInit(void)
{
  Level l;
  
  l.itemFirst     = 0;
  l.itemLast      = 0;
  l.obstacleFirst = 0;
  l.obstacleLast  = 0;
  
  return l;
}

/**
 * Gibt die Position des i-ten Gegenstandes des Levels l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @pre i muss größer  oder gleich dem Index des ersten  Gegenstandes sein.
 * @pre i muss kleiner oder gleich dem Index des letzten Gegenstandes sein.
 * @pre Es muss noch mindestens einen Gegenstand im Level geben.
 *
 * @return Position des i-ten Gegenstandes des Levels l. 
 */
extern Vector2d levelItemAt(Level l, unsigned i)
{
  assert(i >= l.itemFirst);
  assert(i <= l.itemLast);
  assert((int) (l.itemLast - l.itemFirst) >= 0);
  
  return l.items[i];
}

/**
 * Gibt die Position des i-ten Hindernisses des Levels l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @pre i muss größer  oder gleich dem Index des ersten  Hindernisses sein.
 * @pre i muss kleiner oder gleich dem Index des letzten Hindernisses sein.
 * @pre Es muss noch mindestens ein Hindernis im Level geben.
 *
 * @return Position des i-ten Hindernisses des Levels l. 
 */
extern Vector2d levelObstacleAt(Level l, unsigned i)
{
  assert(i >= l.obstacleFirst);
  assert(i <= l.obstacleLast);
  assert((int) (l.obstacleLast - l.obstacleLast) >= 0);
  
  return l.obstacles[i];
}

/**
 * Gibt zurück, wieviele Gegenstände es nich im Level l gibt.
 *
 * @param[in] l Level.
 *
 * @return Anzahl der Gegenstände, die es nich im Level l gibt.
 */
extern unsigned levelItemsLeft(Level l)
{
  return l.itemLast - l.itemFirst;
}

/**
 * Gibt zurück, wieviele Hindernisse es nich im Level l gibt.
 *
 * @param[in] l Level.
 *
 * @return Anzahl der Hindernisse, die es nich im Level l gibt.
 */
extern unsigned levelObstaclesLeft(Level l)
{
  return l.obstacleLast - l.obstacleFirst;
}

/**
 * Fügt einen Gegenstand in das Level l ein.
 *
 * @param[in] l    Level.
 * @param[in] item Gegenstand, der eingefügt werden soll.
 *
 * @return Level, in den der Gegenstand item eingefügt wurde.
 */
extern Level levelItemAppend(Level l, Vector2d item)
{
  assert(l.itemLast < LEVEL_COUNT_ITEMS);
  
  l.items[l.itemLast++] = item;
  
  return l;
}

/**
 * Fügt ein Hindernis in das Level l ein.
 *
 * @param[in] l        Level.
 * @param[in] obstacle Hindernis, das eingefügt werden soll.
 *
 * @return Level, in den das Hindernis obstacle eingefügt wurde.
 */
extern Level levelObstacleAppend(Level l, Vector2d obstacle)
{
  assert(l.obstacleLast < LEVEL_COUNT_OBSTACLES);
  
  l.obstacles[l.obstacleLast++] = obstacle;
  
  return l;
}

/**
 * Löscht den Gegenstand an Position i im Level l.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Level, aus dem der Gegenstand an Position i entfernt wurde.
 */
extern Level levelItemRemoveAt(Level l, unsigned i)
{
  unsigned j;
  
  assert(i >= l.itemFirst);
  assert(i <= l.itemLast );
  assert((int) (l.itemLast - l.itemFirst) >= 0);
  
  for (j = i; j < l.itemLast; ++j)
    l.items[j] = l.items[j + 1];
  
  --l.itemLast;
  
  return l;
}

/**
 * Löscht das Hindernis an Position i im Level l.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Level, aus dem das Hinderniss an Position i entfernt wurde.
 */
extern Level levelObstacleRemoveAt(Level l, unsigned i)
{
  unsigned j;
  
  assert(i >= l.obstacleFirst);
  assert(i <= l.obstacleLast );
  assert((int) (l.obstacleLast - l.obstacleFirst) >= 0);
  
  for (j = i; j < l.obstacleLast; ++j)
    l.obstacles[j] = l.obstacles[j + 1];
  
  --l.obstacleLast;
  
  return l;
}
