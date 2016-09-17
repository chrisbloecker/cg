#ifndef _STRING_OUTPUT_H
#define _STRING_OUTPUT_H
/**
 * @file
 * Einfache Funktion zum Zeichnen von Text fuer GLUT-Programme.
 */

/* ---- System Header einbinden ---- */
#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/* ---- Funktionsprototypen ---- */

/**
 * Zeichnen einer Zeichfolge in den Vordergrund. Gezeichnet wird mit Hilfe von
 * glutBitmapCharacter(...). Kann wie printf genutzt werden.
 *
 * @param x x-Position des ersten Zeichens 0 bis 1 (In).
 * @param y y-Position des ersten Zeichens 0 bis 1 (In).
 * @param color Textfarbe (In).
 * @param format Formatstring fuer die weiteren Parameter (In).
 */
void drawString (GLfloat x, GLfloat y, GLfloat * color, char *format, ...);

/**
 * Zeichnet ein Rechteck mit den Hilfetexten.
 */
extern void drawHelp(void);

#endif
