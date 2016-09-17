#ifndef __DRAWING_H__
#define __DRAWING_H__
/**
 * @file
 *
 * Das Modul stellt Zeichenfunktionen zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

#include "texture.h"

/**
 * Setzt die Rekursionstiefe für das Zeichnen der Objekte und aktualisiert die
 * Displaylisten.
 *
 * @param[in] i Rekursionstiefe.
 */
extern void drawingSetSubdivides(int i);

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * -------------------------------------------------------------------------- */

/**
 * Zeichnet einen Würfel.
 *
 * @param[in] t Name der zu verwendenen Textur.
 */
extern void drawCube(TextureName t);

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawCylinder(TextureName t);

/**
 * Zeichnet einen Zylinder.
 *
 * @param[in] proportion Verhältnis des Radius des oberen Kreises zum unteren.
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawKegel(double proportion, TextureName t);

/**
 * Zeichnet eine Einheitskugel mit dem Radius 1 im Ursprung.
 *
 * @param[in] t          Name der zu verwendenen Textur.
 */
extern void drawSphere(TextureName t);

/**
 * Zeichnet nichts.
 * @param[in] t Name der zu verwendenen Textur.
 */
extern void drawNothing(TextureName t);

#endif
