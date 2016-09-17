#ifndef __IO_H__
#define __IO_H__
/**
 * @file
 * Schnittstelle des Ein-/Ausgabe-Moduls.
 * Das Modul kapselt die Ein- und Ausgabe-Funktionalitaet (insbesondere die GLUT-
 * Callbacks) des Programms.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/**
 * Initialisiert das Programm (inkl. I/O und OpenGL) und startet die
 * Ereignisbehandlung.
 *
 * @param[in] title  Beschriftung des Fensters
 * @param[in] width  Breite des Fensters
 * @param[in] height Höhe des Fensters
 *
 * @return ID des erzeugten Fensters, 0 im Fehlerfall
 */
int initAndStartIO(char * title, int width, int height);

#endif
