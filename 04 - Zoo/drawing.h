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
 * Schaltete zwischen "Normalen anzeigen" und "Normalen nicht anzeigen".
 */
extern void drawingSetNormals(void);

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
 * Zeichnet eine Gitterwand.
 */

extern void drawFence(void);

/**
 * Zeichnet die Steindecke.
 */

extern void drawFenceCeiling(void);

/**
 * Zeichnet eine Glaswand.
 */
extern void drawGlass(void);

/**
 * Zeichnet die Glasdecke.
 */

extern void drawGlassCeiling(void);

/**
 * Zeichnet den Pool.
 */
extern void drawPool(void);

/**
 * Zeichnet das Wasser.
 */
extern void drawWater(void);

/**
 * Zeichnet das Schwein.
 */
extern void drawPig(void);

/**
 * Zeichnet den Kopf der Giraffe.
 */
extern void drawGiraffeHead(void);

/**
 * Zeichnet den Körper der Giraffe.
 */
extern void drawGiraffe(void);

/**
 * Zeichnet den Fisch.
 */
extern void drawFish(void);

/**
 * Zeichnet den Grasboden.
 */
extern void drawGround(void);

/**
 * Zeichnet den Steinsockel.
 */
extern void drawSocket(void);

/**
 * Zeichnet die Sonne.
 */
extern void drawSun(void);

/**
 * Zeichnet einen giraffigen Würfel.
 */
extern void drawGiraffeCube(void);

/**
 * Zeichnet nichts.
 */
extern void drawNothing(void);

#endif
