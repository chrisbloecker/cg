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
 
/** Pi */
#define PI (3.1415926535897932384626433832795029)

/** Ball-Werte */
#define BALL_ITERATIONS (32)

/* ----------------------------------------------------------------------------
 * Typedeklarationen
 * -------------------------------------------------------------------------- */
 
/** Boolean */
typedef enum {FALSE, TRUE} Boolean;

/** Vektor im R² */
typedef struct {
  double x
       , y;
} Vector2d;

/* Eine Gerade */
typedef struct {
  Vector2d p  /* Aufpunkt */
         , q; /* Richtungsvektor */
} Line;

/** RGB-Farbwert */
typedef double RGBColor[3];

/** Ein Quadrat */
typedef Vector2d Rect[4];

/** Spielstatus */
typedef struct {
  unsigned lives;
  unsigned points;
} GameState;

/** Der Schläger */
typedef struct {
  Vector2d p;    /* Mittellpunkt                         */
  double angle;  /* Auslenkungswinkel                    */
  Rect vertices; /* Eckpunkte, errechnet aus p und angle */
} Racket;

/** Der Ball */
typedef struct {
  Vector2d p                          /* Mittelpunkt                 */
         , direction;                 /* Bewegungsrichtung           */
  double speed;                       /* Geschwindigkeit             */
  unsigned color;                     /* Farbindex                   */
  RGBColor c;                         /* Farbe                       */
  Vector2d vertices[BALL_ITERATIONS]; /* Approximierte Balleckpunkte */
} Ball; 

/** Bewegungsrichtungen. */
typedef enum {
  dirLeft
, dirRight
, dirUp
, dirDown} Direction;

#endif
