#ifndef __DISPLAYLIST_H__
#define __DISPLAYLIST_H__
/**
 * @file
 *
 * Das Modul stellt Displaylisten zur Verfügung.
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */
#include "types.h"

/**
 * Initialisiert die Displaylisten.
 */
extern void displaylistInit(void);

/**
 * Befüllt die Displaylisten mit den aktuellen Objekten.
 */
extern void displaylistSet(void);

/**
 * Ruft die Displayliste für den Zeichenfunktionstyp d auf.
 *
 * @param[in] d Zeichenfunktionstyp.
 */
extern void displaylistCall(DrawFunctionType d);

#endif
