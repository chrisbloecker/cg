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

#include "types.h"

/**
 * Setzen der Projektionsmatrix.
 * Setzt die Projektionsmatrix unter Beruecksichtigung des Seitenverhaeltnisses
 * des Anzeigefensters, sodass das Seitenverhaeltnisse der Szene unveraendert
 * bleibt und gleichzeitig entweder in x- oder y-Richtung der Bereich von -1
 * bis +1 zu sehen ist.
 * @param aspect Seitenverhaeltnis des Anzeigefensters (In).
 */
extern void setProjection(double aspect);

/**
 * Setzen der Kameraposition.
 */
extern void setCamera(void);
/**
 * Zeichnet die Szene.
 */
extern void sceneDraw(Boolean names);

/**
 * Initialisierung der Szene.
 */
int sceneInit(void);

/**
 * Legt fest, ob Displaylisten zum Zeichnen verwendet werden sollen oder nicht.
 * Wechselt dabei zwischen "YES" und "NO".
 */
extern void sceneSetDisplayListUsage(void);

#ifdef DEBUG
/**
 * Schaltet das Anzeigen von z-Informationen ein bzw. aus.
 */
extern void sceneSetZInfo(void);
#endif

#endif
