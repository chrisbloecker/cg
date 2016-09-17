#ifndef __MATERIAL_H__
#define __MATERIAL_H__
/**
 * @file
 *
 * Das Modul definiert Materialeigenschaften.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* Lichteigenschaften */
typedef enum {
  LIGHT_AMBIENT
, LIGHT_DIFFUSE
, LIGHT_SPECULAR
, LIGHT_DUMMY
} Light;

/* Materialien */
typedef enum {
  MAT_GRASS
, MAT_METAL
, MAT_GLASS
, MAT_STONE
, MAT_WATER
, MAT_SUN
, MAT_BALL0
, MAT_BALL1
, MAT_BALL2
, MAT_BALL3
, MAT_BALL4
, MAT_BALL5
, MAT_DUMMY
} Material;

/* Datentyp für die Materialeigenschaften */
typedef float MaterialProps[MAT_DUMMY][LIGHT_DUMMY][4];

/**
 * Setzt den aktuellen Materialtyp auf m.
 *
 * @param[in] m Zu verwendender Materialtyp.
 */
extern void materialSet(Material m);

#endif
