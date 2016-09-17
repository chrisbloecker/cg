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

/**
 * Setzt den Zeichenmodus.
 *
 * @param[in] mode Zeichenmodus.
 */
extern void drawingSetMode(int mode);

extern void drawingSetNormals(void);

/**
 * Setzt die Rekursionstiefe für das Zeichnen der Objekte.
 *
 * @param[in] i Rekursionstiefe.
 */
extern void drawingSetSubdivides(int i);

/* ----------------------------------------------------------------------------
 * Zeichenfunktionen
 * ----------------------------------------------------------------------------
 * Ground
 * -------------------------------------------------------------------------- */
 
extern void drawStreet(void);

extern void drawRiver(void);

extern void drawBeach(void);

/* ----------------------------------------------------------------------------
 * Background
 * -------------------------------------------------------------------------- */

extern void drawStreetBackground(void);

extern void drawRiverBackground(void);

extern void drawBeachBackground(void);

/* ----------------------------------------------------------------------------
 * Surface
 * -------------------------------------------------------------------------- */

extern void drawStreetSurface(void);

extern void drawRiverSurface(void);

extern void drawBeachSurface(void);

/* ----------------------------------------------------------------------------
 * Environment
 * -------------------------------------------------------------------------- */

extern void drawTree(void);

extern void drawBuoy(void);

extern void drawPalm(void);

/* ----------------------------------------------------------------------------
 * Items
 * -------------------------------------------------------------------------- */

extern void drawCoin(void);

extern void drawStarfish(void);

extern void drawIce(void);

/* ----------------------------------------------------------------------------
 * Obstacles
 * -------------------------------------------------------------------------- */

extern void drawCar(void);

extern void drawShip(void);

extern void drawSunshade(void);

/* ----------------------------------------------------------------------------
 * Hugo
 * -------------------------------------------------------------------------- */

extern void drawHugo(void);

/* ----------------------------------------------------------------------------
 * Buildings
 * -------------------------------------------------------------------------- */

extern void drawNothing(void);

extern void drawLighthouse(void);

#endif
