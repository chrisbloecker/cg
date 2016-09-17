#ifndef __LEVEL_H__
#define __LEVEL_H__
/**
 * @file
 * Definiert die Datenstrukturen für Level und Spielstand.
 * Bietet Funktionen zur Arbeit mit Levels.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "types.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
 
/** Anzahl der Hindernisse und Items*/
#define LEVEL_COUNT_OBSTACLES (25)
#define LEVEL_COUNT_ITEMS     (15)

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Datenstruktur eines Levels */
typedef struct {
  Vector2d items[LEVEL_COUNT_ITEMS]          /* Gegenstände        */
         , obstacles[LEVEL_COUNT_OBSTACLES]; /* Hindernisse        */
  unsigned itemFirst                         /* Erster  Gegenstand */
         , itemLast                          /* Letzter Gegenstand */
         , obstacleFirst                     /* Erstes  Hindernis  */
         , obstacleLast;                     /* Letztes Hindernis  */
} Level;

/** Spielstatus */
typedef struct {
  int      lives;  /* Leben          */
  unsigned level   /* Level#         */
         , points; /* Punktestand    */
  Level l;         /* Levelzzzzzz    */
  Vector2d hugo;   /* Hugos Position */
} Game;

/**
 * Initialisiert den Level.
 *
 * @return initialisierter Level.
 */
extern Level levelInit(void);

/**
 * Gibt die Position des i-ten Gegenstandes des Levels l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Position des i-ten Gegenstandes des Levels l. 
 */
extern Vector2d levelItemAt(Level l, unsigned i);

/**
 * Gibt die Position des i-ten Hindernisses des Levels l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Position des i-ten Hindernisses des Levels l. 
 */
extern Vector2d levelObstacleAt(Level l, unsigned i);

/**
 * Gibt zurück, wieviele Gegenstände es nich im Level l gibt.
 *
 * @param[in] l Level.
 *
 * @return Anzahl der Gegenstände, die es nich im Level l gibt.
 */
extern unsigned levelItemsLeft(Level l);

/**
 * Gibt zurück, wieviele Hindernisse es nich im Level l gibt.
 *
 * @param[in] l Level.
 *
 * @return Anzahl der Hindernisse, die es nich im Level l gibt.
 */
extern unsigned levelObstaclesLeft(Level l);

/**
 * Fügt einen Gegenstand in das Level l ein.
 *
 * @param[in] l    Level.
 * @param[in] item Gegenstand, der eingefügt werden soll.
 *
 * @return Level, in den der Gegenstand item eingefügt wurde.
 */
extern Level levelItemAppend(Level l, Vector2d item);

/**
 * Fügt ein Hindernis in das Level l ein.
 *
 * @param[in] l        Level.
 * @param[in] obstacle Hindernis, das eingefügt werden soll.
 *
 * @return Level, in den das Hindernis obstacle eingefügt wurde.
 */
extern Level levelObstacleAppend(Level l, Vector2d obstacle);

/**
 * Löscht den Gegenstand an Position i im Level l.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Level, aus dem der Gegenstand an Position i entfernt wurde.
 */
extern Level levelItemRemoveAt(Level l, unsigned i);

/**
 * Löscht das Hindernis an Position i im Level l.
 *
 * @param[in] l Level.
 * @param[in] i Index.
 *
 * @return Level, aus dem das Hinderniss an Position i entfernt wurde.
 */
extern Level levelObstacleRemoveAt(Level l, unsigned i);

#endif
