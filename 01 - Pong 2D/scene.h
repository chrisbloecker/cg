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
 * Zeichen-Funktion.
 * Stellt die Szene mit Balken, Schläger und Ball dar.
 */
void drawScene(void);

/**
 * Initialisierung der Szene.
 */
int initScene(void);

#endif
