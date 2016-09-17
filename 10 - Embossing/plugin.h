#ifndef BBA_PLUGIN_H
#define BBA_PLUGIN_H 1
/**
 * @file plugin.h Schnittstelle eines Plug-ins fuer GIMP 2.2 und hoeher.
 */

#include "libgimp/gimp.h"

/** Interner Bezeicher. Sollte eindeutig sein. */
#define PLUG_IN_NAME           "__MyEmboss__"

/** Kurze Beschreibung der Plug-ins. */
#define PLUG_IN_BLURB          "MyEmboss"

/** Etwas laengere Beschreibung des Plug-ins. */
#define PLUG_IN_HELP           "MyEmboss"

/** Autor */
#define PLUG_IN_AUTHOR         "BBA Group"
/** Copyright info */
#define PLUG_IN_COPYRIGHT      "www.fh-wedel.de - University of Applied Sciences Wedel, Germany"
/** Copyright Jahr */
#define PLUG_IN_COPYRIGHT_DATE "2009"

/**
 * Menueposition fuer das Plug-in.
 * Beginnt mit &lt;Image&gt;/Filters/ fuer ein Filter-Plug-in.
 */
#define PLUG_IN_MENU_ENTRY "<Image>/Filters/MyFilters/MyEmboss"

/**
 * Bildtypen, die das Plug-in bearbeiten kann.
 * Gueltige Werte:                   (ohne alpha, mit alpha oder beides)
 *   Farbbilder (RGB):               RGB, RGBA, RGB*
 *   Graustufenbilder:               GRAY, GRAYA, GRAY*,
 *   Bilder mit indizierten Farben:  INDEXED, INDEXEDA, INDEXED*
 */
#define PLUG_IN_IMAGE_TYPES "RGB, GRAY"



/**
 * DIE Funktion zum Filtern des Drawables
 *
 * @param[in] drawable das Drawable, das aktiv war, als der Filter aufgerufen
 *                     wurde.
 * @param[in] azimuth Azimuth fuer Emboss.
 * @param[in] elevation Elevation fuer Emboss.
 * @param[in] alphaPencilSketch Blendfaktors fuer Pencil-Sketch.
 * @param[in] emboss 1, wenn Emboss, 0, wenn Pencil-Sketch.
 *
 * @return TRUE = alles ok, FALSE = ein Fehler ist aufgetreten
 */
gboolean filterDrawable (GimpDrawable * drawable, gfloat azimuth, gfloat height,
    gfloat alphaPencilSketch, gint emboss);

#endif
