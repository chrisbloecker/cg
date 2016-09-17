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
} Light;

/* Anzahl der Lichteigenschaften eines Materials */
#define LIGHTS (LIGHT_SPECULAR + 1)

/* Materialien */
typedef enum {
  MAT_GRASS
, MAT_FENCE
, MAT_GLASS
, MAT_STONE
, MAT_WATER
, MAT_PIG
, MAT_PIG_EYE
, MAT_PIG_NOSE
, MAT_GIRAFFE
, MAT_FISH
, MAT_SUN
, MAT_DUMMY
} Material;

/* Anzahl der Materialien */
#define MATERIALS (MAT_DUMMY)

/* Datentyp für die Materialeigenschaften */
typedef float MaterialProps[MATERIALS][LIGHTS][4];

/**
 * Setzt den aktuellen Materialtyp auf m.
 *
 * @param[in] m Zu verwendender Materialtyp.
 */
extern void materialSet(Material m);

#endif
