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
  MAT_CAR
, MAT_CAR_CABIN
, MAT_CAR_TYRE
, MAT_CAR_FRONTLIGHT
, MAT_CAR_BACKLIGHT
, MAT_SHIP
, MAT_SHIP_SAIL
, MAT_SUNSHADE_STICK
, MAT_SUNSHADE_TOP_INNER
, MAT_SUNSHADE_TOP_OUTER
, MAT_TREE_TRUNK
, MAT_TREE_CROWN
, MAT_BUOY
, MAT_PALM_TRUNK_DARK
, MAT_PALM_TRUNK_LIGHT
, MAT_PALM_LEAF
, MAT_COCONUT
, MAT_COIN
, MAT_STARFISH
, MAT_ICE_WAFFLE
, MAT_ICE_BANANA
, MAT_ICE_STRAWBERRY
, MAT_ICE_CHOCOLATE
, MAT_STREET
, MAT_STREET_SURFACE
, MAT_STREET_BACKGROUND
, MAT_BEACH
, MAT_BEACH_SURFACE
, MAT_BEACH_BACKGROUND
, MAT_RIVER
, MAT_RIVER_SURFACE
, MAT_RIVER_BACKGROUND
, MAT_LIGHTHOUSE_RED
, MAT_LIGHTHOUSE_WHITE
, MAT_HUGO_BODY
, MAT_HUGO_LEGS
, MAT_HUGO_FEET
, MAT_HUGO_ARMS
, MAT_HUGO_HEAD
} Material;

/* Anzahl der Materialien */
#define MATERIALS (MAT_HUGO_HEAD + 1)

/* Datentyp für die Materialeigenschaften */
typedef float MaterialProps[MATERIALS][LIGHTS][5];

/**
 * Setzt den aktuellen Materialtyp auf m.
 *
 * @param[in] m Zu verwendender Materialtyp.
 */
extern void materialSet(Material m);

#endif
