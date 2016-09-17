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
 * Typen
 * -------------------------------------------------------------------------- */

#include <GL/gl.h>

#include "texture.h"

/** Boolean */
typedef enum {FALSE, TRUE} Boolean;

/** Funktionszeiger für eine Zeichenfunktion */
typedef void (*DrawFunction) (TextureName t);

/** Enum für Elemente, die gezeichnet werden sollen */
typedef enum {
  DF_CUBE
, DF_SPHERE
, DF_CYLINDER
, DF_KEGEL
, DF_DUMMY
} DrawFunctionType;

/** Vektor im 3D-Raum */
typedef GLfloat CGVector3f[3];

/** Vektor im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGVector4f[4];

/** 4x4 Matrix (Column-Major-Ordering) */
typedef GLfloat CGMatrix16f[16];

#endif
