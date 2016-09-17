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
void drawScene(void);

/**
 * Initialisierung der Szene.
 */
int initScene(void);

#endif
