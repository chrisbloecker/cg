#ifndef __TYPES_H__
#define __TYPES_H__
/**
 * @file
 * Typenschnittstelle.
 * Das Modul kapselt die Typdefinitionen des Programms.
 *
 * @author Christopher Blöcker
 */

/* ----------------------------------------------------------------------------
 * System-Header einbinden
 * -------------------------------------------------------------------------- */
#include <GL/gl.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Boolean */
typedef enum { FALSE, TRUE } Boolean;

/** Vektor im 3D-Raum (homogene Koordinaten) */
typedef GLfloat CGVector4f[4];

#endif
