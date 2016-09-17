#ifndef __OBJECT_H__
#define __OBJECT_H__

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
 
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */

#include "vector.h"
#include "material.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
 
#define OBJECT_COUNT (15)

/* ----------------------------------------------------------------------------
 * Typen
 * -------------------------------------------------------------------------- */

/** Figures */
typedef enum {
  FIGURE_CUBE     /* A Cube     */
, FIGURE_SPHERE   /* A Sphere   */
, FIGURE_CYLINDER /* A Cylinedr */
, FIGURE_KEGEL    /* A Kegel    */
} Figure;

/** Object */
typedef struct {
  Figure f;  /* Type of Figure */
  
  Vector3d t /* Translation    */
         , r /* Rotation Axis  */
         , s /* Scaling        */
         ;
  
  DrawFunction d;  /* Function for Drawing */
  
  TextureName tex; /* Texture to be used   */
  Material m;
  
  char * name;
  
  double a
       , size      /* Für die Sortierung   */
       ;
       
  int z;
} Object;

/** Objektsammlungen */
typedef struct {
  Object * objects[OBJECT_COUNT];
  
  int last
    , zMax
    ;
} Objects;

/* ----------------------------------------------------------------------------
 * Funktionen
 * ----------------------------------------------------------------------------
 * Exportiert
 * -------------------------------------------------------------------------- */

/**
 * Erzeugt ein Objekt vom Typ Cube und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeCube(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name);

/**
 * Erzeugt ein Objekt vom Typ Sphere und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeSphere(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name);

/**
 * Erzeugt ein Objekt vom Typ Cylinder und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeCylinder(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name);

/**
 * Erzeugt ein Objekt vom Typ Kegel und gibt dieses zurück.
 *
 * @param[in] t    Verschiebungsvektor.
 * @param[in] r    Rotationsaxe.
 * @param[in] s    Skalierungsvektor.
 * @param[in] a    Rotationswinkel.
 * @param[in] tex  Textur.
 * @param[in] m    Material.
 * @param[in] name Name.
 *
 * @return Erzeugtes Objekt mit den angegebenen Eigenschaften.
 */
extern Object objectMakeKegel(Vector3d t, Vector3d r, Vector3d s, double a, TextureName tex, Material m, char * name);

/**
 * Zeichnet das Objekt o.
 *
 * @param[in] o Zu zeichnendes Objekt.
 */
extern void objectDraw(Object * o);

/**
 * Erzeugt ein Objects und gibt es initialisiert zurück.
 *
 * @return initialisiertes Objects.
 */
extern Objects objectsInit(void);

/**
 * Fügt das Objekt o in den Level os ein.
 *
 * @param[in] l   Level.
 * @param[in] i   Einzufügendes Objekt.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return Level, in den i eingefügt wurde.
 */
extern Objects * objectsInsert(Objects * os, Object * o, Vector3d eye, Vector3d n);

/**
 * Gibt einen Zeiger auf das i-te Object von os zurück.
 *
 * @param[in] os Objekte.
 * @param[in] i  Index des auszuwählenden Elementes.
 *
 * @return Zeiger auf das i-te Element von l.
 */
extern Object * objectsAt(Objects * os, int i);

/**
 * Sortiert die Objekte os.
 *
 * @param[in] os  Objekte.
 * @param[in] eye Augpunkt.
 * @param[in] n   Blickrichtung.
 *
 * @return sortierter Level.
 */
extern Objects * objectsSort(Objects * os, Vector3d eye, Vector3d n);

#ifdef DEBUG
/**
 * Gibt die Objekte os auf stream aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] os     Objekte.
 */
extern void objectsPrint(FILE * stream, Objects * os);

/**
 * Gibt die Zeichenreihenfolge der Objekte von os aus.
 *
 * @param[in] stream Ausgabekanal.
 * @param[in] os     Objekte.
 */
extern void objectsPrintDrawingOrder(FILE * stream, Objects * os);
#endif

#endif
