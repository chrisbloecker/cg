#ifndef __SCENE_H__
#define __SCENE_H__
/**
 * @file
 * Darstellungs-Modul.
 * Das Modul kapselt die Rendering-Funktionalität (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/**
 * Zeichnet die Szene.
 */
void sceneDraw(void);

/**
 * Initialisierung der Szene.
 */
int sceneInit(void);

/**
 * Legt fest, ob Displaylisten zum Zeichnen verwendet werden sollen oder nicht.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetDisplayListUsage(void);

/**
 * Legt fest, ob die globale Lichtquelle um den Ursprung rotiert werden soll.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetLightRotation(void);

/**
 * Schaltet das Licht mit der Nummer i ein bzw. aus.
 */
extern void sceneSetLight(unsigned i);

#endif
