/**
 * @file
 * Hauptprogramm.
 * Startet die IO-Verwaltung ... und somit das Spiel ...
 *
 * @author Christopher Blöcker
 * @author Julius Beckmann
 */

/* ----------------------------------------------------------------------------
 * System Header einbinden
 * -------------------------------------------------------------------------- */
#include <stdio.h>

/* ----------------------------------------------------------------------------
 * Eigene Header einbinden
 * -------------------------------------------------------------------------- */
#include "io.h"

/* ----------------------------------------------------------------------------
 * Konstanten
 * -------------------------------------------------------------------------- */
/* Fenstername */
#define WINDOW_NAME "Hugo"

/* Fenstergröße */
#define WINDOW_SIZE_X 500
#define WINDOW_SIZE_Y 500

/**
 * Initialisierung und Starten der Ereignisbehandlung.
 *
 * @param[in] argc Anzahl der Kommandozeilenparameter.
 * @param[in] argv Kommandozeilenparameter.
 *
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int main(int argc, char * argv[])
{
  /* Initialisierung des I/O-Sytems
     (inkl. Erzeugung des Fensters und Starten der Ereignisbehandlung). */
  if (!initAndStartIO(WINDOW_NAME, WINDOW_SIZE_X, WINDOW_SIZE_Y))
  {
    fprintf (stderr, "Initialisierung fehlgeschlagen!\n");
    return 1;
  }
  else
    return 0;
}
