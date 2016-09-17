#ifndef __TEXTURE_H__
#define __TEXTURE_H__
/**
 * @file
 * Schnittstelle des Texturen-Moduls.
 * Das Modul kapselt die Textur-Funktionalitaet (insbesondere das Laden und
 * Binden) des Programms.
 *
 * Bestandteil eines Beispielprogramms fuer Texturierung mit OpenGL & GLUT.
 *
 * @author copyright (C) Fachhochschule Wedel 1999-2007. All rights reserved.
 */

/**
 * Typ fuer Texturenbezeichner.
 */
typedef enum {
  TEXTURE_METAL
, TEXTURE_STONE
, TEXTURE_WATER
, TEXTURE_SUN
, TEXTURE_EMPTY
} TextureName;


/**
 * Laed Texturen und initialisiert das Texturmapping.
 *
 * @return 1 wenn Laden und Initialisieren erfolgreich war
 *         0 sonst.
 */
extern int initTextures(void);

/**
 * Bindet die Textur texture, so dass sie fuer alle nachfolgende gezeichneten
 * Primitiven verwendet wird.
 *
 * @param texture Bezeichner der Textur, die gebunden werden soll.
 */
extern void bindTexture(TextureName t);

/**
 * Schaltet die Giraffen-Textur um.
 */
extern void textureSwitch(void);

#endif
