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

/** Funktionszeiger für eine Zeichenfunktion */
typedef void (*DrawFunction) (void);

/** Enum für Elemente, die gezeichnet werden sollen */
typedef enum {
  DF_GROUND
, DF_SOCKET
, DF_CAGE
, DF_CEILING
, DF_POOL
, DF_WATER
, DF_PIG
, DF_GIRAFFE
, DF_GIRAFFE_HEAD
, DF_FISH
, DF_SUN
, DF_GIRAFFE_CUBE
, DF_DUMMY
} DrawFunctionType;

/** Datentyp fuer Mausereignisse. */
typedef enum { 
  MOUSE_BUTTON
, MOUSE_MOTION
, MOUSE_PASSIVE
} MouseEventType;

#endif
