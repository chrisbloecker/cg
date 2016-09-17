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

extern void drawShell(void);

/* ----------------------------------------------------------------------------
 * Obstacles
 * -------------------------------------------------------------------------- */

extern void drawCar(void);

extern void drawShip(void);

extern void drawCrab(void);

/* ----------------------------------------------------------------------------
 * Hugo
 * -------------------------------------------------------------------------- */

extern void drawHugoStreetWear(void);

extern void drawHugoSwimmingSuit(void);

extern void drawHugoBermudaShorts(void);

#endif
