#ifndef __LEVEL_H__
#define __LEVEL_H__

/**
 * @file
 *
 * Stellt Funktionen für die Arbeit mit der Level-Datenstruktur zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include "vector.h"

/** Anzahl an Objekten, die ein Level enthalten kann. */
#define LEVEL_ITEMCOUNT (15)

/** Picking Namen */
typedef enum {
  NAME_UNPICKABLE
, NAME_GROUND
, NAME_SOCKET
, NAME_CEILING
, NAME_FENCE_FRONT
, NAME_FENCE_BACK
, NAME_FENCE_LEFT
, NAME_FENCE_RIGHT
, NAME_POOL
, NAME_WATER
, NAME_PIG
, NAME_PIG_HEAD
, NAME_PIG_BODY
, NAME_GIRAFFE
, NAME_GIRAFFE_HEAD
, NAME_GIRAFFE_BODY
, NAME_FISH
, NAME_FISH_HEAD
, NAME_FISH_BODY
, NAME_SUN
, NAME_GIRAFFE_CUBE
} PickingName;

/** Ein LevelItem */
typedef struct {
  Vector3d p           /* Position    */
         , r           /* Rotation    */
         , t           /* Translation */
         ;
  DrawFunctionType f;  /* Funktion, die zum Zeichnen verwendet werden soll */
  
  double size
       , a             /* Angle */
       ;
  
  PickingName n;       /* Name */
  
  unsigned z;          /* z-Ebene */
} LevelItem;

/** Ein Level. */
typedef struct {
  LevelItem * items[LEVEL_ITEMCOUNT]; /* Zeiger auf Elemente */
  
  unsigned last /* Index des letzten Elementes */
         , zMax /* maximale z-Ebene            */
         ;
} Level;

/**
 * Initialisiert einen Level und gibt ihn zurück.
 *
 * @return Level.
 */
extern Level levelInit();

/**
 * Fügt das Item i in den Level l ein.
 *
 * @param[in] l   Level.
 * @param[in] i   Einzufügendes Objekt.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return Level, in den i eingefügt wurde.
 */
extern Level levelInsert(Level l, LevelItem * i, Vector3d eye, Vector3d n);

/**
 * Erzeugt ein LevelItem und gibt es zurück.
 *
 * @param[in] p Position.
 * @param[in] r Rotationsache.
 * @param[in] t Translation.
 * @param[in] f Zeichenfunktion.
 * @param[in] a Drehwinkel.
 * @param[in] n Name.
 * @param[in] z z-Ebene.
 *
 * @return LevelItem mit den oben genannten Eigenschaften.
 */
extern LevelItem levelItemMake(Vector3d p, Vector3d r, Vector3d t, DrawFunctionType f, double a, PickingName n, unsigned z);

/**
 * Gibt einen Zeiger auf das i-te LevelItem von l zurück.
 *
 * @param[in] l Level.
 * @param[in] i Index des auszuwählenden Elementes.
 *
 * @return Zeiger auf das i-te Element von l.
 */
extern LevelItem * levelItemAt(Level l, unsigned i);

/**
 * Sortiert den Level l.
 *
 * @param[in] l   Level.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return sortierter Level.
 */
extern Level levelSort(Level l, Vector3d eye, Vector3d n);

/**
 * Gibt die String-Repräsentation des PickinName n zurück.
 *
 * @param[in] n PickinName.
 *
 * @return String-Repräsentation von n.
 */
extern char * levelPickingNameToString(PickingName n);

#ifdef DEBUG
/**
 * Gibt den Level l aus.
 *
 * @param[in] l Level.
 */
extern void levelPrint(Level l);

/**
 * Gibt die Zeichenreihenfolge der Objekte des Levels l aus.
 *
 * @param[in] l Level.
 */
extern void levelPrintDrawingOrder(Level l);
#endif

#endif
