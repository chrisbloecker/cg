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
 * Zeichnet die Szene.
 *
 * @param[in] shadow Gibt an, ob Schatten gezeichnet werden soll.
 */
extern void sceneDraw(Boolean shadow);

/**
 * Initialisierung der Szene.
 */
extern int sceneInit(void);

/**
 * Schaltet um zwischen Wireframe Mode und Füllmode.
 */
extern void sceneSwitchWireframeMode(void);

/**
 * Räumt auf.
 */
extern void sceneCleanup(void);

#endif
