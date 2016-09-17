#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * "Konstanten"
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Typedeklarationen
 * -------------------------------------------------------------------------- */

/** Boolean */
typedef enum {FALSE, TRUE} Boolean;

/** Ein Vektor im R² */
typedef struct {
  double x
       , y
       , z;
} Vector3d;

/** RGB-Farbwert */
typedef struct {
  double r
       , g
       , b;
} RGBColor;

/** Funktionszeiger für eine Zeichenfunktion */
typedef void (*DrawFunction) (void);

/** Funktionszeiger für eine Animationsfunktion */
typedef void (*AnimationFunction) (double xShift);

/** Funktionszeiger für eine Kollisionserkennungsfunktion, geiles Wort */
typedef Boolean (*CollisionDetectionFunction) (Vector3d hugo, Vector3d object);

/** Enum für Elemente, die gezeichnet werden sollen */
typedef enum {
  DF_GROUND
, DF_BACKGROUND
, DF_SURFACE
, DF_ENVIRONMENT
, DF_ITEM
, DF_OBSTACLE
, DF_HUGO
, DF_BUILDING
} DrawFunctionType;

/** Enum für Kollisionen */
typedef enum {
  COLLISION_ITEM
, COLLISION_OBSTACLE
} CollisionType;

/** Bewegungsrichtung für Hugo */
typedef enum {
  DIR_EMPTY
, DIR_LEFT
, DIR_RIGHT
} Direction;

#endif
