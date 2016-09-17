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
       , y;
} Vector2d;

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
typedef Boolean (*CollisionDetectionFunction) (Vector2d hugo, Vector2d object);

/** Enum für Elemente, die gezeichnet werden sollen */
typedef enum {
  GROUND
, BACKGROUND
, SURFACE
, ENVIRONMENT
, ITEM
, OBSTACLE
, HUGO
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
